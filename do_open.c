#include "dummy.h"
int do_open(const char *path, struct fuse_file_info *fi)
{
	printf("OPENING FILE\n");

	// Create a copy of the path string and store it in a new variable
	char *pathname = path;

	// Retrieve the file from the given path
	filetype *file = filetype_from_path(pathname);

	// Return 0 to indicate that the file was opened successfully
	return 0;
}
