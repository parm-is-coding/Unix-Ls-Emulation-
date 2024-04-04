
#include <stdio.h>
#include "list.h"
#include <stdlib.h> //exit()

#include <sys/stat.h> //stat(), lstat()
#include <dirent.h> //opendir(), readdir(), closedir()
#include <string.h> // strcpy()
#define MAX_BUFFER 100
void (*Display_Info)(struct dirent*);

//Understand pathing in ls


//our booleans for optinos
int optionI = 0, optionL = 0;

//a list of directories we need to call our LS function on
List* dirList;

//elements in argc can be directorynames, filenames, options, or invalid characters
static void freefunc(void* pItem){
    char* item = pItem;
    free(item);
}
//adds all directory names we need to call ls on to our dirList
void parseStrings(int argv, char** argc,List* directoryPathList) {
    //i = 0 is not neccessary -
    for (int i = 1; i < argv; i++) {
        char buffer[MAX_BUFFER];
        strncpy(buffer,argc[i],MAX_BUFFER);
        printf("%s\n",buffer);
        //check if it is a option, we are currently not checking for invalid options
        if (buffer[0] == '-') {
            int j = 1; //need to check for compounded options too
            while (buffer[j] != '\0') {
                if (buffer[j] == 'i') {
                    optionI = 1;
                }

                if (buffer[j] == 'l') {
                    optionL = 1;
                }
                if(buffer[j] != 'j' && buffer[j] != 'i'){
                    printf("Error: invalid flag operations");
                    List_free(directoryPathList,freefunc);
                    exit(1);
                }
                j++;
            }
        }

            continue; //skips adding this to our directories
    }
}
        // //else: it is a dir name, might need to check for case: not a dir
        // //TO DO (add to list of dir names):
        // List_append(dirList, dirName);


void LS_LI(struct dirent* dir) {
    //TO DO (do this last):
}

void LS_I(struct dirent* dir) {
    //TO DO:
}

void LS_L(struct dirent* dir) {
    //TO DO:
}

void LS_None(struct dirent* dir) {
    printf("no arg case was run");
}

//Calls Display_Info() on each dir in our list or dir names
void LS_Function() {
    //iterate through each directory
    int n = List_count(dirList);
    for (int i = 0; i < n; i++) {
        //set up our directory pointer

        //this is basically a linked list, readdir gets the current and iterates the list
        DIR* dir = opendir(List_curr(dirList)); //WARNING: might need to check for null errors (asuming arguments are valid)
        struct dirent *item; //prepend with struct because they haven't done a typedef in their header

        //iterate through each item
        while (true) {
            item = readdir(dir); //gets next
            if (item == NULL) {
                goto close; //exits this while loop
            }

            Display_Info(item);
        }

        close: {
            closedir(dir);
        }

        //update list
        List_next(dirList);
    }
}

void setOptions() {
    if (optionI == 1 && optionL == 1) {
        Display_Info = &LS_LI;
        return;
    }

    if (optionI == 1) {
        Display_Info = &LS_I;
        return;
    }

    if (optionL == 1) {
        Display_Info = &LS_L;
        return;
    }

    //else:
    Display_Info = &LS_None;
}

//if (string)
int main(int argv, char** argc) {
    List* directoryPathList = List_create(); //init list
    parseStrings(argv, argc,directoryPathList);
    //setOptions();
    //LS_Function();
    printf("FOOBAR\n");
    List_free(directoryPathList, freefunc); 
    return 0;
}
