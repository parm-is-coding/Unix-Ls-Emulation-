
#include <stdio.h>
#include "list.h"

#include <sys/stat.h> //stat(), lstat()
#include <dirent.h> //opendir(), readdir(), closedir()

void (*Display_Info)(struct dirent*);

//our booleans for optinos
int optionI = 0, optionL = 0;

//a list of directories we need to call our LS function on
List* dirList;

//adds all directory names we need to call ls on to our dirList
void parseStrings(int argv, char** argc) {
    //i = 0 is not neccessary -
    for (int i = 1; i < argv; i++) {
        char* dirName = argc[i];
        //check if it is a option, we are currently not checking for invalid options
        if (dirName[0] == '-') {
            int j = 1; //need to check for compounded options too
            while (dirName[j] != '\0') {
                if (dirName[j] == 'i') {
                    optionI = 1;
                }

                if (dirName[j] == 'l') {
                    optionL = 1;
                }

                j++;
            }
            
            continue; //skips adding this to our directories
        }

        //else: it is a dir name, might need to check for case: not a dir
        //TO DO (add to list of dir names):
        List_append(dirList, dirName);
    }
}

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
    //TO DO (do this first):
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
    dirList = List_create(); //init list

    parseStrings(argv, argc);
    setOptions();
    LS_Function();

    List_free(dirList, NULL); //TO DO: write the free functions
    return 0;
}
