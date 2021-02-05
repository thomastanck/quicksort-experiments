#!/usr/bin/env bash

# http://redsymbol.net/articles/unofficial-bash-strict-mode/
set -euo pipefail

# Display commands as they run
set -x

# : is the do nothing command
: Set flag default values if unset or null
: CC "${CC:=g++}"
: FLAGS "${FLAGS:=--std=c++17 -Wall}"
: PERFFLAGS "${PERFFLAGS:=-DNDEBUG}"

# Prepare output directory
mkdir -p out

# Compile

