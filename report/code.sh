#!/bin/bash

set -e

OUTPUT=code.txt
rm -f $OUTPUT

for f in `find avl | grep -v "\.git"`
do
	if [ -d $f ]; then
		continue
	fi

	echo "file: $f" >> $OUTPUT
	echo "" >> $OUTPUT
	cat $f >> $OUTPUT
	echo "" >> $OUTPUT
	echo "" >> $OUTPUT
	echo "" >> $OUTPUT
done
