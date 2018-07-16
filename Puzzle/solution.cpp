#include <bits/stdc++.h>
#include <sys/resource.h>
#define DEBUG if(0)
#define lli long long int
#define vv vector<vector<int> >
#define ele pair<vv, pair<int, double> >
using namespace std;
int n, sqn;
vv table;
set<string> visitedSet;
deque<pair<vv, int> > q;
bool compare(ele a, ele b)
{
  return(a.second.second > b.second.second);
}
priority_queue<ele, vector<ele >, function<bool(ele, ele)> > pq(compare);
int dy[4] = {-1, 0, 1, 0}, dx[4] = {0, 1, 0, -1};
int minSteps = -1, scrambleSteps = 1000;


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

string state()
{
  string aux = "";
  for (int i = 0; i < n; i ++)
    for (int j = 0; j < n; j ++)
    {
      aux.push_back('0' + table[i][j]);
      if (i != n - 1 || j != n - 1) aux += "|";
    }
  return(aux);
}

void scramble()
{
  for (int i = 0, k = 1; i < n; i ++) for (int j = 0; j < n; j ++, k ++) table[i][j] = k; table[n - 1][n - 1] = 0;

  int s = scrambleSteps, i = n - 1, j = n - 1;
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

int dfsRecursive(int i, int j, int now)
{
  // DEBUG { printf("hash: %s\n", state()); printTable(); }
  if (i == n - 1 && j == n - 1 && solved())
  {
    minSteps = now;
    return(1);
  }

  for (int k = 0; k < 4; k ++)
    if (valid(i + dy[k], j + dx[k]))
    {
      swap(&table[i][j], &table[i + dy[k]][j + dx[k]]);
      string nowState = state();
      if (!visitedSet.count(nowState))
      {
        visitedSet.insert(nowState);
        if (dfsRecursive(i + dy[k], j + dx[k], now + 1)) return(1);
      }
      swap(&table[i][j], &table[i + dy[k]][j + dx[k]]);
    }
  return(0);
}

void dfs(int i, int j)
{
  q.push_front({table, 0});
  while (!q.empty())
  {
    table = q.front().first; minSteps = q.front().second; q.pop_front();
    for (int l = 0; l < n; l ++) for (int m = 0; m < n; m ++) if (table[l][m] == 0) i = l, j = m;
    // printTable(); printf("\n");
    if (solved()) break;

    for (int k = 0; k < 4; k ++)
      if (valid(i + dy[k], j + dx[k]))
      {
        swap(&table[i][j], &table[i + dy[k]][j + dx[k]]);
        string nowState = state();
        if (!visitedSet.count(nowState))
        {
          visitedSet.insert(nowState);
          q.push_front({table, minSteps + 1});
        }
        swap(&table[i][j], &table[i + dy[k]][j + dx[k]]);
      }
  }
  while (!q.empty()) q.pop_front();
}

void bfs(int i, int j)
{
  q.push_back({table, 0});
  while (!q.empty())
  {
    table = q.front().first; minSteps = q.front().second; q.pop_front();
    for (int l = 0; l < n; l ++) for (int m = 0; m < n; m ++) if (table[l][m] == 0) i = l, j = m;
    if (solved()) break;

    for (int k = 0; k < 4; k ++)
      if (valid(i + dy[k], j + dx[k]))
      {
        swap(&table[i][j], &table[i + dy[k]][j + dx[k]]);
        string nowState = state();
        if (!visitedSet.count(nowState))
        {
          visitedSet.insert(nowState);
          q.push_back({table, minSteps + 1});
        }
        swap(&table[i][j], &table[i + dy[k]][j + dx[k]]);
      }
  }
  while (!q.empty()) q.pop_front();
}

double manhattanDistance()
{
  int dist = 0;
  for (int i = 0; i < n; i ++)
    for (int j = 0; j < n; j ++)
    {
      int at = table[i][j] - 1;
      dist += abs(at / n - i) + abs(at % n - j);
    }
  return(dist);
}

double euclideanDistance()
{
  double dist = 0;
  for (int i = 0; i < n; i ++)
    for (int j = 0; j < n; j ++)
    {
      if (table[i][j] == 0) continue;
      int at = table[i][j] - 1;
      dist += sqrt(pow(at / n - i, 2) + pow(at % n - j, 2)); //sqrt
    }
  return(dist);
}

void aStar(int i, int j, double (*heuristic)())
{
  pq.push({table, {0, heuristic()}});
  while (!pq.empty())
  {
    table = pq.top().first; minSteps = pq.top().second.first; pq.pop();
    for (int l = 0; l < n; l ++) for (int m = 0; m < n; m ++) if (table[l][m] == 0) i = l, j = m;
    if (solved()) break;

    for (int k = 0; k < 4; k ++)
      if (valid(i + dy[k], j + dx[k]))
      {
        swap(&table[i][j], &table[i + dy[k]][j + dx[k]]);
        string nowState = state();
        if (!visitedSet.count(nowState))
        {
          visitedSet.insert(nowState);
          pq.push({table, {minSteps + 1, heuristic()}});
        }
        swap(&table[i][j], &table[i + dy[k]][j + dx[k]]);
      }
  }
  while (!pq.empty()) pq.pop();
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
  while (kk)
  {
    scramble();
    printTable();
    vv aux = table;
    int si, sj;
    for (int i = 0; i < n; i ++) for (int j = 0; j < n; j ++) if (table[i][j] == 0) si = i, sj = j;

    // printf("DFS:\n");
    // visitedSet.clear();
    // dfs(si, sj);
    // printf("\tReached %ld different states\n", visitedSet.size());
    // printf("\tTook %d steps\n", minSteps);
    //
    // table = aux;
    // printf("BFS:\n");
    // visitedSet.clear();
    // bfs(si, sj);
    // printf("\tReached %ld different states\n", visitedSet.size());
    // printf("\tTook %d steps\n\n", minSteps);

    // table = aux;
    // printf("A* (manhattanDistance):\n");
    // visitedSet.clear();
    // aStar(si, sj, manhattanDistance);
    // printf("\tReached %ld different states\n", visitedSet.size());
    // printf("\tTook %d steps\n\n", minSteps);

    table = aux;
    printf("A* (euclideanDistance):\n");
    visitedSet.clear();
    aStar(si, sj, euclideanDistance);
    printf("\tReached %ld different states\n", visitedSet.size());
    printf("\tTook %d steps\n\n", minSteps);
  }
  return(0);
}
