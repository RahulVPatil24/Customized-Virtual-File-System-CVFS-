
# Customized Virtual File System(CVFS) 



## Description:
#### There are 5 tasks of an operating system:

- File Management
- Process Management
- Memory Management
- Hardware Abstraction
- CPU Scheduling


#### The aim of this project is to understand the actual implimentation of Filesystem in Operating Systems and how things works.

File management is task of an operating system which is use to manage every 
operation that user can perform with that file.
eg: Create the new file, open an existing file, read the data from the file,
write the data in to the file,remove the existing file.

We have implemented the above tasks by self-made system calls

#### This is an R&D based project intended to learn about the internals of file systems.This project is an operating system level project so I have used native languages (c,c++). I have implemented all necessary various system calls and commands of file subsystem Open, Close, Read, Write, Lseek, Create, RM, LS, Stat, Fstat etc for performing the operations on files.

In native programming languages such as c and c++, There are Two ways in which we can perform the file manipulation

- By using library functions

- By using system calls

The reason behind using system calls is if we designed the application which uses the 
system calls then the execution speed of thtat application is less and it is much more powerfull as compared to 
application which uses the library functions.

#### As the name suggest it is a virtual because we maintain all records in Primary storage.
#### By using this project we can get overview of UFS (UNIX File System) on any platform.


## Datastructures used in project:-

In this project we created all data structures which are required for File Subsystems
such as Inode Table, File Table, UserFileDescriptorTable, SuperBlock, Disk Inode ListBlock, DataBlock, BootBlock etc.

#### iNode:
An inode contains all information about a file, except its name(in our case we store name also)
#### Internals of an iNode:
- The kernal creates iNode for the file, if possible
- The kernal fills in the iNode with the file status
- The kernal allocates data blocks for the file and stores the file data in these blocks.
- The kernel records the addresses of the data blocks in the iNode
- The kernel creates a directory entry in the scratch directory with the iNode number and file name

#### SB:
SuperBlock contains total records of filesystem
eg: Total i-nodes, used i-nodes, Free i-nodes, TotalBlocks, FreeBlocks.

#### Dlib:
DataBlock also called a physical record, is a sequence of bytes or bits, usually
containing some whole number of records. Each block in DataBlock is of 1kb.

#### UFDT:
It contains array of pointers which points to the address of the file table.
### To run this project use below command:

```bash
  g++ CVFS.cpp -o myexe
```

- Then execute myexe file, if you're using windows type myexe or if you're using linux distro then simply type ./myexe