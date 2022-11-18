#include "header.h"
void ls_cmd(char *directory,char **arguments){
DIR *entered_dir;
int l_used = 0;
char tem[1500];
struct dirent *file_iterator;
struct stat file_info;
entered_dir = opendir(directory);
if(entered_dir==NULL){
    perror("ls: failed");
}
else{
while((file_iterator=readdir(entered_dir))!=NULL){
    tem[0]='\0';
    strcpy(tem,directory);
    tem[strlen(directory)]='/';
    tem[strlen(directory)+1]='\0';
    strcat(tem,file_iterator->d_name);
    stat(tem,&file_info);
    if(strcmp(arguments[1],"-al")==0 || strcmp(arguments[1],"-l" )==0 || strcmp(arguments[1],"-la")==0 ){
       if(strcmp(arguments[1],"-l" )==0){
        if(file_iterator->d_name[0]=='.'){
            continue;
        }
       } 
        l_used = 1;
        printf( (S_ISDIR(file_info.st_mode)) ? "d" : "-");
        printf( (file_info.st_mode & S_IWUSR) ? "w" : "-");
        printf( (file_info.st_mode & S_IRUSR) ? "r" : "-");
        printf( (file_info.st_mode & S_IXUSR) ? "x" : "-");
        printf( (file_info.st_mode & S_IRGRP) ? "r" : "-");
        printf( (file_info.st_mode & S_IWGRP) ? "w" : "-");
        printf( (file_info.st_mode & S_IXGRP) ? "x" : "-");
        printf( (file_info.st_mode & S_IROTH) ? "r" : "-");
        printf( (file_info.st_mode & S_IWOTH) ? "w" : "-");
        printf( (file_info.st_mode & S_IXOTH) ? "x" : "-"); 
        printf(" ");  
        FILE *fp;
        char path[1500];
        char links[500] = "stat "; 
        strcat(links,tem); 
        strcat(links," | grep Links: ");
        fp = popen(links, "r");
        if (fp == NULL) {
            printf("Failed to run command\n" );
            exit(1);
        }
        while (fgets(path, sizeof(path)-1, fp) != NULL) {
        }
        char ** links_number = split_input(path,2);
        printf("%s ",links_number[5]);
        pclose(fp);
     
        char path1[1500];
        char user[500] ="stat -c '%U %G' ";
        strcat(user,tem); 
        fp = popen(user, "r");
        if (fp == NULL) {
          printf("Failed to run command\n" );
          exit(1);
        }
        while (fgets(path1, sizeof(path1)-1, fp) != NULL) {
        }
        int i=0;
        for(i=0;path1[i]!='\n';i++){}
        path1[i]='\0';
        printf("%s  ", path1);
        pclose(fp);

        char path2[1500];
        char size[500] = "stat "; 
        strcat(size,tem); 
        strcat(size," | grep Size: ");
        fp = popen(size, "r");
        if (fp == NULL) {
            printf("Failed to run command\n" );
            exit(1);
        }
        while (fgets(path2, sizeof(path2)-1, fp) != NULL) {
        }
        char ** size_number = split_input(path2,2);
        printf("%s ",size_number[1]);
        pclose(fp);

        char path3[1500];
        char date[500] = "stat "; 
        strcat(date,tem); 
        strcat(date," | grep Modify: ");
        fp = popen(date, "r");
        if (fp == NULL) {
            printf("Failed to run command\n" );
            exit(1);
        }
        while (fgets(path3, sizeof(path3)-1, fp) != NULL) {
        }
        char **date_time = split_input(path3,3);
        month(atoi(date_time[2]));
        printf(" %s",date_time[3]);
        printf(" %s",date_time[4]);
        pclose(fp);

        printf(" %s",file_iterator->d_name);
    }
    else if(strcmp(arguments[1],"-a")!=0){
        if(file_iterator->d_name[0] == '.'){}
        else{
            printf("%s ",file_iterator->d_name);
        }
    }
    else{
        printf("%s  ",file_iterator->d_name);
    }
if(l_used==1)printf("\n");
}
if(l_used==0)printf("\n");
}
}