
/*
   Related to RDCEP work
*/

import io;
import string;
import stats;

(int x) g(int A) "funcs" "0.0"
[ "set <<x>> [ f_983 <<A>> ]" ];

(int x[]) f(int A[], int n)
{
  int t = sum_integer(A);
  foreach i in [0:n-1]
  {
    x[i] = t;
  }
}

main
{
  int B[][][];
  int N = 4;
  int basic[] = [0,0,0,0]; // Should have N entries

  foreach i in [0:N-1]
  {
    foreach j in [0:N-1]
    {
      int t[];
      if (i == 0) {
        t = basic;
      } else {
        t = f(B[i-1][j], N);
      }

      foreach v, k in t
      {
        B[i][j][k] = t[k];
      }
    }
  }

  trace(B[N-1][N-1][N-1]);
}
