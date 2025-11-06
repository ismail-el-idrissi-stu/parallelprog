#include <math.h>
#include "timestep.h"
#define REAL_CELL 1

/*
 * This version adds a private clause to the SIMD pragma for variables
 * that are assigned inside the loop.  Making them private avoids false
 * dependencies across SIMD lanes and may enable better vectorization.
 * It still uses a min-reduction on mymindt.
 */
double timestep(int ncells, double g, double sigma, int* restrict celltype,
                double* H, double* U, double* V, double* dx, double* dy){
    double wavespeed, xspeed, yspeed, dt;
    double mymindt = 1.0e20;
#pragma omp simd private(wavespeed, xspeed, yspeed, dt) reduction(min:mymindt)
    for (int ic=0; ic<ncells ; ic++) {
        if (celltype[ic] == REAL_CELL) {
            wavespeed = sqrt(g*H[ic]);
            xspeed = (fabs(U[ic]) + wavespeed) / dx[ic];
            yspeed = (fabs(V[ic]) + wavespeed) / dy[ic];
            dt = sigma / (xspeed + yspeed);
            if (dt < mymindt) mymindt = dt;
        }
    }
    return(mymindt);
}
