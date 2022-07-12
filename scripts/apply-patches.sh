#!/bin/sh

git submodule foreach --recursive git reset --hard

git submodule foreach 'if [ -f "../patches/$(basename $sm_path).patch" ]; then git apply "../patches/$(basename $sm_path).patch"; echo "Patch $(basename $sm_path).patch applied"; fi'
