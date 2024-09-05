#include "s21_decimal.h"

int s21_negate(s21_decimal value, s21_decimal* result) {
  int error_code = 0;

  if (detectDecimalError(value)) {
    error_code = 1;
  }

  else {
    for (int i = 0; i < 4; i++) {
      result->bits[i] = value.bits[i];
    }

    if (getDecimalSign(value)) {
      clearBit(result, 127);
    } else {
      setDecimalBitToOne(result, 127);
    }
  }

  return error_code;
}

int s21_floor(s21_decimal value, s21_decimal* result) {
  int error_code = 0;

  s21_decimal remainder = {{0}};
  s21_decimal divisor = {{10, 0, 0, 0}};
  s21_decimal multiplier = {{10, 0, 0, 0}};

  if (detectDecimalError(value)) {
    error_code = 1;
  }

  else {
    clearData(result);

    int scale = getDecimalScale(value);
    int sign = getDecimalSign(value);
    value.bits[3] = 0;

    for (int i = 0; i < scale - 1; i++) {
      s21_mul(divisor, multiplier, &divisor);
    }

    if (scale == 0) {
      divisor.bits[0] = divisor.bits[1] = divisor.bits[2] = divisor.bits[3] = 0;
    }

    if (!sign) {
      setDecimalScale(&remainder, scale);

      remainder.bits[0] = value.bits[0];
      remainder.bits[1] = value.bits[1];
      remainder.bits[2] = value.bits[2];

      s21_truncate(remainder, result);
    } else {
      if (scale) {
        s21_add(value, divisor, &remainder);
        setDecimalScale(&remainder, scale);
        setDecimalBitToOne(&remainder, 127);
        s21_truncate(remainder, result);
      }

      else {
        result->bits[0] = value.bits[0];
        result->bits[1] = value.bits[1];
        result->bits[2] = value.bits[2];

        setDecimalScale(result, scale);
        setDecimalBitToOne(result, 127);
      }
    }
  }

  return error_code;
}

int s21_truncate(s21_decimal value, s21_decimal* result) {
  int error_code = 0;

  s21_decimal divisor = {{10, 0, 0, 0}};
  s21_decimal multiplier = {{10, 0, 0, 0}};
  s21_decimal remainder = {0};

  int scale = getDecimalScale(value);
  int sign = getDecimalSign(value);

  if (detectDecimalError(value)) {
    error_code = 1;
  }

  else if (scale) {
    clearData(result);
    value.bits[3] = 0;

    for (int i = 0; i < scale - 1; i++) {
      s21_mul(divisor, multiplier, &divisor);
    }

    calculateModulus(value, divisor, &remainder);
    s21_sub(value, remainder, &remainder);
    s21_div(remainder, divisor, result);
  }

  else {
    result->bits[0] = value.bits[0];
    result->bits[1] = value.bits[1];
    result->bits[2] = value.bits[2];
  }

  if (sign) {
    setDecimalBitToOne(result, 127);
  }

  return error_code;
}

int s21_round(s21_decimal value, s21_decimal* result) {
  int error_code = 0;

  s21_decimal remainder = {{0}};
  s21_decimal remainder2 = {{0}};
  s21_decimal divisor = {{10, 0, 0, 0}};
  s21_decimal multiplier = {{10, 0, 0, 0}};

  if (detectDecimalError(value)) {
    error_code = 1;
  }

  else {
    clearData(result);

    int scale = getDecimalScale(value);
    int sign = getDecimalSign(value);
    value.bits[3] = 0;

    for (int i = 0; i < scale - 1; i++) {
      s21_mul(divisor, multiplier, &divisor);
    }

    calculateModulus(value, divisor, &remainder);
    s21_div(divisor, multiplier, &divisor);
    calculateModulus(remainder, divisor, &remainder2);
    s21_sub(remainder, remainder2, &remainder);
    s21_div(remainder, divisor, &remainder);

    if (remainder.bits[0] >= 5) {
      s21_mul(divisor, multiplier, &divisor);
      s21_add(value, divisor, &remainder);
      setDecimalScale(&remainder, scale);
      s21_truncate(remainder, result);
    }

    else if (scale) {
      s21_mul(divisor, multiplier, &divisor);
      calculateModulus(value, divisor, &remainder);
      s21_sub(value, remainder, &remainder);
      s21_div(remainder, divisor, result);
    }

    else {
      result->bits[0] = value.bits[0];
      result->bits[1] = value.bits[1];
      result->bits[2] = value.bits[2];
    }

    if (sign) {
      setDecimalBitToOne(result, 127);
    }
  }

  return error_code;
}
