#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

typedef unsigned int uint;

struct neighbor2d { int left, right, bottom, top; };

static int **genmatrix(int rows, int cols, size_t elem) {
  int **m = (int**)malloc(rows * sizeof(int*));
  int *data = (int*)calloc((size_t)rows * cols, elem);
  for (int r = 0; r < rows; r++) m[r] = data + r * cols;
  return m;
}

struct neighbor2d *neighbors2d_hashcpu(uint ncells, int mesh_size, int levmx,
                                       int *i, int *j, int *level) {
  struct neighbor2d *neigh2d = (struct neighbor2d*)malloc(ncells * sizeof(*neigh2d));

  int *levtable = (int*)malloc((levmx + 1) * sizeof(int));
  for (int lev = 0; lev < levmx + 1; lev++) levtable[lev] = (int)pow(2, lev);

  int jmaxsize = mesh_size * levtable[levmx];
  int imaxsize = mesh_size * levtable[levmx];

  int **hash = genmatrix(jmaxsize, imaxsize, sizeof(int));

  // write phase
  for (int ic = 0; ic < (int)ncells; ic++) {
    int lev = level[ic];
    if (lev == levmx) {
      hash[j[ic]][i[ic]] = ic;
    } else {
      int mult = levtable[levmx - lev];
      for (int jj = j[ic]*mult; jj < (j[ic]+1)*mult; jj++)
        for (int ii = i[ic]*mult; ii < (i[ic]+1)*mult; ii++)
          hash[jj][ii] = ic;
    }
  }

  // read / neighbor phase
  for (int ic = 0; ic < (int)ncells; ic++) {
    int ii = i[ic], jj = j[ic], lev = level[ic];
    int levmult = levtable[levmx - lev];
    neigh2d[ic].left   = hash[      jj   * levmult               ][MAX(  ii   * levmult - 1, 0)];
    neigh2d[ic].right  = hash[      jj   * levmult               ][MIN( (ii+1)* levmult,     imaxsize-1)];
    neigh2d[ic].bottom = hash[MAX(  jj   * levmult - 1, 0)       ][      ii   * levmult               ];
    neigh2d[ic].top    = hash[MIN( (jj+1)* levmult,   jmaxsize-1)][      ii   * levmult               ];
  }

  free(levtable);
  return neigh2d;
}

int main(void) {
  int mesh_size = 4, levmx = 1;
  int ncells = 5;
  int i[]     = {0,1,0,1, 2};
  int j[]     = {0,0,1,1, 2};
  int level[] = {1,1,1,1, 0};

  struct neighbor2d *n = neighbors2d_hashcpu(ncells, mesh_size, levmx, i, j, level);

  for (int c = 0; c < ncells; c++) {
    printf("Cell %d (i=%d,j=%d,lev=%d): L=%d R=%d B=%d T=%d\n",
           c, i[c], j[c], level[c],
           n[c].left, n[c].right, n[c].bottom, n[c].top);
  }
  free(n);
  return 0;
}