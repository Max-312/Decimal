#include "s21_decimal.h"

int getDecimalSign(s21_decimal decimal) { return getDecimalBit(decimal, 127); }

int getBitAtIndex(s21_decimal decimal_value, int bit_position) {
  return (decimal_value.bits[bit_position / 32] >> (bit_position % 32)) & 1;
}

int getFirstSetBitIndex(s21_long long_data) {
  int first_set_bit_index = 0;
  for (int i = 222; i >= 0 && first_set_bit_index == 0; i--) {
    if (getLongBit(long_data, i)) {
      first_set_bit_index = i;
    }
  }
  return first_set_bit_index;
}

void clearData(s21_decimal *number) {
  s21_decimal empty_value = {{0, 0, 0, 0}};
  *number = empty_value;
}

void clearLongData(s21_long *number) {
  s21_long empty_value = {{0, 0, 0, 0, 0, 0, 0}};
  *number = empty_value;
}

void clearBit(s21_decimal *decimal, int bit_position) {
  int mask = 1;
  int bit_group_index = decimal->bits[bit_position / 32];
  for (int i = 0; i < bit_position % 32; i++) {
    mask <<= 1;
  }
  mask = ~mask;
  decimal->bits[bit_position / 32] = bit_group_index & mask;
}

int shiftLongBits(s21_long *number, int shift_amount) {
  int msb_index = getFirstSetBitIndex(*number);

  if ((msb_index + shift_amount) < 223) {
    for (int i = msb_index; i >= 0; i--) {
      setLongBit(number, i + shift_amount, getLongBit(*number, i));
    }

    for (int i = shift_amount - 1; i >= 0; i--) {
      setLongBit(number, i, 0);
    }

  } else {
    msb_index = -1;
  }

  return msb_index;
}

int getDecimalBit(s21_decimal decimal, int bit_position) {
  return (decimal.bits[bit_position / 32] >> bit_position % 32) & 1;
}
void setDecimalBit(s21_decimal *decimal, int bit_position, int bit_value) {
  decimal->bits[bit_position / 32] ^=
      (getDecimalBit(*decimal, bit_position) ^ bit_value)
      << (bit_position % 32);
}

void setDecimalBitToOne(s21_decimal *decimal, int bit_position) {
  decimal->bits[bit_position / 32] |= (1U << (bit_position % 32));
}

int getLongBit(s21_long long_data, int bit_position) {
  return (long_data.bits[bit_position / 32] >> bit_position % 32) & 1;
}

void setLongBit(s21_long *long_data, int bit_position, int bit_value) {
  long_data->bits[bit_position / 32] ^=
      (getLongBit(*long_data, bit_position) ^ bit_value) << (bit_position % 32);
}

void setDecimalSign(s21_decimal *decimal, int sign) {
  unsigned int signMask = 1u << 31;

  if (sign != 0) {
    decimal->bits[3] |= signMask;
  } else {
    decimal->bits[3] &= ~signMask;
  }
}

s21_long convertDecimalToLong(s21_decimal decimal) {
  s21_long result = {{0, 0, 0, 0, 0, 0, 0}};
  for (int i = 0; i < 3; i++) {
    result.bits[i] = decimal.bits[i];
  }
  return result;
}

s21_decimal convertLongToDecimal(s21_long long_result, int scale, int sign) {
  int error_code = 0;
  s21_decimal decimal_result = {{0, 0, 0, 0}};
  int rounding_digit = 0;
  s21_long temp_result = {{0, 0, 0, 0, 0, 0, 0}};
  while ((getFirstSetBitIndex(long_result) > 95 || scale > 28) && scale > 0) {
    temp_result = long_result;
    s21_longDivision(&temp_result, _LONGDEC10, &long_result);
    rounding_digit = temp_result.bits[0];
    scale--;
    error_code = 2;
  }
  if (getFirstSetBitIndex(long_result) > 95) {
    error_code = 1;
    clearData(&decimal_result);
  }
  for (int i = 0; i < 3; i++) {
    decimal_result.bits[i] = long_result.bits[i];
  }
  s21_decimal one = {{1, 0, 0, 0}};
  if (rounding_digit > 4 && rounding_digit < 10) {
    s21_adder(decimal_result, one, &decimal_result, 0);
  }
  setDecimalScale(&decimal_result, scale);
  setDecimalBit(&decimal_result, 127, sign);
  if (error_code == 1) {
    setDecimalBit(&decimal_result, 100, 1);
  } else if (error_code == 2 && !isNonZeroDecimal(decimal_result)) {
    setDecimalBit(&decimal_result, 101, 1);
  }
  return decimal_result;
}

int convertDecimalToFloat(s21_decimal src, float *dst) {
  unsigned int scale = 0;
  int sign = 0;
  double result = 0.0;

  sign = getBitAtIndex(src, 127);

  for (int i = 0; i < 96; i++) {
    if (getBitAtIndex(src, i)) {
      result += pow(2, i);
    }
  }

  scale = (src.bits[3] >> 16) & 0xFF;
  result /= pow(10, scale);

  if (sign) {
    result = -result;
  }

  result = round(result * 1e7) / 1e7;

  *dst = (float)result;

  return OK;
}
int convertIntToDecimal(int src, s21_decimal *dst) {
  dst->bits[0] = 0;
  dst->bits[1] = 0;
  dst->bits[2] = 0;
  dst->bits[3] = 0;

  if (src < 0) {
    setDecimalSign(dst, 1);
    src = -src;
  }

  dst->bits[0] = (unsigned int)src;

  return OK;
}
long double convertDecimalToLongDouble(s21_decimal decimal) {
  long double result = 0;
  int mask = 1, bitValue = 0;

  for (int i = 0; i < 3; i++) {
    for (int bitPosition = 0; bitPosition < 32; bitPosition++) {
      bitValue = !!(decimal.bits[i] & mask);
      mask <<= 1;
      if (bitValue == 1) result += pow(2, bitPosition + (i * 32));
    }
    mask = 1;
  }

  int scale = getDecimalScale(decimal);
  for (int i = 0; i < scale; i++) result /= 10;

  if (getDecimalSign(decimal)) result *= -1;

  return result;
}

int compareLongDecimal(s21_long long_number_1, s21_long long_number_2) {
  int compare_result = 0;
  for (int i = 222; i >= 0 && compare_result == 0; i--) {
    if (getLongBit(long_number_1, i) > getLongBit(long_number_2, i)) {
      compare_result = 1;
    } else if (getLongBit(long_number_1, i) < getLongBit(long_number_2, i)) {
      compare_result = 2;
    }
  }
  return compare_result;
}

int getDecimalScale(s21_decimal decimal) {
  int scale_value = 0;
  for (int i = 119; i > 111; i--) {
    if (getDecimalBit(decimal, i)) {
      scale_value += pow(2, i - 112);
    }
  }
  return scale_value;
}

int alignDecimalScales(s21_long *first_long_number,
                       s21_long *second_long_number, int scale_1, int scale_2) {
  if (scale_1 < scale_2) {
    while (scale_1 < scale_2) {
      s21_longMultiplication(*first_long_number, _LONGDEC10, first_long_number);
      scale_1++;
    }
  } else if (scale_1 > scale_2) {
    while (scale_2 < scale_1) {
      s21_longMultiplication(*second_long_number, _LONGDEC10,
                             second_long_number);
      scale_2++;
    }
  }
  return scale_1;
}

void setDecimalScale(s21_decimal *decimal, int scale) {
  for (int i = 112; i < 120; i++) {
    setDecimalBit(decimal, i, (scale >> (i - 112)) & 1);
  }
}

int calculateModulus(s21_decimal dividend, s21_decimal divisor,
                     s21_decimal *remainder) {
  int error_code = 0;
  clearData(remainder);
  s21_long long_dividend = convertDecimalToLong(dividend);
  s21_long long_divisor = convertDecimalToLong(divisor);
  s21_long long_remainder = {{0, 0, 0, 0, 0, 0, 0}};
  s21_long temp_result = {{0, 0, 0, 0, 0, 0, 0}};

  if (isNonZeroDecimal(divisor)) {
    int scale =
        alignDecimalScales(&long_dividend, &long_divisor,
                           getDecimalScale(dividend), getDecimalScale(divisor));
    temp_result = long_dividend;
    s21_longDivision(&temp_result, long_divisor, &long_remainder);
    *remainder =
        convertLongToDecimal(temp_result, scale, getDecimalSign(dividend));
    error_code = checkErrorFlags(remainder);
  } else {
    error_code = 3;
  }

  return error_code;
}

int s21_is_zero(s21_decimal number) {
  for (int i = 0; i < 3; i++) {
    if (number.bits[i] != 0) {
      return 0;
    }
  }
  return 1;
}
