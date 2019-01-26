//Pranet Verma
#include <bits/stdc++.h>
using namespace std;

const int DEBUG_MODE = 0;
const int N = 9;

int g[N][N];
set<int> possible[N][N];

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

void log(string s, int r, int c, int h) {
  if (DEBUG_MODE) {
    cerr << s << " " << r << " " << c << " " << h << endl;
  }
} 

bool solved() {
  set<int> rows[N], cols[N], squares[3][3];
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (g[i][j] == 0) {
        return false;
      }
      assert(rows[i].insert(g[i][j]).second);
      assert(cols[j].insert(g[i][j]).second);
      assert(squares[i / 3][j / 3].insert(g[i][j]).second);
    }  
  }
  return true;
}

enum ValidationFailureReason {
  ROW = 0,
  COL = 1,
  SQUARE = 2,
  VALID = 3,
};

struct Failure {
  ValidationFailureReason reason;
  int r;
  int c;
};

pair<bool, Failure> valid() {
  set<int> rows[N], cols[N], squares[3][3];
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (g[i][j] == 0) {
        continue;
      }
      if (!rows[i].insert(g[i][j]).second) {
        return {false, Failure{ValidationFailureReason::ROW, i, j}};
      }
      if (!cols[j].insert(g[i][j]).second) {
        return {false, Failure{ValidationFailureReason::COL, i, j}};
      }
      if (!squares[i / 3][j / 3].insert(g[i][j]).second) {
        return {false, Failure{ValidationFailureReason::SQUARE, i, j}};
      }
    }  
  }
  return {true, Failure{ValidationFailureReason::VALID, -1, -1}};
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
  int r_high = r_low + 3;
  int c_low = sc * 3;
  int c_high = c_low + 3;
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
  auto is_valid = valid();
  if (!is_valid.first) {
    cerr << is_valid.second.reason << " " << is_valid.second.r << " " << is_valid.second.c << endl;
    assert(false);
  }
}

void readGrid(const int source[N][N]) {
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      g[i][j] = source[i][j];
    }
  }
  auto is_valid = valid();
  if (!is_valid.first) {
    cerr << is_valid.second.reason << " " << is_valid.second.r << " " << is_valid.second.c << endl;
    assert(false);
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
      log("hidden_single_row", r, c, h);
      auto is_valid = valid();
      if (!is_valid.first) {
        cerr << is_valid.second.reason << " " << is_valid.second.r << " " << is_valid.second.c << endl;
        assert(false);
      }
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
    if (getPossibleOccurancesInCol(c, h) == 1) {
      g[r][c] = h;
      log("hidden_single_col", r, c, h);
      auto is_valid = valid();
      if (!is_valid.first) {
        cerr << is_valid.second.reason << " " << is_valid.second.r << " " << is_valid.second.c << endl;
        assert(false);
      }
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
    if (getPossibleOccurancesInSquare(r / 3, c / 3, h) == 1) {
      g[r][c] = h;
      log("hidden_single_square", r, c, h);
      auto is_valid = valid();
      if (!is_valid.first) {
        displayHints(r, c);
        cerr << is_valid.second.reason << " " << is_valid.second.r << " " << is_valid.second.c << endl;
        assert(false);
      }
      return true;
    }
  }
  return false;
}

bool nakedSingle(int r, int c) {
  if (possible[r][c].size() == 1) {
    g[r][c] = *possible[r][c].begin();
    log("naked_single", r, c, g[r][c]);
    auto is_valid = valid();
    if (!is_valid.first) {
      cerr << is_valid.second.reason << " " << is_valid.second.r << " " << is_valid.second.c << endl;
      assert(false);
    }
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


bool clearValueFromRowExceptCols(int h, int r, int cl, int ch) {
  bool erased = false;
  for (int c = 0; c < N; ++c) {
    if (c >= cl and c < ch) {
      continue;
    }
    if (g[r][c] > 0) {
      continue;
    }
    erased |= possible[r][c].erase(h);
  }
  if (erased) {
    log("erased", r, -1, h);
  }
  return erased;
}

bool clearValueFromColExceptRows(int h, int c, int rl, int rh) {
  bool erased = false;
  for (int r = 0; r < N; ++r) {
    if (r >= rl and r < rh) {
      continue;
    }
    if (g[r][c] > 0) {
      continue;
    }
    erased |= possible[r][c].erase(h);
  }
  if (erased) {
    log("erased", -1, c, h);
  }
  return erased;
}

bool processPointingForSquare(int sr, int sc) {
  int r_low = sr * 3;
  int r_high = r_low + 3;
  int c_low = sc * 3;
  int c_high = c_low + 3;
  set<int> rows[10], cols[10];
  for (int r = r_low; r < r_high; ++r) {
    for (int c = c_low; c < c_high; ++c) {
      if (g[r][c] == 0) {
        for (auto h : possible[r][c]) {
          rows[h].insert(r);
          cols[h].insert(c);
        }
      }   
    }
  }
  bool change = false;
  for (int h = 1; h <= N; ++h) {
    if (rows[h].size() == 1) {
      int r = *rows[h].begin();
      change |= clearValueFromRowExceptCols(h, r, c_low, c_high);
    }
    if (cols[h].size() == 1) {
      int c = *cols[h].begin();
      change |= clearValueFromColExceptRows(h, c, r_low, r_high);
    }
  }
  return change;
}

/*
 *  Detects whether a hint value is the only possible one in row or column of a
 *  given square. 
 *  If so, remove it as a possibility from that row / column outside the box  
 */
bool processPointing() {
  bool solved = false;
  for (int box = 0; box < N; ++box) {
    solved |= processPointingForSquare(box / 3, box % 3);
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

const int hard[N][N] = {
  {0, 0, 7, 2, 0, 3, 0, 0, 0},
  {0, 0, 0, 5, 0, 0, 9, 0, 0},
  {0, 6, 0, 0, 9, 4, 0, 0, 5},
  {6, 0, 5, 0, 0, 0, 0, 3, 8},
  {0, 0, 9, 0, 0, 0, 1, 0, 0},
  {8, 1, 0, 0, 0, 0, 7, 0, 9},
  {3, 0, 0, 4, 8, 0, 0, 9, 0},
  {0, 0, 8, 0, 0, 7, 0, 0, 0},
  {0, 0, 0, 6, 0, 2, 3, 0, 0},
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

  readGrid(hard);
  populateHints();
  bool progress = true;
  int iterations = 100;
  while (iterations-- > 0 and progress and !solved()) {
    progress = refreshHints();
    progress |= processSingles();
    progress |= processPointing();
  }
  display();
  if (!solved()) {
    assert(false);
  }

}