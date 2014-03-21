#!/bin/bash

declare -a rArray=()

# Argument 1: #elements in the array
# Argument 2: #numbers in each element
genRandom ()
{
	rArray=()
	declare -i n=$(( $1 - 1 ))

	for i in $(seq 0 $n)
	do
		element=$(( ( RANDOM % 20000 ) - 10000 ))

		if [[ $2 -ge 2 ]]
		then
			for j in $(seq 2 $2)
			do
				num=$(( ( RANDOM % 20000 ) - 10000 ))
				element="$element $num"
			done
		fi

		rArray[$i]="$element"
	done
}
