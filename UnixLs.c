
#include <stdio.h>
#include "list.h"
#include <stdlib.h> //exit()
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/stat.h> //stat(), lstat()
#include <dirent.h> //opendir(), readdir(), closedir()
#include <string.h> // strcpy()

#include <unistd.h> //soft links

#define MAX_BUFFER 100
void (*Display_Info)(DIR*, char*);

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

void LS_LI(DIR* dir, char* dirPath) {
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if(entry->d_name[0] != '.'){
            struct stat statbuf; //lstat doesnt resolve symbolic links

            char fullPath[MAX_BUFFER * 3]; //cus adding 2 of em, made it x 3 cus of warnings
            snprintf(fullPath, sizeof(fullPath), "%s/%s", dirPath, entry->d_name);
            if (lstat(fullPath, &statbuf) == 0) {
                //inode
                printf("%lu ", (unsigned long)statbuf.st_ino); 

                //type
                if (S_ISLNK(statbuf.st_mode)) {
                    printf("l"); //Indicate it's a symlink
                } else {
                    printf((S_ISDIR(statbuf.st_mode)) ? "d" : "-");
                }

                //Print file type and permissions
                printf((statbuf.st_mode & S_IRUSR) ? "r" : "-");
                printf((statbuf.st_mode & S_IWUSR) ? "w" : "-");
                printf((statbuf.st_mode & S_IXUSR) ? "x" : "-");
                printf((statbuf.st_mode & S_IRGRP) ? "r" : "-");
                printf((statbuf.st_mode & S_IWGRP) ? "w" : "-");
                printf((statbuf.st_mode & S_IXGRP) ? "x" : "-");
                printf((statbuf.st_mode & S_IROTH) ? "r" : "-");
                printf((statbuf.st_mode & S_IWOTH) ? "w" : "-");
                printf((statbuf.st_mode & S_IXOTH) ? "x" : "-");

                printf(" %lu", (unsigned long)statbuf.st_nlink); //still print # of hard links for symbolic

                //owner name
                struct passwd *pw = getpwuid(statbuf.st_uid);
                if (pw != NULL) {
                    printf(" %s", pw->pw_name);
                }

                //group name
                struct group *gr = getgrgid(statbuf.st_gid);
                if (gr != NULL) {
                    printf(" %s", gr->gr_name);
                }

                //file size
                printf(" %lld", (long long)statbuf.st_size);

                //last modification time
                char timebuf[20];
                struct tm *tm = localtime(&statbuf.st_mtime);
                strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", tm);
                printf(" %s", timebuf);

                //file name
                printf(" %s", entry->d_name);

                if (S_ISLNK(statbuf.st_mode)) {
                    //target of the symlink
                    char target[256];
                    ssize_t len = readlink(fullPath, target, sizeof(target) - 1);
                    if (len != -1) {
                        target[len] = '\0';
                        printf("-> %s", target);
                    } else {
                        printf("ERROR: Couldn't read symlink target\n");
                    }
                }

                printf("\n"); //formatting
            }
        }
    }
}

//good
void LS_L(DIR* dir, char* dirPath) {
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if(entry->d_name[0] != '.'){
            struct stat statbuf; //lstat doesnt resolve symbolic links

            char fullPath[MAX_BUFFER * 3]; //cus adding 2 of em, made it x 3 cus of warnings
            snprintf(fullPath, sizeof(fullPath), "%s/%s", dirPath, entry->d_name);
            if (lstat(fullPath, &statbuf) == 0) {
                if (S_ISLNK(statbuf.st_mode)) {
                    printf("l"); //Indicate it's a symlink
                } else {
                    printf((S_ISDIR(statbuf.st_mode)) ? "d" : "-");
                }

                //Print file type and permissions
                printf((statbuf.st_mode & S_IRUSR) ? "r" : "-");
                printf((statbuf.st_mode & S_IWUSR) ? "w" : "-");
                printf((statbuf.st_mode & S_IXUSR) ? "x" : "-");
                printf((statbuf.st_mode & S_IRGRP) ? "r" : "-");
                printf((statbuf.st_mode & S_IWGRP) ? "w" : "-");
                printf((statbuf.st_mode & S_IXGRP) ? "x" : "-");
                printf((statbuf.st_mode & S_IROTH) ? "r" : "-");
                printf((statbuf.st_mode & S_IWOTH) ? "w" : "-");
                printf((statbuf.st_mode & S_IXOTH) ? "x" : "-");

                printf(" %lu", (unsigned long)statbuf.st_nlink); //still print # of hard links for symbolic

                //owner name
                struct passwd *pw = getpwuid(statbuf.st_uid);
                if (pw != NULL) {
                    printf(" %s", pw->pw_name);
                }

                //group name
                struct group *gr = getgrgid(statbuf.st_gid);
                if (gr != NULL) {
                    printf(" %s", gr->gr_name);
                }

                //file size
                printf(" %lld", (long long)statbuf.st_size);

                //last modification time
                char timebuf[20];
                struct tm *tm = localtime(&statbuf.st_mtime);
                strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", tm);
                printf(" %s", timebuf);

                //file name
                printf(" %s", entry->d_name);

                if (S_ISLNK(statbuf.st_mode)) {
                    //target of the symlink
                    char target[256];
                    ssize_t len = readlink(fullPath, target, sizeof(target) - 1);
                    if (len != -1) {
                        target[len] = '\0';
                        printf("-> %s", target);
                    } else {
                        printf("ERROR: Couldn't read symlink target\n");
                    }
                }

                printf("\n"); //formatting
            }
        }
    }
}

//good
void LS_I(DIR* dir, char* dirPath) {
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if(entry->d_name[0] != '.'){
            struct stat statbuf;

            char fullPath[MAX_BUFFER * 3]; //cus adding 2 of em, made it x 3 cus of warnings
            snprintf(fullPath, sizeof(fullPath), "%s/%s", dirPath, entry->d_name);
            if (lstat(fullPath, &statbuf) == 0) {
                printf("%lu %s\n", (unsigned long)statbuf.st_ino, entry->d_name);
            }
        }
    }
}

//in this case the pathname is .
//this happens when the directory structure isnt specified
void LS_None(DIR* dir, char* dirPath) {
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
        char* path = (char*)List_curr(dirList); //pointer to path name
        //this is basically a linked list, readdir gets the current and iterates the list
        DIR* dir = opendir(path); 
        if(dir == NULL){
            perror("opendir");
            List_free(dirList,freefunc);
            exit(1);
        }
        Display_Info(dir, path);
        closedir(dir);
        
        //update list
        List_next(dirList);

        printf("\n\n"); //formatting
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
