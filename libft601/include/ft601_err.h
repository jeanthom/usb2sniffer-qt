typedef enum {
	E_OK, /* No error */
	E_OTHER, /* Undisclosed error reason */
	E_ALLOC, /* Memory allocation error */
	E_INTERNAL_LIBUSB, /* libusb error */
	E_DEV_NOT_FOUND, /* Device not found */
	E_DISCONNECTED, /* Disconnected device */
	E_TIMEOUT, /* Timeout */
	E_NO_CLAIM, /* Can't claim USB interface */
} ft601_err_t;