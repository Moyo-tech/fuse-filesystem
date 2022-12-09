#include "dummy.h"

// This function writes data to a file.
// It takes as input the path of the file, the data to be written, the size of the data,
// the offset at which to start writing the data, and a pointer to the file info structure.
// It returns the number of bytes written or -1 if an error occurs.
int do_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
	// Print a message indicating that we are writing to the file.
	printf("WRITING TO THE FILE\n");

	// Allocate memory for a copy of the path.
	char * pathname = malloc(sizeof(path) + 1);

	// Copy the path to the allocated memory.
	strcpy(pathname, path);

	// Get the filetype structure for the file with the given path.
	filetype * file = filetype_from_path(pathname);

	// If the file is not found, return -ENOENT.
	if (file == NULL)
		return -ENOENT;

	int indexno = (file->blocks)-1;

	// If the file is empty, we simply write the data to the first block of the file.
	if (file->size == 0)
	{
		// Copy the data to the first block of the file.
		strcpy(&spblock.datablocks[block_size * ((file->datablocks)[0])], buf);

		// Update the size of the file.
		file->size = strlen(buf);

		// Increment the number of blocks in the file.
		(file->blocks)++;
	}
	else
	{
		// Calculate the index of the current block in the file.
		int currblk = (file->blocks) - 1;

		// Calculate the amount of free space in the current block.
		int len1 = 1024 - (file->size % 1024);

		// If there is enough free space in the current block to write the data,
		// we simply append the data to the current block.
		if (len1 >= strlen(buf))
		{
			// Append the data to the current block.
			strcat(&spblock.datablocks[block_size * ((file->datablocks)[currblk])], buf);

			// Update the size of the file.
			file->size += strlen(buf);

			// Print the contents of the current block.
			printf("---> %s\n", &spblock.datablocks[block_size * ((file->datablocks)[currblk])]);
		}
		else
		{
			// If there is not enough free space in the current block to write the data,
			// we need to split the data across two blocks.

			// Allocate memory for a copy of the data.
			char *cpystr = malloc(1024 * sizeof(char));

			// Copy the first part of the data to the copy.
			strncpy(cpystr, buf, len1 - 1);

			// Append the first part of the data to the current block.
			strcat(&spblock.datablocks[block_size * ((file->datablocks)[currblk])], cpystr);
			strcpy(cpystr, buf);
			strcpy(&spblock.datablocks[block_size * ((file->datablocks)[currblk + 1])], (cpystr + len1 - 1));
			file->size += strlen(buf);
			printf("---> %s\n", &spblock.datablocks[block_size * ((file->datablocks)[currblk])]);
			(file->blocks)++;
		}
	}
	save_contents();

	return strlen(buf);
}
