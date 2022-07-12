#!/bin/sh

git submodule foreach 'if [ -n "$(git diff)" ]; then git diff >  "../patches/$(basename $sm_path).patch"; echo "Patch $(basename $sm_path).patch generated"; fi'
