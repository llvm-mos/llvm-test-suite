#!/bin/bash

llvm-mos-sdk/build/bin/sim --cycles build/SingleSource/Benchmarks/Dhrystone/dry 2> out
read cycles unit < out
cat << EOF
{
  "benchmarks": [{
    "name": "Dhrystone",
    "real_time": $cycles,
    "cpu_time": $cycles,
    "time_unit": "cycles",
    "iterations": 1,
    "threads": 1
  }]
}
EOF
