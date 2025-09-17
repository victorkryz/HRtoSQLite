#!/bin/bash
clear start
export $(cat config.env | grep -v ^# | xargs)
# add libraries path:
# export LD_LIBRARY_PATH="POCO libraries path:Oracle client libraries path":$LD_LIBRARY_PATH
./../build/HRtoSQLite_tests/g.tests/GTests_run --gtest_filter=$1
