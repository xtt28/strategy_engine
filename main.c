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

  float zero_zero = matrix.rows[0].values[0].payoffs[offset];
  float zero_one = matrix.rows[0].values[1].payoffs[offset];
  float one_zero = matrix.rows[1].values[0].payoffs[offset];
  float one_one = matrix.rows[1].values[1].payoffs[offset];

  ufunc.xy = zero_zero - zero_one - one_zero + one_one;
  ufunc.y = one_zero - one_one;
  ufunc.x = zero_one - one_one;
  ufunc.con = one_one;

  return ufunc;
}

char* utility_function_tostring(utility_function func) {
  char* string;
  asprintf(&string, "%.2fxy + %.2fx + %.2fy + %.2f", func.xy, func.x, func.y, func.con);
  return string;
}

int main(void) {
  puts("Payoff matrix data entry");
  payoff_matrix matrix;

  for (int i = 0; i < sizeof(matrix.rows) / sizeof(matrix.rows[0]); ++i) {
    printf("Row %d\n", i + 1);
    for (int j = 0; j < 2; ++j) {
      payoff_entry entry;
      printf("Entry %d (separate with spaces): ", j + 1);
      scanf("%f %f", &entry.payoffs[0], &entry.payoffs[1]);
      matrix.rows[i].values[j] = entry;
    }
  }

  utility_function util_x = utility_function_create(matrix, 'x');
  utility_function util_y = utility_function_create(matrix, 'y');  
  
  printf("f_1(x, y) = %s\n", utility_function_tostring(util_x));
  printf("f_2(x, y) = %s\n\n", utility_function_tostring(util_y));

  printf("x = %.2f\n", utility_function_findcritical(util_y, 'y'));
  printf("y = %.2f\n", utility_function_findcritical(util_x, 'x'));
}
