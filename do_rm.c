#include "dummy.h"

int do_rm(const char *path)
{
	// Removes a file at the specified path.
	// If the file is a directory and is not empty, returns -ENOTEMPTY.
	// If the file or its parent directory do not exist, returns -ENOENT.
	// Otherwise, returns 0 on success.

	printf("REMOVING FILE \n");

	// Get the length of the path
	size_t path_length = strlen(path);

	// Get the pathname of the parent directory
	char pathname[path_length + 2];
	strcpy(pathname, path);

	// Find the last slash in the path
	char *last_slash_index = strrchr(pathname, '/');

	// Get the name of the file to delete
	char file_to_delete[strlen(last_slash_index + 1) + 2];
	strcpy(file_to_delete, last_slash_index + 1);

	// Replace the last slash with a null terminator to get the path of the parent directory
	*last_slash_index = '\0';

	// If the pathname is empty, set it to "/"
	if (path_length == 0)
		strcpy(pathname, "/");

	// Get the parent directory
	filetype *parent_directory = filetype_from_path(pathname);
	if (parent_directory == NULL)
		return -ENOENT;

	if (parent_directory->num_children == 0)
		return -ENOENT;

	// Find the file to delete
	filetype *current_child = NULL;
	for (int i = 0; i < parent_directory->num_children; i++)
	{
		current_child = (parent_directory->children)[i];
		if (strcmp(current_child->name, file_to_delete) == 0)
			break;
	}

	if (current_child == NULL)
		return -ENOENT;

	if (current_child->num_children != 0)
		return -ENOTEMPTY;

	// Shift the children of the parent directory to the left to fill the gap
	for (int i = i + 1; i < parent_directory->num_children; i++)
	{
		(parent_directory->children)[i - 1] = (parent_directory->children)[i];
	}

	// Decrement the number of children in the parent directory
	(parent_directory->num_children)--;

	// Save the filesystem
	save_contents();

	return 0;
}
