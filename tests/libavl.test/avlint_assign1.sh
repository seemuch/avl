#!/bin/bash

SUCCESS=0

program="./avlint_assign1"
declare -i n=100
declare -a input=("0")

source genRandom.sh
genRandom $n 1
input=("${input[@]}" "${rArray[@]}")
len=${#input[@]}

for i in $(seq 1 $(( len - 1 )))
do
	num=`echo ${input[$i]} | awk '{print $1}'`
	expected=$(( num ))
	output=`$program ${input[$i]}`

	if [[ "$output" != "$expected" ]]
	then
		SUCCESS=1
	fi
done

exit $SUCCESS
