#!/bin/bash

find . -maxdepth 1 -type f -executable \
  -not -name "clean.sh" \
  -not -name "build.sh"
  -exec rm -f {} \;
