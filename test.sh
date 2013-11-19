#!/bin/sh

for t in list matrix
do
  echo $t
  rm ${t}.data 2> /dev/null
  for i in $(seq 100 100 2000)
  do
    data=$(./generator $i)
    from=$(date +%s%N)
    echo $data | ./dfs $i $t > /dev/null
    echo $i $(echo "$(date +%s%N) - $from" | bc) | tee -a ${t}.data
  done
done
