///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Header files inclusion
///
//////////////////////////////////////////////////////////////////////////////////////////////////
#include<stdio.h>
#include<stdlib.h>
//#include<unistd.h>
#include<string.h>

#include<iostream>
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// User Define Macros
/// 
//////////////////////////////////////////////////////////////////////////////////////////////////

#define MAXFILESIZE 100
#define MAXINODE 5

#define READ 1
#define WRITE 2
#define EXECUTE 4

#define REGULARFILE 1
#define SPECIALFILE 2

#define START 0
#define CURRENT 1
#define END 2

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Structure Name : BootBlock
/// Description    : Hold Information to boot the Operating Systems.
///
//////////////////////////////////////////////////////////////////////////////////////////////////
struct BootBlock
{
    char Information[100];

};

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Structure Name : SuperBlock
/// Description    : Hold Information about the file system.
///
//////////////////////////////////////////////////////////////////////////////////////////////////

struct SuperBlock
{
    int TotalInodes;
    int FreeInodes;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Structure Name : Inode
/// Description    : Hold Information about the file.
///
//////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct Inode
{
    char FileName[50];
    int InodeNumber;
    int FileSize;
    int ActualFileSize;
    int FileType;
    int ReferenceCount;
    int Permission;
    int LinkCount;
    char *Buffer;
    struct Inode * next;
}INODE,*PINODE,**PPINODE;

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Structure Name : FileTable
/// Description    : Hold Information about the Opened file.
///
//////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct FileTable
{
    int ReadOffset;
    int WriteOffset;
    int Count;
    int Mode;
    PINODE ptrinode;
}FILETABLE,*PFILETABLE;

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Structure Name : UAREA
/// Description    : Hold Information about the Process.
///
//////////////////////////////////////////////////////////////////////////////////////////////////

struct UAREA
{
    char ProcessName[50];

    PFILETABLE UFDT[MAXINODE];

};

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Global variables or object used in project.
///
//////////////////////////////////////////////////////////////////////////////////////////////////

SuperBlock superobj;
PINODE head = NULL;
UAREA uareaobj;
BootBlock bootobj;

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Function Name :     InitialiseUAREA
/// Description   :     It is used to initialize the content of UAREA
/// Author        :     Vinay Bharat Khot
/// Date          :     10-08-2025
///
//////////////////////////////////////////////////////////////////////////////////////////////////

void InitialiseUAREA()
{
    strcpy(uareaobj.ProcessName,"Myexe");
    int i = 0;

    while(i < MAXINODE)
    {
        uareaobj.UFDT[i] = NULL;
        i++;
    }

    cout<<"Marvellous CVFS : UAREA initialize Succesfully\n";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Function Name :     InitialiseSuperBlock
/// Description   :     It is used to initialize the superBlock
/// Author        :     Vinay Bharat Khot
/// Date          :     10-08-2025
///
//////////////////////////////////////////////////////////////////////////////////////////////////

void InitialiseSuperBlock()
{
    superobj.TotalInodes = MAXINODE;
    superobj.FreeInodes = MAXINODE;

    cout<<"Marvellous CVFS : superblock initialize Succesfully\n";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Function Name :     CreateDILB
/// Description   :     It is used to create Linked List of  Inode
/// Author        :     Vinay Bharat Khot
/// Date          :     10-08-2025
///
//////////////////////////////////////////////////////////////////////////////////////////////////

void CreateDILB()
{
    int i = 1;
    PINODE newn = NULL;
    PINODE temp = head;

    while(i <= MAXINODE)
    {
        newn = new INODE;
        newn -> InodeNumber = i;
        newn -> FileSize = 0;
        newn -> ActualFileSize = 0;
        newn -> LinkCount = 0;
        newn -> Permission = 0;
        newn -> Buffer = NULL;
        newn -> next = NULL;

        if(temp == NULL)
        {
            head = newn;
            temp = head;
        }
        else
        {
            temp -> next = newn;
            temp = temp -> next;
        }

        i++;
    }

    cout<<"Marvellous CVFS : DILB created Succesfully\n";
}



///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Function Name :     StartAuxilaryDataInitialisation
/// Description   :     It is used to initialize the superBlock
/// Author        :     Vinay Bharat Khot
/// Date          :     10-08-2025
///
//////////////////////////////////////////////////////////////////////////////////////////////////

void StartAuxilaryDataInitialisation()
{
    strcpy (bootobj.Information ,"Boot Process of Operating systems Done");
    cout<<bootobj.Information<<"\n";

    InitialiseSuperBlock();

    CreateDILB();

    InitialiseUAREA();
    
    cout<<"Marvellous CVFS : Auxilary data initialize sucessfully \n";
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Entry point function of project (main)
///
//////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    char str[80] = {'\0'};
    int iCount = 0;
    char Command[5][80];

    StartAuxilaryDataInitialisation();

    cout<<"----------------------------------------------------------------------------------------\n";
    cout<<"--------------------MARVELLOUS CVFS STARTED SUCESSFULY----------------------------------\n";
    cout<<"----------------------------------------------------------------------------------------\n";

    while(1)
    {
        fflush(stdin);

        printf("\nMarvellous CVFS > ");
        fgets(str,sizeof(str),stdin);

        iCount = sscanf(str,"%s %s %s %s",Command[0],Command[1],Command[2],Command[3]);

        printf("Value of icount is : %d\n",iCount);

        if(iCount == 1)
        {
            if(strcmp(Command[0],"exit") == 0)
            {
                printf("Thank You For Using Marvellous CVFS\n");
                printf("Dealocating all resources....!");

                break;
            }

        }   //End of if iCount == 1
        else if(iCount == 2)
        {

        }   //End of if iCount == 2
        else if(iCount == 3)
        {

        }   //End of if iCount == 3
        else if(iCount == 4)
        {

        }   //End of if iCount == 4
        else
        {
            printf("Commmand not found.....\n");
            printf("Please refer Help option or use man Command");
        }   //End of Invlid Command Part

    }// End of while (custome shell)

    

    return 0;
}//End of Main