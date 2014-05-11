#!/bin/bash

set -e

OUTPUT=code.txt
rm -f $OUTPUT

echo "\\begin{verbatim}" >> $OUTPUT

cd ../..
for f in `find avl | grep -v "^avl/\.git" | grep -v "^avl/report"`
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
cd -

echo "\\end{verbatim}" >> $OUTPUT

git log > gitlog.txt
