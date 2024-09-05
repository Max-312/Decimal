#ifndef _S21_BINARY_OPERATIONS_H_
#define _S21_BINARY_OPERATIONS_H_

int getDecimalSign(s21_decimal decimal);

int getBitAtIndex(s21_decimal decimal_value, int bit_position);
int getFirstSetBitIndex(s21_long long_data);

int shiftLongBits(s21_long *number, int shift_amount);

void clearData(s21_decimal *number);
void clearLongData(s21_long *number);
void clearBit(s21_decimal *decimal, int bit_position);

int getDecimalBit(s21_decimal decimal_value, int bit_position);
void setDecimalBit(s21_decimal *decimal_value, int bit_position, int bit_value);
void setDecimalBitToOne(s21_decimal *decimal, int bit_position);

int getLongBit(s21_long long_data, int bit_position);
void setLongBit(s21_long *long_data, int bit_position, int bit_value);

void setDecimalSign(s21_decimal *decimal, int sign);

s21_long convertDecimalToLong(s21_decimal decimal);
s21_decimal convertLongToDecimal(s21_long long_result, int scale, int sign);

int convertDecimalToFloat(s21_decimal src, float *dst);
int convertIntToDecimal(int src, s21_decimal *dst);
long double convertDecimalToLongDouble(s21_decimal decimal);

int compareLongDecimal(s21_long long_number_1, s21_long long_number_2);

int alignDecimalScales(s21_long *first_long_number,
                       s21_long *second_long_number, int scale_1, int scale_2);

int getDecimalScale(s21_decimal decimal);
void setDecimalScale(s21_decimal *decimal, int scale);

int calculateModulus(s21_decimal dividend, s21_decimal divisor,
                     s21_decimal *remainder);

int s21_is_zero(s21_decimal number);

#endif