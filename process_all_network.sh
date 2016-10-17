#!/bin/bash

top_level=$1
for dir in `ls $top_level`;
do
    for file in $top_level/$dir/*.txt;
    do
        file_name=`basename "$file" .txt`

        if [ $file_name == '*' ]
        then
            continue
        elif [[ $file_name == processed* ]] || [[ $file_name == motif* ]]
        then
            continue
        fi

        echo $file_name
        python parse_common.py $file $top_level/$dir/processed_$file_name.txt
    done
done
