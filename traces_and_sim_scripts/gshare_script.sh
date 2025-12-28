#!/bin/bash

# Define the range for m
MIN_M=7
MAX_M=20

# Output file for easy plotting in Excel
OUTPUT_FILE="misprediction_data.csv"

# Clear the output file and add headers
echo "m,n,MispredictionRate" > "$OUTPUT_FILE"

echo "Starting simulation script..."

# Loop through each value of m
for m in $(seq $MIN_M $MAX_M); do
    # Define the range for n based on m (0 to m)
    MAX_N=$m
    
    # Loop through each value of n for the current m
    for n in $(seq 0 $MAX_N); do
        echo "Running command: ./sim gshare $m $n gcc_trace.txt"
        
        # Execute the command and capture the output
        # Assuming './sim' is an executable available in the current directory
        SIM_OUTPUT=$(./sim gshare $m $n gcc_trace.txt)
        
        # Grep the misprediction rate string and extract the numerical value
        # The exact grep/awk command might need adjustment based on the exact output format of './sim'
        # We assume the output line looks something like: "misprediction rate: 0.1234%" or "misprediction rate: 0.1234"
        MISPRED_RATE=$(echo "$SIM_OUTPUT" | grep "misprediction rate:" | awk '{print $NF}' | sed 's/%//')
        
        # Check if we successfully got a rate
        if [ -n "$MISPRED_RATE" ]; then
            # Append the data to the CSV file
            echo "$m,$n,$MISPRED_RATE" >> "$OUTPUT_FILE"
            echo "Recorded: m=$m, n=$n, Rate=$MISPRED_RATE"
        else
            echo "Error: Could not find 'misprediction rate:' in the output for m=$m, n=$n"
        fi
    done
done

echo "Script finished. Data saved to $OUTPUT_FILE"
echo "You can now open $OUTPUT_FILE in Excel to create your plot."
