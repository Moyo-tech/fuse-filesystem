#include "dummy.h"

int do_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{
	// Print a message indicating that we are creating a file
	printf("CREATING FILE\n");

	// Find an unused inode
	int index = find_free_inode();

	// Allocate memory for a filetype struct to represent the new file
	filetype *new_file = malloc(sizeof(filetype));

	// Copy the path to a new string and find the last occurrence of '/' in the path
	char *pathname = malloc(strlen(path) + 2);
	strcpy(pathname, path);
	char *last_slash = strrchr(pathname, '/');

	// Set the name of the new file to the part of the path after the last '/'
	strcpy(new_file->name, last_slash + 1);

	// Set the path of the new file to the full path
	strcpy(new_file->path, pathname);

	// Set the character after the last '/' to '\0' to create a string containing the path to the parent directory
	*last_slash = '\0';

	// If the path is empty, set it to "/" to indicate the root directory
	if (strlen(pathname) == 0)
		strcpy(pathname, "/");

	// Initialize the new file's children and parent, and set the number of links to 0
	new_file->children = NULL;
	new_file->num_children = 0;
	new_file->parent = filetype_from_path(pathname);
	new_file->num_links = 0;

	// Set the validity flag to 1 to indicate that the file is valid
	new_file->valid = 1;

	// If the parent directory is not found, return an error
	if (new_file->parent == NULL)
		return -ENOENT;

	// Add the new file as a child of its parent
	add_child(new_file->parent, new_file);

	// Set the type of the new file to "file"
	strcpy(new_file->type, "file");

	// Set the timestamps for the new file
	time_t now = time(NULL);
	new_file->c_time = now;
	new_file->a_time = now;
	new_file->m_time = now;
	new_file->b_time = now;

	// Set the permissions of the new file
	new_file->permissions = S_IFREG | 0777;

	// Set the size, group ID, and user ID of the new file
	new_file->size = 0;
	new_file->group_id = getgid();
	new_file->user_id = getuid();

	// Set the inode number for the new file
	new_file->number = index;

	// Find 16 free data blocks and assign them to the new file
	for (int i = 0; i < 16; i++)
	{
		(new_file->datablocks)[i] = find_free_db();
	}

	// new_file -> size = 0;
	new_file->blocks = 0;

	save_contents();

	return 0;
}