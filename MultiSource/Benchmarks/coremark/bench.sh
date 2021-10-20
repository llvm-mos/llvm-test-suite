#!/bin/bash

set -u
: $LLVM_MOS
: $LLVM_MOS_SDK

cd $(dirname $0)

if ! timeout 15m make PORT_DIR=mos XCFLAGS="-DPERFORMANCE_RUN=1" REBUILD=1 run1.log >/dev/null; then
  echo "Coremark performance run failed:" >&2
  cat run1.log >&2
  exit 1
fi
if ! grep -q "Correct operation validated." run1.log; then
  echo "Coremark performance run failed:" >&2
  cat run1.log >&2
  exit 1
fi
if ! timeout 15m make PORT_DIR=mos XCFLAGS="-DVALIDATION_RUN=1" REBUILD=1 run2.log >/dev/null; then
  echo "Coremark validation run failed:" >&2
  cat run2.log >&2
  exit 1
fi
if ! grep -q "Correct operation validated." run2.log; then
  echo "Coremark validation run failed:" >&2
  cat run1.log >&2
  exit 1
fi

total_time=$(grep "Total time" run1.log | cut -d ':' -f 2)
iterations=$(grep "Iterations[^/]" run1.log | cut -d ':' -f 2)

cat << EOF
  {
    "name": "CoreMark",
    "real_time": $total_time,
    "cpu_time": $total_time,
    "time_unit": "sec",
    "iterations": $iterations,
    "threads": 1
  }
EOF
