#!/bin/bash

success="0"

result=`./add1 0 0`
if [ "$result" != "0" ]
then
	success="1"
fi

rm -f add1.o add1
exit $success
