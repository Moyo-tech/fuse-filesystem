#include "dummy.h"

int do_read(const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi)
{
	printf("READING THE FILE\n");
	// Reads the contents of the file or directory at the specified path.
	// If the file or directory does not exist, returns -ENOENT.
	// Otherwise, returns the number of bytes read.

	// Get the file or directory to read
	filetype *file = filetype_from_path(path);

	// If the file or directory does not exist, return -ENOENT
	if (file == NULL)
		return -ENOENT;

	// Initialize a string to store the file or directory's contents
	char *str = malloc(sizeof(char) * 1024 * (file->blocks));

	// Copy the file or directory's contents to the string
	strcpy(str, "");
	for (int i = 0; i < file->blocks; i++)
	{
		strncat(str, &spblock.datablocks[block_size * (file->datablocks[i])], 1024);
		printf("--> %s", str);
	}

	// Copy the file or directory's contents to the buffer
	strcpy(buffer, str);

	// Return the number of bytes read
	return file->size;
}
