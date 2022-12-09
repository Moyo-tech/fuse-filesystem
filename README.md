# A BASIC FILESYSTEM BUILT USING THE FUSE LIBARY
A filesystem written in C using the FUSE libary

## How to run the program

### **Installing dependencies**
 After downloading the zip file and extracting it, you would need to install the FUSE libary and make for the Makefile in order to run the system. **Note: This filesytem was designed on a Ubuntu system and was not tested on any other linux system.**To install the FUSE Libary make and run the following command: 

    ```
    sudo apt-get install gcc fuse libfuse-dev make cmake 
    ```
### Setting up the filesystem
- **Creating a Mountpoint DIrectory**: In order to run the filesystem you would need to create a mountpoint, the mountpoint for the program was in /tmp/example. Follow this steps to achieve that:
  - Navigate to your ```computer``` directory (you have your tmp, usr, home, lib and other files in this directory)
  - Open the ```tmp``` folder
  - Create a folder and save it with any name you want your mountpoint to be called in our case it was called ```example```
  
- **Compile and Mount**: 
  - Go back to the directory where you extracted the project
  - run ```make``` on the terminal this would automatically compile the project
  - If you get an error with `pk-config` that means you need to have it installed.
  - After that run this command ```./main -f /tmp/example``` this is basically mounting to the path you want ```-f``` is to print all the statements in the program
  - If you want to mount to a different path then ensure you have created that diretory and perform the commad ```./main -f /path/to/mounto```
  
 - **Checking if it is mounted**
    - Go to the parent directory of the mount and type the command ```$ mount | column -t```you should see the following: 
    - Go to the mounted directory and type in the following ```ls -la``` you should see something like this meaning your device is mounted. 
  
 ## Performing operations
  After you should be able to perform the operations. The filesystem works with linux commands and like every other filesystem. It can perform the following operations: 
 
  - Making New Directories : ```mkdir [directory Name]```
      
  - Navigate to DIrectories: ```cd [directory Name]```
      
  - Creating Files and Writing to them : ```cat > [filetocreate.txt]```
      
  - Opening and Reading Files: ```cat [filename]```
      
  - Renaming Files: ```mv [filename.txt] [newfilename.txt]```
      
  - Removing Files ```rm [filename.txt]```
      
  - Removing Directories: ```rmdir [Directory name] ```
      
  - Listing DIrectories or files ```ls```
## Error Codes and Notes
  You would need to have the terminal of the project running first in order for the terminal of the mounted filesystem directory to run, else this can result in a transport endpoint error. 
  ```ls: cannot open directory '.': Transport endpoint is not connected```
  