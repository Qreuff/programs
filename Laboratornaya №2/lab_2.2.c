#include <stdio.h>
void main(){
float a;
char d;
printf("Input Temperature: ");
scanf(%f %c", &a,&d);
if (d == 'f' || d == 'F'){
a = (a-32)*5/9;
printf("Output Temperature:%fc\n",a);
}else if (d == 'c' || d == 'C'){
a = (a*9/5)+32;
printf("Output Temperature:%fF\n",a);
}else printf("output Error\n");
}
}