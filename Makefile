COMPILER = gcc
FILESYSTEM_FILES = main.c

build: $(FILESYSTEM_FILES)
	$(COMPILER) $(FILESYSTEM_FILES) -o main `pkg-config fuse --cflags --libs`
	echo 'You should run this: ./main -f /tmp/example'

clean:
	rm main