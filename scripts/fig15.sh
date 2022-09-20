#!/usr/bin/env bash

set -euo pipefail

output_prefix="fig15-$(date -Iseconds)"
output="${output_prefix}.csv"
output_dat="${output_prefix}.dat"

NUM_CYCLES_PER_SEC=$(($(lscpu | grep "CPU MHz:" | awk '{print $NF}' | cut -d'.' -f1) * 1000000))

function finish {
  ret=$?
  >&2 echo "----------"
  >&2 echo "Exit Status: $ret"
  >&2 echo "Output CSV: $output"
  exit $ret
}
trap finish EXIT


echo "Building for 128, 256, and 384 Million tuples with NUM_PASSES=1, NUM_RADIX_BITS=12 ..."

make distclean &> build.log
./configure --disable-key8B CPPFLAGS="-DNUM_PASSES=1 -DNUM_RADIX_BITS=12" &>> build.log
make &> build.log

echo "Running with R=S=(128 256 384) million tuples with NUM_PASSES=1, NUM_RADIX_BITS=12 ..."
>&2 echo "Output CSV: $output"
echo "passes, radix bits, runtime total, build, part, total time (usecs), total tuples, cycles per tuple" >$output

for million in 128 256 384 ; do
  tuples=$((million * 1000000))
  >&2 echo "--- Running #$tuples"
  ./src/mchashjoins -n 64 -r "$tuples" -s "$tuples" --basic-numa | tee -a "$output"
done

echo "Building for tuples > 1Billion with NUM_PASSES=2, NUM_RADIX_BITS=18 ..."

make distclean &> build.log
./configure --disable-key8B CPPFLAGS="-DNUM_PASSES=2 -DNUM_RADIX_BITS=18" &> build.log
make &> build.log

echo "Running with R=S=(512 768 1024 1280 1408 1536 1792) million tuples with NUM_PASSES=2, NUM_RADIX_BITS=18 ..."

for million in 512 768 1024 1280 1408 1536 1792 ; do
  tuples=$((million * 1000000))
  >&2 echo "--- Running #$tuples"
  ./src/mchashjoins -n 64 -r "$tuples" -s "$tuples" --basic-numa | tee -a "$output"
done

echo "num_tuples (mill), tuples_per_sec (mill)" > $output_dat
tail -n +2 ${output} | while read line; do
  CYCLES_PER_TUPLE=$(echo ${line} | awk -F',' '{ print $NF }')
  TOTAL_TUPLES=$(echo ${line} | awk -F',' '{ print $(NF-1) }')
  TUPLES_PER_SEC=$(echo "${NUM_CYCLES_PER_SEC} / ${CYCLES_PER_TUPLE} / 1000000" | bc)
  NUM_TUPLES_MIL=$(echo "${TOTAL_TUPLES} / 1000000" | bc)
  echo ${NUM_TUPLES_MIL}, ${TUPLES_PER_SEC} | tee -a "$output_dat"
done
