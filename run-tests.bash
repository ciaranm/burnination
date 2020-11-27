#!/bin/bash

set -x

./burnination path.txt | grep 'solution: 4 ' || exit 1
./burnination notpath.txt | grep 'solution: 2 ' || exit 1

true

