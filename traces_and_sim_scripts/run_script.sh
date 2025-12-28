#!/bin/bash

# Define the command structure
SIM_COMMAND="./sim bimodal"
TRACE_FILE="gcc_trace.txt"
SEARCH_STRING="misprediction rate:"

# Iterate from 7 to 20
for i in $(seq 7 20); do
    echo "--------------------------------------------------"
    echo "Running command for iteration: $i"
    
    # Construct the full command
    FULL_COMMAND="$SIM_COMMAND $i $TRACE_FILE"
    echo "Executing: $FULL_COMMAND"

    # Execute the command, pipe the output to grep, and print the result
    # We use 'eval' to ensure the full command string is executed correctly by the shell
    eval "$FULL_COMMAND" | grep "$SEARCH_STRING"
    
    # Check if grep found anything (optional, for debugging)
    if [ $? -eq 0 ]; then
        echo "Successfully found the misprediction rate for iteration $i"
    else
        echo "Could not find the misprediction rate for iteration $i (or command failed)"
    fi
done

echo "--------------------------------------------------"
echo "Script finished."
