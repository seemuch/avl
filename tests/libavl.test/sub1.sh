#!/bin/bash

success="0"

result=`./sub1 0 0`
if [ "$result" != "0" ]
then
	success="1"
fi

rm -f sub1.o sub1
exit $success
