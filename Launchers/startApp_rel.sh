#!/bin/bash
echo "Start HRtoSQLite!"
./../build/HRtoSQLite_run/HRtoSQLite_run -C//192.168.1.35/xe -N hr -P hr -S./../sql/hrSQLiteDDL.sql -O./../dbs/hr.db