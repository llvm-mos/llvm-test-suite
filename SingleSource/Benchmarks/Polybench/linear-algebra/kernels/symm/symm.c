/**
 * symm.c: This file is part of the PolyBench/C 3.2 test suite.
 *
 *
 * Contact: Louis-Noel Pouchet <pouchet@cse.ohio-state.edu>
 * Web address: http://polybench.sourceforge.net
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

/* Include polybench common header. */
#include <polybench.h>

/* Include benchmark-specific header. */
/* Default data type is double, default size is 4000. */
#include "symm.h"


/* Array initialization. */
static
void init_array(int ni, int nj,
		DATA_TYPE *alpha,
		DATA_TYPE *beta,
		DATA_TYPE POLYBENCH_2D(C,NI,NJ,ni,nj),
#if !FMA_DISABLED
		DATA_TYPE POLYBENCH_2D(C_StrictFP,NI,NJ,ni,nj),
#endif
		DATA_TYPE POLYBENCH_2D(A,NJ,NJ,nj,nj),
		DATA_TYPE POLYBENCH_2D(B,NI,NJ,ni,nj))
{
#pragma STDC FP_CONTRACT OFF
  int i, j;

  *alpha = 32412;
  *beta = 2123;
  for (i = 0; i < ni; i++)
    for (j = 0; j < nj; j++) {
#if !FMA_DISABLED
      C_StrictFP[i][j] =
#endif
	      C[i][j] = ((DATA_TYPE) i*j) / ni;
      B[i][j] = ((DATA_TYPE) i*j) / ni;
    }
  for (i = 0; i < nj; i++)
    for (j = 0; j < nj; j++)
      A[i][j] = ((DATA_TYPE) i*j) / ni;
}


/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static
void print_array(int ni, int nj,
		 DATA_TYPE POLYBENCH_2D(C,NI,NJ,ni,nj))
{
  int i, j;
  char *printmat = malloc(nj*16 + 1); printmat[nj*16] = 0;

  for (i = 0; i < ni; i++) {
    for (j = 0; j < nj; j++)
      print_element(C[i][j], j*16, printmat);
    fputs(printmat, stderr);
  }
  free(printmat);
}


/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_symm(int ni, int nj,
		 DATA_TYPE alpha,
		 DATA_TYPE beta,
		 DATA_TYPE POLYBENCH_2D(C,NI,NJ,ni,nj),
		 DATA_TYPE POLYBENCH_2D(A,NJ,NJ,nj,nj),
		 DATA_TYPE POLYBENCH_2D(B,NI,NJ,ni,nj))
{
  int i, j, k;
  DATA_TYPE acc;

#pragma scop
  /*  C := alpha*A*B + beta*C, A is symetric */
  for (i = 0; i < _PB_NI; i++)
    for (j = 0; j < _PB_NJ; j++)
      {
	acc = 0;
	for (k = 0; k < j - 1; k++)
	  {
	    C[k][j] += alpha * A[k][i] * B[i][j];
	    acc += B[k][j] * A[k][i];
	  }
	C[i][j] = beta * C[i][j] + alpha * A[i][i] * B[i][j] + alpha * acc;
      }
#pragma endscop

}

#if !FMA_DISABLED
// NOTE: FMA_DISABLED is true for targets where FMA contraction causes
// discrepancies which cause the accuracy checks to fail.
// In this case, the test runs with the option -ffp-contract=off
static
void kernel_symm_StrictFP(int ni, int nj,
                          DATA_TYPE alpha,
                          DATA_TYPE beta,
                          DATA_TYPE POLYBENCH_2D(C,NI,NJ,ni,nj),
                          DATA_TYPE POLYBENCH_2D(A,NJ,NJ,nj,nj),
                          DATA_TYPE POLYBENCH_2D(B,NI,NJ,ni,nj))
{
#pragma STDC FP_CONTRACT OFF
  int i, j, k;
  DATA_TYPE acc;

  /*  C := alpha*A*B + beta*C, A is symetric */
  for (i = 0; i < _PB_NI; i++)
    for (j = 0; j < _PB_NJ; j++)
      {
	acc = 0;
	for (k = 0; k < j - 1; k++)
	  {
	    C[k][j] += alpha * A[k][i] * B[i][j];
	    acc += B[k][j] * A[k][i];
	  }
	C[i][j] = beta * C[i][j] + alpha * A[i][i] * B[i][j] + alpha * acc;
      }
}

/* Return 0 when one of the elements of arrays A and B do not match within the
   allowed FP_ABSTOLERANCE.  Return 1 when all elements match.  */
static inline int
check_FP(int ni, int nj,
         DATA_TYPE POLYBENCH_2D(A,NI,NJ,ni,nj),
         DATA_TYPE POLYBENCH_2D(B,NI,NJ,ni,nj)) {
  int i, j;
  double AbsTolerance = FP_ABSTOLERANCE;
  for (i = 0; i < _PB_NI; i++)
    for (j = 0; j < _PB_NJ; j++)
      {
        double V1 = A[i][j];
        double V2 = B[i][j];
        double Diff = fabs(V1 - V2);
        if (Diff > AbsTolerance) {
          fprintf(stderr, "A[%d][%d] = %lf and B[%d][%d] = %lf differ more than"
                  " FP_ABSTOLERANCE = %lf\n", i, j, V1, i, j, V2, AbsTolerance);
          return 0;
        }
      }

  /* All elements are within the allowed FP_ABSTOLERANCE error margin.  */
  return 1;
}
#endif

int main(int argc, char** argv)
{
  /* Retrieve problem size. */
  int ni = NI;
  int nj = NJ;

  /* Variable declaration/allocation. */
  DATA_TYPE alpha;
  DATA_TYPE beta;
  POLYBENCH_2D_ARRAY_DECL(C,DATA_TYPE,NI,NJ,ni,nj);
#if !FMA_DISABLED
  POLYBENCH_2D_ARRAY_DECL(C_StrictFP,DATA_TYPE,NI,NJ,ni,nj);
#endif
  POLYBENCH_2D_ARRAY_DECL(A,DATA_TYPE,NJ,NJ,nj,nj);
  POLYBENCH_2D_ARRAY_DECL(B,DATA_TYPE,NI,NJ,ni,nj);

  /* Initialize array(s). */
  init_array (ni, nj, &alpha, &beta,
	      POLYBENCH_ARRAY(C),
#if !FMA_DISABLED
	      POLYBENCH_ARRAY(C_StrictFP),
#endif
	      POLYBENCH_ARRAY(A),
	      POLYBENCH_ARRAY(B));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_symm (ni, nj,
	       alpha, beta,
	       POLYBENCH_ARRAY(C),
	       POLYBENCH_ARRAY(A),
	       POLYBENCH_ARRAY(B));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

#if FMA_DISABLED
  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  polybench_prevent_dce(print_array(ni, nj, POLYBENCH_ARRAY(C)));
#else
  kernel_symm_StrictFP(ni, nj,
                       alpha, beta,
                       POLYBENCH_ARRAY(C_StrictFP),
                       POLYBENCH_ARRAY(A),
                       POLYBENCH_ARRAY(B));
  if (!check_FP(ni, nj, POLYBENCH_ARRAY(C), POLYBENCH_ARRAY(C_StrictFP)))
    return 1;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  polybench_prevent_dce(print_array(ni, nj, POLYBENCH_ARRAY(C_StrictFP)));
#endif

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(C);
#if !FMA_DISABLED
  POLYBENCH_FREE_ARRAY(C_StrictFP);
#endif
  POLYBENCH_FREE_ARRAY(A);
  POLYBENCH_FREE_ARRAY(B);

  return 0;
}
