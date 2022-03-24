#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<iostream>
#include<io.h>
#include "Datastructures.h"

void man(char *name)
{
    if (name == NULL)
    {
        return;
    }
    if (strcmp(name, "create") == 0)
    {
        printf("Description: Used to create new regular file\n");
        printf("Usage: Create File_name Permission\n");
    }
    else if (strcmp(name, "read") == 0) 
    {
        printf("Description: Used to read in to regular file \n");
        printf("Usage: read File_name No_Of_Bytes_To_Read\n");
    }
    else if (strcmp(name,"write") == 0)
    {
        printf("Descripton: Used to write in to regular file\n");
        printf("Usage: write File_name\n After this enter the data that we want to write\n");
    }
    else if (strcmp(name,"ls") == 0)
    {
        printf("Descripton: Used to list all informatiion of  files\n");
        printf("Usage:ls\n");
    }
    else if (strcmp(name,"stat") == 0)
    {
        printf("Descripton: Used to display informatiion of  file\n");
        printf("Usage:stat File_name\n");
    }
    else if (strcmp(name,"fstat") == 0)
    {
        printf("Descripton: Used to display informatiion of  file\n");
        printf("Usage: stat File_Descriptor\n");
    }
    else if (strcmp(name,"truncat") == 0)
    {
        printf("Descripton: Used to remove data from file\n");
        printf("Usage:truncate File_name\n");
    }
    else if (strcmp(name,"open") == 0)
    {
        printf("Descripton: Used to open existing  file\n");
        printf("Usage:stat open File_name mode\n");
    }
    else if (strcmp(name,"close") == 0)
    {
        printf("Descripton: Used to close opened  file\n");
        printf("Usage:close File_name\n");
    }
    else if (strcmp(name,"closeall") == 0)
    {
        printf("Descripton: Used to close all opened  file\n");
        printf("Usage: closeall\n");
    }    
    else if (strcmp(name,"lseek") == 0)
    {
        printf("Descripton: Used to change file offset\n");
        printf("Usage: lseek File_Name ChangeInOffset StartPoint\n");
    }
    else if (strcmp(name,"rm") == 0)
    {
        printf("Descripton: Used to delete the file\n");
        printf("Usage:rm File_Name\n");
    }
    else
    {
        printf("ERROR : No manual entry available. \n");
    }    
}


void DisplayHelp()
{
    printf("ls: To List out all files\n");
    printf("clear: To clear console\n");
    printf("open: To open the file\n");
    printf("close: To close the file\n");
    printf("closeall: To close all opened files\n");
    printf("read: To Read the contents from file\n");
    printf("write: To Write contents in to file\n");
    printf("exit: To Terminate file system\n");
    printf("stat: To Display information of file using name\n");
    printf("fstat: To Display information of file using file descriptor\n");
    printf("rm: To Delete the file\n");
}

int GetFDFromName(char *name)
{
    int i = 0;

    while (i < MAXINODE)
    {
        if (UFDTArr[i].ptrfiletable != NULL)
        {
            if (strcmp((UFDTArr[i].ptrfiletable->ptrinode->FileName),name) == 0)
            {
                break;
            }
            i++;
        }
    }
    
    if (i == MAXINODE)
    {
        return -1;
    }
    else
    {
        return i;
    }
}


PINODE Get_Inode(char *name)
{
    PINODE temp = head;
    int i = 0;

    if (name == NULL)
    {
        return NULL;
    }
    
    while (temp != NULL)
    {
        if(strcmp(name,temp->FileName) == 0)
        {
            break;
        }
        temp = temp->next;
    }
    return temp;
}

void CreateDILB()
{
    int i = 1;
    PINODE newn = NULL;
    PINODE temp = head;

    while (i < MAXINODE)
    {
        newn = (PINODE)malloc(sizeof(INODE));

        newn->LinkCount = 0;
        newn->RefernceCount = 0;
        newn->FileType = 0;
        newn->FileSize = 0;

        newn->Buffer = NULL;
        newn->next = NULL;

        newn->InodeNumber = i;

        if (temp == NULL)
        {
            head = newn;
            temp = head;
        }
        else
        {
            temp->next = newn;
            temp = temp->next;
        }        
        i++;
    }
    printf("DILB created sucessfully\n");
}

void InitialiseSuperBlock()
{
    int i = 0;
    while (i < MAXINODE)
    {
        UFDTArr[i].ptrfiletable = NULL;
        i++;
    }

    SUPERBLOCKobj.TotalInodes = MAXFILESIZE;
    SUPERBLOCKobj.FreeInode = MAXINODE;
}

int CreateFile(char *name, int permission)
{
    int i = 0;
    PINODE temp = head;

    if ((name == NULL) || (permission == 0) || (permission > 3))
    {
        return -1;
    }
    
    if (SUPERBLOCKobj.FreeInode == 0)
    {
        return -2;
    }
    (SUPERBLOCKobj.FreeInode)--;

    if (Get_Inode(name) != NULL)
    {
        (SUPERBLOCKobj.FreeInode)++;
        return -3;
    }
    while (temp != NULL)
    {
        if(temp->FileType == 0)
        {
            break;
        }
        temp = temp->next;
    }
    while (i < MAXINODE)
    {
        if(UFDTArr[i].ptrfiletable == NULL)
        {
            break;
        }
        i++;
    }
    UFDTArr[i].ptrfiletable = (PFILETABLE)malloc(sizeof(FILETABLE));

    UFDTArr[i].ptrfiletable->count = 1;
    UFDTArr[i].ptrfiletable->mode = permission;
    UFDTArr[i].ptrfiletable->readoffset = 0;
    UFDTArr[i].ptrfiletable->writeoffset = 0;

    UFDTArr[i].ptrfiletable->ptrinode = temp;

    strcpy(UFDTArr[i].ptrfiletable->ptrinode->FileName,name);
    UFDTArr[i].ptrfiletable->ptrinode->FileType = REGULAR;
    UFDTArr[i].ptrfiletable->ptrinode->RefernceCount = 1;
    UFDTArr[i].ptrfiletable->ptrinode->LinkCount = 1;
    UFDTArr[i].ptrfiletable->ptrinode->FileSize = MAXFILESIZE;
    UFDTArr[i].ptrfiletable->ptrinode->FileActualSize = 0;
    UFDTArr[i].ptrfiletable->ptrinode->permission = permission;
    UFDTArr[i].ptrfiletable->ptrinode->Buffer = (char*)malloc(MAXFILESIZE);

    return i;
}

//rm_File("Demp.txt")
int rm_File(char* name)
{
    int fd = 0;

    fd  = GetFDFromName(name);
    if (fd == -1)
    {
        return -1;
    }
    (UFDTArr[fd].ptrfiletable->ptrinode->LinkCount)--;

    if (UFDTArr[fd].ptrfiletable->ptrinode->LinkCount == 0)
    {
        UFDTArr[fd].ptrfiletable->ptrinode->Buffer = 0;
        //free(UFDTArr[fd].ptrfiletable);
        free( UFDTArr[fd].ptrfiletable);
    }
        UFDTArr[fd].ptrfiletable = NULL;
        (SUPERBLOCKobj.FreeInode)++;
}

int ReadFile(int fd, char *arr, int isize)
{
    int read_size = 0;

    if (UFDTArr[fd].ptrfiletable == NULL)
    {
        return -1;
    }
    if (UFDTArr[fd].ptrfiletable->mode != READ && UFDTArr[fd].ptrfiletable->mode != READ+WRITE)
    {
        return -2;
    }
    if (UFDTArr[fd].ptrfiletable->ptrinode->permission != READ && UFDTArr[fd].ptrfiletable->ptrinode->permission != READ+WRITE )
    {
        return -2;
    }
    if (UFDTArr[fd].ptrfiletable->readoffset == UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)
    {
        return -3;
    }
    if (UFDTArr[fd].ptrfiletable->ptrinode->FileType != REGULAR)
    {
        return -4;
    }

    read_size = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) - (UFDTArr[fd].ptrfiletable->readoffset);

    if (read_size < isize)
    {
        strncpy(arr,(UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->readoffset),read_size);
        UFDTArr[fd].ptrfiletable->readoffset = UFDTArr[fd].ptrfiletable->readoffset + read_size;
    }
    else
    {
        strncpy(arr,(UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->readoffset),isize);

            (UFDTArr[fd].ptrfiletable->readoffset) = (UFDTArr[fd].ptrfiletable->readoffset) + isize;
    }
    
    return isize;    
}

int WriteFile(int fd, char *arr, int isize)
{
    if (((UFDTArr[fd].ptrfiletable->mode) != WRITE) && ((UFDTArr[fd].ptrfiletable->mode) != READ+WRITE))
    {
        return -1;
    }
    if ((UFDTArr[fd].ptrfiletable->ptrinode->permission != WRITE) && ((UFDTArr[fd].ptrfiletable->ptrinode->permission) != READ+WRITE))
    {
        return -1;
    }
    if ((UFDTArr[fd].ptrfiletable -> writeoffset) == MAXFILESIZE)
    {
        return -2;
    }
    if ((UFDTArr[fd].ptrfiletable->ptrinode->FileType) !=  REGULAR)
    {
        return -3;
    }
    
    strncpy((UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->writeoffset),arr,isize);

    (UFDTArr[fd].ptrfiletable->writeoffset) = (UFDTArr[fd].ptrfiletable->writeoffset)+ isize;

    ((UFDTArr[fd].ptrfiletable -> ptrinode -> FileActualSize) = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)) + isize;

    return isize;
}

int OpenFile(char *name, int mode)
{
        int i = 0;
        PINODE temp = NULL;

        if (name == NULL || mode <= 0)
        {
            return -1;
        }
        temp = Get_Inode(name);
        if (temp == NULL)
        {
            return -2;
        }
        if (temp->permission < mode)
        {
            return -3;
        }

        while (i < MAXINODE)
        {
            if (UFDTArr[i].ptrfiletable == NULL)
            {
                break;
            }
            i++;
        }

        UFDTArr[i].ptrfiletable = (PFILETABLE)malloc(sizeof(FILETABLE));
        if (UFDTArr[i].ptrfiletable == NULL)
        {
            return -1;
        }
        UFDTArr[i].ptrfiletable->count =1;
        UFDTArr[i].ptrfiletable->mode = mode;
        if (mode == READ + WRITE)
        {
            UFDTArr[i].ptrfiletable->readoffset = 0;
            UFDTArr[i].ptrfiletable->writeoffset = 0;
        }
        else if (mode == READ)
        {
            UFDTArr[i].ptrfiletable->readoffset = 0;
        }
        else if (mode == WRITE)
        {
            UFDTArr[i].ptrfiletable->writeoffset = 0;
        }
        UFDTArr[i].ptrfiletable->ptrinode = temp;
        (UFDTArr[i].ptrfiletable->ptrinode->RefernceCount)++;

        return i;    
}

int CloseFileByName(int fd)
{
    UFDTArr[fd].ptrfiletable->readoffset = 0;
    UFDTArr[fd].ptrfiletable->writeoffset = 0;
    (UFDTArr[fd].ptrfiletable->ptrinode->RefernceCount)--;
}

int CloseFileByName(char *name)
{
    int i = 0;
    i = GetFDFromName(name);
    
    if (i == -1)
    {
        return -1;
    }
    UFDTArr[i].ptrfiletable->readoffset = 0;
    UFDTArr[i].ptrfiletable->writeoffset = 0;
    (UFDTArr[i].ptrfiletable->ptrinode->RefernceCount)--;

    return 0;
}

void CloseAllFile()
{
    int i = 0;
    while (i<MAXINODE)
    {
        if (UFDTArr[i].ptrfiletable != NULL)
        {
            UFDTArr[i].ptrfiletable->readoffset = 0;
            UFDTArr[i].ptrfiletable->writeoffset = 0;
            (UFDTArr[i].ptrfiletable->ptrinode->RefernceCount)--;
            break;
        }
        i++;
    }
}

int LseekFile(int fd, int size, int from)
{
    if ((fd < 0) || (from > 2))
    {
        return -1;
    }
    if (UFDTArr[fd].ptrfiletable == NULL)
    {
        return -1;
    }
    if ((UFDTArr[fd].ptrfiletable->mode == READ) || (UFDTArr[fd].ptrfiletable->mode == READ + WRITE))
    {
        if (from == CURRENT)
        {
            if (((UFDTArr[fd].ptrfiletable->readoffset)+size) > UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)
            {
                return -1;
            }
            if (((UFDTArr[fd].ptrfiletable->readoffset)+size) < 0)
            {
                return -1;
            }
            (UFDTArr[fd].ptrfiletable->readoffset) = (UFDTArr[fd].ptrfiletable->readoffset)+size;
        }
        else if (from == START)
        {
            if (size > (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize))
            {
                return -1;
            }
            if (size < 0)
            {
                return -1;
            }
            (UFDTArr[fd].ptrfiletable->readoffset) = size;
        }
        else if (from == END)
        {
            if (((UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)+ size) < 0)
            {
                return -1;
            }
            if (((UFDTArr[fd].ptrfiletable->readoffset)+size)< 0)
            {
                return -1;
            }
            (UFDTArr[fd].ptrfiletable->readoffset) = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + size;
        }
    }
    else if (UFDTArr[fd].ptrfiletable->mode == WRITE)
    {
        if (from == CURRENT)
        {
            if (((UFDTArr[fd].ptrfiletable->writeoffset) + size) > MAXFILESIZE)
            {
                return -1;
            }
            if (((UFDTArr[fd].ptrfiletable->writeoffset) + size) < 0)
            {
                return -1;
            }
            if (((UFDTArr[fd].ptrfiletable->writeoffset) + size) > (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize))
            {
                (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) = (UFDTArr[fd].ptrfiletable->writeoffset) + size;
                (UFDTArr[fd].ptrfiletable->writeoffset) = (UFDTArr[fd].ptrfiletable->writeoffset) + size;
            }
            else if (from == START)
            {
                if (size > MAXFILESIZE)
                {
                    return -1;
                }
                if (size < 0)
                {
                    return -1;
                }
                if (size > (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize))
                {
                    (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) = size;
                }
                (UFDTArr[fd].ptrfiletable->writeoffset) = size;
            }
            else if (from == END)
            {
                if ((UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)+ size > MAXFILESIZE)
                {
                    return -1;
                }
                if (((UFDTArr[fd].ptrfiletable->writeoffset)+size)< 0)
                {
                    return -1;
                }
                (UFDTArr[fd].ptrfiletable->writeoffset) = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)+size;
            }
        }
        
    }
    
}

void ls_file()
{
    int i = 0;
    PINODE temp = head;

    if (SUPERBLOCKobj.FreeInode == MAXINODE)
    {
        printf("Error: There are no files\n");
        return;
    }
    printf("\nFile Name\tInode number\tFile size\tLink Count\n");    
    printf("-------------------------------------------------------\n");
    while (temp != NULL)
    {
        if(temp ->FileType != 0)
        {
            printf("%s\t\t%d\t\t%d\t\t%d\n",temp->FileName,temp->InodeNumber,temp->FileActualSize,temp->LinkCount);
        }
        temp = temp->next;
    }
    printf("-------------------------------------------------------------\n");
}

int fstat_file(int fd)
{
    PINODE temp = head;
    int i = 0;

    if (fd < 0)
    {
        return -1;
    }
    if (UFDTArr[fd].ptrfiletable->ptrinode == NULL)
    {
        return -2;
    }
    temp = UFDTArr[fd].ptrfiletable->ptrinode;

    printf("\n--------- Statistical Information about file---------\n");
    printf("File name: %s\n",temp->FileName);
    printf("Inode Number %d\n",temp->InodeNumber);
    printf("File size: %d\n",temp->FileSize);
    printf("Actual File size: %d\n",temp->FileActualSize);
    printf("Link Count: %d\n",temp->LinkCount);
    printf("Reference count: %d\n",temp->RefernceCount);

        if (temp->permission ==1)
    {
        printf("File Permission : Read only\n");
    }
    else if (temp->permission == 2)
    {
        printf("File Permission: Write\n");
    }
    else if (temp->permission == 3)
    {
        printf("File permission: Read & Write\n");
    }
    printf("-----------------------------------------------------\n\n");

    return 0;
}

int stat_file(char *name)
{
    PINODE temp = head;
    int i = 0;

    if (name == NULL)
    {
        return -1;
    }
    while (temp != NULL)
    {
        if(strcmp(name,temp->FileName) == 0);
        {
            break;
        }
        temp = temp->next;
    }
    if (temp == NULL)
    {
        return -2;
    }
    printf("\n---------Statistical Information about file---------\n");
    printf("File name: %s\n",temp->FileName);
    printf("Inode Number %d\n",temp->InodeNumber);
    printf("File size: %d\n",temp->FileSize);
    printf("Actual File size: %d\n",temp->FileActualSize);
    printf("Link Count: %d\n",temp->LinkCount);
    printf("Reference count: %d\n",temp->RefernceCount);

    if (temp->permission == 1)
    {
        printf("File Permission: Read only\n");
    }
    else if (temp->permission == 2)
    {
        printf("File Permission: Write\n");
    }
    else if (temp->permission == 3)
    {
        printf("File permission: Read & Write\n");
    }
    printf("-----------------------------------------------------\n\n");


    return 0;
}

int truncate_File(char *name)
{
    int fd = GetFDFromName(name);
    if(fd == -1)
    {
        return -1;
    }
    memset(UFDTArr[fd].ptrfiletable->ptrinode->Buffer,0,1024);
    UFDTArr[fd].ptrfiletable->readoffset = 0;
    UFDTArr[fd].ptrfiletable->writeoffset = 0;
    UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize = 0;
}
