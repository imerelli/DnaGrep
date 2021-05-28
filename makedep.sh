#!/bin/sh
head -n `grep -n '^# DEPENDENCIES_BEGIN$' Makefile | head -n 1 | sed -e 's_:.*$__'` Makefile > tmp
CPP=$1
shift
$CPP -MM -MG $* >> tmp
mv tmp Makefile

