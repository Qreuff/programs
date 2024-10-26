#include <stdio.h>

int main() 
{
    int a,b,c;
    a = 1;
    b = 2;
    c = 3; int c1 = 3;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 1; j++)
        {
            printf("%d\t",a);
            printf("%d\t",b);
            printf("%d\t",c);
            printf("\n");
        }
        a = a + c1;
        b = b + c1;
        c = c + c1;
    }
}
