import java.util.Set;
import java.util.HashSet;
import java.util.Queue;
import java.util.ArrayDeque;
int table[][] = new int[1024][1024];
int size, sqSize, biggest; float blockSize;
int[] dy = {1, 0, -1, 0}, dx = {0, -1, 0, 1};
int ni, nj, startTime = millis(), endTime = -1, rainbow, waitTime = 100;
int minSteps;
Set<String> visitedSet = new HashSet<String>();
Queue<int[][]> queue = new ArrayDeque<int[][]>();

boolean solved() {
  for (int i = 0, k = 1; i < size; i ++)
    for (int j = 0; j < size; j ++, k ++)
    {
      if (i == size - 1 && j == size - 1) continue;
      if (table[i][j] != k) return(false);
    }
  return(true);
}

void setup() {
  String[] lines = loadStrings("./../in");
  size = int(lines[0]); sqSize = size * size;
  biggest = size * size - 1;
  size(900, 900);
  blockSize = float(min(height, width)) / size;
  colorMode(HSB, 360, 100, 100);
  textAlign(CENTER,CENTER);
  textSize(blockSize / 2.34);

  scramble();
}

void drawBlock(int i, int j) {
  if (table[i][j] == 0) fill(0, 0, 0);
  else fill(340*float(table[i][j])/biggest, 100, 100);
  rect(j * blockSize, i * blockSize, blockSize, blockSize);
  fill(0, 0, 0);
  text(str(table[i][j]), (j + 0.50) * blockSize, (i + 0.45) * blockSize);
}

void draw() {
  if (!solved())
    for (int i = 0; i < size; i ++)
      for (int j = 0; j < size; j ++)
        drawBlock(i, j);
  else {
    background(rainbow % 360, 100, 100);
    rainbow = (rainbow + 4) % 36000;
    if (endTime == -1) endTime = millis();
    text(str((endTime - startTime) / 1000.0) + "s", height / 2, width / 2);
  }
}

boolean invalid(int i, int j) {
  return((i < 0 || j < 0 || i >= size || j >= size));
}

void movement(int dir, int i, int j) {
  if (invalid(i + dy[dir], j + dx[dir])) return;
  // print("WTF", table[i][j], dy[dir], dx[dir], "\n");
  int aux = table[i][j];
  table[i][j] = table[i + dy[dir]][j + dx[dir]];
  table[i + dy[dir]][j + dx[dir]] = aux;
  ni += dy[dir]; nj += dx[dir];
  minSteps ++;
}

void keyReleased() {
  if (keyCode == UP) {
    movement(0, ni, nj);
  } else if (keyCode == RIGHT) {
    movement(1, ni, nj);
  } else if (keyCode == DOWN) {
    movement(2, ni, nj);
  } else if (keyCode == LEFT) {
    movement(3, ni, nj);
  } else if (key == 'r') {
    visitedSet.clear();
    scramble();
  } else if (key == 's') {
    thread("startSolve");
  }
}

void scramble() {
  for (int i = 0, k = 1; i < size; i ++) for (int j = 0; j < size; j ++, k ++)
    table[i][j] = k;
  table[size - 1][size - 1] = 0;
  ni = size - 1; nj = size - 1;
  // table[size - 1][size - 2] = 0; table[size - 1][size - 1] = 8;

  int s = 100;
  while (s -- > 0) {
    int dir; do dir = int(random(0, 4)); while (invalid(ni + dy[dir], nj + dx[dir]));
    int aux = table[ni][nj];
    table[ni][nj] = table[ni + dy[dir]][nj + dx[dir]];
    table[ni + dy[dir]][nj + dx[dir]] = aux;
    ni += dy[dir]; nj += dx[dir];
  }
  startTime = millis(); endTime = -1; minSteps = 0;
}

void startSolve() {
  // dfs(ni, nj, 0);
  bfs();
}

String state() {
  String nowState = "|";
  for (int i = 0; i < size; i ++)
    for (int j = 0; j < size; j ++)
      nowState += str(table[i][j]) + "|";
  return(nowState);
}

boolean dfs(int i, int j, int now) {
  if (i == size - 1 && j == size - 1 && solved()) {
    minSteps = now;
    return(true);
  }
  String nowState = state();
  if (visitedSet.contains(nowState)) { print("Holly\n"); delay(10000); return(false); };
  visitedSet.add(nowState);

  delay(waitTime);
  for (int k = 0; k < 4; k ++)
    if (!invalid(i + dy[k], j + dx[k])) {
      int aux = table[i][j]; table[i][j] = table[i + dy[k]][j + dx[k]]; table[i + dy[k]][j + dx[k]] = aux;
      if (dfs(i + dy[k], j + dx[k], now + 1)) return(true);
      aux = table[i][j]; table[i][j] = table[i + dy[k]][j + dx[k]]; table[i + dy[k]][j + dx[k]] = aux;
    }
  return(false);
}

void bfs() {
  queue.clear();
  queue.add(table.clone());

  while (queue.size() > 0) {
    print("Entered\n");
    int bi = 0, bj = 0;
    table = queue.poll();
    delay(waitTime);
    for (int i = 0; i < size; i ++) for (int j = 0; j < size; j ++) if (table[i][j] == 0) { bi = i; bj = j; }
    if (solved()) break;

    for (int k = 0; k < 4; k ++)
      if (!invalid(bi + dy[k], bj + dx[k])) {
        int aa = table[bi][bj]; table[bi][bj] = table[bi + dy[k]][bj + dx[k]]; table[bi + dy[k]][bj + dx[k]] = aa;
        String nowState = state();
        if (!visitedSet.contains(nowState)) {
          visitedSet.add(nowState);
          queue.add(table.clone());
        }
        aa = table[bi][bj]; table[bi][bj] = table[bi + dy[k]][bj + dx[k]]; table[bi + dy[k]][bj + dx[k]] = aa;
      }
  }
  print("Solved" + str(solved()) + "\n");
}