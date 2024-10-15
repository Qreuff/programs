#include <stdio.h>
int main(){
int a;
printf("Введите число: ");
scanf("%d", &a);
if ((a & 15) == 0) {
printf("%d да", a);
}else{
printf("%d нет", a); 
}
}
//номер 11
