# s21_decimal

Implementation of the native library s21_decimal.h.

## Contents

    1.1. [Introduction](#introduction)
1. [Chapter II](#chapter-i) \
    2.1. [Information](#information)
2. [Chapter II](#chapter-ii) \
    3.1. [Part 1](#part-1-implementation-functions-libraries-decimalh)  



## Introduction

In this project, you will have to implement the s21_decimal.h library in the C programming language. This library should add the ability to work with the "decimal" type, which is missing from the language standard. Nevertheless, this type is critically important, for example, for financial calculations, where calculation errors inherent in floating-point types are unacceptable. Within the framework of this project, you will get acquainted with the tasks of processing financial information, immerse yourself in the issues of internal representation of various types of data and consolidate the structural approach.   

## Chapter I
### Binary representation  

The binary representation of Decimal consists of a 1-bit sign, a 96-bit integer, and a scaling factor used to divide an integer and indicate which part of it is a decimal fraction. The scaling factor is implicitly equal to the number 10, raised to a power in the range from 0 to 28.

A decimal number can be implemented as a four-element array of signed 32-bit integers (`int bits[4];`).

`bits[0]`, `bits[1]`, and `bits[2]` contain the low, middle, and high 32 bits of a 96-bit integer, respectively.

`bits[3]` contains the zoom factor and the sign and consists of the following parts:
- Bits 0 to 15, the lowest word, are not used and must be zero.
- Bits 16 to 23 must contain an exponent from 0 to 28, which indicates the power of 10 to separate an integer.
- Bits 24 to 30 are not used and must be zero.
- Bit 31 contains a sign; 0 means positive and 1 means negative.

Note that the bit representation distinguishes between negative and positive zeros. These values can be considered equivalent in all operations.

### Example:

```c
typedef struct 
{
    int bits[4];
} s21_decimal;
```

### Arithmetic operators

| Name of the operator | Operator | Function | 
| ------ | ------ |------------------------------------------------------------------------------------|
| Addition | + | int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) |
| Subtraction | - | int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) |
| Multiplication | * | int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) | 
| Division | / | int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) |

The functions return an error code:  
- 0 — OK;  
- 1 — the number is too large or equal to infinity;
- 2 — the number is too small or equal to negative infinity;
- 3 — division by 0.

*Clarification about numbers that do not fit into the mantissa:*
- *When getting numbers that do not fit into the mantissa for arithmetic operations, use bank rounding (for example, 79,228,162,514,264,337,593,543,950,335 — 0.6 = 79,228,162,514,264,337,593,543,950,334)*.


### Comparison operators

| Name of the operator | Operator | Function |
| ------ | ------ | ------ |
| Less | < | int s21_is_less(s21_decimal, s21_decimal) |
| Less than or equal to | <= | int s21_is_less_or_equal(s21_decimal, s21_decimal) | 
| More | \> | int s21_is_greater(s21_decimal, s21_decimal) |
| Greater than or equal to | \>= | int s21_is_greater_or_equal(s21_decimal, s21_decimal) | 
| Equal to | == | int s21_is_equal(s21_decimal, s21_decimal) |
| Not equal to | != |  int s21_is_not_equal(s21_decimal, s21_decimal) |

Return value:
- 0 — FALSE;
- 1 — TRUE.

### Converters 

| Converter | Function |
| ------ | ------ |
| From int | int s21_from_int_to_decimal(int src, s21_decimal *dst) |
| From float | int s21_from_float_to_decimal(float src, s21_decimal *dst) |
| To int | int s21_from_decimal_to_int(s21_decimal src, int *dst) |
| In float | int s21_from_decimal_to_float(s21_decimal src, float *dst) |

The return value is the error code:
- 0 — OK;
 - 1 — conversion error.

*Clarification about converting a float number:*
- *If the numbers are too small (0 < |x| < 1e-28), return an error and a value equal to 0*.
- *If the numbers are too large (|x| > 79,228,162,514,264,337,593,543,950,335) or equal to infinity, return an error*.
- *When processing a float number, convert all significant decimal digits contained in it. If there are more than 7 such digits, then the value of the number should be rounded to the nearest one, which has no more than 7 significant digits.*

*Clarification about converting from a decimal number to an int type:*
- *If there is a fractional part in a decimal number, then it should be discarded (for example, 0.9 is converted to 0)*.


### Other functions

| Description | Function | 
| ------ |----------------------------------------------------------|
| Rounds the specified Decimal number to the nearest integer towards negative infinity. | int s21_floor(s21_decimal value, s21_decimal *result)    |	
| Rounds Decimal to the nearest integer. | int s21_round(s21_decimal value, s21_decimal *result)    |
| Returns whole digits of the specified Decimal number; any fractional digits are discarded, including trailing zeros. | int s21_truncate(s21_decimal value, s21_decimal *result) |
| Returns the result of multiplying the specified Decimal by -1. | int s21_negate(s21_decimal value, s21_decimal *result) |

The return value is the error code:
- 0 — OK;
 - 1 — calculation error.

## Chapter II

## Part 1. Implementation of the decimal.h library function

The library functions described [above](#information) are implemented:

- The library must be developed in the C language of the C11 standard using the gcc compiler.
- The library code is located in the src folder.
- Do not use outdated and disused language constructs and library functions. Pay attention to the legacy and obsolete marks in the official documentation on the language and the libraries used. Focus on the POSIX.1-2017 standard.
- When writing code, you must adhere to Google Style.
- Form the solution as a static library named *s21_decimal.a* (with the header file s21_decimal.h).
- The library must be designed in accordance with the principles of structured programming.
- Use the s21_ prefix before each function.
- Prepare a complete coverage of the library functions with unit tests using the Check library.
- Unit tests must cover at least 80% of each function.
- Provide a Makefile for building the library and tests (with the goals of all, clean, test, s21_decimal.a, gcov_report).
- The gcov_report goal should generate a gcov report in the form of an html page. To do this, unit tests must be run with gcov flags.
- When implementing decimal, focus on [binary representation](#binary representation) with an integer array of `bits`, as indicated in [example above](#example). Observe the position of the digits of the number in the `bits` array.
- The use of the __int128 type is prohibited.
- Trailing zeros can be either left or deleted (with the exception of the `s21_truncate` function).
- The type being defined must support numbers from -79,228,162,514,264,337,593,543,950,335 to +79,228,162,514,264,337,593,543,950,335.