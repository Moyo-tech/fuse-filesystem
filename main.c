#include "dummy.h"
#include "do_mkdir.c"
#include "do_rmdir.c"
#include "do_rm.c"
#include "do_create.c"
#include "do_open.c"
#include "do_read.c"
#include "do_rename.c"
#include "do_write.c"

static struct fuse_operations operations =
	{
		.mkdir = do_mkdir,
		.getattr = mygetattr,
		.readdir = myreaddir,
		.rmdir = do_rmdir,
		.open = do_open,
		.read = do_read,
		.write = do_write,
		.create = do_create,
		.rename = do_rename,
		.unlink = do_rm,
};

void create_filesystem(){


	// Open the file that contains the file system
	FILE *fd = fopen("file_structure.bin", "rb");
	if (fd)
	{
		printf("FILESYSTEM IS LOADING ...\n");

		// Read the file system from the file
		fread(&file_array, sizeof(filetype) * 31, 1, fd);

		// Set up the parent-child relationships in the file system
		int child_startindex = 1;
		file_array[0].parent = NULL;

		for (int i = 0; i < 6; i++)
		{
			file_array[i].num_children = 0;
			file_array[i].children = NULL;
			for (int j = child_startindex; j < child_startindex + 5; j++)
			{
				if (file_array[j].valid)
				{
					add_child(&file_array[i], &file_array[j]);
				}
			}
			child_startindex += 5;
		}

		// Set the root directory
		root = &file_array[0];

		// Read the superblock from a file
		FILE *fd1 = fopen("super.bin", "rb");
		fread(&spblock, sizeof(superblock), 1, fd1);
	}
	else
	{
		// If the file system file doesn't exist, initialize a new file system
		initialize_superblock();
		initialize_root_directory();
	}
}

int main(int argc, char *argv[])
{
	create_filesystem();
	return fuse_main(argc, argv, &operations, NULL);
}