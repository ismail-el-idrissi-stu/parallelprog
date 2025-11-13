#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "malloc2D.h"
#include "timer.h"

#define SWAP_PTR(xnew,xold,xtmp) (xtmp=xnew, xnew=xold, xold=xtmp)

int main(int argc, char* argv[])
{
    struct timespec tstart;
    double runtime;

    int imax = 2002;
    int jmax = 2002;

    double **x = malloc2D(jmax, imax);
    double **xnew = malloc2D(jmax, imax);
    double **xtmp;

    cpu_timer_start(&tstart);


    #pragma omp parallel
    {
        #pragma omp for nowait
        for (int j = 0; j < jmax; j++)
        for (int i = 0; i < imax; i++)
            x[j][i] = 0.0;

        #pragma omp for nowait
        for (int j = 0; j < jmax; j++)
        for (int i = 0; i < imax; i++)
            xnew[j][i] = 0.0;


        #pragma omp for nowait
        for (int j = 1; j < jmax - 1; j++)
        for (int i = 1; i < imax - 1; i++)
            xnew[j][i] = 0.25 * (x[j][i+1] + x[j][i-1] +
                                 x[j+1][i] + x[j-1][i]);
    }


    SWAP_PTR(xnew, x, xtmp);

    runtime = cpu_timer_stop(tstart);
    printf("stencil_opt4 runtime: %lf ms\n", runtime);

    free(x);
    free(xnew);

    return 0;
}
