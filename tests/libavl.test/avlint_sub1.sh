#!/bin/bash

SUCCESS="0"

program="./avlint_sub1"
declare -i n=100
declare -a input=( "0 0" "3 1" "1 3" "-1 -2" "-2 -1" "-1 2" "-2 1" "1 -2")
declare -a output=("0"   "2"   "-2"  "1"     "-1"    "-3"   "-3"   "3")

for i in $(seq 1 n)
do
	num1=$(( ( RANDOM % 20000 ) - 10000 ))
	num2=$(( ( RANDOM % 20000 ) - 10000 ))
	sub=$(( num1 - num2 ))
	input=("${input[@]}" "$num1 $num2")
	output=("${output[@]}" "$sub")
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
