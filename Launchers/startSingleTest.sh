#!/bin/bash
clear start
export $(cat config.env | grep -v ^# | xargs)
./../cmake-build-debug/HRtoSQLite_tests/g.tests/GTests_run --gtest_filter=$1
