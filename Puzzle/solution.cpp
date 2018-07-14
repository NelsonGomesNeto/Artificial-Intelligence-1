#include <bits/stdc++.h>
#define DEBUG if(0)
#define lli long long int
using namespace std;
int table[1000][1000], n, sqn;
int *visited;
unordered_map<lli, int> visitedMap;
int dy[4] = {-1, 0, 1, 0}, dx[4] = {0, 1, 0, -1};
int states = 0, minSteps = -1;

void printTable()
{
  int biggest = log10(n * n) + 1;
  for (int i = 0; i < n; i ++)
    for (int j = 0; j < n; j ++)
      printf("%*d%c", biggest, table[i][j], j < n - 1 ? ' ' : '\n');
}

void swap(int *a, int *b)
{
  int aux = *a;
  *a = *b;
  *b = aux;
}

int valid(int i, int j)
{
  return(!(i < 0 || i >= n || j < 0 || j >= n));
}

lli state()
{
  lli nowState = 0, k = 1;
  for (int i = 0; i < n; i ++)
    for (int j = 0; j < n; j ++, k *= sqn)
      nowState += k * table[i][j];
  return(nowState);
}

void scramble()
{
  for (int i = 0, k = 1; i < n; i ++) for (int j = 0; j < n; j ++, k ++) table[i][j] = k; table[n - 1][n - 1] = 0;

  int s = 100, i = n - 1, j = n - 1;
  while (s --)
  {
    int dir; do dir = rand() % 4; while (!valid(i + dy[dir], j + dx[dir]));
    swap(&table[i][j], &table[i + dy[dir]][j + dx[dir]]);
    i += dy[dir], j += dx[dir];
  }
}

int solved()
{
  for (int i = 0, k = 1; i < n; i ++)
    for (int j = 0; j < n; j ++, k ++)
    {
      if (i == n - 1 && j == n - 1) continue;
      if (table[i][j] != k) return(0);
    }
  return(1);
}

int dfs(int i, int j, int now)
{
  DEBUG { printf("hash: %lld\n", state()); printTable(); }
  if (i == n - 1 && j == n - 1 && solved())
  {
    minSteps = now;
    return(1);
  }
  lli nowState = state();
  if (visitedMap[nowState]) return(0);
  states ++; visitedMap[nowState] = 1;

  for (int k = 0; k < 4; k ++)
    if (valid(i + dy[k], j + dx[k]))
    {
      swap(&table[i][j], &table[i + dy[k]][j + dx[k]]);
      if (dfs(i + dy[k], j + dx[k], now + 1)) return(1);
      swap(&table[i][j], &table[i + dy[k]][j + dx[k]]);
    }
  return(0);
}

int main()
{
  srand(time(NULL));
  // visited = (int*) malloc((int) 4e8 * nof(int));
  // memset(visited, 0, (int) 4e8 * nof(int));
  scanf("%d", &n); sqn = n * n;
  while (1)
  {
    scramble();
    int si, sj;
    for (int i = 0; i < n; i ++) for (int j = 0; j < n; j ++)
      if (table[i][j] == 0) si = i, sj = j;

    printTable();
    states = 0; minSteps = -1;
    visitedMap.clear();
    dfs(si, sj, 0);
    // printf("\n"); printTable();
    printf("Reached %d different states\n", states);
    printf("Took %d steps\n\n", minSteps);
  }
  // free(visited);
  return(0);
}