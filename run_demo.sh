#!/bin/bash

# Check if demo.html exists
if [ -f demo.html ]; then
    # Copy to tst.html for xml_generator.c
    cp demo.html tst.html
    
    # Run the generator
    echo "Running demo from demo.html file..."
    
    # Try using build.sh to compile and run the demo
    ./build.sh xml_generator.c
    
    # If build.sh fails, try running the compiled executable directly
    if [ $? -ne 0 ]; then
        echo "Falling back to direct executable execution..."
        ./main.exe
    fi
else
    echo "Error: demo.html file not found!"
    exit 1
fi
