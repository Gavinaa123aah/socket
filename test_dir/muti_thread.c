#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int sharedi = 0;
void increse_num(char *ar);

int main(){
    int ret;
    pthread_t thrd1, thrd2, thrd3;
    char *TEM = "HELLO WORLD";

    ret = pthread_create(&thrd1, NULL, (void *)increse_num, TEM);
    ret = pthread_create(&thrd2, NULL, (void *)increse_num, NULL);
    ret = pthread_create(&thrd3, NULL, (void *)increse_num, NULL);

    pthread_join(thrd1, NULL);
    pthread_join(thrd2, NULL);
    pthread_join(thrd3, NULL);

    // puts(thrd1);
    printf("sharedi = %d\n", sharedi);

    return 0;

}

void increse_num(char *ar) {
    long i,tmp;
    for(i=0; i<=10; i++) {
        tmp = sharedi;
        tmp = tmp + 1;
        sharedi = tmp;
    }
    pthread_t thId = pthread_self();
    puts(thId);
    puts(ar);
}