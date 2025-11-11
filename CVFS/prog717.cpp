///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Header files inclusion
///
//////////////////////////////////////////////////////////////////////////////////////////////////
#include<stdio.h>
#include<stdlib.h>
//#include<unistd.h>
#include<string.h>
#include<stdbool.h>

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
/// User Define Macros for error handling
/// 
//////////////////////////////////////////////////////////////////////////////////////////////////

#define ERR_INVALID_PARAMETER -1
#define ERR_NO_INODES -2
#define ERR_FILE_ALREADY_EXISTS -3

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
/// Function Name :     DisplayHelp
/// Description   :     It is used to Display the information abiut command
/// Author        :     Vinay Bharat Khot
/// Date          :     11-08-2025
///
//////////////////////////////////////////////////////////////////////////////////////////////////

void DisplayHelp()
{
   printf("----------------------------------------------------------------------------------------\n");
   printf("-----------------------Command Manual of Marvellous CVFS--------------------------------\n");
   printf("----------------------------------------------------------------------------------------\n");

   printf("exit : It is used to terminate the shell of Marvellous CVFS\n");
   printf("clear : It is used to clear the console of Marvellous CVFS\n");

   //Add more option here
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Function Name :     ManPage
/// Description   :     It is used to Display the Manual Page of command
/// Input         :     It Accept the command Name
/// OutPut        :     Display manual details of Command
/// Author        :     Vinay Bharat Khot
/// Date          :     11-08-2025
///
//////////////////////////////////////////////////////////////////////////////////////////////////

void ManPage(
            char *name //name of command
            )
{
    if(strcmp(name,"creat") == 0)
    {
        printf("Description : This Command use to Create new regualr file on our file Systems\n");
        printf("Usage : creat File_Name Permissions\n");
        printf("File_Name : the name of file that you to create\n");
        printf("permissions : \n1 : Read \n2: Write \n3: Read + Write\n");
    }
    else if(strcmp(name,"exit") == 0)
    {
        printf("Description : This Command use to terminate the file Systems\n");
        printf("Usage : exit\n");
        
    }
    //Add more option here
    else
    {
        printf("No manual entry for %s\n",name);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Function Name :     IsFileExists
/// Description   :     It is used to check wheather the given file name is exist or not
/// Input         :     It Accept the Name 
/// OutPut        :     It return the boolean value (true : if present )
/// Author        :     Vinay Bharat Khot
/// Date          :     11-08-2025
///
//////////////////////////////////////////////////////////////////////////////////////////////////

bool IsFileExists(
                char *name //Name of file that we want to check
                )
{
    PINODE temp = head;
    bool Bflag = false;

    while(temp != NULL)
    {
        if((strcmp(name,temp->FileName) == 0) && temp -> FileType == REGULARFILE)
        {
            Bflag = true;
            break;
        }

        temp = temp -> next;
    }

    return Bflag;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Function Name :     createFile
/// Description   :     It is used to create a new regular file
/// Input         :     It Accept the Name and permission
/// OutPut        :     It return file Descriptor
/// Author        :     Vinay Bharat Khot
/// Date          :     11-08-2025
///
//////////////////////////////////////////////////////////////////////////////////////////////////

int CreateFile(
                char *name , // Name of file
                int permission // file permission
                )
{
    
    
    //FILTERS

    //if file name is missing
    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    // if enterd permission is invalid
    if(permission < 1 || permission > 3)
    {
        return ERR_INVALID_PARAMETER;
    }

    //check wheather empty inode is there or not
    if(superobj.FreeInodes == 0)
    {
        return ERR_NO_INODES;
    }

    //Check wheather already exist or not
    if(IsFileExists(name) == true)
    {
        return ERR_FILE_ALREADY_EXISTS;
    }

    
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
    int iret = 0;
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

        if(iCount == 1)
        {
            // Marvellous CVFS > exit
            if(strcmp(Command[0],"exit") == 0)
            {
                printf("Thank You For Using Marvellous CVFS\n");
  
                break;
            }

            // Marvellous CVFS > help
            else if(strcmp(Command[0],"help") == 0)
            {
                DisplayHelp();
            }

            // Marvellous CVFS > clear
            else if(strcmp(Command[0],"clear") == 0)
            {
                system("cls");
            }

            

        }   //End of if iCount == 1
        else if(iCount == 2)
        {
            // Marvellous CVFS > man create

            if(strcmp(Command[0],"man") == 0)
            {
                ManPage(Command[1]);
            }

        }   //End of if iCount == 2
        else if(iCount == 3)
        {
            // Marvellous CVFS > creat Ganesh.txt 3   
            if(strcmp(Command[0],"creat") == 0)
            {
                iret = CreateFile(Command[1],atoi(Command[2]));

                if(iret == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid parameter for the functioin\n");
                    printf("Please check Man Page for details\n");
                }
                else if(iret = ERR_NO_INODES)
                {
                    printf("Error : there is no inode");
                }
                else if(iret == ERR_FILE_ALREADY_EXISTS)
                {
                    printf("Error : Unable to create file as file is a already existing\n");
                }
            }
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