#!/bin/bash

SUCCESS="0"

program="./avlint_add1"
declare -i n=100
declare -a input=( "0 0" "1 2" "-1 -2" "-1 2" "-2 1")
declare -a output=("0"   "3"   "-3"    "1"    "-1")

for i in $(seq 1 n)
do
	num1=$(( ( RANDOM % 20000 ) - 10000 ))
	num2=$(( ( RANDOM % 20000 ) - 10000 ))
	sum=$(( num1 + num2 ))
	input=("${input[@]}" "$num1 $num2")
	output=("${output[@]}" "$sum")
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
