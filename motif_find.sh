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
                echo $file

                ./finder $file $top_level/$dir/out_${i}_${file_name}.txt 1 50000
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

            ./finder $file ${top_level}/motif_${i}_${file_name} 1 50000
        fi

    done
fi
