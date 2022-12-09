#include "dummy.h"

typedef struct superblock
{
	char datablocks[block_size * 100]; // represents total number of data blocks
	char data_bitmap[105];			   // represents array of data block numbers that are available
	char inode_bitmap[105];			   // represents array of inode numbers that are available
} superblock;

// Inode are special disk blocks they are created when the file system is created.
typedef struct inode
{
	int datablocks[16]; // represents data block number that the inode points to
	int number;			// represents the inode number
	int blocks;			// represents number of blocks to which a specific inode points
	int size;			// represents the size of the file/directory associated with the inode
} inode;

typedef struct filetype
{
	int valid;
	char test[10];
	char path[100];
	char name[100]; // name
	inode *inum;	// inode number
	struct filetype **children;
	int num_children;
	int num_links;
	struct filetype *parent;
	char type[20];		//==file extension
	mode_t permissions; // Permissions
	uid_t user_id;		// userid
	gid_t group_id;		// groupid
	time_t a_time;		// Access time
	time_t m_time;		// Modified time
	time_t c_time;		// Status change time
	time_t b_time;		// Creation time
	off_t size;			// Size of the node

	int datablocks[16];
	int number;
	int blocks;

} filetype;

superblock spblock;

void initialize_superblock()
{

	memset(spblock.data_bitmap, '0', 100 * sizeof(char));
	memset(spblock.inode_bitmap, '0', 100 * sizeof(char));
}

filetype *root;

filetype file_array[50];

// This function converts a tree to an array by performing a breadth-first traversal of the tree.
// It takes as input a queue for storing the nodes as they are visited,
// pointers to the front and rear of the queue, and a pointer to the current index in the array.
void tree_to_array(filetype *queue, int *front_index, int *rear_index, int *index)
{
	// Return if the queue is empty or the end of the array has been reached.
	if (rear_index < front_index || *index > 30)
		return;

	// Get the current node from the queue and store it in a local variable.
	filetype curr_node = queue[*front_index];

	// Increment the front of the queue.
	*front_index += 1;

	// Add the current node to the array.
	file_array[*index] = curr_node;

	// Increment the index.
	*index += 1;

	// If the index is less than 6, we add the children of the current node to the queue.
	// This is necessary because we only want to add the first 6 children of the node to the array.
	if (*index < 6)
	{
		// If the current node is valid (i.e., it is not a null node), we add its children to the queue.
		if (curr_node.valid)
		{
			// Loop through all of the children of the current node.
			int i;
			for (i = 0; i < curr_node.num_children; i++)
			{
				// If the rear of the queue is less than the front, it means the queue is empty,
				// so we set the rear to the front to make sure it is not less than the front.
				if (*rear_index < *front_index)
					*rear_index = *front_index;

				// Add the child to the queue.
				queue[*rear_index] = *(curr_node.children[i]);

				// Increment the rear of the queue.
				*rear_index += 1;
			}

			// If the number of children added to the queue is less than 5, we add null nodes to the queue
			// until we reach a total of 5 children.
			// This is necessary because we only want to add the first 5 children of the node to the array.
			while (i < 5)
			{
				// Create a null node.
				filetype waste_node;
				waste_node.valid = 0;

				// Add the null node to the queue.
				queue[*rear_index] = waste_node;

				// Increment the rear of the queue.
				*rear_index += 1;

				// Increment the number of children added to the queue.
				i++;
			}
		}
		// If the current node is not valid, we simply add null nodes to the queue until we reach a total of 5 children.
		else
		{
			// Add waste nodes to the queue if the current node is invalid
			int i = 0;
			while (i < 5)
			{
				filetype waste_node;
				waste_node.valid = 0;
				queue[*rear_index] = waste_node;
				*rear_index += 1;
				i++;
			}
		}
	}

	// Recursively call the function to process the next node in the queue
	tree_to_array(queue, front_index, rear_index, index);
}

// Utility function for saving the contents of a tree-like data structure to a binary file.
int save_contents()
{
	printf("SAVING\n");
	// Allocate memory for a queue of filetype structures. The queue has a
	// maximum capacity of 60 elements.
	filetype *queue = malloc(sizeof(filetype) * 60);

	// Initialize the front and rear indices of the queue to 0.
	int front_index = 0;
	int rear_index = 0;

	// Add the root node to the queue.
	queue[0] = *root;

	// Initialize the current index to 0.
	int current_index = 0;

	// Populate the queue with the nodes of the filesystem tree by calling
	// the tree_to_array() function.
	tree_to_array(queue, &front_index, &rear_index, &current_index);

	// Print the "valid" field of each element in the file_array.
	for (int i = 0; i < 31; i++)
	{
		printf("%d", file_array[i].valid);
	}

	// Open the "file_structure.bin" and "super.bin" file in binary write mode.
	FILE *fd = fopen("file_structure.bin", "wb");
	FILE *fd1 = fopen("super.bin", "wb");

	// Write the contents of the file_array to the "file_structure.bin"  and "super.bin"file.
	fwrite(file_array, sizeof(filetype) * 31, 1, fd);
	fwrite(&spblock, sizeof(superblock), 1, fd1);

	// Close the "file_structure.bin" and "super.bin" file.
	fclose(fd);
	fclose(fd1);
	printf("\n");
}

// Initializes the root directory in a file system and assigns it various properties.
void initialize_root_directory()
{

	spblock.inode_bitmap[1] = 1; // marking it with 0
	root = (filetype *)malloc(sizeof(filetype));

	strcpy(root->path, "/");
	strcpy(root->name, "/");

	root->children = NULL;
	root->num_children = 0;
	root->parent = NULL;
	root->num_links = 2;
	root->valid = 1;
	strcpy(root->test, "test");
	strcpy(root->type, "directory");

	root->c_time = time(NULL);
	root->a_time = time(NULL);
	root->m_time = time(NULL);
	root->b_time = time(NULL);

	root->permissions = S_IFDIR | 0777;

	root->size = 0;
	root->group_id = getgid();
	root->user_id = getuid();

	root->number = 2;
	root->blocks = 0;

	save_contents();
}

filetype *filetype_from_path(char *path)
{
	/* implements a function that traverses a file system starting from the
	 root directory and following a given path to find a specific file or directory.*/

	// Allocate memory for a copy of the input path
	char curr_folder[100];

	char *path_name = malloc(strlen(path) + 2);
	strcpy(path_name, path);

	// Start from the root directory
	filetype *curr_node = root;

	// Check if the input path is the root directory
	if (strcmp(path_name, "/") == 0)
	{
		return curr_node;
	}

	// Check if the input path is an absolute path
	if (path_name[0] != '/')
	{
		printf("INCORRECT PATH\n");
		exit(1);
	}
	else
	{
		// Remove the leading "/" character
		path_name++;
	}

	// Check if the input path ends with a "/" character
	if (path_name[strlen(path_name) - 1] == '/')
	{
		// Remove the trailing "/" character
		path_name[strlen(path_name) - 1] = '\0';
	}

	// Loop until the input path is empty
	while (strlen(path_name) != 0)
	{
		// Find the next "/" character in the path
		char *index = strchr(path_name, '/');

		if (index != NULL)
		{
			// Extract the name of the next folder in the path
			strncpy(curr_folder, path_name, index - path_name);
			curr_folder[index - path_name] = '\0';

			// Search the current directory for a child with the correct name
			int flag = 0;
			for (int i = 0; i < curr_node->num_children; i++)
			{
				if (strcmp((curr_node->children)[i]->name, curr_folder) == 0)
				{
					curr_node = (curr_node->children)[i];
					flag = 1;
					break;
				}
			}
			if (flag == 0)
			{
				return NULL;
			}
		}
		else
		{
			// If the input path does not contain any more "/" characters,
			// it must be the final folder in the path
			strcpy(curr_folder, path_name);

			// Search the current directory for a child with the correct name
			int flag = 0;
			for (int i = 0; i < curr_node->num_children; i++)
			{
				if (strcmp((curr_node->children)[i]->name, curr_folder) == 0)
				{
					curr_node = (curr_node->children)[i];
					return curr_node;
				}
			}
			return NULL;
		}

		// Move to the next folder in the path
		path_name = index + 1;
	}
}

// Find and allocate a free inode in the file system
int find_free_inode()
{

	// Loop through all inode numbers in the file system
	for (int i = 2; i < 100; i++)
	{
		// Check if the inode is free
		if (spblock.inode_bitmap[i] == '0')
		{
			// Mark the inode as in use
			spblock.inode_bitmap[i] = '1';
		}
		// Return the inode number
		return i;
	}

	// If no free inodes were found, return an error code
	return -1;
}

// Find and allocate a free data block in the file system
int find_free_db()
{
	// Loop through all data blocks in the file system
	for (int i = 1; i < 100; i++)
	{
		// Check if the data block is free
		if (spblock.inode_bitmap[i] == '0')
		{
			// Mark the data block as in use
			spblock.inode_bitmap[i] = '1';
		}
		// Return the data block number
		return i;
	}

	// If no free data blocks were found, return an error code
	return -1;
}

// Add a child file or directory to a parent file or directory
void add_child(filetype *parent, filetype *child)
{
	// Increment the number of children in the parent
	parent->num_children++;

	// Reallocate memory for the children array to make room for the new child
	parent->children = realloc(parent->children, parent->num_children * sizeof(filetype *));

	// Add the child to the end of the children array
	parent->children[parent->num_children - 1] = child;
}

// Get the list of files and directories in a given directory
int myreaddir(const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
	printf("[READDIR]: Getting the list of files of \n");

	// Add the current and parent directories to the list
	filler(buffer, ".", NULL, 0);
	filler(buffer, "..", NULL, 0);

	// Get the filetype structure for the directory
	char *pathname = malloc(strlen(path) + 2);
	strcpy(pathname, path);

	filetype *dir_node = filetype_from_path(pathname);

	if (dir_node == NULL) // If the directory doesn't exist
	{
		return -ENOENT;
	}
	else
	{
		// Update the access time of the directory
		dir_node->a_time = time(NULL);

		// Loop through all the children of the directory
		for (int i = 0; i < dir_node->num_children; i++)
		{
			// Print the file or directory name
			printf(":%s:\n", dir_node->children[i]->name);

			// Add the file or directory to the list
			filler(buffer, dir_node->children[i]->name, NULL, 0);
		}
	}

	return 0;
}

// Get the attributes of a file or directory
static int mygetattr(const char *path, struct stat *st)
{
	// Get the filetype structure for the file or directory
	char *pathname = malloc(strlen(path) + 2);
	strcpy(pathname, path);

	printf("[GETATTR] Attributes of %s requested\n", pathname);

	filetype *file_node = filetype_from_path(pathname);

	if (file_node == NULL) // If the file or directory doesn't exist
	{
		return -ENOENT;
	}

	// Set the attributes of the file or directory
	st->st_uid = file_node->user_id;
	st->st_gid = file_node->group_id;
	st->st_atime = file_node->a_time;
	st->st_mtime = file_node->m_time;
	st->st_ctime = file_node->c_time;
	st->st_mode = file_node->permissions;
	st->st_nlink = file_node->num_links + file_node->num_children;
	st->st_size = file_node->size;
	st->st_blocks = file_node->blocks;

	return 0;
}
