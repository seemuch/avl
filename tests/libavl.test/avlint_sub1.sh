#!/bin/bash

SUCCESS=0

program="./avlint_sub1"
declare -i n=20
declare -a input=( "0 0" "3 1" "1 3" "-1 -2" "-2 -1" "-1 2" "-2 1" "1 -2")

source genRandom.sh
genRandom $n 2
input=("${input[@]}" "${rArray[@]}")
len=${#input[@]}

for i in $(seq 1 $(( len - 1 )))
do
	num1=`echo ${input[$i]} | awk '{print $1}'`
	num2=`echo ${input[$i]} | awk '{print $2}'`
	expected=$(( num1 - num2 ))
	output=`$program ${input[$i]}`

	if [[ "$output" != "$expected" ]]
	then
		SUCCESS=1
	fi
done


exit $SUCCESS
