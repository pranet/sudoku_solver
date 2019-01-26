//Pranet Verma
#include <bits/stdc++.h>
using namespace std;
int DEBUG = false;
const int N = 9;


int g[N][N];
unordered_set<int> possible[N][N];


void display() {
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      cerr << g[i][j] << " ";
    }
    cerr << endl;
  }
}

void displayHints(int r, int c) {
  cerr << "Possible values at " << r << " " << c << " : ";
  if (g[r][c] > 0) {
    cerr << g[r][c];
  } else {
    for (auto h : possible[r][c]) {
      cerr << h << " ";
    }
  }
  cerr << endl;
}

bool solved() {
  unordered_set<int> rows[N], cols[N], squares[3][3];
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (g[i][j] == 0) {
        return false;
      }
      rows[i].insert(g[i][j]);
      cols[j].insert(g[i][j]);
      squares[i / 3][j / 3].insert(g[i][j]);
    }  
  }
  for (int i = 0; i < N; ++i) {
    assert(rows[i].size() == N);
    assert(cols[i].size() == N);
    assert(squares[i / 3][i % 3].size() == N);
  }
  return true;
}

bool existsInRow(int r, int v) {
  for (int c = 0; c < N; ++c) {
    if (g[r][c] == v) {
      return true;
    }
  }
  return false;
}

bool existsInCol(int c, int v) {
  for (int r = 0; r < N; ++r) {
    if (g[r][c] == v) {
      return true;
    }
  }
  return false;
}

bool existsInSquare(int sr, int sc, int v) {
  int r_low = sr * 3;
  int r_high = r_low + 2;
  int c_low = sc * 3;
  int c_high = c_low + 2;
  for (int r = r_low; r < r_high; ++r) {
    for (int c = c_low; c < c_high; ++c) {
      if (g[r][c] == v) {
        return true;
      } 
    }
  }
  return false;
}

bool refreshHints() {
  bool progress = false;
  for (int r = 0; r < N; ++r) {
    for (int c = 0; c < N; ++c) {
      if (g[r][c] > 0) {
        continue;
      }
      vector<int> impossible;
      for (int h : possible[r][c]) {
        bool existsInRow = ::existsInRow(r, h);
        bool existsInCol = ::existsInCol(c, h);
        bool existsInSquare = ::existsInSquare(r / 3, c / 3, h);
        if (existsInRow or existsInSquare or existsInCol) {
          impossible.push_back(h);
        } 
      }
      for (auto h : impossible) {
        progress = true;
        possible[r][c].erase(h);
      }
    }
  }
  return progress;
}

void populateHints() {
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (g[i][j] == 0) {
        for (int v = 1; v <= N; ++v) {
          possible[i][j].insert(v);
        }
      }
    }
  }
}

void readGrid() {
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      cin >> g[i][j];
    }
  }
}

void readGrid(const int source[N][N]) {
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      g[i][j] = source[i][j];
    }
  }
}

int getPossibleOccurancesInRow(int r, int h) {
  int cnt = 0;
  for (int c = 0; c < N; ++c) {
    if (g[r][c] == h) {
      ++cnt;
    }
    else if (g[r][c] == 0 and possible[r][c].find(h) != possible[r][c].end()) {
      ++cnt;
    }
  }
  return cnt;
}

int getPossibleOccurancesInCol(int c, int h) {
  int cnt = 0;
  for (int r = 0; r < N; ++r) {
    if (g[r][c] == h) {
      ++cnt;
    }
    else if (g[r][c] == 0 and possible[r][c].find(h) != possible[r][c].end()) {
      ++cnt;
    }
  }
  return cnt;
}

int getPossibleOccurancesInSquare(int sr, int sc, int h) {
  int r_low = sr * 3;
  int r_high = r_low + 3;
  int c_low = sc * 3;
  int c_high = c_low + 3;
  int cnt = 0;
  for (int r = r_low; r < r_high; ++r) {
    for (int c = c_low; c < c_high; ++c) {
      if (g[r][c] == h) {
      ++cnt;
      }
      else if (g[r][c] == 0 and possible[r][c].find(h) != possible[r][c].end()) {
        ++cnt;
      } 
    }
  }
  return cnt;
}

/*
 *  Detects whether a hint value is the only possible one in its row  
 */
bool hiddenSinglesByRow(int r, int c) {
  for (int h : possible[r][c]) {
    if (getPossibleOccurancesInRow(r, h) == 1) {
      g[r][c] = h;
      return true;
    }
  }
  return false;
}

/*
 *  Detects whether a hint value is the only possible one in its col  
 */
bool hiddenSinglesByCol(int r, int c) {
  for (int h : possible[r][c]) {
    if(getPossibleOccurancesInCol(c, h) == 1) {
      g[r][c] = h;
      return true;
    }
  }
  return false;
}

/*
 *  Detects whether a hint value is the only possible one in its square
 */
bool hiddenSinglesBySquare(int r, int c) {
  for (int h : possible[r][c]) {
    if(getPossibleOccurancesInSquare(r / 3, c / 3, h) == 1) {
      g[r][c] = h;
      return true;
    }
  }
  return false;
}

bool nakedSingle(int r, int c) {
  if (possible[r][c].size() == 1) {
    g[r][c] = *possible[r][c].begin();
    return true;
  }
  return false;
}

bool processSingles() {
  bool solved = false;
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (g[i][j] == 0) {
        solved |= nakedSingle(i, j);
      }
      if (g[i][j] == 0) {
        solved |= hiddenSinglesByRow(i, j);
      }
      if (g[i][j] == 0) {
        solved |= hiddenSinglesByCol(i, j);
      }
      if (g[i][j] == 0) {
        solved |= hiddenSinglesBySquare(i, j);
      }
    }    
  }
  return solved;
}


// should be solvable using only one of the 4 single strategies
void testNakedSingle(const int source[N][N]) {
  readGrid(source);
  populateHints();
  bool progress = true;
  int iterations = 100;
  while (iterations-- > 0 and progress and !solved()) {
    progress = refreshHints();
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j) {
        if (g[i][j] == 0) {
          progress |= nakedSingle(i, j);
        }
      }
    }
  }
  assert(solved());
}

// should be solvable using only one of the 4 single strategies
void testSinglesByRow(const int source[N][N]) {
  readGrid(source);
  populateHints();
  bool progress = true;
  int iterations = 100;
  while (iterations-- > 0 and progress and !solved()) {
    progress = refreshHints();
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j) {
        if (g[i][j] == 0) {
          progress |= hiddenSinglesByRow(i, j);
        }
      }
    }
  }
  assert(solved());
}

// should be solvable using only one of the 4 single strategies
void testSinglesByCol(const int source[N][N]) {
  readGrid(source);
  populateHints();
  bool progress = true;
  int iterations = 100;
  while (iterations-- > 0 and progress and !solved()) {
    progress = refreshHints();
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j) {
        if (g[i][j] == 0) {
          progress |= hiddenSinglesByCol(i, j);
        }
      }
    }
  }
  assert(solved());
}

// should be solvable using only one of the 4 single strategies
void testSinglesBySquare(const int source[N][N]) {
  readGrid(source);
  populateHints();
  bool progress = true;
  int iterations = 100;
  while (iterations-- > 0 and progress and !solved()) {
    progress = refreshHints();
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j) {
        if (g[i][j] == 0) {
          progress |= hiddenSinglesBySquare(i, j);
        }
      }
    }
  }
  assert(solved());
}

void testAllSingles(const int source[N][N]) {
  readGrid(source);
  populateHints();
  bool progress = true;
  int iterations = 100;
  while (iterations-- > 0 and progress and !solved()) {
    progress = refreshHints();
    progress |= processSingles();
  }

  assert(solved());
}

const int breezy[N][N] = {
  {5, 0, 0, 4, 0, 0, 6, 8, 9},
  {8, 0, 0, 3, 6, 9, 1, 0, 0},
  {6, 2, 0, 5, 8, 1, 0, 0, 0},
  {0, 6, 2, 8, 0, 3, 4, 5, 1},
  {0, 8, 5, 0, 1, 0, 7, 9, 0},
  {4, 9, 1, 6, 0, 7, 2, 3, 0},
  {0, 0, 0, 9, 2, 6, 0, 4, 7},
  {0, 0, 7, 1, 3, 5, 0, 0, 2},
  {2, 5, 6, 0, 0, 8, 0, 0, 3},
};

const int easy[N][N] = {
  {0, 0, 0, 0, 6, 0, 8, 0 ,0},
  {0, 0, 9, 5, 7, 8, 0, 0, 0},
  {0, 5, 0, 2, 0, 3, 0, 0, 9},
  {0, 4, 7, 0, 0, 0, 2, 8, 0},
  {2, 8, 0, 0, 0, 0, 0, 1, 3},
  {0, 3, 1, 0, 0, 0, 9, 7, 0},
  {1, 0, 0, 6, 0, 9, 0, 2, 0},
  {0, 0, 0, 3, 2, 7, 6, 0, 0},
  {0, 0, 2, 0, 4, 0, 0, 0, 0},
};  

const int medium[N][N] = {
  {0, 0, 0, 1, 0, 4, 0, 0, 0},
  {5, 0, 1, 0, 0, 0, 6, 0, 7},
  {0, 4, 3, 0, 0, 0, 5, 1, 0},
  {0, 3, 0, 9, 0, 1, 0, 6, 0},
  {0, 1, 7, 6, 0, 8, 9, 2, 0},
  {0, 0, 0, 0, 2, 0, 0, 0, 0},
  {0, 0, 0, 0, 8, 0, 0, 0, 0},
  {0, 9, 4, 3, 0, 7, 8, 5, 0},
  {0, 0, 0, 5, 0, 9, 0, 0, 0},
};

void breezyTests() {
  testNakedSingle(breezy);
  testSinglesByRow(breezy);
  testSinglesByCol(breezy);
  testSinglesBySquare(breezy);
}

void easyTests() {
  testAllSingles(easy);
}

void mediumTests() {
  testAllSingles(medium);
}


int main() {
  breezyTests();
  easyTests();
  mediumTests();

  readGrid(breezy);
  populateHints();
  bool progress = true;
  int iterations = 100;
  while (iterations-- > 0 and progress and !solved()) {
    progress = refreshHints();
    progress |= processSingles();
  }
  display();
  if (!solved()) {
    assert(false);
  }

}