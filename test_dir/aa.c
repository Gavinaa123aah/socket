#include<stdio.h>
#include<unistd.h>
int factorial (int n); 

int main (){
    int a = 10;
    int b = 0;
    b = factorial(a);
    printf("result : %d",b);

    return 0;
}