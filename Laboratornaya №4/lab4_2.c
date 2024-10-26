#include <stdio.h>
int main() {
for (int i = 0; i < 7; i++) {
for (int j = 0; j < i; j++) {
printf(" "); }
for (int k = i; k < 7; k++) {
printf("%c", 'A' + i); }
printf("\n");
}
return 0;
}
