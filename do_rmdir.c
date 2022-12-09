#include "dummy.h"

// The function do_rmdir removes the directory specified by path
int do_rmdir(const char *path)
{
    printf("REMOVING DIRECTORY \n");

    // The pathname variable is created by copying the path argument and appending a slash at the end
    char *pathname = malloc(strlen(path) + 2);
    strcpy(pathname, path);

    // The rindex variable is assigned the result of a call to strrchr, which finds the last occurrence of a slash in pathname
    char *rindex = strrchr(pathname, '/');

    // The folder_delete variable is created by copying the part of pathname after the last slash and appending a slash at the end
    char *folder_delete = malloc(strlen(rindex + 1) + 2);
    strcpy(folder_delete, rindex + 1);

    // The code sets the character at the position of the last slash in pathname to a null terminator, effectively splitting the string into the parent path and the target directory name
    *rindex = '\0';

    // If the resulting parent path is empty, it is set to the root directory
    if (strlen(pathname) == 0)
        strcpy(pathname, "/");

    // The parent variable is assigned the result of calling filetype_from_path with the parent path
    filetype *parent = filetype_from_path(pathname);

    // If the parent variable is NULL, the function returns -ENOENT
    if (parent == NULL)
        return -ENOENT;

    // If the parent variable has no children, the function returns -ENOENT
    if (parent->num_children == 0)
        return -ENOENT;

    // The code iterates over the children of the parent variable to find the target directory
    filetype *curr_child = (parent->children)[0];
    int index = 0;
    while (index < (parent->num_children))
    {
        if (strcmp(curr_child->name, folder_delete) == 0)
        {
            break;
        }
        index++;
        curr_child = (parent->children)[index];
    }

    // If the target directory is not found, the function returns -ENOENT
    if (index < (parent->num_children))
    {
        // If the target directory has children, the function returns -ENOTEMPTY
        if (((parent->children)[index]->num_children) != 0)
            return -ENOTEMPTY;
        // If the target directory is found and has no children, it is removed from the parent's list of children
        for (int i = index + 1; i < (parent->num_children); i++)
        {
            (parent->children)[i - 1] = (parent->children)[i];
        }
        (parent->num_children) -= 1;
    }

    else
    {
        // If the target directory is not found, the function returns -ENOENT
        return -ENOENT;
    }

    // The save_contents function is called to update the file system data
    save_contents();
	return 0;
}
