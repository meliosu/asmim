#!/bin/env bash

output_file="output.txt"

min=0
max=400
inc=20
samples=5

> "$output_file"

for ((i=0; i<=500; i+=10)); do
    gcc -DNUM_NOPS=$i -O1 -o main main.c

    if [[ $? -ne 0 ]]; then
        echo "Compilation Failed"
        break
    fi

    outputs=()

    for ((j=0; j<samples; j++)); do
        outputs+="$(./main) "
    done

    echo "$i ${outputs[*]}" | tee -a $output_file
    rm main
done
