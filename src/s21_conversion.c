#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal* dst) {
  int status = OK;

  if (dst == NULL) {
    status = CONVERSION_ERROR;
  } else {
    status = convertIntToDecimal(src, dst);
  }

  return status;
}

int s21_from_decimal_to_float(s21_decimal src, float* dst) {
  int status = OK;

  if (dst == NULL) {
    status = CONVERSION_ERROR;
  } else {
    status = convertDecimalToFloat(src, dst);
  }

  return status;
}

int s21_from_float_to_decimal(float src, s21_decimal* dst) {
  clearData(dst);
  int error_code = 0;
  if (isNan(src) || isInf(src) || fabs(src) > S21_DEC_MAX ||
      fabs(src) < 1e-28f) {
    error_code = 1;
  } else {
    int float_exp = 0, neg_flag = src < 0;
    double double_src = (double)fabs(src);
    float float_temp = 0;
    int scale = prepareFloat(double_src, &float_temp, &float_exp);
    unsigned int int_src = *((unsigned int*)&float_temp);
    if (src != 0) setDecimalBitToOne(dst, float_exp);
    float_exp--;
    for (int i = 22; i >= 0; float_exp--, i--) {
      if ((int_src >> i) & 1) setDecimalBitToOne(dst, float_exp);
    }
    setDecimalScale(dst, scale);
    if (neg_flag) setDecimalBitToOne(dst, 127);
  }
  return error_code;
}

int s21_from_decimal_to_int(s21_decimal src, int* dst) {
  int error = 0;
  if (detectDecimalError(src)) {
    error = 1;
  } else {
    long double temp = convertDecimalToLongDouble(src);
    if (fabsl(temp) >= INT_MAX) {
      error = 1;
    } else {
      *dst = (int)temp;
    }
  }
  return error;
}

int prepareFloat(double input, float* roundedFloat, int* exponent) {
  int scale = 0;

  for (; scale <= 28 && !((int)input / 1000000) && (int)input != input;
       input *= 10, scale++) {
  }

  *roundedFloat = (float)round(input);

  for (; scale > 0 && !((int)(*roundedFloat) % 10);
       (*roundedFloat) /= 10, scale--) {
  }

  *exponent = extractFloatExponent(*roundedFloat);

  return scale;
}

int extractFloatExponent(float number) {
  int exponent = 0;
  int bitValue = 0;
  int mask = 1 << 23;
  unsigned int floatBits = *((unsigned int*)&number);

  for (int i = 0; i < 8; i++) {
    bitValue = !!(floatBits & mask);
    mask <<= 1;
    if (bitValue == 1) exponent += pow(2, i);
  }

  exponent -= 127;

  return exponent;
}
