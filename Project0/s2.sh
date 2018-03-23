#!/bin/bash

ls -l /bin |grep ^-  |awk '$9 ~/^b/ {print $9" "$3" "$1}'|sort > output.txt
chmod  744 output.txt
