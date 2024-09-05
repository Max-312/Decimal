#ifndef _S21_BINARY_ARITHMETIC_H_
#define _S21_BINARY_ARITHMETIC_H_

int s21_bitAdder(int bit_1, int bit_2, int *carry);
void s21_adder(s21_decimal number_1, s21_decimal number_2, s21_decimal *result,
               int operation);
void s21_longAdder(s21_long number_1, s21_long number_2, s21_long *result,
                   int operation);
void s21_longDivision(s21_long *dividend, s21_long divisor, s21_long *quotient);
int s21_longMultiplication(s21_long multiplier_1, s21_long multiplier_2,
                           s21_long *result);

#endif