#include <check.h>

#include "s21_decimal.h"

START_TEST(int_to_decimal_test_1) {
  s21_decimal result = {{0}};
  int value = 254;
  s21_from_int_to_decimal(value, &result);

  s21_decimal expected = {{254, 0, 0, 0}};
  ck_assert_mem_eq(&result, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(int_to_decimal_test_2) {
  s21_decimal result;
  int garbage_data[4] = {rand(), rand(), rand(), rand()};
  memcpy(&result, garbage_data, sizeof(s21_decimal));

  int value = 254;
  s21_from_int_to_decimal(value, &result);

  s21_decimal expected = {{254, 0, 0, 0}};

  ck_assert_mem_eq(&result, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(int_to_decimal_test_3) {
  int value = -5;
  s21_decimal result = {{0}};

  s21_from_int_to_decimal(value, &result);

  s21_decimal expected = {{0x5, 0x0, 0x0, 0x80000000}};
  ck_assert_int_eq(result.bits[0], expected.bits[0]);
  ck_assert_int_eq(result.bits[3], expected.bits[3]);
}
END_TEST

START_TEST(int_to_decimal_test_4) {
  s21_decimal result = {{0}};
  int value = 1073741824;
  s21_from_int_to_decimal(value, &result);

  s21_decimal expected = {{0x40000000, 0x0, 0x0, 0x0}};
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(int_to_decimal_test_5) {
  s21_decimal result;
  int value = -1073741824;
  s21_from_int_to_decimal(value, &result);

  s21_decimal expected = {{0x40000000, 0x0, 0x0, 0x80000000}};
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(int_to_decimal_test_6) {
  int value = 2147483647;
  s21_decimal result = {{0}};

  s21_from_int_to_decimal(value, &result);

  s21_decimal expected = {{0x7fffffff, 0x0, 0x0, 0x0}};
  ck_assert_mem_eq(&result, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(int_to_decimal_test_7) {
  int value = -2147483648;
  s21_decimal result = {{0}};

  s21_from_int_to_decimal(value, &result);

  s21_decimal expected = {{0x80000000, 0x0, 0x0, 0x80000000}};
  ck_assert_mem_eq(&result, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(int_to_decimal_test_8) {
  int value = 42;
  int expected_error_code = CONVERSION_ERROR;

  int result_code = s21_from_int_to_decimal(value, NULL);

  ck_assert_int_eq(result_code, expected_error_code);
}
END_TEST

START_TEST(decimal_to_float_test_1) {
  s21_decimal decimal = {{987654, 0, 0, 0}};
  setDecimalScale(&decimal, 10);
  float expected_result = 0.0000987654f;
  float result;
  int code = s21_from_decimal_to_float(decimal, &result);
  ck_assert_float_eq_tol(expected_result, result, 0.00001);
  ck_assert_int_eq(code, OK);
}
END_TEST

START_TEST(decimal_to_float_test_2) {
  s21_decimal decimal = {{123456, 0, 0, 0}};
  setDecimalScale(&decimal, 2);
  float expected_result = 1234.56f;
  float result;
  int code = s21_from_decimal_to_float(decimal, &result);
  ck_assert_float_eq_tol(expected_result, result, 0.01);
  ck_assert_int_eq(code, OK);
}
END_TEST

START_TEST(decimal_to_float_test_3) {
  s21_decimal zero_decimal = {{0, 0, 0, 0}};
  float expected_result = 0.0f;
  float result;
  int code = s21_from_decimal_to_float(zero_decimal, &result);
  ck_assert_float_eq(expected_result, result);
  ck_assert_int_eq(code, OK);
}
END_TEST

START_TEST(decimal_to_float_test_4) {
  for (int i = 0; i < 50; i++) {
    s21_decimal decimal = {{rand(), 0, 0, 0}};
    float expected_value = (float)decimal.bits[0];
    float result;
    int code = s21_from_decimal_to_float(decimal, &result);
    ck_assert_float_eq(expected_value, result);
    ck_assert_int_eq(code, OK);
  }
}
END_TEST

START_TEST(decimal_to_float_test_5) {
  s21_decimal negative_decimal = {{0x1, 0x0, 0x0, 0x80000000}};
  float result = 0.0;
  int code = s21_from_decimal_to_float(negative_decimal, &result);

  ck_assert_float_eq(result, -1.0f);
  ck_assert_int_eq(code, 0);
}
END_TEST

START_TEST(decimal_to_float_test_6) {
  s21_decimal decimal = {{0x1, 0x0, 0x0, 0x0}};
  int expected_error_code = CONVERSION_ERROR;
  int result_code = s21_from_decimal_to_float(decimal, NULL);
  ck_assert_int_eq(result_code, expected_error_code);
}
END_TEST

START_TEST(decimal_to_int_test_1) {
  s21_decimal decimal = {{123456, 0, 0, 0}};
  int expected_result = 123456;
  int result;
  int code = s21_from_decimal_to_int(decimal, &result);
  ck_assert_int_eq(expected_result, result);
  ck_assert_int_eq(code, OK);
}
END_TEST

START_TEST(decimal_to_int_test_2) {
  s21_decimal decimal = {{123456, 0, 0, 0}};
  setDecimalSign(&decimal, 1);
  int expected_result = -123456;
  int result;
  int code = s21_from_decimal_to_int(decimal, &result);
  ck_assert_int_eq(expected_result, result);
  ck_assert_int_eq(code, OK);
}
END_TEST

START_TEST(decimal_to_int_test_3) {
  s21_decimal decimal = {{123456, 0, 0, 0}};
  setDecimalScale(&decimal, 2);
  int expected_result = 1234;
  int result;
  int code = s21_from_decimal_to_int(decimal, &result);
  ck_assert_int_eq(expected_result, result);
  ck_assert_int_eq(code, OK);
}
END_TEST

START_TEST(decimal_to_int_test_4) {
  s21_decimal decimal = {{0, 0, 0, 0}};
  int expected_result = 0;
  int result;
  int code = s21_from_decimal_to_int(decimal, &result);
  ck_assert_int_eq(expected_result, result);
  ck_assert_int_eq(code, OK);
}
END_TEST

START_TEST(decimal_to_int_test_5) {
  s21_decimal decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}};
  int result;
  int code = s21_from_decimal_to_int(decimal, &result);
  ck_assert_int_eq(code, CONVERSION_ERROR);
}
END_TEST

START_TEST(decimal_to_int_test_6) {
  s21_decimal decimal = {{987654, 0, 0, 0}};
  setDecimalScale(&decimal, 3);
  setDecimalSign(&decimal, 1);
  int expected_result = -987;
  int result;
  int code = s21_from_decimal_to_int(decimal, &result);
  ck_assert_int_eq(expected_result, result);
  ck_assert_int_eq(code, OK);
}
END_TEST

START_TEST(decimal_to_int_test_7) {
  for (int i = 0; i < 50; i++) {
    int random_value = rand();
    s21_decimal decimal = {{random_value, 0, 0, 0}};
    int result;
    int code = s21_from_decimal_to_int(decimal, &result);
    ck_assert_int_eq(random_value, result);
    ck_assert_int_eq(code, OK);
  }
}
END_TEST

START_TEST(decimal_to_int_test_8) {
  for (int i = 0; i < 50; i++) {
    int random_value = rand();
    s21_decimal decimal = {{random_value, 0, 0, 0}};
    setDecimalSign(&decimal, 1);
    int result;
    int code = s21_from_decimal_to_int(decimal, &result);
    ck_assert_int_eq(-random_value, result);
    ck_assert_int_eq(code, OK);
  }
}
END_TEST

START_TEST(float_to_decimal_test_1) {
  s21_decimal decimal_value = {{0, 0, 0, 0}};
  float actual_value = 0.0f;
  float expected_value = 12345.6789f;
  float precision = 0.001f;
  int status = s21_from_float_to_decimal(expected_value, &decimal_value);
  ck_assert_int_eq(status, OK);

  status = s21_from_decimal_to_float(decimal_value, &actual_value);
  ck_assert_int_eq(status, OK);

  ck_assert_float_eq_tol(actual_value, expected_value, precision);
}
END_TEST

START_TEST(float_to_decimal_test_2) {
  s21_decimal decimal_value = {{0, 0, 0, 0}};
  float actual_value = 0.0f;
  float expected_value = -9876.54321f;
  float precision = 0.001f;
  int status = s21_from_float_to_decimal(expected_value, &decimal_value);
  ck_assert_int_eq(status, OK);

  status = s21_from_decimal_to_float(decimal_value, &actual_value);
  ck_assert_int_eq(status, OK);

  ck_assert_float_eq_tol(actual_value, expected_value, precision);
}
END_TEST

START_TEST(float_to_decimal_test_3) {
  s21_decimal decimal_value = {{0, 0, 0, 0}};
  float actual_value = 0.0f;
  float expected_value = 987654321.123456f;
  float precision = 0.01f;
  int status = s21_from_float_to_decimal(expected_value, &decimal_value);
  ck_assert_int_eq(status, OK);

  status = s21_from_decimal_to_float(decimal_value, &actual_value);
  ck_assert_int_eq(status, OK);

  ck_assert_float_eq_tol(actual_value, expected_value, precision);
}
END_TEST

START_TEST(float_to_decimal_test_4) {
  s21_decimal decimal_value = {{0, 0, 0, 0}};
  float actual_value = 0.0f;
  float expected_value = 0.000001234567f;
  float precision = 0.0000001f;
  int status = s21_from_float_to_decimal(expected_value, &decimal_value);
  ck_assert_int_eq(status, OK);

  status = s21_from_decimal_to_float(decimal_value, &actual_value);
  ck_assert_int_eq(status, OK);

  ck_assert_float_eq_tol(actual_value, expected_value, precision);
}
END_TEST

START_TEST(float_to_decimal_test_5) {
  s21_decimal decimal_value;
  int status = s21_from_float_to_decimal(INFINITY, &decimal_value);
  ck_assert_int_eq(status, CONVERSION_ERROR);

  status = s21_from_float_to_decimal(-INFINITY, &decimal_value);
  ck_assert_int_eq(status, CONVERSION_ERROR);
}
END_TEST

START_TEST(float_to_decimal_test_6) {
  s21_decimal decimal_value;
  int status = s21_from_float_to_decimal(NAN, &decimal_value);
  ck_assert_int_eq(status, CONVERSION_ERROR);

  status = s21_from_float_to_decimal(-NAN, &decimal_value);
  ck_assert_int_eq(status, CONVERSION_ERROR);
}
END_TEST

START_TEST(add_decimal_test_1) {
  float value_1 = 1.22f, value_2 = 0.12f, result = 0.0f, expected = 1.34f;
  s21_decimal decimal1 = {{0}}, decimal2 = {{0}}, decimal_result = {{0}};

  s21_from_float_to_decimal(value_1, &decimal1);
  s21_from_float_to_decimal(value_2, &decimal2);

  s21_add(decimal1, decimal2, &decimal_result);
  s21_from_decimal_to_float(decimal_result, &result);

  ck_assert_float_eq(result, expected);
}
END_TEST

START_TEST(add_decimal_test_2) {
  float expected = 1.32f;
  s21_decimal decimal1 = {{12, 0, 0, 0}}, decimal2 = {{12, 0, 0, 0}};
  s21_decimal decimal_result = {{0}};

  setDecimalScale(&decimal1, 1);
  setDecimalScale(&decimal2, 2);

  s21_add(decimal1, decimal2, &decimal_result);
  float result = 0.0f;
  s21_from_decimal_to_float(decimal_result, &result);

  ck_assert_float_eq(result, expected);
}
END_TEST

START_TEST(add_decimal_test_3) {
  float expected = 1.0f;
  s21_decimal decimal1 = {{12, 0, 0, 0}}, decimal2 = {{2, 0, 0, 0}};
  s21_decimal decimal_result = {{0}};

  setDecimalScale(&decimal1, 1);
  setDecimalScale(&decimal2, 1);
  setDecimalSign(&decimal2, 1);

  s21_add(decimal1, decimal2, &decimal_result);
  float result = 0.0f;
  s21_from_decimal_to_float(decimal_result, &result);

  ck_assert_float_eq(result, expected);
}
END_TEST

START_TEST(add_decimal_test_4) {
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1C0000}};
  s21_decimal expected = {{0xFFFFFFF7, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  s21_decimal result = {{0}};

  s21_add(decimal1, decimal2, &result);

  int comparison_result = s21_is_equal(result, expected);
  ck_assert_int_eq(comparison_result, 1);
}
END_TEST

START_TEST(add_decimal_test_5) {
  float value_1 = 0.0f, value_2 = 0.0f, result = 0.0f, expected = 0.0f;
  s21_decimal decimal1 = {{0}}, decimal2 = {{0}}, decimal_result = {{0}};

  s21_from_float_to_decimal(value_1, &decimal1);
  s21_from_float_to_decimal(value_2, &decimal2);

  s21_add(decimal1, decimal2, &decimal_result);
  s21_from_decimal_to_float(decimal_result, &result);

  ck_assert_float_eq(result, expected);
}
END_TEST

START_TEST(add_decimal_test_6) {
  float value_1 = 3.1415f, result = 0.0f, expected = 3.1415f;
  s21_decimal decimal1 = {{0}}, decimal2 = {{0}}, decimal_result = {{0}};

  s21_from_float_to_decimal(value_1, &decimal1);
  s21_from_float_to_decimal(0.0f, &decimal2);

  s21_add(decimal1, decimal2, &decimal_result);
  s21_from_decimal_to_float(decimal_result, &result);

  ck_assert_float_eq(result, expected);
}
END_TEST

START_TEST(add_decimal_test_7) {
  s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}};
  s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}};
  s21_decimal result = {{0}};

  int status = s21_add(decimal1, decimal2, &result);
  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(add_decimal_test_8) {
  float value_1 = 12345.6789f;
  float value_2 = -12345.6789f;
  float result = 0.0f;
  float expected = 0.0f;

  s21_decimal decimal1 = {{0}}, decimal2 = {{0}}, decimal_result = {{0}};
  s21_from_float_to_decimal(value_1, &decimal1);
  s21_from_float_to_decimal(value_2, &decimal2);

  s21_add(decimal1, decimal2, &decimal_result);
  s21_from_decimal_to_float(decimal_result, &result);

  ck_assert_float_eq(result, expected);
}
END_TEST

START_TEST(sub_decimal_test_1) {
  float val1 = 1.5f, val2 = 0.3f, result = 0.0f, expected = 1.2f;
  s21_decimal dec1 = {{0}}, dec2 = {{0}}, dec_result = {{0}};

  s21_from_float_to_decimal(val1, &dec1);
  s21_from_float_to_decimal(val2, &dec2);

  s21_sub(dec1, dec2, &dec_result);
  s21_from_decimal_to_float(dec_result, &result);

  ck_assert_float_eq(result, expected);
}
END_TEST

START_TEST(sub_decimal_test_2) {
  float result = 0.0f, expected = 1.08f;
  s21_decimal dec1 = {{12, 0, 0, 0}}, dec2 = {{12, 0, 0, 0}},
              dec_result = {{0}};

  setDecimalScale(&dec1, 1);
  setDecimalScale(&dec2, 2);

  s21_sub(dec1, dec2, &dec_result);
  s21_from_decimal_to_float(dec_result, &result);

  ck_assert_float_eq(result, expected);
}
END_TEST

START_TEST(sub_decimal_test_3) {
  float val1 = -203e-3f, val2 = 204e-3f, result = 0.0f, expected = -0.407f;
  s21_decimal dec1 = {{0}}, dec2 = {{0}}, dec_result = {{0}};

  s21_from_float_to_decimal(val1, &dec1);
  s21_from_float_to_decimal(val2, &dec2);

  s21_sub(dec1, dec2, &dec_result);
  s21_from_decimal_to_float(dec_result, &result);

  ck_assert_float_eq(result, expected);
}
END_TEST

START_TEST(sub_decimal_test_4) {
  float val1 = -303e-3f, val2 = -102e-3f, result = 0.0f, expected = -0.201f;
  s21_decimal dec1 = {{0}}, dec2 = {{0}}, dec_result = {{0}};

  s21_from_float_to_decimal(val1, &dec1);
  s21_from_float_to_decimal(val2, &dec2);

  s21_sub(dec1, dec2, &dec_result);
  s21_from_decimal_to_float(dec_result, &result);

  ck_assert_float_eq(result, expected);
}
END_TEST

START_TEST(sub_decimal_test_5) {
  float val1 = 5.0f, val2 = 5.0f, result = 0.0f, expected = 0.0f;
  s21_decimal dec1 = {{0}}, dec2 = {{0}}, dec_result = {{0}};

  s21_from_float_to_decimal(val1, &dec1);
  s21_from_float_to_decimal(val2, &dec2);

  s21_sub(dec1, dec2, &dec_result);
  s21_from_decimal_to_float(dec_result, &result);

  ck_assert_float_eq(result, expected);
}
END_TEST

START_TEST(sub_decimal_test_6) {
  float val1 = 0.0001f, val2 = 0.00001f, result = 0.0f, expected = 0.00009f;
  s21_decimal dec1 = {{0}}, dec2 = {{0}}, dec_result = {{0}};

  s21_from_float_to_decimal(val1, &dec1);
  s21_from_float_to_decimal(val2, &dec2);

  s21_sub(dec1, dec2, &dec_result);
  s21_from_decimal_to_float(dec_result, &result);

  ck_assert_float_eq_tol(result, expected, 1e-7);
}
END_TEST

START_TEST(sub_decimal_test_7) {
  float val1 = 2.0f, val2 = 3.5f, result = 0.0f, expected = -1.5f;
  s21_decimal dec1 = {{0}}, dec2 = {{0}}, dec_result = {{0}};

  s21_from_float_to_decimal(val1, &dec1);
  s21_from_float_to_decimal(val2, &dec2);

  s21_sub(dec1, dec2, &dec_result);
  s21_from_decimal_to_float(dec_result, &result);

  ck_assert_float_eq(result, expected);
}
END_TEST

START_TEST(mul_decimal_test_1) {
  float val1 = 1.23f, val2 = 0.11f, result = 0.0f, expected = 0.1353f;
  s21_decimal dec1 = {{0}}, dec2 = {{0}}, dec_result = {{0}};

  s21_from_float_to_decimal(val1, &dec1);
  s21_from_float_to_decimal(val2, &dec2);

  s21_mul(dec1, dec2, &dec_result);
  s21_from_decimal_to_float(dec_result, &result);

  ck_assert_float_eq(result, expected);
}
END_TEST

START_TEST(mul_decimal_test_2) {
  float result = 0.0f, expected = 0.12915f;

  s21_decimal dec1 = {{105, 0, 0, 0}}, dec2 = {{123, 0, 0, 0}};
  setDecimalScale(&dec1, 3);
  setDecimalScale(&dec2, 2);

  setDecimalSign(&dec1, 1);
  setDecimalSign(&dec2, 1);

  s21_decimal dec_result = {{0, 0, 0, 0}};

  s21_mul(dec1, dec2, &dec_result);
  s21_from_decimal_to_float(dec_result, &result);

  ck_assert_float_eq(result, expected);
}
END_TEST

START_TEST(mul_decimal_test_3) {
  float result = 0.0f, expected = 0.18f;

  s21_decimal dec1 = {{15, 0, 0, 0}}, dec2 = {{120, 0, 0, 0}};
  setDecimalScale(&dec1, 2);
  setDecimalScale(&dec2, 2);

  setDecimalSign(&dec1, 1);
  setDecimalSign(&dec2, 1);

  s21_decimal dec_result = {{0, 0, 0, 0}};

  s21_mul(dec1, dec2, &dec_result);
  s21_from_decimal_to_float(dec_result, &result);

  ck_assert_float_eq(result, expected);
}
END_TEST

START_TEST(mul_decimal_test_4) {
  float v1 = -0.15f, v2 = -1.20f, result = 0.0f, expected = 0.18f;

  s21_decimal dec1 = {{0, 0, 0, 0}}, dec2 = {{0, 0, 0, 0}};
  s21_from_float_to_decimal(v1, &dec1);
  s21_from_float_to_decimal(v2, &dec2);

  s21_decimal dec_result = {{0, 0, 0, 0}};

  s21_mul(dec1, dec2, &dec_result);
  s21_from_decimal_to_float(dec_result, &result);

  ck_assert_float_eq(result, expected);
}
END_TEST

START_TEST(mul_decimal_test_5) {
  s21_decimal dec1 = {{0xFFFFFFF, 0x3E250261, 0x204FCE5E, 0x1C0000}};
  s21_decimal dec2 = {{0x19, 0x0, 0x0, 0x100000}};

  s21_decimal dec_result = {0};

  int status = s21_mul(dec1, dec2, &dec_result);

  ck_assert_int_eq(status, OK);
}
END_TEST

START_TEST(div_decimal_test_1) {
  int value_1 = 1000000, value_2 = 1000, result = 0, expected = 1000;
  s21_decimal dec1 = {{0, 0, 0, 0}}, dec2 = {{0, 0, 0, 0}},
              dec_result = {{0, 0, 0, 0}};

  s21_from_int_to_decimal(value_1, &dec1);
  s21_from_int_to_decimal(value_2, &dec2);

  s21_div(dec1, dec2, &dec_result);
  s21_from_decimal_to_int(dec_result, &result);

  ck_assert_int_eq(result, expected);
}
END_TEST

START_TEST(div_decimal_test_2) {
  float value_1 = 5.0f, value_2 = 2.0f, result = 0.0f, expected = 2.5f;
  s21_decimal dec1 = {{0, 0, 0, 0}}, dec2 = {{0, 0, 0, 0}},
              dec_result = {{0, 0, 0, 0}};

  s21_from_float_to_decimal(value_1, &dec1);
  s21_from_float_to_decimal(value_2, &dec2);

  s21_div(dec1, dec2, &dec_result);
  s21_from_decimal_to_float(dec_result, &result);

  ck_assert_float_eq_tol(result, expected, 0.001);
}
END_TEST

START_TEST(div_decimal_test_3) {
  float value_1 = -10.0f, value_2 = 4.0f, result = 0.0f, expected = -2.5f;
  s21_decimal dec1 = {{0, 0, 0, 0}}, dec2 = {{0, 0, 0, 0}},
              dec_result = {{0, 0, 0, 0}};

  s21_from_float_to_decimal(value_1, &dec1);
  s21_from_float_to_decimal(value_2, &dec2);

  s21_div(dec1, dec2, &dec_result);
  s21_from_decimal_to_float(dec_result, &result);

  ck_assert_float_eq_tol(result, expected, 0.001);
}
END_TEST

START_TEST(div_decimal_test_4) {
  float value_1 = 1e-6f, value_2 = 1e-4f, result = 0.0f, expected = 0.01f;
  s21_decimal dec1 = {{0, 0, 0, 0}}, dec2 = {{0, 0, 0, 0}},
              dec_result = {{0, 0, 0, 0}};

  s21_from_float_to_decimal(value_1, &dec1);
  s21_from_float_to_decimal(value_2, &dec2);

  s21_div(dec1, dec2, &dec_result);
  s21_from_decimal_to_float(dec_result, &result);

  ck_assert_float_eq_tol(result, expected, 0.001);
}
END_TEST

START_TEST(div_decimal_test_5) {
  float value_1 = -5.0f, value_2 = -2.0f, result = 0.0f, expected = 2.5f;
  s21_decimal dec1 = {{0, 0, 0, 0}}, dec2 = {{0, 0, 0, 0}},
              dec_result = {{0, 0, 0, 0}};

  s21_from_float_to_decimal(value_1, &dec1);
  s21_from_float_to_decimal(value_2, &dec2);

  s21_div(dec1, dec2, &dec_result);
  s21_from_decimal_to_float(dec_result, &result);

  ck_assert_float_eq_tol(result, expected, 0.001);
}
END_TEST

START_TEST(div_decimal_test_6) {
  float value_1 = 0.12345f, result = 0.0f, expected = 1.0f;
  s21_decimal dec1 = {{0, 0, 0, 0}}, dec_result = {{0, 0, 0, 0}};

  s21_from_float_to_decimal(value_1, &dec1);

  s21_div(dec1, dec1, &dec_result);
  s21_from_decimal_to_float(dec_result, &result);

  ck_assert_float_eq_tol(result, expected, 0.001);
}
END_TEST

START_TEST(div_decimal_test_7) {
  int value_1 = 1, value_2 = 1000000;
  float result = 0.0f, expected = 1e-6f;
  s21_decimal dec1 = {{0, 0, 0, 0}}, dec2 = {{0, 0, 0, 0}},
              dec_result = {{0, 0, 0, 0}};

  s21_from_int_to_decimal(value_1, &dec1);
  s21_from_int_to_decimal(value_2, &dec2);

  s21_div(dec1, dec2, &dec_result);
  s21_from_decimal_to_float(dec_result, &result);

  ck_assert_float_eq_tol(result, expected, 0.001);
}
END_TEST

START_TEST(div_decimal_test_8) {
  int value = 123456;
  s21_decimal dec1 = {{0, 0, 0, 0}}, dec2 = {{0, 0, 0, 0}},
              dec_result = {{0, 0, 0, 0}};
  int result = 0;

  s21_from_int_to_decimal(value, &dec1);
  s21_from_int_to_decimal(1, &dec2);

  s21_div(dec1, dec2, &dec_result);
  s21_from_decimal_to_int(dec_result, &result);

  ck_assert_int_eq(result, value);
}
END_TEST

START_TEST(is_equal_decimal_test_1) {
  s21_decimal decimal_1 = {.bits = {123456, 0, 0, 0x80000000}};
  s21_decimal decimal_2 = {.bits = {123456, 0, 0, 0x00000000}};

  ck_assert_int_eq(s21_is_equal(decimal_1, decimal_2), 0);
}
END_TEST

START_TEST(is_equal_decimal_test_2) {
  s21_decimal decimal_1 = {.bits = {987654, 0, 0, 0x00000000}};
  s21_decimal decimal_2 = {.bits = {987654, 0, 0, 0x00000000}};

  ck_assert_int_eq(s21_is_equal(decimal_1, decimal_2), 1);
}
END_TEST

START_TEST(is_equal_decimal_test_3) {
  s21_decimal decimal_1 = {.bits = {999999, 0, 0, 0x80000000}};
  s21_decimal decimal_2 = {.bits = {999999, 0, 0, 0x80000000}};

  ck_assert_int_eq(s21_is_equal(decimal_1, decimal_2), 1);
}
END_TEST

START_TEST(is_equal_decimal_test_4) {
  s21_decimal decimal_1 = {.bits = {0, 0, 0, 0x00000000}};
  s21_decimal decimal_2 = {.bits = {123456, 0, 0, 0x00000000}};

  ck_assert_int_eq(s21_is_equal(decimal_1, decimal_2), 0);
}
END_TEST

START_TEST(is_equal_decimal_test_5) {
  s21_decimal decimal_1 = {.bits = {123456, 0, 0, 0x00020000}};
  s21_decimal decimal_2 = {.bits = {123456, 0, 0, 0x00020000}};

  ck_assert_int_eq(s21_is_equal(decimal_1, decimal_2), 1);
}
END_TEST

START_TEST(is_equal_decimal_test_6) {
  s21_decimal decimal_1 = {
      .bits = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}};
  s21_decimal decimal_2 = {
      .bits = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}};

  ck_assert_int_eq(s21_is_equal(decimal_1, decimal_2), 1);
}
END_TEST

START_TEST(is_not_equal_decimal_test_1) {
  s21_decimal decimal_1 = {.bits = {100, 0, 0, 0x00000000}};
  s21_decimal decimal_2 = {.bits = {200, 0, 0, 0x00000000}};

  ck_assert_int_eq(s21_is_not_equal(decimal_1, decimal_2), 1);
}
END_TEST

START_TEST(is_not_equal_decimal_test_2) {
  s21_decimal decimal_1 = {.bits = {500, 0, 0, 0x00000000}};
  s21_decimal decimal_2 = {.bits = {500, 0, 0, 0x00000000}};

  ck_assert_int_eq(s21_is_not_equal(decimal_1, decimal_2), 0);
}
END_TEST

START_TEST(is_not_equal_decimal_test_3) {
  s21_decimal decimal_1 = {.bits = {300, 0, 0, 0x00000000}};
  s21_decimal decimal_2 = {.bits = {150, 0, 0, 0x00000000}};

  ck_assert_int_eq(s21_is_not_equal(decimal_1, decimal_2), 1);
}
END_TEST

START_TEST(is_not_equal_decimal_test_4) {
  s21_decimal decimal_1 = {.bits = {0, 0, 0, 0x00000000}};
  s21_decimal decimal_2 = {.bits = {12345, 0, 0, 0x00000000}};

  ck_assert_int_eq(s21_is_not_equal(decimal_1, decimal_2), 1);
}
END_TEST

START_TEST(is_not_equal_decimal_test_5) {
  s21_decimal decimal_1 = {.bits = {1000, 0, 0, 0x00010000}};
  s21_decimal decimal_2 = {.bits = {1010, 0, 0, 0x00010000}};

  ck_assert_int_eq(s21_is_not_equal(decimal_1, decimal_2), 1);
}
END_TEST

START_TEST(is_not_equal_decimal_test_6) {
  s21_decimal decimal_1 = {
      .bits = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}};
  s21_decimal decimal_2 = {
      .bits = {0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}};

  ck_assert_int_eq(s21_is_not_equal(decimal_1, decimal_2), 1);
}
END_TEST

START_TEST(is_less_decimal_test_1) {
  s21_decimal decimal_1 = {.bits = {100, 0, 0, 0x00000000}};
  s21_decimal decimal_2 = {.bits = {200, 0, 0, 0x00000000}};

  ck_assert_int_eq(s21_is_less(decimal_1, decimal_2), 1);
}
END_TEST

START_TEST(is_less_decimal_test_2) {
  s21_decimal decimal_1 = {.bits = {500, 0, 0, 0x00000000}};
  s21_decimal decimal_2 = {.bits = {500, 0, 0, 0x00000000}};

  ck_assert_int_eq(s21_is_less(decimal_1, decimal_2), 0);
}
END_TEST

START_TEST(is_less_decimal_test_3) {
  s21_decimal decimal_1 = {.bits = {300, 0, 0, 0x00000000}};
  s21_decimal decimal_2 = {.bits = {150, 0, 0, 0x00000000}};

  ck_assert_int_eq(s21_is_less(decimal_1, decimal_2), 0);
}
END_TEST

START_TEST(is_less_decimal_test_4) {
  s21_decimal decimal_1 = {.bits = {0, 0, 0, 0x00000000}};
  s21_decimal decimal_2 = {.bits = {12345, 0, 0, 0x00000000}};

  ck_assert_int_eq(s21_is_less(decimal_1, decimal_2), 1);
}
END_TEST

START_TEST(is_less_decimal_test_5) {
  s21_decimal decimal_1 = {.bits = {10, 0, 0, 0x80000000}};
  s21_decimal decimal_2 = {.bits = {5, 0, 0, 0x80000000}};

  ck_assert_int_eq(s21_is_less(decimal_1, decimal_2), 1);
}
END_TEST

START_TEST(is_less_decimal_test_6) {
  s21_decimal decimal_1 = {.bits = {10, 0, 0, 0x80000000}};
  s21_decimal decimal_2 = {.bits = {0, 0, 0, 0x00000000}};

  ck_assert_int_eq(s21_is_less(decimal_1, decimal_2), 1);
}
END_TEST

START_TEST(is_less_decimal_test_7) {
  s21_decimal decimal_1 = {.bits = {0, 0, 0, 0x00000000}};
  s21_decimal decimal_2 = {.bits = {10, 0, 0, 0x80000000}};

  ck_assert_int_eq(s21_is_less(decimal_1, decimal_2), 0);
}
END_TEST

START_TEST(is_less_decimal_test_8) {
  s21_decimal decimal_1 = {.bits = {-133248880, 240, 0, -2147483648}};
  s21_decimal decimal_2 = {.bits = {333425, 0, 0, 0}};
  ck_assert_int_eq(s21_is_less(decimal_1, decimal_2), 1);

  decimal_1 = (s21_decimal){{0, 0, 0, -133248880}};
  decimal_2 = (s21_decimal){{0, 0, 0, 0}};
  ck_assert_int_eq(s21_is_less(decimal_1, decimal_2), 0);
}
END_TEST

START_TEST(is_less_or_equal_test_1) {
  s21_decimal decimal_1 = {.bits = {100, 0, 0, 0x00000000}};
  s21_decimal decimal_2 = {.bits = {200, 0, 0, 0x00000000}};

  ck_assert_int_eq(s21_is_less_or_equal(decimal_1, decimal_2), 1);
}
END_TEST

START_TEST(is_less_or_equal_test_2) {
  s21_decimal decimal_1 = {.bits = {500, 0, 0, 0x00000000}};
  s21_decimal decimal_2 = {.bits = {500, 0, 0, 0x00000000}};

  ck_assert_int_eq(s21_is_less_or_equal(decimal_1, decimal_2), 1);
}
END_TEST

START_TEST(is_less_or_equal_test_3) {
  s21_decimal decimal_1 = {.bits = {300, 0, 0, 0x00000000}};
  s21_decimal decimal_2 = {.bits = {150, 0, 0, 0x00000000}};

  ck_assert_int_eq(s21_is_less_or_equal(decimal_1, decimal_2), 0);
}
END_TEST

START_TEST(is_less_or_equal_test_4) {
  s21_decimal decimal_1 = {.bits = {0, 0, 0, 0x00000000}};
  s21_decimal decimal_2 = {.bits = {12345, 0, 0, 0x00000000}};

  ck_assert_int_eq(s21_is_less_or_equal(decimal_1, decimal_2), 1);
}
END_TEST

START_TEST(is_less_or_equal_test_5) {
  s21_decimal decimal_1 = {
      .bits = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x001C0000}};
  s21_decimal decimal_2 = {
      .bits = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}};

  ck_assert_int_eq(s21_is_less_or_equal(decimal_1, decimal_2), 1);
}
END_TEST

START_TEST(is_less_or_equal_test_6) {
  s21_decimal decimal_1 = {.bits = {-133248880, 240, 0, -2147483648}};
  s21_decimal decimal_2 = {.bits = {333425, 0, 0, 0}};
  ck_assert_int_eq(s21_is_less_or_equal(decimal_1, decimal_2), 1);

  decimal_1 = (s21_decimal){{0, 0, 0, 0}};
  decimal_2 = (s21_decimal){{0, 0, 0, -133248880}};
  ck_assert_int_eq(s21_is_less_or_equal(decimal_1, decimal_2), 1);
}
END_TEST

START_TEST(is_greater_test_1) {
  s21_decimal decimal_1 = {.bits = {100, 0, 0, 0x00000000}};
  s21_decimal decimal_2 = {.bits = {200, 0, 0, 0x00000000}};

  ck_assert_int_eq(s21_is_greater(decimal_1, decimal_2), 0);
}
END_TEST

START_TEST(is_greater_test_2) {
  s21_decimal decimal_1 = {.bits = {500, 0, 0, 0x00000000}};
  s21_decimal decimal_2 = {.bits = {500, 0, 0, 0x00000000}};

  ck_assert_int_eq(s21_is_greater(decimal_1, decimal_2), 0);
}
END_TEST

START_TEST(is_greater_test_3) {
  s21_decimal decimal_1 = {.bits = {300, 0, 0, 0x00000000}};
  s21_decimal decimal_2 = {.bits = {150, 0, 0, 0x00000000}};

  ck_assert_int_eq(s21_is_greater(decimal_1, decimal_2), 1);
}
END_TEST

START_TEST(is_greater_test_4) {
  s21_decimal decimal_1 = {.bits = {0, 0, 0, 0x00000000}};
  s21_decimal decimal_2 = {.bits = {12345, 0, 0, 0x00000000}};

  ck_assert_int_eq(s21_is_greater(decimal_1, decimal_2), 0);
}
END_TEST

START_TEST(is_greater_test_5) {
  s21_decimal decimal_1 = {
      .bits = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x001C0000}};
  s21_decimal decimal_2 = {
      .bits = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}};

  ck_assert_int_eq(s21_is_greater(decimal_1, decimal_2), 0);
}
END_TEST

START_TEST(is_greater_test_6) {
  s21_decimal decimal_1 = {.bits = {-133248880, 240, 0, -2147483648}};
  s21_decimal decimal_2 = {.bits = {333425, 0, 0, 0}};
  ck_assert_int_eq(s21_is_greater(decimal_1, decimal_2), 0);

  decimal_1 = (s21_decimal){{0, 0, 0, 0}};
  decimal_2 = (s21_decimal){{0, 0, 0, -133248880}};
  ck_assert_int_eq(s21_is_greater(decimal_1, decimal_2), 0);
}
END_TEST

START_TEST(is_greater_or_equal_test_1) {
  s21_decimal decimal_1 = {.bits = {100, 0, 0, 0x00000000}};
  s21_decimal decimal_2 = {.bits = {200, 0, 0, 0x00000000}};

  ck_assert_int_eq(s21_is_greater_or_equal(decimal_1, decimal_2), 0);
}
END_TEST

START_TEST(is_greater_or_equal_test_2) {
  s21_decimal decimal_1 = {.bits = {500, 0, 0, 0x00000000}};
  s21_decimal decimal_2 = {.bits = {500, 0, 0, 0x00000000}};

  ck_assert_int_eq(s21_is_greater_or_equal(decimal_1, decimal_2), 1);
}
END_TEST

START_TEST(is_greater_or_equal_test_3) {
  s21_decimal decimal_1 = {.bits = {300, 0, 0, 0x00000000}};
  s21_decimal decimal_2 = {.bits = {150, 0, 0, 0x00000000}};

  ck_assert_int_eq(s21_is_greater_or_equal(decimal_1, decimal_2), 1);
}
END_TEST

START_TEST(is_greater_or_equal_test_4) {
  s21_decimal decimal_1 = {.bits = {0, 0, 0, 0x00000000}};
  s21_decimal decimal_2 = {.bits = {12345, 0, 0, 0x00000000}};

  ck_assert_int_eq(s21_is_greater_or_equal(decimal_1, decimal_2), 0);
}
END_TEST

START_TEST(is_greater_or_equal_test_5) {
  s21_decimal decimal_1 = {.bits = {0, 0, 0, 0x00000000}};
  s21_decimal decimal_2 = {.bits = {0, 0, 0, 0x80000000}};

  ck_assert_int_eq(s21_is_greater_or_equal(decimal_1, decimal_2), 1);
}
END_TEST

START_TEST(is_greater_or_equal_test_6) {
  s21_decimal decimal_1 = {.bits = {0, 0, 0, 0x00000000}};
  s21_decimal decimal_2 = {.bits = {0, 0, 0, 0x80000000}};

  ck_assert_int_eq(s21_is_greater_or_equal(decimal_1, decimal_2), 1);
}
END_TEST

START_TEST(floor_decimal_test_1) {
  float value = 3.7f, expected = 3.0f, result_float = 0.0f;
  s21_decimal dec_input = {{0, 0, 0, 0}}, dec_result = {{0, 0, 0, 0}};

  s21_from_float_to_decimal(value, &dec_input);
  s21_floor(dec_input, &dec_result);
  s21_from_decimal_to_float(dec_result, &result_float);

  ck_assert_float_eq(result_float, expected);
}
END_TEST

START_TEST(is_greater_or_equal_test_7) {
  s21_decimal decimal_1 = {.bits = {-133248880, 240, 0, -2147483648}};
  s21_decimal decimal_2 = {.bits = {333425, 0, 0, 0}};
  ck_assert_int_eq(s21_is_greater_or_equal(decimal_1, decimal_2), 0);

  decimal_1 = (s21_decimal){{0, 0, 0, -133248880}};
  decimal_2 = (s21_decimal){{0, 0, 0, 0}};
  ck_assert_int_eq(s21_is_greater_or_equal(decimal_1, decimal_2), 1);
}
END_TEST

START_TEST(floor_decimal_test_2) {
  float value = -5.9f, expected = -6.0f, result_float = 0.0f;
  s21_decimal dec_input = {{0, 0, 0, 0}}, dec_result = {{0, 0, 0, 0}};

  s21_from_float_to_decimal(value, &dec_input);
  s21_floor(dec_input, &dec_result);
  s21_from_decimal_to_float(dec_result, &result_float);

  ck_assert_float_eq(result_float, expected);
}
END_TEST

START_TEST(floor_decimal_test_3) {
  float value = 0.0f, expected = 0.0f, result_float = 0.0f;
  s21_decimal dec_input = {{0, 0, 0, 0}}, dec_result = {{0, 0, 0, 0}};

  s21_from_float_to_decimal(value, &dec_input);
  s21_floor(dec_input, &dec_result);
  s21_from_decimal_to_float(dec_result, &result_float);

  ck_assert_float_eq(result_float, expected);
}
END_TEST

START_TEST(floor_decimal_test_4) {
  float value = 1.999999f, expected = 1.0f, result_float = 0.0f;
  s21_decimal dec_input = {{0, 0, 0, 0}}, dec_result = {{0, 0, 0, 0}};

  s21_from_float_to_decimal(value, &dec_input);
  s21_floor(dec_input, &dec_result);
  s21_from_decimal_to_float(dec_result, &result_float);

  ck_assert_float_eq(result_float, expected);
}
END_TEST

START_TEST(floor_decimal_test_5) {
  float value = -0.0001f, expected = -1.0f, result_float = 0.0f;
  s21_decimal dec_input = {{0, 0, 0, 0}}, dec_result = {{0, 0, 0, 0}};

  s21_from_float_to_decimal(value, &dec_input);
  s21_floor(dec_input, &dec_result);
  s21_from_decimal_to_float(dec_result, &result_float);

  ck_assert_float_eq(result_float, expected);
}
END_TEST

START_TEST(floor_decimal_test_6) {
  int value = 42, expected = 42, result_int = 0;
  s21_decimal dec_input = {{0, 0, 0, 0}}, dec_result = {{0, 0, 0, 0}};

  s21_from_int_to_decimal(value, &dec_input);
  s21_floor(dec_input, &dec_result);
  s21_from_decimal_to_int(dec_result, &result_int);

  ck_assert_int_eq(result_int, expected);
}
END_TEST

START_TEST(floor_decimal_test_7) {
  float value = -123.456f, expected = -124.0f, result_float = 0.0f;
  s21_decimal dec_input = {{0, 0, 0, 0}}, dec_result = {{0, 0, 0, 0}};

  s21_from_float_to_decimal(value, &dec_input);
  s21_floor(dec_input, &dec_result);
  s21_from_decimal_to_float(dec_result, &result_float);

  ck_assert_float_eq(result_float, expected);
}
END_TEST

START_TEST(floor_decimal_test_8) {
  float value = 987654321.12345f, expected = 987654321.0f, result_float = 0.0f;
  s21_decimal dec_input = {{0, 0, 0, 0}}, dec_result = {{0, 0, 0, 0}};

  s21_from_float_to_decimal(value, &dec_input);
  s21_floor(dec_input, &dec_result);
  s21_from_decimal_to_float(dec_result, &result_float);

  ck_assert_float_eq(result_float, expected);
}
END_TEST

START_TEST(round_decimal_test_1) {
  s21_decimal value = {{148, 0, 0, 0}}, result = {{0, 0, 0, 0}},
              expected = {{15, 0, 0, 0}};
  setDecimalScale(&value, 1);

  int code = s21_round(value, &result);
  ck_assert_int_eq(code, OK);

  int is_equal = (memcmp(&result, &expected, sizeof(s21_decimal)) == 0);
  ck_assert_int_eq(is_equal, 1);
}
END_TEST

START_TEST(round_decimal_test_2) {
  s21_decimal value = {{73, 0, 0, 0}}, result = {{0, 0, 0, 0}},
              expected = {{7, 0, 0, 0}};
  setDecimalSign(&value, 1);
  setDecimalSign(&expected, 1);
  setDecimalScale(&value, 1);

  int code = s21_round(value, &result);
  ck_assert_int_eq(code, OK);

  int is_equal = (memcmp(&result, &expected, sizeof(s21_decimal)) == 0);
  ck_assert_int_eq(is_equal, 1);
}
END_TEST

START_TEST(round_decimal_test_3) {
  s21_decimal value = {{9999995, 0, 0, 0}}, result = {{0, 0, 0, 0}},
              expected = {{1000000, 0, 0, 0}};
  setDecimalScale(&value, 1);

  int code = s21_round(value, &result);
  ck_assert_int_eq(code, OK);

  int is_equal = (memcmp(&result, &expected, sizeof(s21_decimal)) == 0);
  ck_assert_int_eq(is_equal, 1);
}
END_TEST

START_TEST(round_decimal_test_4) {
  s21_decimal value = {{999, 0, 0, 0}}, result = {{0, 0, 0, 0}},
              expected = {{1, 0, 0, 0}};
  setDecimalSign(&value, 1);
  setDecimalSign(&expected, 1);
  setDecimalScale(&value, 3);

  int code = s21_round(value, &result);
  ck_assert_int_eq(code, OK);

  int is_equal = (memcmp(&result, &expected, sizeof(s21_decimal)) == 0);
  ck_assert_int_eq(is_equal, 1);
}
END_TEST

START_TEST(round_decimal_test_5) {
  s21_decimal value = {{49, 0, 0, 0}}, result = {{0, 0, 0, 0}},
              expected = {{0, 0, 0, 0}};
  setDecimalScale(&value, 2);

  int code = s21_round(value, &result);
  ck_assert_int_eq(code, OK);

  int is_equal = (memcmp(&result, &expected, sizeof(s21_decimal)) == 0);
  ck_assert_int_eq(is_equal, 1);
}
END_TEST

START_TEST(round_decimal_test_6) {
  s21_decimal value = {{123456789, 0, 0, 0}}, result = {{0, 0, 0, 0}},
              expected = {{123, 0, 0, 0}};
  setDecimalScale(&value, 6);

  int code = s21_round(value, &result);
  ck_assert_int_eq(code, OK);

  int is_equal = (memcmp(&result, &expected, sizeof(s21_decimal)) == 0);
  ck_assert_int_eq(is_equal, 1);
}
END_TEST

START_TEST(round_decimal_test_7) {
  s21_decimal value = {{123999, 0, 0, 0}}, result = {{0, 0, 0, 0}},
              expected = {{124, 0, 0, 0}};
  setDecimalSign(&value, 1);
  setDecimalSign(&expected, 1);
  setDecimalScale(&value, 3);

  int code = s21_round(value, &result);
  ck_assert_int_eq(code, OK);

  int is_equal = (memcmp(&result, &expected, sizeof(s21_decimal)) == 0);
  ck_assert_int_eq(is_equal, 1);
}
END_TEST

START_TEST(round_decimal_test_8) {
  s21_decimal value = {{-1, -1, -1, 0}}, result = {{0, 0, 0, 0}};
  setDecimalScale(&value, 30);

  int code = s21_round(value, &result);
  ck_assert_int_eq(code, 1);
}
END_TEST

START_TEST(truncate_decimal_test_1) {
  s21_decimal value = {{123456, 0, 0, 0x00000000}};
  s21_decimal result = {{0}};
  s21_decimal expected = {{123456, 0, 0, 0x00000000}};

  s21_truncate(value, &result);

  ck_assert_mem_eq(&result, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(truncate_decimal_test_2) {
  s21_decimal value = {{123456, 0, 0, 0x80000000}};
  s21_decimal result = {{0}};
  s21_decimal expected = {{123456, 0, 0, 0x80000000}};

  s21_truncate(value, &result);

  ck_assert_mem_eq(&result, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(truncate_decimal_test_3) {
  s21_decimal value = {{0, 0, 0, 0x00000000}};
  s21_decimal result = {{0}};
  s21_decimal expected = {{0, 0, 0, 0x00000000}};

  s21_truncate(value, &result);

  ck_assert_mem_eq(&result, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(truncate_decimal_test_4) {
  s21_decimal value = {{123456, 0, 0, 0x00000000}};
  s21_decimal result = {{0}};
  s21_decimal expected = {{123456, 0, 0, 0x00000000}};

  s21_truncate(value, &result);

  ck_assert_mem_eq(&result, &expected, sizeof(s21_decimal));
}
END_TEST

START_TEST(negate_decimal_test_1) {
  s21_decimal test = {{0, 0, 0, 0}}, test_modify = {{0, 0, 0, 0}};
  s21_from_int_to_decimal(123456789, &test);

  int code = s21_negate(test, &test_modify);
  ck_assert_int_eq(code, OK);

  int expected = -123456789, result = 0;

  code = s21_from_decimal_to_int(test_modify, &result);
  ck_assert_int_eq(code, OK);

  ck_assert_int_eq(expected, result);
}
END_TEST

START_TEST(negate_decimal_test_2) {
  s21_decimal test = {{0, 0, 0, 0}}, test_modify = {{0, 0, 0, 0}};
  s21_from_int_to_decimal(-987654321, &test);

  int code = s21_negate(test, &test_modify);
  ck_assert_int_eq(code, OK);

  int expected = 987654321, result = 0;

  code = s21_from_decimal_to_int(test_modify, &result);
  ck_assert_int_eq(code, OK);

  ck_assert_int_eq(expected, result);
}
END_TEST

START_TEST(negate_decimal_test_3) {
  s21_decimal test = {{0, 0, 0, 0}}, test_modify = {{0, 0, 0, 0}};

  int code = s21_negate(test, &test_modify);
  ck_assert_int_eq(code, OK);

  int expected = 0, result = 0;

  code = s21_from_decimal_to_int(test_modify, &result);
  ck_assert_int_eq(code, OK);

  ck_assert_int_eq(expected, result);
}
END_TEST

START_TEST(negate_decimal_test_4) {
  s21_decimal test = {{1, 0, 0, 0}}, test_modify = {{0, 0, 0, 0}};
  setDecimalSign(&test, 1);

  int code = s21_negate(test, &test_modify);
  ck_assert_int_eq(code, OK);

  int expected = 1, result = 0;

  code = s21_from_decimal_to_int(test_modify, &result);
  ck_assert_int_eq(code, OK);

  ck_assert_int_eq(expected, result);
}
END_TEST

START_TEST(negate_decimal_test_5) {
  s21_decimal test = {{314, 0, 0, 0}}, test_modify = {{0, 0, 0, 0}};
  float expected = -3.14, result = 0.0;

  setDecimalScale(&test, 2);

  int code = s21_negate(test, &test_modify);
  ck_assert_int_eq(code, OK);

  code = s21_from_decimal_to_float(test_modify, &result);
  ck_assert_int_eq(code, OK);

  ck_assert_float_eq(expected, result);
}
END_TEST

Suite *suite_core() {
  Suite *suite = suite_create("Decimal Creation Tests");
  TCase *test_case = tcase_create("Decimal Creation");

  tcase_add_test(test_case, int_to_decimal_test_1);
  tcase_add_test(test_case, int_to_decimal_test_2);
  tcase_add_test(test_case, int_to_decimal_test_3);
  tcase_add_test(test_case, int_to_decimal_test_4);
  tcase_add_test(test_case, int_to_decimal_test_5);
  tcase_add_test(test_case, int_to_decimal_test_6);
  tcase_add_test(test_case, int_to_decimal_test_7);
  tcase_add_test(test_case, int_to_decimal_test_8);

  tcase_add_test(test_case, decimal_to_float_test_1);
  tcase_add_test(test_case, decimal_to_float_test_2);
  tcase_add_test(test_case, decimal_to_float_test_3);
  tcase_add_test(test_case, decimal_to_float_test_4);
  tcase_add_test(test_case, decimal_to_float_test_5);
  tcase_add_test(test_case, decimal_to_float_test_6);

  tcase_add_test(test_case, decimal_to_int_test_1);
  tcase_add_test(test_case, decimal_to_int_test_2);
  tcase_add_test(test_case, decimal_to_int_test_3);
  tcase_add_test(test_case, decimal_to_int_test_4);
  tcase_add_test(test_case, decimal_to_int_test_5);
  tcase_add_test(test_case, decimal_to_int_test_6);
  tcase_add_test(test_case, decimal_to_int_test_7);
  tcase_add_test(test_case, decimal_to_int_test_8);

  tcase_add_test(test_case, float_to_decimal_test_1);
  tcase_add_test(test_case, float_to_decimal_test_2);
  tcase_add_test(test_case, float_to_decimal_test_3);
  tcase_add_test(test_case, float_to_decimal_test_4);
  tcase_add_test(test_case, float_to_decimal_test_5);
  tcase_add_test(test_case, float_to_decimal_test_6);

  tcase_add_test(test_case, add_decimal_test_1);
  tcase_add_test(test_case, add_decimal_test_2);
  tcase_add_test(test_case, add_decimal_test_3);
  tcase_add_test(test_case, add_decimal_test_4);
  tcase_add_test(test_case, add_decimal_test_5);
  tcase_add_test(test_case, add_decimal_test_6);
  tcase_add_test(test_case, add_decimal_test_7);
  tcase_add_test(test_case, add_decimal_test_8);

  tcase_add_test(test_case, sub_decimal_test_1);
  tcase_add_test(test_case, sub_decimal_test_2);
  tcase_add_test(test_case, sub_decimal_test_3);
  tcase_add_test(test_case, sub_decimal_test_4);
  tcase_add_test(test_case, sub_decimal_test_5);
  tcase_add_test(test_case, sub_decimal_test_6);
  tcase_add_test(test_case, sub_decimal_test_7);

  tcase_add_test(test_case, mul_decimal_test_1);
  tcase_add_test(test_case, mul_decimal_test_2);
  tcase_add_test(test_case, mul_decimal_test_3);
  tcase_add_test(test_case, mul_decimal_test_4);
  tcase_add_test(test_case, mul_decimal_test_5);

  tcase_add_test(test_case, div_decimal_test_1);
  tcase_add_test(test_case, div_decimal_test_2);
  tcase_add_test(test_case, div_decimal_test_3);
  tcase_add_test(test_case, div_decimal_test_4);
  tcase_add_test(test_case, div_decimal_test_5);
  tcase_add_test(test_case, div_decimal_test_6);
  tcase_add_test(test_case, div_decimal_test_7);
  tcase_add_test(test_case, div_decimal_test_8);

  tcase_add_test(test_case, is_equal_decimal_test_1);
  tcase_add_test(test_case, is_equal_decimal_test_2);
  tcase_add_test(test_case, is_equal_decimal_test_3);
  tcase_add_test(test_case, is_equal_decimal_test_4);
  tcase_add_test(test_case, is_equal_decimal_test_5);
  tcase_add_test(test_case, is_equal_decimal_test_6);

  tcase_add_test(test_case, is_not_equal_decimal_test_1);
  tcase_add_test(test_case, is_not_equal_decimal_test_2);
  tcase_add_test(test_case, is_not_equal_decimal_test_3);
  tcase_add_test(test_case, is_not_equal_decimal_test_4);
  tcase_add_test(test_case, is_not_equal_decimal_test_5);
  tcase_add_test(test_case, is_not_equal_decimal_test_6);

  tcase_add_test(test_case, is_less_decimal_test_1);
  tcase_add_test(test_case, is_less_decimal_test_2);
  tcase_add_test(test_case, is_less_decimal_test_3);
  tcase_add_test(test_case, is_less_decimal_test_4);
  tcase_add_test(test_case, is_less_decimal_test_5);
  tcase_add_test(test_case, is_less_decimal_test_6);
  tcase_add_test(test_case, is_less_decimal_test_7);
  tcase_add_test(test_case, is_less_decimal_test_8);

  tcase_add_test(test_case, is_less_or_equal_test_1);
  tcase_add_test(test_case, is_less_or_equal_test_2);
  tcase_add_test(test_case, is_less_or_equal_test_3);
  tcase_add_test(test_case, is_less_or_equal_test_4);
  tcase_add_test(test_case, is_less_or_equal_test_5);
  tcase_add_test(test_case, is_less_or_equal_test_6);

  tcase_add_test(test_case, is_greater_test_1);
  tcase_add_test(test_case, is_greater_test_2);
  tcase_add_test(test_case, is_greater_test_3);
  tcase_add_test(test_case, is_greater_test_4);
  tcase_add_test(test_case, is_greater_test_5);
  tcase_add_test(test_case, is_greater_test_6);

  tcase_add_test(test_case, is_greater_or_equal_test_1);
  tcase_add_test(test_case, is_greater_or_equal_test_2);
  tcase_add_test(test_case, is_greater_or_equal_test_3);
  tcase_add_test(test_case, is_greater_or_equal_test_4);
  tcase_add_test(test_case, is_greater_or_equal_test_5);
  tcase_add_test(test_case, is_greater_or_equal_test_6);
  tcase_add_test(test_case, is_greater_or_equal_test_7);

  tcase_add_test(test_case, floor_decimal_test_1);
  tcase_add_test(test_case, floor_decimal_test_2);
  tcase_add_test(test_case, floor_decimal_test_3);
  tcase_add_test(test_case, floor_decimal_test_4);
  tcase_add_test(test_case, floor_decimal_test_5);
  tcase_add_test(test_case, floor_decimal_test_6);
  tcase_add_test(test_case, floor_decimal_test_7);
  tcase_add_test(test_case, floor_decimal_test_8);

  tcase_add_test(test_case, round_decimal_test_1);
  tcase_add_test(test_case, round_decimal_test_2);
  tcase_add_test(test_case, round_decimal_test_3);
  tcase_add_test(test_case, round_decimal_test_4);
  tcase_add_test(test_case, round_decimal_test_5);
  tcase_add_test(test_case, round_decimal_test_6);
  tcase_add_test(test_case, round_decimal_test_7);
  tcase_add_test(test_case, round_decimal_test_8);

  tcase_add_test(test_case, truncate_decimal_test_1);
  tcase_add_test(test_case, truncate_decimal_test_2);
  tcase_add_test(test_case, truncate_decimal_test_3);
  tcase_add_test(test_case, truncate_decimal_test_4);

  tcase_add_test(test_case, negate_decimal_test_1);
  tcase_add_test(test_case, negate_decimal_test_2);
  tcase_add_test(test_case, negate_decimal_test_3);
  tcase_add_test(test_case, negate_decimal_test_4);
  tcase_add_test(test_case, negate_decimal_test_5);

  suite_add_tcase(suite, test_case);
  return suite;
}

int main() {
  Suite *s = suite_core();
  SRunner *sr = srunner_create(s);
  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);
  int number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
