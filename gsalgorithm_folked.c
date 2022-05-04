#include <stdio.h>
#define N 5 /* 人数 */

int main(void)
{
  int male[][N + 1] = {{0, 0, 0, 0, 0, 0},
                       {0, 2, 5, 1, 3, 4},
                       {0, 1, 2, 3, 4, 5},
                       {0, 2, 3, 5, 4, 1},
                       {0, 1, 3, 2, 4, 5},
                       {0, 5, 3, 2, 1, 4}};
  int female[][N + 1] = {{N + 1, 0, 0, 0, 0, 0},
                         {N + 1, 2, 4, 5, 3, 1},
                         {N + 1, 4, 3, 5, 1, 2},
                         {N + 1, 1, 3, 4, 2, 5},
                         {N + 1, 4, 2, 1, 3, 5},
                         {N + 1, 5, 2, 3, 1, 4}};

  int next[N + 1] = {0};
  int fiancee[N + 1] = {0};
  int m, s, w, t, f;

  for (m = 1; m <= N; m++)
  {
    for (s = m; s != 0;)
    {
      next[s]++;
      w = male[s][next[s]];
      if (female[w][s] < female[w][fiancee[w]])
      {
        t = fiancee[w];
        fiancee[w] = s;
        s = t;
      }
    }
  }

  printf("f -- m\n======\n");
  for (f = 1; f <= N; f++)
  {
    printf("%d -- %d(%c)\n", f, fiancee[f], 'A' - 1 + fiancee[f]);
  }
  return 0;
}