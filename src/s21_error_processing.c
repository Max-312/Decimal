#include "s21_decimal.h"

int isNonZeroDecimal(s21_decimal decimal) {
  int isNonZero = 0;
  for (int i = 0; i < 3 && !isNonZero; i++) {
    if (decimal.bits[i] != 0) {
      isNonZero = 1;
    }
  }
  return isNonZero;
}

int isNonZeroLong(s21_long longDecimal) {
  int isNonZero = 0;
  for (int i = 0; i < 7 && !isNonZero; i++) {
    if (longDecimal.bits[i] != 0) {
      isNonZero = 1;
    }
  }
  return isNonZero;
}

int checkErrorFlags(s21_decimal *decimalResult) {
  int errorCode = 0;
  if (getDecimalBit(*decimalResult, 100) &&
      !getDecimalBit(*decimalResult, 127)) {
    errorCode = 1;
  } else if (getDecimalBit(*decimalResult, 101) ||
             (getDecimalBit(*decimalResult, 100) &&
              getDecimalBit(*decimalResult, 127))) {
    errorCode = 2;
  }
  setDecimalBit(decimalResult, 100, 0);
  setDecimalBit(decimalResult, 101, 0);
  return errorCode;
}

int detectDecimalError(s21_decimal decimal) {
  int error_detected = 0;
  int current_bit_value = 0;

  for (int i = 0; i < 4 && !error_detected; i++) {
    current_bit_value = decimal.bits[i];
    if (current_bit_value == S2_INF || isNan(current_bit_value) ||
        getDecimalScale(decimal) > 28 || current_bit_value == -S2_INF) {
      error_detected = 1;
    }
  }

  return error_detected;
}

int isNan(long double value) { return value != value ? 1 : 0; }
int isInf(float value) { return (value == 1.0 / 0.0 || value == -1.0 / 0.0); }