#include <stdio.h>
#include <ctype.h>
#include <unistd.h>

int main(){
    char c;
    char *msg = "1 \n";
    char *msg1 = "No es vocal\n";
    char may;
    int n;
    do {
        n = read(STDIN_FILENO,&c,1);
        may = toupper(c);
        if(may == 'A' || may == 'E'|| may == 'I' || may == 'O' || may == 'U' || may == 'a' || may == 'e'|| may == 'i' || may == 'o' || may == 'u'){
            write(STDOUT_FILENO,msg,14);
        } 
    }while(n != 0);

    return 0;
}