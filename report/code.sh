#!/bin/bash

set -e

OUTPUT=code.txt
rm -f $OUTPUT
rm -f gitlog.txt

echo "\\begin{verbatim}" >> $OUTPUT

cd ../..
for f in `find avl | grep -v "^avl/\.git" | grep -v "^avl/report"`
do
	if [ -d $f ]; then
		continue
	fi

	echo "file: $f" >> avl/report$OUTPUT
	echo "" >> avl/report/$OUTPUT
	cat $f >> avl/report/$OUTPUT
	echo "" >> avl/report/$OUTPUT
	echo "" >> avl/report/$OUTPUT
	echo "" >> avl/report/$OUTPUT
done
cd -

echo "\\end{verbatim}" >> $OUTPUT

echo "\\begin{verbatim}" >> gitlog.txt
git log >> gitlog.txt
echo "\\end{verbatim}" >> gitlog.txt
