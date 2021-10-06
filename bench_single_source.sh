#!/bin/bash

file=$(mktemp)
$SIM --cycles $2 2> $file
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
