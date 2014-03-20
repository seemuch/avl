#!/bin/bash

SUCCESS="0"

program="./AvlInt_assign1"
declare -i n=100
declare -a input=( "-100" "-1" "0" "1" "100")
declare -a output=("-100" "-1" "0" "1" "100")

for i in $(seq 1 n)
do
	num=$(( ( RANDOM % 20000 ) - 10000 ))
	input=("${input[@]}" "$num")
	output=("${output[@]}" "$num")
done

len=${#input[@]}

for i in $(seq 0 $(( $len - 1 )))
do
	result=`$program ${input[$i]}`
	if [[ "$result" != "${output[$i]}" ]]
	then
		SUCCESS="1"
	fi
done

exit $SUCCESS
