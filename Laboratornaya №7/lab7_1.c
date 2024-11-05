#include <stdio.h>
#include <string.h>

int main() {
    char words[]="Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ";
    int l = strlen(words);
    for(int i = 0; i<l / 2;i++)
    {
       char r = words[i];
       words[i] = words[l - 1 - i];
       words[l - 1 - i] = r;
    }
    printf("%s\n",words);
}