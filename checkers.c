#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define N 8
#define W (-1)
#define B (1)

// basic structure holding the state
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

void print(state *s) {
  // score
  printf("Turn: %d, score WHITE=%d, score BLACK=%d\n",
    s->who, score(s, W), score(s, B));
  // top row of indices
  printf("   ");
  for (int j=0; j<N; j++) printf("%3d", j);
  printf("\n");
  // data
  for (int i=0; i<N; i++) {
    // left column with indices
    printf("%2d: ", i);
    for (int j=0; j<N; j++) {
      printf(" %c ", s->board[i][j]==0 ? '_' : (s->board[i][j]==W ? 'W' : 'B'));
    }
    puts("");
  }
}

void reset(state *s) {
  memcpy(s, &initial, sizeof(state));
}

void get_moves(state *s) {

  //enumerate valid moves

}

int main(int argc, char **argv) {

  state g;
  reset(&g);
  print(&g);
  get_moves(&g);
}

