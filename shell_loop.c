#include "header.h"
int fir;
char temp_inp[500];
pid_t kaka;
int jobs_id[25],jobs_pid[25];
char jobs_cmd[25][100];
char jobs_status[25][100];
int jobs_iter=1;
char proc_names[150][100];
int proc_pid[150];
int proc_iter;
int stored_id;
char **split_input(char *input,int stage){          // Function to tokenise according to delimiters required
strcpy(temp_inp,input);
char *delimiter;
int bufsize = 10;
char **total_inputs = malloc(bufsize*sizeof(char*));
char *current_input;
int it = 0;
if(!total_inputs){
    perror("Memory allocation error");
    exit(1);
}
if(stage==1){
    current_input = strtok(input,";");}
else if(stage==2){
    current_input = strtok(input," \t\r\n\a");}
else if(stage==3){
    current_input = strtok(input," .-\t\r\n\a");}
 else if(stage==4){
    current_input = strtok(input,"|");}
while(current_input != NULL){
    total_inputs[it++] = current_input;
    if (it >= bufsize) {
      bufsize += 50;
      total_inputs = realloc(total_inputs, bufsize * sizeof(char*));
      if (!total_inputs) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
    if(stage==1){
    current_input = strtok(NULL,";");}
    else if(stage==2){
    current_input = strtok(NULL," \t\r\n\a");}
    else if(stage==3){
    current_input = strtok(NULL," .-\t\r\n\a");}
    else if(stage==4){
    current_input = strtok(NULL,"|");}
}
    total_inputs[it]=NULL;
    return total_inputs;
}
int find_id(int pid_here){                          // Returning the job id of given pid
    for(int i=0;i<25;i++){
        if(jobs_pid[i]==pid_here){
            return i;
        }
    }
    return -1;
}
void sigint_handler(int signal) {
}
void set_status(int id,char status_here[]){         // Updating the status of processes 
    if(strcmp(jobs_status[id],"done")!=0){
        strcpy(jobs_status[id],status_here);
    }
}
int find_pid(int id_here){                  // Function to return pid of given job id
    for(int i=0;i<25;i++){
        if(jobs_id[i]==id_here){
            return jobs_pid[i];
        }
    }
    return -1;
}
void handl(int sig){                            // handl function to handle background process
            pid_t wpid;
            pid_t pid;
            int status;
            while ((wpid=waitpid(fir,&status,WNOHANG)) > 0)
            {
                if ( WIFEXITED(status)) 
                { 
                    int exit_status = WEXITSTATUS(status);         
                    set_status(find_id(wpid),"done");
                    printf("Exit status of the child was %d\n",exit_status); 
                    printf("%d exited normally\n",fir);
                }
                else if (WIFSTOPPED(status)) {
                    strcpy(jobs_status[stored_id],"suspended");
                }    
                else if (WIFCONTINUED(status)) {
                    strcpy(jobs_status[stored_id],"continued");
                }    
            }
}
char *find_procname(int temp_pid){          // Function to return process name of given pid
    for(int i=0;i<proc_iter;i++){
        if(proc_pid[i]==temp_pid){
            return proc_names[i];
        }
    }
}
void just(int sig){                     // Function to handle ctrl z interrupt
    pid_t hhpid = kaka;
    if(kaka==0){
        return;
    }
    else{
        kill(hhpid,19);
        jobs_id[jobs_iter]=jobs_iter;
                strcpy(jobs_cmd[jobs_iter],find_procname(hhpid));       //  Updating the status of processes by
                strcpy(jobs_status[jobs_iter],"suspended");             //  adding this process to our background
                jobs_pid[jobs_iter]=hhpid;                              // processes arrays
                jobs_iter++; 
                int ppid_here,status_here;
                ppid_here = fork();
                if(ppid_here==0){
                    while(!WIFEXITED(status_here));
                    exit(1);
                }
                else{
                    signal(SIGCHLD,handl);
                }

    kaka = 0;   
    }
}

void kid(int sig){                          // Function kid to handle ctrl c interrupt
    if(kaka==0)return;
    else{
        setpgid(kaka,kaka);
    if (kill(kaka, SIGKILL) < 0) {
        }
    }
    kaka = 0;
}
void kill_func(int pid_here){                       // kill_func function to kill the processes of pid given
int status;
        int stat=0;
        if (kill(pid_here, SIGKILL) < 0) {
        printf("kill %d: job not found\n", pid_here);
        }
        strcpy(jobs_status[find_id(pid_here)],"done");
}
void danger (int background_proc,ssize_t input_read,char **piped_arguments,int l,char home[],char username[],char systemname[],char curr_dir[]){
    char **arguments;
    arguments = split_input(piped_arguments[l],2);              //Tokenizing with delimiters like space,tab etc
    int p = 0;
    if(arguments[p]==NULL){perror("");return;}                  //If input read from prompt is empty throwing error
    for(p=0;arguments[p]!=NULL;p++){}
    if(strcmp(arguments[p-1],"&")==0){                          //Checking whether the process is foreground or background
        arguments[p-1] = NULL;
        background_proc +=1;                                    //Updating background process if the process is background
    }
    if(input_read!=-1){
        int file_des,idx =0,temp_out=-1,temp_err=-1,temp_in=-1;
        while (arguments[idx]!=NULL) {   
            int f = strcmp(arguments[idx],">");                 //If output redirection 
            if (f==0) {
                if ((file_des = open (arguments[idx+1], 
                            O_WRONLY | O_CREAT | O_TRUNC, 
                            S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1) {
                    perror (arguments[idx+1]);                  //Error if the output file doesnt exists
                }
                temp_out = dup(fileno(stdout));                 //Storing fd 1 so as to restore at end
                temp_err = dup(fileno(stderr));                 //Storing fd 2 so as to restore at end
                if(dup2 (file_des, 1)==-1)perror("cmd failed");         //Updating fd 1 as of fd of output file
                if(dup2 (file_des, 2)==-1)perror("cmd failed");         //Updating fd 2 as of fd of output file
                close(file_des);
                
                for(int i=idx;arguments[i]!=NULL;i++){
                    arguments[i]=arguments[i+2];                    //Removing the arrows and file names from arguments
                }
            }
            else if (*arguments[idx] == '<' && arguments[idx+1]) {          // If input redirection
                if ((file_des = open (arguments[idx+1], O_RDONLY)) == -1) {
                    perror (arguments[idx+1]);
                }
                temp_in =dup(fileno(stdin));                        //Storing fd 0 so as to restore back
                if(dup2 (file_des, 0)==-1)perror("cmd failed");     //Updating fd 0 as of fd of input file
                close (file_des);

                for(int i=idx;arguments[i]!=NULL;i++){
                    arguments[i]=arguments[i+2];                    //Removing the arrows and file names from arguments
                }
            }
            else if ((strcmp(arguments[idx],">>")==0) && arguments[idx+1]) {        //If append operation
                if ((file_des = open (arguments[idx+1], O_WRONLY | O_APPEND, 
                            S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1) {
                    perror (arguments[idx+1]);
                }
                temp_out = dup(fileno(stdout));                     //Storing fd 1 so as to restore back at end
                temp_err = dup(fileno(stderr));                     //Storing fd 2 so as to restore back at end
                if(dup2 (file_des, 1)==-1)perror("cmd failed");     //Updating fd 1 as of fd of output file
                if(dup2 (file_des, 2)==-1)perror("cmd failed");     //Updating fd 2 as of fd of output file
                close(file_des);
                
                for(int i=idx;arguments[i]!=NULL;i++){
                    arguments[i]=arguments[i+2];                //Removing the arrows and file names from arguments
                }
            }
            else{
                idx++;                                      //If no arrow is seen just increment the idx
            }
        }
        if(strcmp(arguments[0],"cd")==0){                   // "cd" command handling
                if(arguments[1]==NULL){arguments[1]="~";}
                builtin_cd(arguments,username,systemname,home);      //Passing to builtin_cd function   
            }
        else if(strcmp(arguments[0],"pwd")==0){             //"pwd" command handling
            printf("%s\n",curr_dir);
        }
        else if(strcmp(arguments[0],"echo")==0){            //"echo" command handling
            int temp_flag = 0;
            for(int i=0;temp_inp[i]!='\0';i++){
                if(temp_flag==1 && temp_inp[i]!='"'){
                    printf("%c",temp_inp[i]);
                }
                if(temp_inp[i]=='"'){
                    temp_flag=1;
                }
            }
        }
        else if(strcmp(arguments[0],"pinfo")==0){           //"pinfo" command handling
            char *s;
            char arr[500];
            char dir[500];
            long int current_pid;
            if(arguments[1]==NULL){
            current_pid = getpid();
            sprintf(arr,"%d",getpid());
            s = arr;
            }
            else{
                s = (arguments[1]);
                current_pid = atoi(s);
            }
            struct stat sts;
            char result[3000];
            char **print;
            char proc_check[]= "/proc/";
            strcat(proc_check,s);
            if (stat(proc_check, &sts) == -1) {
                printf("process not found with given pid\n");
            }
            else{
            strcat(proc_check,"/stat");    
            int fg = open(proc_check,O_RDONLY);
            if(fg==-1){perror("sai:");}
            else{
            read(fg,result,300);
            print=split_input(result,2);
            char exe_path[]="/proc/";
            char path3[1500];
            strcat(exe_path,s);
            strcat(exe_path,"/exe");
            int st = readlink(exe_path,path3,1500);
            if(st==-1){
                strcpy(path3,"The link is broken");
            }
            else{
                path3[st]='\0';
            }
            printf("pid -- %ld\n",current_pid);
            printf("Process Status -- %s\n", print[2]);
            printf("memory -- %s {​ Virtual Memory ​ }\n",print[23]);
            printf("Executable Path --​ %s\n",path3 );
            }
        }
        }
        else if(strcmp(arguments[0],"ls")==0){          //"ls" command handling
            if(arguments[1]==NULL){
                arguments[1]=".";
                ls_cmd(arguments[1],arguments);         //Passing to ls_cmd function    
            }
            else if(strcmp(arguments[1],"-a")==0 || strcmp(arguments[1],"-l")==0 || strcmp(arguments[1],"-al")==0 || strcmp(arguments[1],"-la")==0){
                if(arguments[2]==NULL){arguments[2]=".";}
                ls_cmd(arguments[2],arguments);         //Passing to ls_cmd function
            }
            else{
                ls_cmd(arguments[1],arguments);         //Passing to ls_cmd function
            }
        }
        else if(strcmp(arguments[0],"history")==0){     //"history" command handling
            print_history(home,arguments);              //Passing to print_history function
        }
        else if(strcmp(arguments[0],"fg")==0){          //"fg" command handling
            int status;
            int stat=0;
            pid_t hpid;
            int id_here;
            if(arguments[1]==NULL){perror("fg:error");}     //If no arguments returns an error
            else{
            id_here = atoi(arguments[1]);                   //Converting input string to integer
            hpid = find_pid(id_here);
            if(hpid==-1){                                   //If entered job number doesnt exists returns an error                
            printf("sai: fg %d: job not found\n", id_here);
            }
            else{
            if (kill(hpid, SIGCONT) < 0) {                  //Continuing the process        
            printf("mysh: fg %d: job not found\n", hpid);   
            }
            tcsetpgrp(0, hpid);

        waitpid(hpid, &stat, WUNTRACED);                    //Waiting until the process ends
                if ( WIFEXITED(stat)) 
                { 
                    int exit_status = WEXITSTATUS(stat);        //Checking the status of process and updating its status         
                    set_status(find_id(hpid), "done");
                } else if (WIFSTOPPED(stat)) {
                    set_status(find_id(hpid), "suspended");
                } else if (WIFCONTINUED(stat)) {
                    set_status(find_id(hpid), "continued");
                }
                int l;
                for(l=1;l<25;l++){                        //Updating the arrays which need to be printed for the cmd "jobs"      
                    if(jobs_pid[l]==hpid){
                        break;
                    }
                }
                for(int p=l;p<25;p++){
                    jobs_pid[p]=jobs_pid[p+1];
                    strcpy(jobs_cmd[p],jobs_cmd[p+1]);
                    strcpy(jobs_status[p],jobs_status[p+1]);
                }
                if(jobs_iter >1)
                    jobs_iter--;
                for(int p=1;p<jobs_iter;p++){
                    jobs_id[p]=p;
                }
                
                signal(SIGTTOU, SIG_IGN);
                tcsetpgrp(0, getpid());
                signal(SIGTTOU, SIG_DFL);
            }
        }
        }
        else if(strcmp(arguments[0],"bg")==0){                      //"bg" command handling
            int status;
            int stat=0;
            pid_t hpid;
            int id_here;
            if(arguments[1]==NULL){perror("bg:error");}             //Returning an error if no input given
            else{
            id_here = atoi(arguments[1]);
            hpid = find_pid(id_here);                               //Returning error if invalid job number given    
            if(hpid==-1){
                printf("sai: bg %d: job not found\n", id_here);
            }
            else{
                if (kill(hpid, SIGCONT) < 0) {                      //Continuing the process                 
                printf("mysh: fg %d: job not found\n", hpid);
                }
            }
        }
        }
        else if(strcmp(arguments[0],"jobs")==0){                    //"jobs" command handling    
            int status, pid;

        while ((pid = waitpid(-1, &status, WNOHANG|WUNTRACED|WCONTINUED)) > 0) {
            if (WIFEXITED(status)) {
                set_status(find_id(pid), "done");                   // Updating the status of processes which to be printed
            } else if (WIFSTOPPED(status)) {
                set_status(find_id(pid), "suspended");
            } else if (WIFCONTINUED(status)) {
                set_status(find_id(pid), "continued");
            }
        }
            for(int i=1;i<jobs_iter;i++){                          // Printing the output of "jobs" command      
                printf("[%d]    ",jobs_id[i]);
                printf("%s  ",jobs_status[i]);
                printf("%s  ",jobs_cmd[i]);
                printf("[%d]\n",jobs_pid[i]);
            }
            int m=1;
        //     while(m<25){                                            // Removing the processes which have status "done" 
        //         int l=0;
        //         for(l=1;l<25;l++){
        //             if(strcmp(jobs_status[l],"done")==0){
        //                 break;
        //             }
        //         }
        //         if(l!=25){
        //         for(int p=l;p<25;p++){                              
        //             jobs_pid[p]=jobs_pid[p+1];
        //             strcpy(jobs_cmd[p],jobs_cmd[p+1]);
        //             strcpy(jobs_status[p],jobs_status[p+1]);
        //         }
        //         if(jobs_iter >1)
        //             jobs_iter--;
        //         for(int p=1;p<jobs_iter;p++){
        //             jobs_id[p]=p;
        //         }
        //     }    
        //     m++;    
        // }
        }
        else if(strcmp(arguments[0],"overkill")==0){        //"overkill" command handling
            for(int h=1;h<jobs_iter;h++){
                if(strcpy(jobs_status[h],"done")!=0){       // Killing the processes which have status other than done
                    kill_func(jobs_pid[h]);                 // Passing to kill_func function
                }
            }
        }   
        else if(strcmp(arguments[0],"kjob")==0){            //"kjob" command handling
            if(arguments[1]==NULL || arguments[2]==NULL)    //Handling invalid number of arguments passed
            {
                perror("Invalid number of arguments");
            }
            else{
                if(1<=atoi(arguments[2]) && atoi(arguments[2])<=31){    //Checking whether the signal number is valid or not
                    if ( kill(find_pid(atoi(arguments[1])),atoi(arguments[2]) ) < 0) {
                    printf("kjob %d: job not found\n", atoi(arguments[1]));
                    }
                }
                else{perror("kjob:Entered signumber is invalid");}  //Returning an error if signal number is invalid
            }
        }
        else if(strcmp(arguments[0],"quit")==0){        //"quit" command handling
            for(int h=1;h<jobs_iter;h++){
                if(strcpy(jobs_status[h],"done")!=0){       // Killing the processes which have status other than done
                    kill_func(jobs_pid[h]);                 // Passing to kill_func function
                }
            }
            exit(0);                                    // Exiting the shell
        }
        else if(strcmp(arguments[0],"setenv")==0){      //"setenv" command handling
            if(arguments[1]==NULL || arguments[3]!=NULL){printf("Number of arguments invalid\n");}
            else {                                                  //Returning error if number of arguments are invalid            
                if(arguments[2]==NULL){arguments[2]=" ";}
                setenv(arguments[1],arguments[2],1);    // running setenv command
            }
        }
        else if(strcmp(arguments[0],"unsetenv")==0){        //"unsetenv" command handling
            if(arguments[1]==NULL){printf("Number of arguments invalid\n");}    //Returning error if number of arguments are invalid
            else{
                unsetenv(arguments[1]);                 // running unsetenv command
            }
        }
        else if (strcmp(arguments[0], "cronjob") == 0)
        {
            int checker;
            pid_t pid_here;
            for(checker=0;arguments[checker]!=NULL;checker++){}
            if (checker>=6)
            {
                int size = checker + 2;
                char **here = (char**)malloc(sizeof(char*)*(size));
                int end = checker-4;
                for (int i=0;i<end;i++)
                {
                    if(i>=2){
                    int j=i-2;
                    here[j] = (char*)malloc(1000);
                    strcpy(here[j],arguments[i]);
                    }
                }
                here[checker-6] = NULL;
                int frequency = atoi(arguments[checker-3]);
                int max_time = atoi(arguments[checker-1]);
                pid_here = fork();
                if (pid_here == 0)
                {
                    for (int i=0;i<max_time/frequency;i++)
                    {
                        sleep(frequency);
                        int child = fork();
                        if (child==0)
                        {
                            if (execvp(here[0], here) < 0)
                            {
                                perror("sai:cmd not found");
                            }
                            exit(1);    
                        }
                    }
                }
            }
            else if(checker<6)
            {
                perror("sai:invalid number of arguments");
            }
        }
        else{
        pid_t pid,wpid;
        int status;
        pid = fork();
        kaka = pid;

        if(pid==0){
            signal(SIGINT, SIG_DFL);
            signal(SIGTSTP, SIG_DFL);
            setpgid(0,0);
            execvp(arguments[0],arguments);
            exit(EXIT_FAILURE);} 
            
        else {
            if(background_proc==0){                         // Handling foreground processes
                kaka = pid;
                proc_pid[proc_iter]=pid;
                strcpy(proc_names[proc_iter],piped_arguments[0]);
                proc_iter++;
                wpid = waitpid(pid, &status, WUNTRACED);        // Waiting the process until it is done

                if ( WIFEXITED(status)) 
                { 
                    int exit_status = WEXITSTATUS(status);         
                    // printf("Exit status of the child was %d\n",exit_status); 
                    // printf("%s with %d exited normally\n",arguments[0],pid);
                    set_status(find_id(wpid), "done");
                } else if (WIFSTOPPED(status)) {
                    set_status(find_id(wpid), "suspended");
                } else if (WIFCONTINUED(status)) {
                    set_status(find_id(wpid), "continued");
                }
            }
            else{
                    fir = pid;                                  // Handling background processes
                    jobs_id[jobs_iter]=jobs_iter;
                    strcpy(jobs_cmd[jobs_iter],piped_arguments[0]);
                    strcpy(jobs_status[jobs_iter],"running");
                    jobs_pid[jobs_iter]=fir;
                    jobs_iter++;
                    signal(SIGCHLD,handl);                      // passing to handl function to handle the exit of background
        }                                                       // process and printing messages
        }
        free(arguments);
        }
        if(temp_out>=0 && temp_err>=0){
            if(dup2(temp_out,1)==-1)perror("cmd failed");       // Restoring the fd 1 back
            if(dup2(temp_err,2)==-1)perror("cmd failed");       // Restoring the fd 2 back
            close(temp_out);
            close(temp_err);
        }
        if(temp_in>=0){
            if(dup2(temp_in,0)==-1)perror("cmd failed");        // Restoring the fd 0 back
            close(temp_in);
        }

    }
    return;
}
void shell_loop(){
    char username[500],systemname[500],curr_dir[500],home[500],print_cwd[500];
    char *input=NULL;
    char **total_args;
    char **piped_arguments;
    int it_chdir = -1;
    size_t input_length;
    ssize_t input_read;
    int home_status=0,check_home_currdir;
    while(1){
        
    cuserid(username);                              //Storing username/userid in variable username 
    gethostname(systemname,sizeof(systemname));     //Storing hostname in variable systemname
    getcwd(curr_dir,sizeof(curr_dir));              //Getting the current working directory and storing it in curr_dir
                                                    //  and will treat it as home directory of my shell(only for the forst iteration)                    
    int below_home=0;                               
    if(home_status == 0){
    strcpy(home,curr_dir);                          //Fixing the first directory accessed as home directory
    home_status = 1;
    }

    check_home_currdir = strcmp(home,curr_dir);     //Comparing the cwd and home directory    
    if(check_home_currdir != 0){                    //If cwd and home are not same updating the prompt
        for(int i=0;home[i]!='\0';i++){
            if(home[i]!=curr_dir[i])
            {
                if(strlen(home)>strlen(curr_dir)){      //Comparing the path of cwd with home dir of shell
                    below_home=1;
                    strcpy(print_cwd,curr_dir);
                }            
                break;
            }
                it_chdir = i;
        }
        if(it_chdir!=-1 && below_home==0){
        int count = 1;
        print_cwd[0]='~';
        for(int j=it_chdir+1;curr_dir[j]!='\0';){
            print_cwd[count++] = curr_dir[j++];         //Updating the prompt the path of directory from the home dir of shell
        }
        print_cwd[count]='\0';
        it_chdir=-1;
        }
    }
    else{                                           //If cwd and home are same displaying ~ in the prompt
        print_cwd[0]='~';
        print_cwd[1]='\0';
    }

    printf("<%s@%s:%s>",username,systemname,print_cwd);         //Printing the prompt
    int background_proc = 0;                                    //To check whether a process is background or not
    input_read = getline(&input,&input_length,stdin);           //Reading the input entered
    total_args = split_input(input,1);                          //Tokenizing the input with ; as delimiter
    history_cmd(input,home);                                    //Updating history with latest command entered                

    for(int k=0;total_args[k]!=NULL;k++){ 
        int temp_out = dup(fileno(stdout));                     //  Storing the stdout and stdin fd's so as to restore them back
        int temp_in = dup(fileno(stdin));                       //  after piping 
        piped_arguments = split_input(total_args[k],4);         //Tokenizing the input with | as delimiter
        int fd[2],in=0,out;
        pid_t procid;
        int l=0;
        for(l=0;piped_arguments[l+1]!=NULL;l++){
            pipe(fd);
            out = fd[1];
            pid_t pid;
                if(in!=0){
                    if(dup2(in,0)==-1)perror("cmd failed");         //Duplicating fd 0 as read end 
                    close(in);
                }
                if(out!=1){
                    if(dup2(out,1)==-1)perror("cmd failed");        //Duplicating fd 1 as write end
                    close(out);
                }
            danger(0,input_read,piped_arguments,l,home,username,systemname,curr_dir);   //Calling the execute function
            close(fd[1]);
            in = fd[0]; 
        }
        if(in!=0){
            if(dup2(in,0)==-1)perror("cmd failed");
            close(in);
        }
        if(out!=1){
            if(dup2(temp_out,1)==-1)perror("cmd failed");       //  Restoring fd 1 with its original as we reached to
            close(temp_out);                                    //  last command of pipe to be executed
        }
        danger(0,input_read,piped_arguments,l,home,username,systemname,curr_dir);   //Last cmd of pipe is passed to be executed
        if(in!=0){
            if(dup2(temp_in,0)==-1)perror("cmd failed");        //Restoring fd 0 with its original as pipes are done
            close(temp_in);
        }
    }
}
}