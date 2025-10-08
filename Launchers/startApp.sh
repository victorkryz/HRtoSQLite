#!/bin/bash
echo "Start HRtoSQLite..."
# add libraries path:
# export LD_LIBRARY_PATH="POCO libraries path:Oracle client libraries path":$LD_LIBRARY_PATH
./../build/HRtoSQLite_run/HRtoSQLite_run -C//localhost:1521/FREEPDB1 -N hr -P hr -S./../sql/hrSQLiteDDL.sql -O./../dbs/hr.db
