#!/bin/sh

run_stat() {
	file=$1
	extra=$2
	if [ x$(uname -s) = xDarwin ]; then
		stat -f "%N: size=%z bytes $extra" $file
	else
		stat -c "%n: size=%s bytes $extra" $file
	fi
}

TIMES=1000000
AA=./atomic_append
FILES="f1 f2"

# delete files if already present
rm -f $FILES

# run with append
echo "Running with append $TIMES times"
$AA f1 $TIMES & $AA f1 $TIMES &
wait

# run without append
echo "Running without append $TIMES times"
$AA f2 $TIMES x & $AA f2 $TIMES x &
wait

# print file sizes
run_stat f1 "(with append)"
run_stat f2 "(without append)"

# clean up
rm -f $FILES

