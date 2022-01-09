#!/bin/bash

set -u

file=$(mktemp)
$LLVM_MOS_SDK/bin/mos-sim --cycles $2 2> $file > /dev/null
if [ $? -ne 0 ]; then
  cat $file
  exit
fi
read cycles unit < $file
cat << EOF
  {
    "name": "$1",
    "real_time": $cycles,
    "cpu_time": $cycles,
    "time_unit": "cycles",
    "iterations": 1,
    "threads": 1
  }
EOF
