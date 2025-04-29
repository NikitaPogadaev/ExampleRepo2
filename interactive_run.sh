#!/bin/bash

echo "Fibonacci Calculator is running..."
echo "Enter numbers one by one, or 'exit' to quit"

while true; do
    read -p "Enter number: " input

    if [[ "$input" == "exit" ]]; then
        break
    fi

    if [[ "$input" =~ ^[0-9]+$ ]]; then
        ./bin/exe.out $input
    else
        echo "Please enter a valid number!"
    fi
done
