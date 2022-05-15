#include <stdio.h>
#include <stdlib.h>
#include <random>
#include <time.h>
#include <chrono>
#define N 5 /* 人数 */

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

void showpair(int **male, int **female)
{
    printf("male[N+1][N+1]:\n");
    showarray(male, N + 1);
    printf("female[N+1][N+1]:\n");
    showarray(female, N + 1);
    printf("\n");
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
    for (int i = 1; i < N + 1; i++)
    {
        for (int j = 1; j < N + 1; j++)
        {
            int r = get_rand(j, N);
            int tmp = num[i][j];
            num[i][j] = num[i][r];
            num[i][r] = tmp;
        }
    }
}

//メモリ確保関数
int **malloc_array(int **name)
{
    int n = N + 1;
    name = (int **)malloc(sizeof(int *) * n);
    for (int i = 0; i < n; i++)
    {
        name[i] = (int *)malloc(sizeof(int) * n);
    }
    resetarray(name, n);
    shuffle(name, N + 1);
    return (name);
}

//女性の希望リストを最適化(希望リストの変形)
// female[1][1]=2の場合、女性1は男性Aを希望リストの2番に設定している。
void woman_optimization(int **num, int numline)
{
    int temp[numline][numline];
    // temp arrayにnum arrayをコピー
    for (int i = 1; i < N + 1; i++)
    {
        for (int j = 1; j < N + 1; j++)
        {
            temp[i][j] = num[i][j];
        }
    }
    //最適化
    for (int i = 1; i < N + 1; i++)
    {
        for (int j = 1; j < N + 1; j++)
        {
            for (int k = 1; k < N + 1; k++)
            {
                if (temp[i][k] == j)
                {
                    num[i][j] = k;
                }
            }
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

void free_array(int **name)
{
    for (int i = 0; i < N + 1; i++)
    {
        free(name[i]);
    }
    free(name);
}

int main(void)
{
    int **male;
    int **female;

    male = malloc_array(male);
    female = malloc_array(female);

    //生成した問題を表示
    showpair(male, female);

    // womanのarrayの調整
    woman_optimization(female, N + 1);
    for (int i = 0; i < N + 1; i++)
    {
        female[i][0] = N + 1;
    }

    //時間計測
    using namespace std;
    chrono::system_clock::time_point start, end;

    start = chrono::system_clock::now();

    int *fiancee = galeshapley(male, female);

    end = chrono::system_clock::now();

    //結果表示
    showresults(fiancee);
    double time = static_cast<double>(chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0);
    printf("N= %d time %lf[ms]\n", N, time);

    //メモリ解放
    free_array(male);
    free_array(female);
}
