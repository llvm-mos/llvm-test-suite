#!/bin/bash

: ${SIM:=llvm-mos-sdk/build/bin/sim}

cat << EOF
{
  "benchmarks": [
EOF

./bench_single_source.sh 6502-compilers/bench/ccgame/game_01_start build/SingleSource/Benchmarks/6502-compilers-bench/ccgame/game_01_start
echo ","
./bench_single_source.sh 6502-compilers/bench/ccgame/game_modern_optims build/SingleSource/Benchmarks/6502-compilers-bench/ccgame/game_modern_optims
echo ","
./bench_single_source.sh 6502-compilers/bench/ccgame/game_modern_optims_structarray build/SingleSource/Benchmarks/6502-compilers-bench/ccgame/game_modern_optims_structarray
echo ","
./bench_single_source.sh 6502-compilers/bench/coroutine build/SingleSource/Benchmarks/6502-compilers-bench/coroutine
echo ","
./bench_single_source.sh 6502-compilers/bench/memcpy build/SingleSource/Benchmarks/6502-compilers-bench/memcpy
echo ","
./bench_single_source.sh 6502-compilers/bench/rpg build/SingleSource/Benchmarks/6502-compilers-bench/rpg
echo ","
./bench_single_source.sh 6502-compilers/bench/unzip build/SingleSource/Benchmarks/6502-compilers-bench/unzip
echo ","
./bench_single_source.sh Dhrystone build/SingleSource/Benchmarks/Dhrystone/dry

cat << EOF
  ]
}
EOF
