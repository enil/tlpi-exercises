#!/bin/sh

./envtest | grep -e "^\(PATH\|USER\|foo\|hello\)="

