#!/bin/bash

top_level=$1
two_level=$2

if [[ $two_level == 1 ]]
then 
    for dir in `ls $top_level`;
    do
        for file in $top_level/$dir/*.txt;
        do
            file_name=`basename "$file" .txt`

            if [[ $file_name == processed* ]]
            then
                file_name=${file_name#processed_} 
                echo $file_name

                for ((i=3; i<=4; i++))
                do
                    ./mfinder $file -nd -f $top_level/$dir/motif_${i}_${file_name} -r 0 -s $i -p 50000
                done
            fi

        done
    done
else
    for file in $top_level/*.txt;
    do
        file_name=`basename "$file" .txt`

        if [[ $file_name == processed* ]]
        then
            file_name=${file_name#processed_} 
            echo $file_name

            for ((i=3; i<=4; i++))
            do
                ./mfinder $file -nd -f ${top_level}/motif_${i}_${file_name} -r 0 -s $i -p 50000
            done
        fi

    done
fi
