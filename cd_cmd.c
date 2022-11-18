#include "header.h"
void builtin_cd(char** arguments,char *username,char *systemname,char *home){
    if(strcmp(arguments[0],"cd")==0){
        if(strcmp(arguments[1],".")==0){
            return;
        }
        else if(strcmp(arguments[1],"~")==0){
            chdir(home);
            return;
        }
        else{
            int err = chdir(arguments[1]);
            if(err!=0)perror("cd failed ");
            return;
        }
    }
}