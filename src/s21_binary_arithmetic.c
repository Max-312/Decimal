#include "s21_decimal.h"

int s21_bitAdder(int bit_1, int bit_2, int *carry) {
  int sum = (bit_1 ^ bit_2) ^ *carry;
  *carry = (bit_1 & bit_2) | ((bit_1 ^ bit_2) & *carry);

  return sum;
}

void s21_adder(s21_decimal number_1, s21_decimal number_2, s21_decimal *result,
               int operation) {
  int carry = operation;

  for (int bit_index = 0; bit_index < 96; bit_index++) {
    setDecimalBit(
        result, bit_index,
        s21_bitAdder(getDecimalBit(number_1, bit_index),
                     operation ^ getDecimalBit(number_2, bit_index), &carry));

    setDecimalBit(result, bit_index + 1, carry);
  }
}

void s21_longAdder(s21_long number_1, s21_long number_2, s21_long *result,
                   int operation) {
  int carry = operation;

  for (int bit_index = 0; bit_index < 223; bit_index++) {
    setLongBit(
        result, bit_index,
        s21_bitAdder(getLongBit(number_1, bit_index),
                     operation ^ getLongBit(number_2, bit_index), &carry));

    setLongBit(result, bit_index + 1, carry);
  }
}

void s21_longDivision(s21_long *dividend, s21_long divisor,
                      s21_long *quotient) {
  clearLongData(quotient);

  s21_long remainder = {{0, 0, 0, 0, 0, 0, 0}};
  s21_long shifted_divisor = {{0, 0, 0, 0, 0, 0, 0}};

  int bit_difference =
      getFirstSetBitIndex(*dividend) - getFirstSetBitIndex(divisor);

  for (int i = 0; i <= bit_difference; i++) {
    clearLongData(&remainder);

    shifted_divisor = divisor;

    shiftLongBits(&shifted_divisor, bit_difference - i);
    s21_longAdder(*dividend, shifted_divisor, &remainder, 1);

    if (getLongBit(remainder, 223)) {
      setLongBit(quotient, 0, 1);
      setLongBit(&remainder, 223, 0);
      s21_longAdder(*dividend, shifted_divisor, dividend, 1);
      setLongBit(dividend, 223, 0);
    }

    if (i < bit_difference) {
      shiftLongBits(quotient, 1);
    }
  }
}

int s21_longMultiplication(s21_long multiplier_1, s21_long multiplier_2,
                           s21_long *result) {
  clearLongData(result);
  int overflow_flag = 1;

  for (int bit_index_1 = 0; bit_index_1 < 221 && overflow_flag; bit_index_1++) {
    s21_long temp_product = {{0, 0, 0, 0, 0, 0, 0}};
    clearLongData(&temp_product);

    for (int bit_index_2 = 0; bit_index_2 < 221 && overflow_flag;
         bit_index_2++) {
      if (getLongBit(multiplier_2, bit_index_1)) {
        setLongBit(&temp_product, bit_index_2 + bit_index_1,
                   getLongBit(multiplier_1, bit_index_2) &
                       getLongBit(multiplier_2, bit_index_1));

        if (getLongBit(multiplier_1, bit_index_2) &&
            bit_index_1 + bit_index_2 >= 221) {
          overflow_flag = 0;
        }
      }
    }

    if (getLongBit(multiplier_2, bit_index_1)) {
      s21_longAdder(*result, temp_product, result, 0);
    }
  }

  return overflow_flag;
}
