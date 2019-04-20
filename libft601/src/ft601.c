#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <libusb.h>
#include <ft601_err.h>

#define USB_TYPE_VENDOR (0x02 << 5)
#define USB_DIR_OUT 0
#define USB_DIR_IN 0x80

struct ft60x_config {
	/* Device Descriptor */
	uint16_t VendorID;
	uint16_t ProductID;
	/* String Descriptors */
	uint8_t StringDescriptors[128];
	/* Configuration Descriptor */
	uint8_t Reserved;
	uint8_t PowerAttributes;
	uint16_t PowerConsumption;
	/* Data Transfer Configuration */
	uint8_t Reserved2;
	uint8_t FIFOClock;
	uint8_t FIFOMode;
	uint8_t ChannelConfig;
	/* Optional Feature Support */
	uint16_t OptionalFeatureSupport;
	uint8_t BatteryChargingGPIOConfig;
	uint8_t FlashEEPROMDetection; /* Read-only */
	/* MSIO and GPIO Configuration */
	uint32_t MSIO_Control;
	uint32_t GPIO_Control;
} __attribute__ ((packed));

struct ft60x_ctrlreq {
	uint32_t idx;
	uint8_t pipe;
	uint8_t cmd;
	uint8_t unk1;
	uint8_t unk2;
	uint32_t len;
	uint32_t unk4;
	uint32_t unk5;
} __attribute__ ((packed));

typedef struct {
	libusb_context *usb_ctx;
	libusb_device_handle *usb_handle;
	int timeout;
	struct ft60x_ctrlreq ctrlreq;
	struct ft60x_config cfg;
} ft601_t;

static ft601_err_t ft601_get_unkwn(ft601_t *handle) {
	uint32_t val;
	libusb_control_transfer(handle->usb_handle, USB_TYPE_VENDOR | USB_DIR_IN, 0xf1, 0, 0, &val, sizeof(uint32_t), handle->timeout);

	return E_OK; // One day I will write proper error handling code
}

static ft601_err_t ft601_ctrl_req(ft601_t *handle) {
	size_t written_bytes;
	int res;

	res = libusb_bulk_transfer(handle->usb_handle, 1, &(handle->ctrlreq), sizeof(struct ft60x_ctrlreq), &written_bytes, 1000);
	if (res == 0) {
		return E_OK;
	} else if (res == LIBUSB_ERROR_TIMEOUT) {
		return E_TIMEOUT;
	} else if (res == LIBUSB_ERROR_NO_DEVICE) {
		return E_DISCONNECTED;
	} else {
		fprintf(stderr, "ft601_ctrl_req: Unidentified error from libusb_bulk_transfer() (error code is %d)\n", res);
		fprintf(stderr, "Message is: %s\n", libusb_strerror(res));
		return E_OTHER;
	}

	if (written_bytes != sizeof(struct ft60x_ctrlreq)) {
		fprintf(stderr, "ft601_ctrl_req: Written bytes count does not match with expected value\n");
		return E_OTHER;
	}
}

static ft601_err_t ft601_read_cfg(ft601_t *handle, struct ft60x_config *cfg) {
	libusb_control_transfer(handle->usb_handle, USB_TYPE_VENDOR | USB_DIR_IN, 0xcf, 1, 0, cfg, sizeof(struct ft60x_config), handle->timeout);

	return E_OK;
}

ft601_err_t ft601_open(ft601_t **handle, uint16_t vid, uint16_t pid, unsigned int timeout) {
	int res;
	ft601_t *tmp_handle;

	tmp_handle = malloc(sizeof(ft601_t));
	if (!tmp_handle) {
		*handle = NULL;
		return E_ALLOC;
	}

	res = libusb_init(&(tmp_handle->usb_ctx));
	if (res != 0) {
		free(tmp_handle);
		*handle = NULL;
		return E_INTERNAL_LIBUSB;
	}

	tmp_handle->usb_handle = libusb_open_device_with_vid_pid(tmp_handle->usb_ctx, vid, pid);
	if (!(tmp_handle->usb_handle)) {
		libusb_exit(tmp_handle->usb_ctx);
		free(tmp_handle);
		*handle = NULL;
		return E_DEV_NOT_FOUND;
	}

	/* Mandatory on macOS, otherwise we get "Entity not found" errors */
	res = libusb_claim_interface(tmp_handle->usb_handle, 0);
	if (res != 0) {
		libusb_close(tmp_handle->usb_handle);
		libusb_exit(tmp_handle->usb_ctx);
		free(tmp_handle);
		*handle = NULL;
		return E_NO_CLAIM;
	}
	res = libusb_claim_interface(tmp_handle->usb_handle, 1);
	if (res != 0) {
		libusb_close(tmp_handle->usb_handle);
		libusb_exit(tmp_handle->usb_ctx);
		free(tmp_handle);
		*handle = NULL;
		return E_NO_CLAIM;
	}

	memset(&(tmp_handle->ctrlreq), 0, sizeof(struct ft60x_ctrlreq));

	tmp_handle->timeout = timeout;

	ft601_read_cfg(tmp_handle, &(tmp_handle->cfg));
	ft601_get_unkwn(tmp_handle);

	*handle = tmp_handle;

	return E_OK;
}

ft601_err_t ft601_readpipe(ft601_t *handle, uint8_t pipe_id, void *buf, size_t len, size_t *read_bytes) {
	int res;

	*read_bytes = 0;

	printf("Debug: reading %d bytes\n", len);

	handle->ctrlreq.idx++;
	handle->ctrlreq.pipe = pipe_id;
	handle->ctrlreq.cmd = 1;
	//handle->ctrlreq.len = libusb_get_max_packet_size(handle->usb_handle, pipe_id) * 128;
	handle->ctrlreq.len = 1024 * 128;
	ft601_ctrl_req(handle);

	res = libusb_bulk_transfer(handle->usb_handle, pipe_id, buf, len, (int*)read_bytes, handle->timeout);
	if (res == 0) {
		return E_OK;
	} else if (res == LIBUSB_ERROR_TIMEOUT) {
		return E_TIMEOUT;
	} else if (res == LIBUSB_ERROR_NO_DEVICE) {
		return E_DISCONNECTED;
	} else {
		fprintf(stderr, "ft601_readpipe: Unidentified error from libusb_bulk_transfer() (error code is %d)\n", res);
		fprintf(stderr, "Message is: %s\n", libusb_strerror(res));
		return E_OTHER;
	}
}

ft601_err_t ft601_writepipe(ft601_t *handle, uint8_t pipe_id, void *buf, size_t len, size_t *written_bytes) {
	int res;
	
	*written_bytes = 0;

	printf("Debug: writing %d bytes\n", len);

	res = libusb_bulk_transfer(handle->usb_handle, pipe_id, buf, len, (int*)written_bytes, handle->timeout);
	if (res == 0) {
		return E_OK;
	} else if (res == LIBUSB_ERROR_TIMEOUT) {
		return E_TIMEOUT;
	} else if (res == LIBUSB_ERROR_NO_DEVICE) {
		return E_DISCONNECTED;
	} else {
		fprintf(stderr, "ft601_writepipe: Unidentified error from libusb_bulk_transfer() (error code is %d)\n", res);
		fprintf(stderr, "Message is: %s\n", libusb_strerror(res));
		return E_OTHER;
	}
}

void ft601_close(ft601_t *handle) {
	assert(handle != NULL);

	libusb_release_interface(handle->usb_handle, 1);
	libusb_close(handle->usb_handle);
	libusb_exit(handle->usb_ctx);

	free(handle);
}
