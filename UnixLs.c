
#include <stdio.h>
#include <list.h>

#include <sys/stat.h> //stat(), lstat()
#include <dirent.h> //opendir(), readdir(), closedir()

void (*Display_Info)(DIR*);

//our booleans for optinos
int optionI = 0, optionL = 0;

//a list of directories we need to call our LS function on
List* dirList;

//adds all directory names we need to call ls on to our dirList
void parseStrings(int argv, char** argc) {
    for (int i = 0; i < argv; i++) {
        char* dirName = argc[i];
        //check if it is a option, we are currently not checking for invalid options
        if (dirName[0] == '-') {
            int j = 1; //need to check for compounded options too
            while (dirName[j] != '\0') {
                if (dirName[1] == 'i') {
                    optionI = 1;
                }

                if (dirName[1] == 'l') {
                    optionL = 1;
                }
            }
            
            continue;
        }

        //else: it is a dir name, might need to check for case: not a dir
        //TO DO (add to list of dir names):
        List_append(dirList, dirName);
    }
}

void LS_LI(DIR* dir) {
    //TO DO:
}

void LS_I(DIR* dir) {
    //TO DO:
}

void LS_L(DIR* dir) {
    //TO DO:
}

void LS_None(DIR* dir) {
    //TO DO:
}

//Calls Display_Info() on each dir in our list or dir names
void LS_Function() {
    //iterate through each directory
    int n = List_count(dirList);
    for (int i = 0; i < n; i++) {
        //set up our directory pointer
        DIR* dir = opendir(List_current(dirList)); //WARNING: might need to check for null errors (asuming arguments are valid)
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

    parseString(argv, argc);
    setOptions();
    LS_Function();

    return 0;
}
