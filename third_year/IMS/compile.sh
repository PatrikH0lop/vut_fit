#!/bin/bash
#
# script for compilation of SIMLIB/C++ model
# (for Linux/bash)
#
SIMLIB="$HOME/Desktop/simlib/src"  # home of SIMLIB library and header files
SOURCE="$1"                 # source file
MODEL="${1%.cpp}"            # executable name
if [ -z "$SOURCE" -o ! -f "$SOURCE" -o "$MODEL" = "$SOURCE" ]; then
  echo "Usage: " 
  echo "         SIMLIB model.cc"
  exit 1
fi
echo
echo "### Creating model $MODEL"
echo
#static: g++ -O2 -I$SIMLIB -o "$MODEL" "$SOURCE" $SIMLIB/simlib.a -lm
g++ -g -O2 -I$SIMLIB -o "$MODEL" "$SOURCE" $SIMLIB/simlib.so -lm
echo

