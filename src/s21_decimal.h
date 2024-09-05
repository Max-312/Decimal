#ifndef _S21_DECIMAL_H_
#define _S21_DECIMAL_H_

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int bits[4];
} s21_decimal;

typedef struct {
  int bits[7];
} s21_long;

#define OK 0
#define CONVERSION_ERROR 1

#define S2_INF 1.0 / 0.0
#define S21_DEC_MAX pow(2, 96)

extern s21_long _LONGDEC10;

#include "s21_arithmetic.h"
#include "s21_binary_arithmetic.h"
#include "s21_binary_operations.h"
#include "s21_comparison.h"
#include "s21_conversion.h"
#include "s21_error_processing.h"
#include "s21_other_functions.h"

#endif
