#include "s21_decimal.h"

int s21_is_equal(s21_decimal number_1, s21_decimal number_2) {
  int result = 1;
  if (s21_is_zero(number_1) && s21_is_zero(number_2)) {
  } else {
    int sign_number_1 = getDecimalSign(number_1);
    int sign_number_2 = getDecimalSign(number_2);
    s21_long long_number_1 = convertDecimalToLong(number_1);
    s21_long long_number_2 = convertDecimalToLong(number_2);
    alignDecimalScales(&long_number_1, &long_number_2,
                       getDecimalScale(number_1), getDecimalScale(number_2));
    if (sign_number_1) {
      setLongBit(&long_number_1, 223, 1);
    }
    if (sign_number_2) {
      setLongBit(&long_number_2, 223, 1);
    }
    for (int i = 0; i < 224; i++) {
      if (getLongBit(long_number_1, i) != getLongBit(long_number_2, i)) {
        result = 0;
      }
    }
  }
  return result;
}

int s21_is_not_equal(s21_decimal number_1, s21_decimal number_2) {
  int result = 0;
  if (s21_is_zero(number_1) && s21_is_zero(number_2)) {
  } else {
    int sign_number_1 = getDecimalSign(number_1);
    int sign_number_2 = getDecimalSign(number_2);
    s21_long long_number_1 = convertDecimalToLong(number_1);
    s21_long long_number_2 = convertDecimalToLong(number_2);
    alignDecimalScales(&long_number_1, &long_number_2,
                       getDecimalScale(number_1), getDecimalScale(number_2));
    if (sign_number_1) {
      setLongBit(&long_number_1, 223, 1);
    }
    if (sign_number_2) {
      setLongBit(&long_number_2, 223, 1);
    }
    for (int i = 0; i < 224; i++) {
      if (getLongBit(long_number_1, i) != getLongBit(long_number_2, i)) {
        result = 1;
      }
    }
  }
  return result;
}

int s21_is_less(s21_decimal number_1, s21_decimal number_2) {
  int result = 1;
  if (s21_is_equal(number_1, number_2)) {
    result = 0;
  } else {
    int sign_number_1 = getDecimalSign(number_1);
    int sign_number_2 = getDecimalSign(number_2);

    if (!sign_number_1 && sign_number_2) {
      result = 0;
    } else if (sign_number_1 && !sign_number_2) {
      result = 1;
    } else if (sign_number_1 && sign_number_2) {
      clearBit(&number_1, 127);
      clearBit(&number_2, 127);
      result = s21_is_greater(number_1, number_2);
    } else {
      s21_long long_number_1 = convertDecimalToLong(number_1);
      s21_long long_number_2 = convertDecimalToLong(number_2);

      alignDecimalScales(&long_number_1, &long_number_2,
                         getDecimalScale(number_1), getDecimalScale(number_2));

      int tempres = compareLongDecimal(long_number_1, long_number_2);

      if (tempres == 2) {
        result = 1;
      } else {
        result = 0;
      }
    }
  }

  return result;
}

int s21_is_less_or_equal(s21_decimal number_1, s21_decimal number_2) {
  int result = 1;
  if (s21_is_equal(number_1, number_2)) {
    result = 1;
  } else {
    int sign_number_1 = getDecimalSign(number_1);
    int sign_number_2 = getDecimalSign(number_2);

    if (!sign_number_1 && sign_number_2) {
      result = 0;
    } else if (sign_number_1 && !sign_number_2) {
      result = 1;
    } else if (sign_number_1 && sign_number_2) {
      clearBit(&number_1, 127);
      clearBit(&number_2, 127);
      result = s21_is_greater_or_equal(number_1, number_2);
    } else {
      s21_long long_number_1 = convertDecimalToLong(number_1);
      s21_long long_number_2 = convertDecimalToLong(number_2);
      alignDecimalScales(&long_number_1, &long_number_2,
                         getDecimalScale(number_1), getDecimalScale(number_2));
      int tempres = compareLongDecimal(long_number_1, long_number_2);
      if (tempres == 2 || s21_is_equal(number_1, number_2)) {
        result = 1;
      } else if (tempres == 0) {
        result = 1;
      } else {
        result = 0;
      }
    }
  }
  return result;
}

int s21_is_greater(s21_decimal number_1, s21_decimal number_2) {
  return (s21_is_less(number_2, number_1));
}

int s21_is_greater_or_equal(s21_decimal number_1, s21_decimal number_2) {
  return (s21_is_less_or_equal(number_2, number_1));
}
