#!/bin/sh

OUTPUT_FILE=$(mktemp -t appendstart)
APPEND_START=./appendstart

echo "Original content" > $OUTPUT_FILE

# run the program
$APPEND_START $OUTPUT_FILE

cat $OUTPUT_FILE

# clean up
rm -r $OUTPUT_FILE
