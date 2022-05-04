#include <stdio.h>
#include <stdlib.h>
#include <random>
#include <time.h>
#include <chrono>
#define N 30000 /* 人数 */

//生成した2次元配列を表示
void showarray(int **num, int numline)
{
  for (int i = 1; i < numline; i++)
  {
    for (int j = 1; j < N + 1; j++)
    {
      printf("%d ", num[i][j]);
    }
    printf("\n");
  }
}

// 2次元配列の初期化
void resetarray(int **num, int numline)
{
  for (int i = 0; i < N + 1; i++)
  {
    for (int j = 0; j < N + 1; j++)
    {
      num[i][j] = j; // 初期化
    }
  }
  return;
}

// min_valからmax_val-1の範囲で整数の乱数を返す関数(STL)
uint64_t get_rand(uint64_t min_val, uint64_t max_val)
{
  // 乱数生成器_メルセンヌツイスター(mt19937_64)にtime(NULL)を入れて毎回実行結果を変えたい
  static std::mt19937_64 mt64(time(NULL));

  // [min_val, max_val] の一様分布整数 (int) の分布生成器
  std::uniform_int_distribution<uint64_t> get_rand_uni_int(min_val, max_val);

  // 乱数を生成
  return get_rand_uni_int(mt64);
}

// 2次元配列をシャッフルする関数
void shuffle(int **num, int numline)
{
  for (int i = 0; i < N; i++)
  {
    for (int j = 1; j < N; j++)
    {
      int r = get_rand(j, N);
      int tmp = num[i][j];
      num[i][j] = num[i][r];
      num[i][r] = tmp;
    }
  }
}

int *galeshapley(int **male, int **female)
{
  int next[N + 1] = {0};
  int n = N + 1;
  int *fiancee;
  fiancee = (int *)malloc(sizeof(int) * n);
  // fiancee[n] = {0};

  for (int m = 1; m < N + 1; m++)
  {
    for (int s = m; s != 0;)
    {
      next[s]++;
      int w = male[s][next[s]];
      if (female[w][s] < female[w][fiancee[w]])
      {
        int t = fiancee[w];
        fiancee[w] = s;
        s = t;
      }
    }
  }
  return (fiancee);
}

void showresults(int *fiancee)
{
  printf("f -- m\n======\n");
  for (int f = 1; f < N + 1; f++)
  {
    printf("%d -- %d(%c)\n", f, fiancee[f], 'A' - 1 + fiancee[f]);
  }
  return;
}

int main(void)
{
  // male[1][1]=2の場合、男性Aは女性2を希望リストの1番に設定している。
  // female[1][1]=2の場合、女性1は男性Aを希望リストの2番に設定している。

  int **male;
  int **female;
  int n, m;
  n = N + 1, m = N + 1;

  // maleの分のメモリ確保
  male = (int **)malloc(sizeof(int *) * n);
  for (int i = 0; i < n; i++)
  {
    male[i] = (int *)malloc(sizeof(int) * m);
  }
  resetarray(male, N + 1);

  // femaleの分のメモリ確保
  female = (int **)malloc(sizeof(int *) * n);
  for (int i = 0; i < n; i++)
  {
    female[i] = (int *)malloc(sizeof(int) * m);
  }
  resetarray(female, N + 1);

  for (int i = 0; i < N + 1; i++)
  {
    female[i][0] = N + 1;
  }

  shuffle(male, N + 1);
  shuffle(female, N + 1);

  /*printf("male[N+1][N+1]:\n");
  showarray(male, N + 1);
  printf("female[N+1][N+1]:\n");
  showarray(female, N + 1);
  printf("\n");*/

  //時間計測開始
  using namespace std;
  chrono::system_clock::time_point start, end;

  start = chrono::system_clock::now();

  // gale-shapley algorithmの実行
  int *fiancee = galeshapley(male, female);

  end = chrono::system_clock::now();

  //結果表示
  // showresults(fiancee);
  double time = static_cast<double>(chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0);
  printf("N= %d time %lf[ms]\n", N, time);

  //メモリ解放：fiancee
  free(fiancee);

  //メモリ解放：male
  for (int i = 0; i < n; i++)
  {
    free(male[i]);
  }
  free(male);

  //メモリ解放：female
  for (int i = 0; i < n; i++)
  {
    free(female[i]);
  }
  free(female);
}
