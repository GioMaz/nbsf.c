#!/usr/bin/env bash

DS=ds.csv
FIRST_LINE=1024

{
    ./build.sh
}

IFS=","
sed -n '2048,$ p' $DS | while read -a line
do
    res=$(./nbsf $DS ${line[1]})

    if [[ $(echo "$res <= 0.5" | bc) = 1 ]] && [[ ${line[0]} = ham ]]
    then
        # echo "HAM DETECTED"
        :
    elif [[ $(echo "$res >= 0.5" | bc) = 1 ]] && [[ ${line[0]} = spam ]]
    then
        echo "SPAM DETECTED"
        :
    elif [[ $(echo "$res <= 0.5" | bc) = 1 ]] && [[ ${line[0]} = spam ]]
    then
        echo "SPAM UNDETECTED (FAIL)"
        :
    else
        # echo "HAM UNDETECTED (FAIL)"
        :
    fi
done
