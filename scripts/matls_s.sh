#!/bin/bash
#rm -f /home/mmlab/client_log_accum.txt

#for i in {1..100}
URL=$1
PORT=$2
ADDR=${URL}:${PORT}
NUM=$3
DIRECTORY=/home/dist/split/mb_${NUM}_split
LOG_FILE=${DIRECTORY}/mb_${NUM}_split.csv

[ -d ${DIRECTORY} ] || mkdir ${DIRECTORY}

#for i in {1..100}
cd /home/dist/matls/apps
for i in {1..100}
do
   echo ${i}:${FILE}
   make cstart PORT=${PORT} LOG_FILE=${LOG_FILE}
   sleep 1
done
echo 'done'