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

void displayHintsForRow(int r) {
  for (int c = 0; c < N; ++c) {
    displayHints(r, c);
  }
}

void displayHintsForCol(int c) {
  for (int r = 0; r < N; ++r) {
    displayHints(r, c);
  }
}

void displayHintsForSquare(int sr, int sc) {
  int r_low = sr * 3;
  int r_high = r_low + 3;
  int c_low = sc * 3;
  int c_high = c_low + 3;
  for (int r = r_low; r < r_high; ++r) {
    for (int c = c_low; c < c_high; ++c) {
      displayHints(r, c);
    }
  }
}

void log(string s, int r, int c, int h) {
  if (DEBUG_MODE) {
    cerr << s << " " << r << " " << c << " " << h << endl;
  }
} 

bool eraseValuesFromCells(set<int> value, set<pair<int, int> > cells) {
  bool atleast_one_erased = false;
  for (auto cell : cells) {
    int r = cell.first;
    int c = cell.second;
    for (auto v : value) {
      atleast_one_erased |= possible[r][c].erase(v);
    }
  }
  return atleast_one_erased;
}

vector<pair<int, int> > getUnassignedCellsForRow(int r) {
  vector<pair<int, int> > ret;
  for (int c = 0; c < N; ++c) {
    if (g[r][c] == 0) {
      ret.push_back({r, c});
    }
  }
  return ret;
}

vector<pair<int, int> > getUnassignedCellsForCol(int c) {
  vector<pair<int, int> > ret;
  for (int r = 0; r < N; ++r) {
    if (g[r][c] == 0) {
      ret.push_back({r, c});
    }
  }
  return ret;
}

vector<pair<int, int> > getUnassignedCellsForSquare(int sr, int sc) {
  int r_low = sr * 3;
  int r_high = r_low + 3;
  int c_low = sc * 3;
  int c_high = c_low + 3;
  vector<pair<int, int> > ret;
  for (int r = r_low; r < r_high; ++r) {
    for (int c = c_low; c < c_high; ++c) {
      if (g[r][c] == 0) {
        ret.push_back({r, c});
      }
    }  
  }
  return ret;
}

bool clearValueFromSquareExceptCells(int h, int sr, int sc, set<pair<int, int> > allowed) {
  bool erased = false;
  int r_low = sr * 3;
  int r_high = r_low + 3;
  int c_low = sc * 3;
  int c_high = c_low + 3;
  for (int r = r_low; r < r_high; ++r) {
    for (int c = c_low; c < c_high; ++c) {
      if (allowed.find({r, c}) != allowed.end()) {
        continue;
      }
      if (g[r][c] > 0) {
        continue;
      }
      erased |= possible[r][c].erase(h);
    }
  }
  if (erased) {
    log("erased_from_square", sr, sc, h);
  }
  return erased;
}

bool clearValueFromRowExceptCols(int h, int r, set<int> allowed) {
  bool erased = false;
  for (int c = 0; c < N; ++c) {
    if (allowed.find(c) != allowed.end()) {
      continue;
    }
    if (g[r][c] > 0) {
      continue;
    }
    erased |= possible[r][c].erase(h);
  }
  if (erased) {
    log("erased_from_row", r, -1, h);
  }
  return erased;
}

bool clearValueFromColExceptRows(int h, int c, set<int> allowed) {
  bool erased = false;
  for (int r = 0; r < N; ++r) {
    if (allowed.find(r) != allowed.end()) {
      continue;
    }
    if (g[r][c] > 0) {
      continue;
    }
    erased |= possible[r][c].erase(h);
  }
  if (erased) {
    log("erased_from_col", -1, c, h);
  }
  return erased;
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

void setCell(int r, int c, int h) {
  g[r][c] = h;
  clearValueFromColExceptRows(h, c, set<int>());
  clearValueFromRowExceptCols(h, r, set<int>());
  clearValueFromSquareExceptCells(h, r / 3, c / 3, set<pair<int, int> > ());
  if (DEBUG_MODE) {
    auto is_valid = valid();
    if (!is_valid.first) {
      displayHints(r, c);
      cerr << is_valid.second.reason << " " << is_valid.second.r << " " << is_valid.second.c << endl;
      assert(false);
    }  
  }
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
      setCell(r, c, h);
      log("hidden_single_row", r, c, h);
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
      setCell(r, c, h);
      log("hidden_single_col", r, c, h);
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
      setCell(r, c, h);
      log("hidden_single_square", r, c, h);
      return true;
    }
  }
  return false;
}

bool nakedSingle(int r, int c) {
  if (possible[r][c].size() == 1) {
    setCell(r, c, *possible[r][c].begin());
    log("naked_single", r, c, g[r][c]);
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
      change |= clearValueFromRowExceptCols(h, r, set<int> {c_low, c_low + 1, c_low + 2});
    }
    if (cols[h].size() == 1) {
      int c = *cols[h].begin();
      change |= clearValueFromColExceptRows(h, c, set<int> {r_low, r_low + 1, r_low + 2});
    }
  }
  return change;
}

/*
 *  Detects whether a hint value is the only possible in one row or column of a
 *  given square. 
 *  If so, remove it as a possibility from that row / column outside the square 
 */
bool processPointing() {
  bool solved = false;
  for (int box = 0; box < N; ++box) {
    solved |= processPointingForSquare(box / 3, box % 3);
  }
  return solved;
}


set<pair<int, int> > getRowSquareIntersection(int r, int sr, int sc) {
  int r_low = sr * 3;
  int r_high = r_low + 3;
  int c_low = sc * 3;
  int c_high = c_low + 3;
  if (r >= r_low and r < r_high) {
    set<pair<int, int> > ret;
    for (int c = c_low; c < c_high; ++c) {
      ret.insert({r, c});
    }
    return ret;
  }
  assert(false);
}

set<pair<int, int> > getColSquareIntersection(int c, int sr, int sc) {
  int r_low = sr * 3;
  int r_high = r_low + 3;
  int c_low = sc * 3;
  int c_high = c_low + 3;
  if (c >= c_low and c < c_high) {
    set<pair<int, int> > ret;
    for (int r = r_low; r < r_high; ++r) {
      ret.insert({r, c});
    }
    return ret;
  }
  assert(false);
}

bool processClaimingForRow(int r) {
  set<pair<int, int> > squares[10];
  for (int c = 0; c < N; ++c) {
    if (g[r][c] == 0) {
      for (auto h : possible[r][c]) {
        squares[h].insert({r / 3, c / 3});
      }
    }
  }

  bool change = false;
  for (int h = 1; h <= N; ++h) {
    if (squares[h].size() == 1) {
      int sr = squares[h].begin()->first;
      int sc = squares[h].begin()->second;
      change |= clearValueFromSquareExceptCells(h, sr, sc, getRowSquareIntersection(r, sr, sc));
    }
  }
  return change;
}

bool processClaimingForCol(int c) {
  set<pair<int, int> > squares[10];
  for (int r = 0; r < N; ++r) {
    if (g[r][c] == 0) {
      for (auto h : possible[r][c]) {
        squares[h].insert({r / 3, c / 3});
      }
    }
  }

  bool change = false;
  for (int h = 1; h <= N; ++h) {
    if (squares[h].size() == 1) {
      int sr = squares[h].begin()->first;
      int sc = squares[h].begin()->second;
      change |= clearValueFromSquareExceptCells(h, sr, sc, getColSquareIntersection(c, sr, sc));
    }
  }
  return change;
}

/*
 *  Detects whether a hint value is the only possible in one square for a given
 *  of row/column 
 *  If so, remove it as a possibility from all other rows of that square 
 */
bool processClaiming() {
  bool solved = false;
  for (int r = 0; r < N; ++r) {
    solved |= processClaimingForRow(r);
  }
  for (int c = 0; c < N; ++c) {
    solved |= processClaimingForCol(c);
  }
  return solved;
}


bool processHiddenSubsets(vector<pair<int, int> > cells) {
  bool atleast_one_erased = false;
  for (int mask = 1; mask < (1 << cells.size()); ++mask) {
    int number_of_cells = __builtin_popcount(mask);
    set<int> distinct_values;
    set<pair<int, int> > unselected_cells, selected_cells;
    for (int j = 0; j < cells.size(); ++j) {
      if (mask & (1 << j)) {
        int r = cells[j].first;
        int c = cells[j].second;
        for (auto h : possible[r][c]) {
          distinct_values.insert(h);
        }
        selected_cells.insert(cells[j]);
      } else {
        unselected_cells.insert(cells[j]);
      }
    } 
    assert (distinct_values.size() >= number_of_cells);
    if (distinct_values.size() == number_of_cells) {
      atleast_one_erased |= eraseValuesFromCells(distinct_values, unselected_cells);
    }
  }
  return atleast_one_erased;
}

bool processHiddenSubsetsForRow(int r) {
  return processHiddenSubsets(getUnassignedCellsForRow(r));
}

bool processHiddenSubsetsForCol(int c) {
  return processHiddenSubsets(getUnassignedCellsForCol(c));
}

bool processHiddenSubsetsForSquare(int sr, int sc) {
  return processHiddenSubsets(getUnassignedCellsForSquare(sr, sc));
}

/**
 *  Detects a subset of X cells with contains exactly X distinct elements.
 *  If such a subset exists, we can erase those elements from all other cells in that
 *  row, col or square.
 */
bool processHiddenSubsets() {
  bool solved = false;
  for (int i = 0; i < N; ++i) {
    solved |= processHiddenSubsetsForRow(i);
    solved |= processHiddenSubsetsForCol(i);
    solved |= processHiddenSubsetsForSquare(i / 3, i % 3);
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

void testAllStrategies(const int source[N][N]) {
  readGrid(source);
  populateHints();
  bool progress = true;
  int iterations = 100;
  while (iterations-- > 0 and progress and !solved()) {
    progress = refreshHints();
    progress |= processSingles();
    progress |= processPointing();
    progress |= processClaiming();
    progress |= processHiddenSubsets();
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

void hardTests() {
  testAllStrategies(hard);
}

int main() {
  breezyTests();
  easyTests();
  mediumTests();
  hardTests();
}