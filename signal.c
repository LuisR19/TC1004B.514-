#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int i = 1;

void hola(int signalNumb){
    printf("Recibi la senial %d ", signalNumb);
}

void terminarwhile(int signalNumber){
    printf("Terminando while");
    i = 0;
}

int main(){
    signal(12,terminarwhile);
    signal(2,hola);
    while(1 == i){
        printf("trabajando\n");
        sleep(1);
    }
    printf("Aqui nunca llega\n");
    return 0;
}

