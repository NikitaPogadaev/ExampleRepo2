#!/bin/bash

if [ $# -gt 0 ]; then
    ./bin/exe.out $1
else
    echo "Аргументы не переданы!!!"
fi
