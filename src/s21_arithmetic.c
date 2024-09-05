#include "s21_decimal.h"

s21_long _LONGDEC10 = {{10, 0, 0, 0, 0, 0, 0}};

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  clearData(result);
  int sign = 0;
  s21_long long_dec1 = convertDecimalToLong(value_1);
  s21_long long_dec2 = convertDecimalToLong(value_2);
  s21_long long_res = {{0, 0, 0, 0, 0, 0, 0}};

  int scale =
      alignDecimalScales(&long_dec1, &long_dec2, getDecimalScale(value_1),
                         getDecimalScale(value_2));
  int flagOne = compareLongDecimal(long_dec1, long_dec2);
  if (getDecimalBit(value_1, 127) || getDecimalBit(value_2, 127)) {
    if (flagOne == 1) {
      s21_longAdder(long_dec1, long_dec2, &long_res, 1);
      sign = getDecimalBit(value_1, 127);
    } else if (flagOne == 2) {
      s21_longAdder(long_dec2, long_dec1, &long_res, 1);
      sign = getDecimalBit(value_2, 127);
    }
  } else {
    s21_longAdder(long_dec1, long_dec2, &long_res, 0);
    sign = getDecimalBit(value_1, 127);
  }
  *result = convertLongToDecimal(long_res, scale, sign);
  return checkErrorFlags(result);
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int znak = 0;
  s21_long long_dec1 = convertDecimalToLong(value_1);
  s21_long long_dec2 = convertDecimalToLong(value_2);
  s21_long long_res = {{0, 0, 0, 0, 0, 0, 0}};
  int scale =
      alignDecimalScales(&long_dec1, &long_dec2, getDecimalScale(value_1),
                         getDecimalScale(value_2));
  int flagOne = compareLongDecimal(long_dec1, long_dec2);
  if (getDecimalBit(value_1, 127) ^ getDecimalBit(value_2, 127)) {
    s21_longAdder(long_dec1, long_dec2, &long_res, 0);
    znak = getDecimalBit(value_1, 127);
  } else {
    if (flagOne == 1) {
      s21_longAdder(long_dec1, long_dec2, &long_res, 1);
      znak = getDecimalBit(value_1, 127);
    } else if (flagOne == 2) {
      s21_longAdder(long_dec2, long_dec1, &long_res, 1);
      znak = !getDecimalBit(value_2, 127);
    }
  }
  setLongBit(&long_res, 223, 0);
  *result = convertLongToDecimal(long_res, scale, znak);
  return checkErrorFlags(result);
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  clearData(result);
  int scale1 = getDecimalScale(value_1);
  int scale2 = getDecimalScale(value_2);
  int scale = scale1 + scale2;
  s21_long longDec1 = convertDecimalToLong(value_1);
  s21_long longDec2 = convertDecimalToLong(value_2);
  s21_long longRes = {{0, 0, 0, 0, 0, 0, 0}};

  int flScale = s21_longMultiplication(longDec1, longDec2, &longRes);

  if (flScale < 0) {
    printf("aboba");
  }
  *result = convertLongToDecimal(
      longRes, scale,
      getDecimalBit(value_1, 127) ^ getDecimalBit(value_2, 127));
  return checkErrorFlags(result);
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int flag = 0;
  clearData(result);
  s21_long long_dec1 = convertDecimalToLong(value_1);
  s21_long long_dec2 = convertDecimalToLong(value_2);
  s21_long long_res = {{0, 0, 0, 0, 0, 0, 0}};
  s21_long temp = {{0, 0, 0, 0, 0, 0, 0}};
  if (isNonZeroDecimal(value_2)) {
    int znak = getDecimalBit(value_1, 127) ^ getDecimalBit(value_2, 127);
    alignDecimalScales(&long_dec1, &long_dec2, getDecimalScale(value_1),
                       getDecimalScale(value_2));
    s21_longDivision(&long_dec1, long_dec2, &long_res);
    int i = 0;
    for (; i < 29 && isNonZeroLong(long_dec1); i++) {
      clearLongData(&temp);
      s21_longMultiplication(long_dec1, _LONGDEC10, &long_dec1);
      s21_longDivision(&long_dec1, long_dec2, &temp);
      s21_longMultiplication(long_res, _LONGDEC10, &long_res);
      s21_longAdder(long_res, temp, &long_res, 0);
    }
    *result = convertLongToDecimal(long_res, i, znak);
    flag = checkErrorFlags(result);
  } else {
    flag = 3;
  }
  return flag;
}
