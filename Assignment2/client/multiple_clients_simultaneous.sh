#!/bin/bash
# Author : Rajat Khanduja
# Date : 16/1/2012
# Script that runs various clients at once.

num=$1
i=0
while [ $i -le $num ]
do
  
  echo $i
  i=$((i+1))
  (./bin/client 127.0.0.1 /home/rajat &)

done
