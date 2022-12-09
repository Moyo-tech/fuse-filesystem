#include "dummy.h"

int do_rename(const char *from, const char *to)
{
	// Renames a file or directory at the specified path.
	// If the file or directory does not exist, returns -ENOENT.
	// Otherwise, returns 0 on success.

	printf("RENAMING: %s\n", from);
	printf("RENAMED AS: %s\n", to);

	// Get the pathname of the file or directory to rename
	char pathname[strlen(from) + 2];
	strcpy(pathname, from);

	// Find the last slash in the pathname
	char *last_slash_index1 = strrchr(pathname, '/');

	// Get the file or directory to rename
	filetype *file = filetype_from_path(pathname);

	// Replace the last slash with a null terminator to get the path of the parent directory
	*last_slash_index1 = '\0';

	// If the file or directory does not exist, return -ENOENT
	if (file == NULL)
		return -ENOENT;

	// Get the pathname of the new name
	char pathname2[strlen(to) + 2];
	strcpy(pathname2, to);

	// Find the last slash in the new pathname
	char *last_slash_index2 = strrchr(pathname2, '/');

	// Set the file or directory's name to the new name
	strcpy(file->name, last_slash_index2 + 1);

	// Set the file or directory's path to the new path
	strcpy(file->path, to);

	// Save the filesystem
	save_contents();

	return 0;
}