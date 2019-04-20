OBJS = src/ft601.o
CFLAGS = $(shell pkg-config --cflags libusb-1.0)
LDFLAGS = $(shell pkg-config --libs libusb-1.0)

libft601.a: $(OBJS)
	ar rcs libft601.a $<

%.o: %.c
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

clean:
	rm -rf libft601.a libft601.so libft601.dylib src/*.o
