#!/usr/bin/env bash

set -euo pipefail

output="fig15-$(date -Iseconds).csv"

function finish {
  ret=$?
  >&2 echo "----------"
  >&2 echo "Exit Status: $ret"
  >&2 echo "Output CSV: $output"
  exit $ret
}
trap finish EXIT

>&2 echo "Output CSV: $output"
echo "passes, radix bits, runtime total, build, part, total time, total tuples, cycles per tuple" >$output

#for million in 128 384 640 896 1152 1408 1664 1920; do
for million in 1152 1408 1664 1920; do
  tuples=$((million * 1000000))
  >&2 echo "--- Running #$tuples"
  ./src/mchashjoins -n 64 -r "$tuples" -s "$tuples" | tee -a "$output"
done
