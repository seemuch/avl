#!/bin/bash

SUCCESS="0"

program="./avlint_add1"
declare -a input=("0 0" "3 1" "1 3" "-1 -2" "-2 -1" "-1 2" "-2 1" "1 -2")
declare -a output=("0" "2" "-2" "1" "-1" "-3" "-3" "3")

len=${#input[@]}

for i in $(seq 0 $(( $len - 1 )))
do
	result=`$program ${input[$i]}`
	if [[ "$result" != "${output[$i]}" ]]
	then
		SUCCESS="1"
	fi
done

exit $success
