int table[][] = new int[1024][1024];
int size, biggest; float blockSize;
int[] dy = {1, 0, -1, 0}, dx = {0, -1, 0, 1};
int ni, nj, startTime = millis(), endTime = -1, rainbow;

int solved() {
  for (int i = 0, k = 1; i < size; i ++)
    for (int j = 0; j < size; j ++, k ++)
    {
      if (i == size - 1 && j == size - 1) continue;
      if (table[i][j] != k) return(0);
    }
  return(1);
}

void setup() {
  String[] lines = loadStrings("./../in");
  size = int(lines[0]);
  biggest = size * size - 1;
  size(900, 900);
  blockSize = float(min(height, width)) / size;
  colorMode(HSB, 360, 100, 100);
  textAlign(CENTER,CENTER);
  textSize(blockSize / 2.34);

  scramble();
  thread("startSolve");
}

void drawBlock(int i, int j) {
  if (table[i][j] == -1) fill(0, 0, 0);
  else fill(340*float(table[i][j])/biggest, 100, 100);
  rect(j * blockSize, i * blockSize, blockSize, blockSize);
  fill(0, 0, 0);
  text(str(table[i][j]), (j + 0.50) * blockSize, (i + 0.45) * blockSize);
}

void draw() {
  if (solved() == 0)
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
    scramble();
  }
}

void scramble() {
  for (int i = 0, k = 1; i < size; i ++) for (int j = 0; j < size; j ++, k ++)
    table[i][j] = k;
  table[size - 1][size - 1] = -1;
  ni = size - 1; nj = size - 1;

  int s = 100;
  while (s -- > 0) {
    int dir; do dir = int(random(0, 4)); while (invalid(ni + dy[dir], nj + dx[dir]));
    int aux = table[ni][nj];
    table[ni][nj] = table[ni + dy[dir]][nj + dx[dir]];
    table[ni + dy[dir]][nj + dx[dir]] = aux;
    ni += dy[dir]; nj += dx[dir];
  }
  startTime = millis(); endTime = -1;
}

void startSolve() {
  solve();
}

void solve() {

}