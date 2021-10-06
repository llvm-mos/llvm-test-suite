#!/bin/bash

file=$(mktemp)
$SIM --cycles $2 2> $file
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
