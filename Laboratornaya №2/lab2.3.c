#include <stdio.h>
void main(){
int d;
int a;
printf("Type year: ");
scanf(" %d", &a);
if (a%400==0){printf("yes \n");}
else if (a%100==0){printf("no \n");}
else if (a % 4 == 0){printf("yes \n");}
else{printf("yes \n");
}
}