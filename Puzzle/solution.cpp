#include <bits/stdc++.h>
#include <sys/resource.h>
#define DEBUG if(0)
#define lli long long int
using namespace std;
int n, sqn; // table[1000][1000]
vector<vector<int> > table;
set<vector<vector<int> > > visitedSet;
deque<pair<vector<vector<int> >, pair<int, pair<int, int> > > > q;
int dy[4] = {-1, 0, 1, 0}, dx[4] = {0, 1, 0, -1};
int minSteps = -1;

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

// string state()
// {
//   string nowState = "";
//   for (int i = 0; i < n; i ++)
//     for (int j = 0; j < n; j ++)
//       nowState += ('0' + table[i][j]) + "|";
//   return(nowState);
// }

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
  // DEBUG { printf("hash: %s\n", state()); printTable(); }
  if (i == n - 1 && j == n - 1 && solved())
  {
    minSteps = now;
    return(1);
  }
  if (visitedSet.count(table)) return(0);
  visitedSet.insert(table);

  for (int k = 0; k < 4; k ++)
    if (valid(i + dy[k], j + dx[k]))
    {
      swap(&table[i][j], &table[i + dy[k]][j + dx[k]]);
      if (dfs(i + dy[k], j + dx[k], now + 1)) return(1);
      swap(&table[i][j], &table[i + dy[k]][j + dx[k]]);
    }
  return(0);
}

void bfs(int i, int j)
{
  q.push_back({table, {0, {i, j}}});
  while (!q.empty())
  {
    table = q.front().first; minSteps = q.front().second.first; i = q.front().second.second.first; j = q.front().second.second.second; q.pop_front();
    // printTable(); printf("\n");
    if (solved()) break;

    for (int k = 0; k < 4; k ++)
      if (valid(i + dy[k], j + dx[k]))
      {
        swap(&table[i][j], &table[i + dy[k]][j + dx[k]]);
        if (!visitedSet.count(table))
        {
          visitedSet.insert(table);
          q.push_back({table, {minSteps + 1, {i + dy[k], j + dx[k]}}});
        }
        swap(&table[i][j], &table[i + dy[k]][j + dx[k]]);
      }
  }
  // printTable();
  while (!q.empty()) q.pop_front();
}

int main()
{
  rlimit R;
  getrlimit(RLIMIT_STACK, &R);
  R.rlim_cur = R.rlim_max;
  setrlimit(RLIMIT_STACK, &R);
  srand(time(NULL));

  scanf("%d", &n); sqn = 9; int kk = 1;
  for (int i = 0; i < n; i ++) table.push_back(vector<int>(n));
  while (1)
  {
    scramble();
    vector<vector<int> > aux = table;
    int si, sj;
    for (int i = 0; i < n; i ++) for (int j = 0; j < n; j ++) if (table[i][j] == 0) si = i, sj = j;

    printTable();
    printf("DFS:\n");
    minSteps = -1;
    visitedSet.clear();
    dfs(si, sj, 0);
    printf("\tReached %ld different states\n", visitedSet.size());
    printf("\tTook %d steps\n", minSteps);

    table = aux;
    printf("BFS:\n");
    visitedSet.clear();
    bfs(si, sj);
    printf("\tReached %ld different states\n", visitedSet.size());
    printf("\tTook %d steps\n\n", minSteps);
  }
  return(0);
}