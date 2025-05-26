// test_bit_utils.c
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

// Функция, которую тестируем
int find_first_zero_bit(int number) {
    for (int i = 0; i < 32; i++) {
        if ((number & (1 << i)) == 0)
            return i;
    }
    return -1;
}

// Тесты
static void test_has_zero_bit(void **state) {
    (void)state;

    assert_int_equal(find_first_zero_bit(0b10101111), 4);  // Первый ноль на 4-й позиции
    assert_int_equal(find_first_zero_bit(0b11111110), 0);  // Первый ноль на 0-й позиции
    assert_int_equal(find_first_zero_bit(0b11111101), 1);  // Первый ноль на 1-й позиции
}

static void test_all_bits_one(void **state) {
    (void)state;

    assert_int_equal(find_first_zero_bit(-1), -1); // Все биты равны 1 (-1 в доп коде)
}

int main(void) {
    const struct CMUnitTest tests[] = {
        int main(void) {
            const struct CMUnitTest tests[] = {
                cmocka_unit_test(test_has_zero_bit),  // Добавляем тест test_has_zero_bit
                cmocka_unit_test(test_all_bits_one),  // Добавляем тест test_all_bits_one
            };
            return cmocka_run_group_tests(tests, NULL, NULL);  // Запускаем все тесты
        }
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}