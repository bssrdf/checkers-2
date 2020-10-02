#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define N 8
#define W (-1)
#define B (1)

// basic structure holding the state
// if needed, this can be compressed
// also an additional structure may be
// needed if we have a very large board
// i.e. maybe we can use a hashtable holding
// non-zero elements
typedef struct {
  char board[N][N];
  char who;
} state;

// this is the initial state after reset
state initial = (state) {
  .who=B,
  .board={
    {0,B,0,B,0,B,0,B},
    {B,0,B,0,B,0,B,0},
    {0,B,0,B,0,B,0,B},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {W,0,W,0,W,0,W,0},
    {0,W,0,W,0,W,0,W},
    {W,0,W,0,W,0,W,0},
  }
};

// count the number of pieces
int score(state *s, char who) {
  int sum = 0;
  for (int i=0; i<N; i++)
    for (int j=0; j<N; j++)
      sum += (s->board[i][j]==who?1:0);
  return sum;
}

// just an ugly function to print the board
void print(state *s) {
  // score
  printf("Turn: %s, score WHITE=%d, score BLACK=%d\n",
    s->who == B ? "BLACK" : "WHITE", score(s, W), score(s, B));
  // top row of indices
  printf("   ");
  for (int j=0; j<N; j++) printf("%3d", j);
  printf("\n");
  // data
  for (int i=0; i<N; i++) {
    // left column with indices
    printf("%2d: ", i);
    for (int j=0; j<N; j++) {
      printf(" %c ",
        s->board[i][j]==0 ? '_' : (s->board[i][j]==W ? 'W' : 'B'));
    }
    puts("");
  }
}

// bring the board to the initial state
void reset(state *s) {
  memcpy(s, &initial, sizeof(state));
}

// assume no more than 12 valid moves
// if not the case, this can be implemented as
// a resizable vector
#define MAX_MOVES 12
state temp[MAX_MOVES];

// given state s (board + current player)
// 1. enumerate possible moves
// 2. ask for a move
// 3. apply the move
// 4. if no move possible, finish

// at the moment it return 1 (valid move)
// or 0 - finished
// it can be modified to return the vector of possible moves
// instead, then this could be easily used to build a tree
// of possible game trajectories

int get_moves(state *s) {

  //enumerate valid moves
  int k = 0; // number of valid moves
  int capture = 0;
  char player = s->who;
  print(s);
  printf("PLAYER %c: choose a move (q quits):\n", player==B?'B':'W');

  // just iterate over the board pieces
  // if the board is large we may iterate over non-zero elements instead

  for (int i=0; i<N; i++)
    for (int j=0; j<N; j++) {
      // 1. identify places where we have a piece
      if (s->board[i][j] == player) {
        // non-capturing moves, check rules
        int dir = player;
        // player B can move down, W can move 'UP'
        // for every valid move, print it and increment the
        // valid move count variable 'k'
        // also store the board after the move is applied to
        // temp[k], so that we can simply put it in place of the
        // actual board
        // if we score a piece, we get an extra turn,
        // otherwise the opposite side plays (who = -player)
        if  ((player == B && i<7) || (player == W && i>0)) {

            if (j>0 && s->board[i+dir][j-1] == 0) {

              printf("[%d] move (%d,%d) to (%d,%d)\n",
                k, i, j, i+dir, j-1);
              memcpy(&temp[k], s->board, N*N*sizeof(char));
              temp[k].board[i+dir][j-1] = player;
              temp[k].board[i][j] = 0;
              temp[k].who = -player;
              k++;
            }

            if (j<7 && s->board[i+dir][j+1] == 0) {

              printf("[%d] move (%d,%d) to (%d,%d)\n",
                k, i, j, i+dir, j+1);
              memcpy(&temp[k], s->board, N*N*sizeof(char));
              temp[k].board[i+dir][j+1] = player;
              temp[k].board[i][j] = 0;
              temp[k].who = -player;
              k++;

            }

        }
        // capturing
        if  ((player==B && i<6) || (player==W && i>1)) {
          if (j>1 && s->board[i+2*dir][j-2] == 0 &&
                  s->board[i+dir][j-1] == -player) {

            printf("[%d] capturing move (%d,%d) to (%d,%d)\n",
              k, i, j, i+2*dir, j-2);
            memcpy(&temp[k], s->board, N*N*sizeof(char));
            temp[k].board[i+2*dir][j-2] = player;
            temp[k].board[i][j] = 0;
            temp[k].board[i+dir][j-1] = 0;
            temp[k].who = player; // we keep playing
            k++;

          }
          if (j<6 && s->board[i+2*dir][j+2] == 0 &&
                  s->board[i+dir][j+1] == -player) {

            printf("[%d] capturing move (%d,%d) to (%d,%d)\n",
              k, i, j, i+2*dir, j+2);
            memcpy(&temp[k], s->board, N*N*sizeof(char));
            temp[k].board[i+2*dir][j+2] = player;
            temp[k].board[i][j] = 0;
            temp[k].board[i+dir][j+1] = 0;
            temp[k].who = player; // we keep playing
            k++;
          }
        }
      }
    }

    // there was a valid move
    if (k>0) {
      int m;
      if (!scanf("%d", &m)) return 0;
      printf("OK. Applying move %d...\n", m);
      memcpy(s->board, &temp[m].board, N*N*sizeof(char));
      s->who = temp[m].who;
      return 1;
    } else {

    // no valid moves possible -> finish ?
    return 0;
  }
}

int main(int argc, char **argv) {

  state g;
  reset(&g);
  while (get_moves(&g));

  return 0;
}

