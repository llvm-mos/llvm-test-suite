#!/bin/bash

set -u
: $LLVM_MOS_SDK
export LLVM_MOS_SDK

cat << EOF
{
  "benchmarks": [
EOF

SingleSource/Benchmarks/bench.sh 6502-compilers/bench/ccgame/game_01_start build/SingleSource/Benchmarks/6502-compilers-bench/ccgame/game_01_start
echo ","
SingleSource/Benchmarks/bench.sh 6502-compilers/bench/ccgame/game_modern_optims build/SingleSource/Benchmarks/6502-compilers-bench/ccgame/game_modern_optims
echo ","
SingleSource/Benchmarks/bench.sh 6502-compilers/bench/ccgame/game_modern_optims_structarray build/SingleSource/Benchmarks/6502-compilers-bench/ccgame/game_modern_optims_structarray
echo ","
SingleSource/Benchmarks/bench.sh 6502-compilers/bench/coroutine build/SingleSource/Benchmarks/6502-compilers-bench/coroutine
echo ","
SingleSource/Benchmarks/bench.sh 6502-compilers/bench/memcpy build/SingleSource/Benchmarks/6502-compilers-bench/memcpy
echo ","
SingleSource/Benchmarks/bench.sh 6502-compilers/bench/rpg build/SingleSource/Benchmarks/6502-compilers-bench/rpg
echo ","
SingleSource/Benchmarks/bench.sh 6502-compilers/bench/unzip build/SingleSource/Benchmarks/6502-compilers-bench/unzip
echo ","
SingleSource/Benchmarks/bench.sh Dhrystone build/SingleSource/Benchmarks/Dhrystone/dry
echo ","
SingleSource/Benchmarks/bench.sh "BYTE Sieve" build/SingleSource/Benchmarks/byte/sieve
echo ","
MultiSource/Benchmarks/coremark/bench.sh

cat << EOF
  ]
}
EOF
