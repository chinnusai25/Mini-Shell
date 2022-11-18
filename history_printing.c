#include "header.h"
void print_history(char *home,char**arguments){
	int fptr;
	char curr_dir[500];
	getcwd(curr_dir,sizeof(curr_dir));
	chdir(home);
	fptr = open("history",O_RDONLY);
	chdir(curr_dir);
	char readed[2000];
	int count=0;
	int end = lseek(fptr,0,SEEK_END);
	lseek(fptr,0,SEEK_SET);
	int g =read(fptr,readed,end);
	for(int i=0;i<=end;i++){
	if(readed[i]=='\n')count++;
	}
	for(int i=0;i<end;i++){
	if(arguments[1]==NULL){
		if(count<=10)printf("%c",readed[i]);
	}
	else{
		if(count<=atoi(arguments[1]))printf("%c",readed[i]);
	}
	if(readed[i]=='\n')count--;
	if(count<0)break;
	}
}