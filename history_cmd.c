#include "header.h"
void history_cmd(char *input,char*home){
int fptr,count=0;
char readed[5000];
char write_arr[5000];
int it=0;
char curr_dir[500];
getcwd(curr_dir,sizeof(curr_dir));
chdir(home);
fptr = open("history",O_RDONLY|O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
chdir(curr_dir);
int k = lseek(fptr,0,SEEK_END);
lseek(fptr,0,SEEK_SET);

if(k>=0){
int h =read(fptr,readed,k);
for(int i=0;i<k;i++){
if(readed[i]=='\n'){
	count++;
}
}
for(int i=0;i<k;i++){
if(readed[i]=='\n')count--;
if(count<=20){
	write_arr[it++]=readed[i];
}
}
lseek(fptr,0,SEEK_SET);
write(fptr,write_arr,strlen(write_arr));
lseek(fptr,0,SEEK_END);}
write(fptr,input,strlen(input));
return;
}