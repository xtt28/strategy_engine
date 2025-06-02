#include <stdio.h>

struct payoff_entry {
  float payoffs[2];
};

typedef struct payoff_entry payoff_entry;

struct payoff_matrix_row {
  payoff_entry values[2];
};

typedef struct payoff_matrix_row payoff_matrix_row;
  
struct payoff_matrix {
  payoff_matrix_row rows[2];
};

typedef struct payoff_matrix payoff_matrix;

struct utility_function {
  float xy, x, y, con;
};

typedef struct utility_function utility_function;

utility_function utility_function_diff(utility_function util, char wrt) {
  utility_function diff = { 0 };
  if (wrt == 'x') {
    diff.con = util.x;
    diff.y = util.xy;
  } else if (wrt == 'y') {
    diff.con = util.y;
    diff.x = util.xy;
  }
  return diff;
}

float utility_function_findcritical(utility_function util, char wrt) {
  utility_function diff = utility_function_diff(util, wrt);
  return (-1 * diff.con) / (wrt == 'y' ? diff.x : diff.y);
}

utility_function utility_function_create(payoff_matrix matrix, char wrt) {
  int offset = wrt == 'x' ? 0 : 1;
  utility_function ufunc;

  double zero_zero = matrix.rows[0].values[0].payoffs[offset];
  double zero_one = matrix.rows[0].values[1].payoffs[offset];
  double one_zero = matrix.rows[1].values[0].payoffs[offset];
  double one_one = matrix.rows[1].values[1].payoffs[offset];

  ufunc.xy = zero_zero - zero_one - one_zero + one_one;
  ufunc.y = zero_one - one_one;
  ufunc.x = one_zero - one_one;
  ufunc.con = one_one;

  return ufunc;
}

int main(void) {
  payoff_entry zero_zero = { { 4, 7 } };
  payoff_entry zero_one = { { 2, 10 } };
  payoff_entry one_zero = { { 0, 20 } };
  payoff_entry one_one = { { 3, 3 } };
  payoff_matrix_row row_zero = { zero_zero, zero_one };
  payoff_matrix_row row_one = { one_zero, one_one };
  payoff_matrix matrix = { { row_zero, row_one } };
  utility_function func_x = utility_function_create(matrix, 'x');
  utility_function func_y = utility_function_create(matrix, 'y');
  printf("util 1: %.2fxy + %.2fx + %.2fy + %.2f\n", func_x.xy, func_x.x, func_x.y, func_x.con);
  float y = utility_function_findcritical(func_x, 'x');
  float x = utility_function_findcritical(func_y, 'y');
  printf("x=%.2f,y=%.2f", x, y);
}
