#!/bin/env bash

min=0
max=400
inc=20
samples=3

for ((i=min; i<=max; i+=inc)); do
    gcc -DNUM_NOPS=$i -O1 -o main main.c

    if [[ $? -ne 0 ]]; then
        echo "Compilation Failed"
        break
    fi

    outputs=()

    for ((j=0; j<samples; j++)); do
        outputs+="$(./main) "
    done

    echo "$i ${outputs[*]}"
    rm main
done
