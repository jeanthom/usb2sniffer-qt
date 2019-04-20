#include <ft601_err.h>

typedef struct _ft601 ft601_t;

ft601_err_t ft601_open(ft601_t **handle, uint16_t vid, uint16_t pid, unsigned int timeout);
ft601_err_t ft601_readpipe(ft601_t *handle, uint8_t pipe_id, void *buf, size_t len, size_t *read_bytes);
ft601_err_t ft601_writepipe(ft601_t *handle, uint8_t pipe_id, void *buf, size_t len, size_t *written_bytes);
void ft601_close(ft601_t *handle);
