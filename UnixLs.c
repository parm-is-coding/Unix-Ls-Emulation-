
#include <stdio.h>
#include "list.h"
#include <stdlib.h> //exit()
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/stat.h> //stat(), lstat()
#include <dirent.h> //opendir(), readdir(), closedir()
#include <string.h> // strcpy()
#define MAX_BUFFER 100
void (*Display_Info)(DIR*);

//Understand pathing in ls
    //a path can either start from / ~home or the current directory
        // root and home directory paths require usernames however invalid paths will break opendir
    //assume directery names given are from .

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
    int argMode = 1; //once we find a non arg, we are no longer taking args
    //i = 0 is not neccessary -
    for (int i = 1; i < argv; i++) {
        char buffer[MAX_BUFFER];
        strncpy(buffer,argc[i],MAX_BUFFER);

        //check if it is a option, we are currently not checking for invalid options
        if (argMode == 1 && buffer[0] == '-') {
            int j = 1; //need to check for compounded options too
            while (buffer[j] != '\0') {
                if (buffer[j] == 'i') {
                    optionI = 1;
                }

                if (buffer[j] == 'l') {
                    optionL = 1;
                }

                if(buffer[j] != 'l' && buffer[j] != 'i'){
                    printf("Error: invalid flag operations");
                    List_free(directoryPathList, freefunc);
                    exit(1);
                }

                j++;
            }
        } else {
            argMode = 0; //no longer argMode
            char* path = malloc(MAX_BUFFER);
            strncpy(path, buffer, MAX_BUFFER);
            List_append(directoryPathList, path);
        }
    }

    if (directoryPathList->count == 0) {
        char* path = malloc(MAX_BUFFER);
        strncpy(path, ".", MAX_BUFFER);
        List_append(directoryPathList, path);
    }
}

void LS_LI(DIR* dir) {
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if(entry->d_name[0] != '.'){
            struct stat statbuf;
            if (stat(entry->d_name, &statbuf) == 0) {
                // Print InodeNumbers
                printf("%lu ", (unsigned long)statbuf.st_ino);
                // Print file type and permissions
                printf((S_ISDIR(statbuf.st_mode)) ? "d" : "-");
                printf((statbuf.st_mode & S_IRUSR) ? "r" : "-");
                printf((statbuf.st_mode & S_IWUSR) ? "w" : "-");
                printf((statbuf.st_mode & S_IXUSR) ? "x" : "-");
                printf((statbuf.st_mode & S_IRGRP) ? "r" : "-");
                printf((statbuf.st_mode & S_IWGRP) ? "w" : "-");
                printf((statbuf.st_mode & S_IXGRP) ? "x" : "-");
                printf((statbuf.st_mode & S_IROTH) ? "r" : "-");
                printf((statbuf.st_mode & S_IWOTH) ? "w" : "-");
                printf((statbuf.st_mode & S_IXOTH) ? "x" : "-");

                // Print number of hard links
                printf(" %lu", (unsigned long)statbuf.st_nlink);

                // Print owner name
                struct passwd *pw = getpwuid(statbuf.st_uid);
                if (pw != NULL) {
                    printf(" %s", pw->pw_name);
                }

                // Print group name
                struct group *gr = getgrgid(statbuf.st_gid);
                if (gr != NULL) {
                    printf(" %s", gr->gr_name);
                }

                // Print file size
                printf(" %lld", (long long)statbuf.st_size);

                // Print last modification time
                char timebuf[20];
                struct tm *tm = localtime(&statbuf.st_mtime);
                strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", tm);
                printf(" %s", timebuf);

                // Print file name
                printf(" %s\n", entry->d_name);
            }
        }
    }
}

void LS_I(DIR* dir) {
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if(entry->d_name[0] != '.'){
            struct stat statbuf;
            if (stat(entry->d_name, &statbuf) == 0) {
                printf("%lu %s ", (unsigned long)statbuf.st_ino, entry->d_name);
            }
        }
        
    }
}

void LS_L(DIR* dir) {
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if(entry->d_name[0] != '.'){
            struct stat statbuf;
            if (stat(entry->d_name, &statbuf) == 0) {
                // Print file type and permissions
                printf((S_ISDIR(statbuf.st_mode)) ? "d" : "-");
                printf((statbuf.st_mode & S_IRUSR) ? "r" : "-");
                printf((statbuf.st_mode & S_IWUSR) ? "w" : "-");
                printf((statbuf.st_mode & S_IXUSR) ? "x" : "-");
                printf((statbuf.st_mode & S_IRGRP) ? "r" : "-");
                printf((statbuf.st_mode & S_IWGRP) ? "w" : "-");
                printf((statbuf.st_mode & S_IXGRP) ? "x" : "-");
                printf((statbuf.st_mode & S_IROTH) ? "r" : "-");
                printf((statbuf.st_mode & S_IWOTH) ? "w" : "-");
                printf((statbuf.st_mode & S_IXOTH) ? "x" : "-");

                // Print number of hard links
                printf(" %lu", (unsigned long)statbuf.st_nlink);

                // Print owner name
                struct passwd *pw = getpwuid(statbuf.st_uid);
                if (pw != NULL) {
                    printf(" %s", pw->pw_name);
                }

                // Print group name
                struct group *gr = getgrgid(statbuf.st_gid);
                if (gr != NULL) {
                    printf(" %s", gr->gr_name);
                }

                // Print file size
                printf(" %lld", (long long)statbuf.st_size);

                // Print last modification time
                char timebuf[20];
                struct tm *tm = localtime(&statbuf.st_mtime);
                strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", tm);
                printf(" %s", timebuf);

                // Print file name
                printf(" %s\n", entry->d_name);
            }
        }
    }
}
//in this case the pathname is .
// this happens when the directory structure isnt specified
void LS_None(DIR* dir) {
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if(entry->d_name[0] != '.'){
            printf("%s ",entry->d_name);
        }
    }
}

//Calls Display_Info() on each dir in our list or dir names
void LS_Function(List* dirList) {
    //iterate through each directory
    int n = dirList->count;
    List_first(dirList);
    for (int i = 0; i < n; i++) {
        //set up our directory pointer

        //this is basically a linked list, readdir gets the current and iterates the list
        DIR* dir = opendir((char*)List_curr(dirList)); 
        if(dir == NULL){
            perror("opendir");
            List_free(dirList,freefunc);
            exit(1);
        }
        Display_Info(dir);
        closedir(dir);
        
        //update list
        List_next(dirList);
    }
    printf("\n"); //formatting
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
void testOptions(List* dir){
    printf("-i %d\n-l %d\n",optionI,optionL);
    List_first(dir);
    for(int i = 0; i < dir->count;i++){
        char* curr = List_curr(dir);
        List_next(dir);
        printf("%s\n",curr);
    }
    
}
//if (string)
int main(int argv, char** argc) {
    List* directoryPathList = List_create(); //init list
    parseStrings(argv, argc,directoryPathList);
    //testOptions(directoryPathList);
    setOptions();
    LS_Function(directoryPathList);
    List_free(directoryPathList, freefunc); 
    return 0;
}
