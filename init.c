#include "header.h"
int main(int argc, char const *argv[]){
    signal(SIGTSTP, just);
signal(SIGINT,kid);
    
shell_loop();
}