#include "dummy.h"

static int do_mkdir(const char *path, mode_t mode)
{
	/* funtions that creates a new file at the specified path with the specified mode.*/

    // Print a message indicating that we are creating a new folder
    printf("CREATING NEW FOLDER\n");

    // Find an unused inode
    int free_inode = find_free_inode();

    // Allocate memory for a filetype struct to represent the new folder
    filetype *folder = malloc(sizeof(filetype));

    // Copy the path to a new string and find the last occurrence of '/' in the path
    char *pathname = malloc(strlen(path) + 2);
    strcpy(pathname, path);
    char *last_slash = strrchr(pathname, '/');

    // Set the name of the new folder to the part of the path after the last '/'
    strcpy(folder->name, last_slash + 1);

    // Set the path of the new folder to the full path
    strcpy(folder->path, pathname);

    // Set the character after the last '/' to '\0' to create a string containing the path to the parent folder
    *last_slash = '\0';

    // If the path is empty, set it to "/" to indicate the root folder
    if (strlen(pathname) == 0)
        strcpy(pathname, "/");

    // Initialize the new folder's children and parent, and set the number of links to 2
    folder->children = NULL;
    folder->num_children = 0;
    folder->parent = filetype_from_path(pathname);
    folder->num_links = 2;

    // Set the validity flag to 1 to indicate that the folder is valid
    folder->valid = 1;

    // Set the test field to "test"
    strcpy(folder->test, "test");

    // If the parent folder is not found, return an error
    if (folder->parent == NULL)
        return -ENOENT;

    // Add the new folder as a child of its parent
    add_child(folder->parent, folder);

    // Set the type of the new folder to "directory"
    strcpy(folder->type, "directory");

    // Set the timestamps for the new folder
    folder->c_time = time(NULL);
    folder->a_time = time(NULL);
    folder->m_time = time(NULL);
    folder->b_time = time(NULL);

    // Set the permissions of the new folder
    folder->permissions = S_IFDIR | 0777;

    // Set the size, group ID, and user ID of the new folder
    folder->size = 0;
    folder->group_id = getgid();
    folder->user_id = getuid();

    // Set the inode number and number of blocks for the new folder
    folder->number = free_inode;
    folder->blocks = 0;

    // Save the new folder information to persistent storage
    save_contents();

    // Return 0 to indicate success
    return 0;
}
