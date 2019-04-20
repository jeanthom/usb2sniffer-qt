#include <stdio.h>
#include <stdint.h>

#ifdef LIBFT601
#include <ft601.h>
#endif

#include "fops.h"

int ft60x_open(ftdev_t *fd, const char *path)
{
#ifdef __linux__
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

    *fd = open(path, O_RDWR, mode);
    return *fd;
#elif LIBFT601
    ft601_err_t status;

    status = ft601_open((ft601_t**)fd, 0x0403, 0x601f, 100);
    if (status != E_OK) {
        return -1;
    } else {
        return 0;
    }
#else
    FT_STATUS ftStatus = FT_OK;

    // XXX pass index as argument ?
    ftStatus = FT_Create(0, FT_OPEN_BY_INDEX, fd);
    if (FT_FAILED(ftStatus))
    {
        return -1;
    }

    return 0;
#endif
}

int ft60x_close(ftdev_t fd)
{
#ifdef __linux__
    return close(fd);
#elif LIBFT601
    ft601_close((ft601_t*)fd);
    return 0;
#else
    FT_STATUS ftStatus = FT_OK;

    ftStatus = FT_Close(fd);
    if (FT_FAILED(ftStatus))
    {
        return -1;
    }
    return 0;
#endif
}

ssize_t ft60x_read(ftdev_t fd, void *buf, size_t len)
{
#ifdef __linux__
    return read(fd, buf, len);
#elif LIBFT601
    size_t ulBytesRead = 0;
    ft601_readpipe((ft601_t*)fd, 0x82, buf, len, &ulBytesRead);
    return ulBytesRead;
#else
    FT_STATUS ftStatus = FT_OK;
    unsigned long ulBytesRead = 0;
    unsigned char pipeID = 0x82;

    ftStatus = FT_ReadPipe(fd, pipeID, buf, len, &ulBytesRead, NULL);
    if (FT_FAILED(ftStatus))
    {
        fprintf(stderr, "ft60x_read error : %d\n", ftStatus);
        FT_AbortPipe(fd, pipeID);
        return -1;
    }
    return ulBytesRead;
#endif
}

ssize_t ft60x_write(ftdev_t fd, const void *buf, size_t len)
{
#ifdef __linux__
    return write(fd, buf, len);
#elif LIBFT601
    size_t ulBytesRead = 0;
    
    ft601_writepipe((ft601_t*)fd, 0x02, buf, len, &ulBytesRead);
    return ulBytesRead;
#else
    FT_STATUS ftStatus = FT_OK;
    unsigned long ulBytesWritten = 0;
    unsigned char pipeID = 0x02;

    ftStatus = FT_WritePipe(fd, pipeID, buf, len, &ulBytesWritten, NULL);
    if (FT_FAILED(ftStatus))
    {
        fprintf(stderr, "ft60x_write error : %d\n", ftStatus);
        FT_AbortPipe(fd, pipeID);
        return -1;
    }
    return ulBytesWritten;
#endif
}
