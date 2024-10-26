#include <stdio.h>
int main(){
char a,b;
printf("Введите число: ");
scanf("%hhd",&a);
printf("Введите кол-во позиций сдвига: ");
scanf("%hhd",&b);
int k = (a<<b)|(a>>(8-b));
printf("%d",k);
}
//5я задача
