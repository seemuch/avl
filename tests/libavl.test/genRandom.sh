#!/bin/bash

declare -a rArray=()

# Argument 1: #elements in the array
# Argument 2: #numbers in each element
genRandom ()
{
	for i in $(seq 1 $1)
	do
		element=$(( ( RANDOM % 20000 ) - 10000 ))

		for j in $(seq 2 $2)
		do
			num=$(( ( RANDOM % 20000 ) - 10000 ))
			element="$element $num"
		done

		rArray=("${rArray[@]}" "$element")
	done
}
