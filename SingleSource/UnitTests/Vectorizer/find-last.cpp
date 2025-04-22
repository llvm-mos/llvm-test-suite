#include <algorithm>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <stdint.h>

#include "common.h"

template <typename RetTy, typename Ty>
using Fn2Ty = std::function<RetTy(Ty *, Ty *, RetTy)>;
template <typename RetTy, typename Ty>
static void checkVectorFunction(Fn2Ty<RetTy, Ty> ScalarFn,
                                Fn2Ty<RetTy, Ty> VectorFn, const char *Name) {
  std::cout << "Checking " << Name << "\n";

  unsigned N = 1000;
  std::unique_ptr<Ty[]> Src1(new Ty[N]);
  std::unique_ptr<Ty[]> Src2(new Ty[N]);
  init_data(Src1, N);
  init_data(Src2, N);

  // Test VectorFn with different input data.
  {
    // Check with random inputs.
    auto Reference = ScalarFn(&Src1[0], &Src2[0], N);
    auto ToCheck = VectorFn(&Src1[0], &Src2[0], N);
    if (Reference != ToCheck) {
      std::cerr << "Miscompare\n";
      exit(1);
    }
  }

  {
    // Check with Src1 > Src2 for all elements.
    for (unsigned I = 0; I != N; ++I) {
      Src1[I] = std::numeric_limits<Ty>::max();
      Src2[I] = std::numeric_limits<Ty>::min();
    }
    auto Reference = ScalarFn(&Src1[0], &Src2[0], N);
    auto ToCheck = VectorFn(&Src1[0], &Src2[0], N);
    if (Reference != ToCheck) {
      std::cerr << "Miscompare\n";
      exit(1);
    }
  }

  {
    // Check with Src1 < Src2 for all elements.
    for (unsigned I = 0; I != N; ++I) {
      Src1[I] = std::numeric_limits<Ty>::min();
      Src2[I] = std::numeric_limits<Ty>::max();
    }
    auto Reference = ScalarFn(&Src1[0], &Src2[0], N);
    auto ToCheck = VectorFn(&Src1[0], &Src2[0], N);
    if (Reference != ToCheck) {
      std::cerr << "Miscompare\n";
      exit(1);
    }
  }

  {
    // Check with only Src1[998] > Src2[998].
    for (unsigned I = 0; I != N; ++I)
      Src1[I] = Src2[I] = std::numeric_limits<Ty>::min();
    Src1[998] = std::numeric_limits<Ty>::max();
    auto Reference = ScalarFn(&Src1[0], &Src2[0], N);
    auto ToCheck = VectorFn(&Src1[0], &Src2[0], N);
    if (Reference != ToCheck) {
      std::cerr << "Miscompare\n";
      exit(1);
    }
  }

  {
    // Check with only Src1[0] > Src2[0].
    for (unsigned I = 0; I != N; ++I)
      Src1[I] = Src2[I] = std::numeric_limits<Ty>::min();
    Src1[0] = std::numeric_limits<Ty>::max();
    auto Reference = ScalarFn(&Src1[0], &Src2[0], N);
    auto ToCheck = VectorFn(&Src1[0], &Src2[0], N);
    if (Reference != ToCheck) {
      std::cerr << "Miscompare\n";
      exit(1);
    }
  }

  {
    // Check with only Src1[N - 1] > Src2[N - 1].
    for (unsigned I = 0; I != N; ++I)
      Src1[I] = Src2[I] = std::numeric_limits<Ty>::min();
    Src1[N - 1] = std::numeric_limits<Ty>::max();
    auto Reference = ScalarFn(&Src1[0], &Src2[0], N);
    auto ToCheck = VectorFn(&Src1[0], &Src2[0], N);
    if (Reference != ToCheck) {
      std::cerr << "Miscompare\n";
      exit(1);
    }
  }

  {
    // Check with only Src1[0] > Src2[0] and Src1[N - 1] > Src2[N - 1].
    for (unsigned I = 0; I != N; ++I)
      Src1[I] = Src2[I] = std::numeric_limits<Ty>::min();
    Src1[0] = Src1[N - 1] = std::numeric_limits<Ty>::max();
    auto Reference = ScalarFn(&Src1[0], &Src2[0], N);
    auto ToCheck = VectorFn(&Src1[0], &Src2[0], N);
    if (Reference != ToCheck) {
      std::cerr << "Miscompare\n";
      exit(1);
    }
  }
}

int main(void) {
  rng = std::mt19937(15);

#define INC_COND(Start, Step, RetTy) for (RetTy I = Start; I < TC; I += Step)
#define DEC_COND(End, Step, RetTy) for (RetTy I = TC; I > End; I -= Step)

#define DEFINE_FINDLAST_LOOP_BODY(TrueVal, FalseVal, ForCond)                  \
  ForCond { Rdx = A[I] > B[I] ? TrueVal : FalseVal; }                          \
  return Rdx;

  {
    // Find the last index where A[I] > B[I] and update 32-bits Rdx when the
    // condition is true.
    DEFINE_SCALAR_AND_VECTOR_FN2_TYPE(
	int32_t Rdx = -1;,
	DEFINE_FINDLAST_LOOP_BODY(
	    /* TrueVal= */ I, /* FalseVal= */ Rdx,
	    /* ForCond= */
	    INC_COND(/* Start= */ 0, /* Step= */ 1, /* RetTy= */ int32_t)),
	int32_t);
    checkVectorFunction<int32_t, int32_t>(ScalarFn, VectorFn,
					  "findlast_icmp_s32_true_update");
    checkVectorFunction<int32_t, float>(ScalarFn, VectorFn,
					"findlast_fcmp_s32_true_update");
  }

  {
    // Find the last index where A[I] > B[I] and update 16-bits Rdx when the
    // condition is true.
    DEFINE_SCALAR_AND_VECTOR_FN2_TYPE(
	int16_t Rdx = -1;,
	DEFINE_FINDLAST_LOOP_BODY(
	    /* TrueVal= */ I, /* FalseVal= */ Rdx,
	    /* ForCond= */
	    INC_COND(/* Start= */ 0, /* Step= */ 1, /* RetTy= */ int16_t)),
	int16_t);
    checkVectorFunction<int16_t, int16_t>(ScalarFn, VectorFn,
					  "findlast_icmp_s16_true_update");
  }

  {
    // Update 32-bits Rdx when the condition A[I] > B[I] is false.
    DEFINE_SCALAR_AND_VECTOR_FN2_TYPE(
	int32_t Rdx = -1;,
	DEFINE_FINDLAST_LOOP_BODY(
	    /* TrueVal= */ Rdx, /* FalseVal= */ I,
	    /* ForCond= */
	    INC_COND(/* Start= */ 0, /* Step= */ 1, /* RetTy= */ int32_t)),
	int32_t);
    checkVectorFunction<int32_t, int32_t>(ScalarFn, VectorFn,
					  "findlast_icmp_s32_false_update");
    checkVectorFunction<int32_t, float>(ScalarFn, VectorFn,
					"findlast_fcmp_s32_false_update");
  }

  {
    // Update 16-bits Rdx when the condition A[I] > B[I] is false.
    DEFINE_SCALAR_AND_VECTOR_FN2_TYPE(
	int16_t Rdx = -1;,
	DEFINE_FINDLAST_LOOP_BODY(
	    /* TrueVal= */ Rdx, /* FalseVal= */ I,
	    /* ForCond= */
	    INC_COND(/* Start= */ 0, /* Step= */ 1, /* RetTy= */ int16_t)),
	int16_t);
    checkVectorFunction<int16_t, int16_t>(ScalarFn, VectorFn,
					  "findlast_icmp_s16_false_update");
  }

  {
    // Find the last 32-bits index with the start value TC.
    DEFINE_SCALAR_AND_VECTOR_FN2_TYPE(
	int32_t Rdx = TC;,
	DEFINE_FINDLAST_LOOP_BODY(
	    /* TrueVal= */ I, /* FalseVal= */ Rdx,
	    /* ForCond= */
	    INC_COND(/* Start= */ 0, /* Step= */ 1, /* RetTy= */ int32_t)),
	int32_t);
    checkVectorFunction<int32_t, int32_t>(ScalarFn, VectorFn,
					  "findlast_icmp_s32_start_TC");
    checkVectorFunction<int32_t, float>(ScalarFn, VectorFn,
					"findlast_fcmp_s32_start_TC");
  }

  {
    // Find the last 16-bits index with the start value TC.
    DEFINE_SCALAR_AND_VECTOR_FN2_TYPE(
	int16_t Rdx = TC;,
	DEFINE_FINDLAST_LOOP_BODY(
	    /* TrueVal= */ I, /* FalseVal= */ Rdx,
	    /* ForCond= */
	    INC_COND(/* Start= */ 0, /* Step= */ 1, /* RetTy= */ int16_t)),
	int16_t);
    checkVectorFunction<int16_t, int16_t>(ScalarFn, VectorFn,
					  "findlast_icmp_s16_start_TC");
  }

  {
    // Increment the 32-bits induction variable by 2.
    DEFINE_SCALAR_AND_VECTOR_FN2_TYPE(
	int32_t Rdx = -1;,
	DEFINE_FINDLAST_LOOP_BODY(
	    /* TrueVal= */ I, /* FalseVal= */ Rdx,
	    /* ForCond= */
	    INC_COND(/* Start= */ 0, /* Step= */ 2, /* RetTy= */ int32_t)),
	int32_t);
    checkVectorFunction<int32_t, int32_t>(ScalarFn, VectorFn,
					  "findlast_icmp_s32_inc_2");
    checkVectorFunction<int32_t, float>(ScalarFn, VectorFn,
					"findlast_fcmp_s32_inc_2");
  }

  {
    // Increment the 16-bits induction variable by 2.
    DEFINE_SCALAR_AND_VECTOR_FN2_TYPE(
	int16_t Rdx = -1;,
	DEFINE_FINDLAST_LOOP_BODY(
	    /* TrueVal= */ I, /* FalseVal= */ Rdx,
	    /* ForCond= */
	    INC_COND(/* Start= */ 0, /* Step= */ 2, /* RetTy= */ int16_t)),
	int16_t);
    checkVectorFunction<int16_t, int16_t>(ScalarFn, VectorFn,
					  "findlast_icmp_s16_inc_2");
  }

  {
    // Check with decreasing 32-bits induction variable.
    DEFINE_SCALAR_AND_VECTOR_FN2_TYPE(
	int32_t Rdx = -1;,
	DEFINE_FINDLAST_LOOP_BODY(
	    /* TrueVal= */ I, /* FalseVal= */ Rdx,
	    /* ForCond= */
	    DEC_COND(/* End= */ 0, /* Step= */ 1, /* RetTy= */ int32_t)),
	int32_t);
    checkVectorFunction<int32_t, int32_t>(
        ScalarFn, VectorFn, "findlast_icmp_s32_start_decreasing_induction");
    checkVectorFunction<int32_t, float>(
        ScalarFn, VectorFn, "findlast_fcmp_s32_start_decreasing_induction");
  }

  {
    // Check with decreasing 16-bits induction variable.
    DEFINE_SCALAR_AND_VECTOR_FN2_TYPE(
	int16_t Rdx = -1;,
	DEFINE_FINDLAST_LOOP_BODY(
	    /* TrueVal= */ I, /* FalseVal= */ Rdx,
	    /* ForCond= */
	    DEC_COND(/* End= */ 0, /* Step= */ 1, /* RetTy= */ int16_t)),
	int16_t);
    checkVectorFunction<int16_t, int16_t>(
        ScalarFn, VectorFn, "findlast_icmp_s16_start_decreasing_induction");
  }

  {
    // Check with 32-bits the induction variable starts from 3.
    DEFINE_SCALAR_AND_VECTOR_FN2_TYPE(
	int32_t Rdx = -1;,
	DEFINE_FINDLAST_LOOP_BODY(
	    /* TrueVal= */ I, /* FalseVal= */ Rdx,
	    /* ForCond= */
	    INC_COND(/* Start= */ 3, /* Step= */ 1, /* RetTy= */ int32_t)),
	int32_t);
    checkVectorFunction<int32_t, int32_t>(ScalarFn, VectorFn,
					  "findlast_icmp_s32_iv_start_3");
    checkVectorFunction<int32_t, float>(ScalarFn, VectorFn,
					"findlast_fcmp_s32_iv_start_3");
  }

  {
    // Check with 16-bits the induction variable starts from 3.
    DEFINE_SCALAR_AND_VECTOR_FN2_TYPE(
	int16_t Rdx = -1;,
	DEFINE_FINDLAST_LOOP_BODY(
	    /* TrueVal= */ I, /* FalseVal= */ Rdx,
	    /* ForCond= */
	    INC_COND(/* Start= */ 3, /* Step= */ 1, /* RetTy= */ int16_t)),
	int16_t);
    checkVectorFunction<int16_t, int16_t>(ScalarFn, VectorFn,
					  "findlast_icmp_s16_iv_start_3");
  }

  {
    // Check with start value of 3 and 32-bits induction variable starts at 3.
    DEFINE_SCALAR_AND_VECTOR_FN2_TYPE(
	int32_t Rdx = 3;,
	DEFINE_FINDLAST_LOOP_BODY(
	    /* TrueVal= */ I, /* FalseVal= */ Rdx,
	    /* ForCond= */
	    INC_COND(/* Start= */ 3, /* Step= */ 1, /* RetTy= */ int32_t)),
	int32_t);
    checkVectorFunction<int32_t, int32_t>(
	ScalarFn, VectorFn, "findlast_icmp_s32_start_3_iv_start_3");
    checkVectorFunction<int32_t, float>(ScalarFn, VectorFn,
					"findlast_fcmp_s32_start_3_iv_start_3");
  }

  {
    // Check with start value of 3 and 16-bits induction variable starts at 3.
    DEFINE_SCALAR_AND_VECTOR_FN2_TYPE(
	int16_t Rdx = 3;,
	DEFINE_FINDLAST_LOOP_BODY(
	    /* TrueVal= */ I, /* FalseVal= */ Rdx,
	    /* ForCond= */
	    INC_COND(/* Start= */ 3, /* Step= */ 1, /* RetTy= */ int16_t)),
	int16_t);
    checkVectorFunction<int16_t, int16_t>(
	ScalarFn, VectorFn, "findlast_icmp_s16_start_3_iv_start_3");
  }

  {
    // Check with start value of 2 and 32-bits induction variable starts at 3.
    DEFINE_SCALAR_AND_VECTOR_FN2_TYPE(
	int32_t Rdx = 2;,
	DEFINE_FINDLAST_LOOP_BODY(
	    /* TrueVal= */ I, /* FalseVal= */ Rdx,
	    /* ForCond= */
	    INC_COND(/* Start= */ 3, /* Step= */ 1, /* RetTy= */ int32_t)),
	int32_t);
    checkVectorFunction<int32_t, int32_t>(
	ScalarFn, VectorFn, "findlast_icmp_s32_start_2_iv_start_3");
    checkVectorFunction<int32_t, float>(ScalarFn, VectorFn,
					"findlast_fcmp_s32_start_2_iv_start_3");
  }

  {
    // Check with start value of 2 and 16-bits induction variable starts at 3.
    DEFINE_SCALAR_AND_VECTOR_FN2_TYPE(
	int16_t Rdx = 2;,
	DEFINE_FINDLAST_LOOP_BODY(
	    /* TrueVal= */ I, /* FalseVal= */ Rdx,
	    /* ForCond= */
	    INC_COND(/* Start= */ 3, /* Step= */ 1, /* RetTy= */ int16_t)),
	int16_t);
    checkVectorFunction<int16_t, int16_t>(
	ScalarFn, VectorFn, "findlast_icmp_s16_start_2_iv_start_3");
  }

  {
    // Check with start value of 4 and 32-bits induction variable starts at 3.
    DEFINE_SCALAR_AND_VECTOR_FN2_TYPE(
	int32_t Rdx = 4;,
	DEFINE_FINDLAST_LOOP_BODY(
	    /* TrueVal= */ I, /* FalseVal= */ Rdx,
	    /* ForCond= */
	    INC_COND(/* Start= */ 3, /* Step= */ 1, /* RetTy= */ int32_t)),
	int32_t);
    checkVectorFunction<int32_t, int32_t>(
	ScalarFn, VectorFn, "findlast_icmp_s32_start_4_iv_start_3");
    checkVectorFunction<int32_t, float>(ScalarFn, VectorFn,
					"findlast_fcmp_s32_start_4_iv_start_3");
  }

  {
    // Check with start value of 4 and 16-bits induction variable starts at 3.
    DEFINE_SCALAR_AND_VECTOR_FN2_TYPE(
	int16_t Rdx = 4;,
	DEFINE_FINDLAST_LOOP_BODY(
	    /* TrueVal= */ I, /* FalseVal= */ Rdx,
	    /* ForCond= */
	    INC_COND(/* Start= */ 3, /* Step= */ 1, /* RetTy= */ int16_t)),
	int16_t);
    checkVectorFunction<int16_t, int16_t>(
	ScalarFn, VectorFn, "findlast_icmp_s16_start_4_iv_start_3");
  }

  return 0;
}
