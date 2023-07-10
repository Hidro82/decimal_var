#include "s21_decimal.h"

/* Данная функция делает смещение числа типа decimal
вперёд на один символ. Нужна при выполнении операции умножения */

// void s21_shift(s21_decimal *number) {

/* Данная функция выравнивает степень десятки у двух чисел */

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int bit_1;
  int sign_1 = get_sign(&value_1);
  int sign_2 = get_sign(&value_2);
  s21_decimal adder = value_1;
  int exp1;
  int exp2;
  int error = 0;
  exp1 = s21_10_conv(value_1);
  exp2 = s21_10_conv(value_2);
  s21_rev_10_conv(result, exp1 + exp2);
  set_0_bit(&adder.bits[3], 31);  // модуль копии вал_1
  for (int i = 0; i < 3; i++) {  // проходимся по всей мантисе
    for (int j = 0; j < 32; j++) {
      bit_1 = test_bit(value_2.bits[i], j);
      if (bit_1) {
        s21_addiction_logic(*result, adder, result);
      }
      error = s21_shift(&adder);
    }
  }
  if (sign_1 != sign_2)
    set_1_bit(&result->bits[3], 31);
  else
    set_0_bit(&result->bits[3], 31);
  return error;
}
