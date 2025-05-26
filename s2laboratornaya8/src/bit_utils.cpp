#include <stdio.h>
// Ищет номер первого (самого правого) нулевого бита в числе
int find_first_zero_bit(int number) {
    for (int i = 0; i < 32; i++) {
        if ((number & (1 << i)) == 0) {
            return i;
        }
    }
    return -1; // Все биты равны 1
}