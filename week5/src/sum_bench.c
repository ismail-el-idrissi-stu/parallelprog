#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <string.h>

typedef struct { const char* name; double value; double ms; } result_t;

static inline double now_ms(void){
  struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts);
  return (double)ts.tv_sec*1000.0 + (double)ts.tv_nsec/1.0e6;
}

static double rand_uniform(){ static uint64_t s=0x12345678ULL; s^=s<<13; s^=s>>7; s^=s<<17; return (double)(s&0xFFFFFFFFULL)/4294967296.0; }

static void fill_data(double* a, long n, const char* pattern){
  if (!strcmp(pattern,"alternating")) { for (long i=0;i<n;i++) a[i]=(i&1)?1e-8:1.0; }
  else if (!strcmp(pattern,"random")) { for (long i=0;i<n;i++){ double r=rand_uniform(); a[i]=(r-0.5)*((i%7==0)?1e6:1.0);} }
  else { for (long i=0;i<n;i++) a[i]=1.0; }
}

double do_sum_double(const double* a, long n){ double s=0.0; for(long i=0;i<n;i++) s+=a[i]; return s; }
long double do_sum_longdouble(const double* a, long n){ long double s=0.0L; for(long i=0;i<n;i++) s+=(long double)a[i]; return s; }
double do_sum_kahan(const double* a, long n){ double sum=0.0,c=0.0; for(long i=0;i<n;i++){ double y=a[i]-c; double t=sum+y; c=(t-sum)-y; sum=t; } return sum; }
double do_sum_knuth(const double* a, long n){ double sum=0.0,c=0.0; for(long i=0;i<n;i++){ double t=sum+a[i]; if(fabs(sum)>=fabs(a[i])) c+=(sum-t)+a[i]; else c+=(a[i]-t)+sum; sum=t; } return sum+c; }
double do_sum_pairwise(const double* in, long n){
  if(n==0) return 0.0;
  double* p=(double*)malloc((size_t)n*sizeof(double));
  for(long i=0;i<n;i++) p[i]=in[i];
  long m=n;
  while(m>1){
    long pairs=m/2;
    for(long i=0;i<pairs;i++) p[i]=p[2*i]+p[2*i+1];
    if(m&1L){ p[pairs]=p[m-1]; m=pairs+1; } else { m=pairs; }
  }
  double ans=p[0]; free(p); return ans;
}

static result_t run_one(const char* name, double(*fn)(const double*, long), const double* a, long n, long iters){
  volatile double w=0; for(int k=0;k<2;k++) w+=fn(a,n);
  double t0=now_ms(); double out=0.0; for(long r=0;r<iters;r++) out+=fn(a,n); double t1=now_ms();
  result_t res={name, out/((double)iters), (t1-t0)}; return res;
}

int main(int argc, char** argv){
  if(argc<4){ fprintf(stderr,"Usage: %s <N> <pattern: ones|alternating|random> <iters>\n", argv[0]); return 1; }
  long N=atol(argv[1]); const char* pattern=argv[2]; long iters=atol(argv[3]);
  double* a=(double*)malloc((size_t)N*sizeof(double)); fill_data(a,N,pattern);
  long double ref=do_sum_longdouble(a,N);

  result_t results[5]; int idx=0;
  results[idx++]=run_one("double",   do_sum_double,   a,N,iters);
  results[idx++]=run_one("pairwise", do_sum_pairwise, a,N,iters);
  results[idx++]=run_one("kahan",    do_sum_kahan,    a,N,iters);
  results[idx++]=run_one("knuth",    do_sum_knuth,    a,N,iters);

  double t0=now_ms(); long double ld_accum=0; for(long r=0;r<iters;r++) ld_accum+=do_sum_longdouble(a,N); double t1=now_ms();
  result_t ldres={"longdouble",(double)(ld_accum/iters),(t1-t0)}; results[idx++]=ldres;

  printf("method,error_relative,runtime_ms\n");
  for(int i=0;i<idx;i++){
    double err=fabsl(((long double)results[i].value - ref)/(ref==0?1.0L:ref));
    printf("%s,%.16e,%.3f\n",results[i].name,err,results[i].ms);
  }
  free(a);
  return 0;
}