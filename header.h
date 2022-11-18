#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include"sys/wait.h"
#include"sys/types.h"
#include<sys/stat.h>
#include<fcntl.h>
#include<dirent.h>
#include<time.h>
#include<pwd.h>
#include<grp.h>
#include<ctype.h>
// cd unexist dir (DONE)
// pinfo ~ visible
// cd --- seg fault (DONE)
// ls --- seg fault(DONE)
// ls -la direc --- seg(DONE)
// ls unexistsent
// 		;
// extern char temp_inp[500];
// extern int fir;//for echo purpose
extern void month(int arg);
extern void print_history(char *home,char ** arguments);
extern void history_cmd(char *input,char *home);
extern void builtin_cd(char** arguments,char *username,char *systemname,char *home);
extern char **split_input(char *input,int stage);
extern void handl(int sig);
extern void kid(int sig);
extern void ls_cmd(char *directory,char **arguments);
extern void shell_loop();
extern void just(int sig);
