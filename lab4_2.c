#include <stdio.h>
int main() {
for (int i = 0; i < 5; i++) {
for (int j = 0; j < i; j++) {
printf(" "); }
for (int k = i; k < 5; k++) {
printf("%c", 'A' + i); }
printf("\n");
}
return 0;
}
