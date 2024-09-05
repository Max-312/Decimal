#ifndef _S21_ERROR_PROCESSING_H_
#define _S21_ERROR_PROCESSING_H_

int isNonZeroDecimal(s21_decimal dec);
int isNonZeroLong(s21_long dec);
int isNan(long double value);
int isInf(float value);

int checkErrorFlags(s21_decimal *result);
int detectDecimalError(s21_decimal decimal);

#endif