#!/usr/bin/env bash

DS=ds.csv
FIRST_LINE=1024

{
    ./build.sh
}

IFS=","
sed -n '2,$ p' $DS | while read -a line
do
    res=$(./nbsf $DS ${line[1]})
    echo $res
done
