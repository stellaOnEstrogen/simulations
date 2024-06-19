#!/bin/bash

# Function to normalize a word (converts hyphens to spaces and capitalizes each word)
normalize_word() {
    echo "$1" | sed -e 's/-/ /g' -e 's/\b\(.\)/\u\1/g'
}

# Function to denormalize a word (converts spaces to hyphens and lowercases each word)
denormalize_word() {
    echo "$1" | tr '[:upper:]' '[:lower:]' | awk '{print tolower($0)}' | sed -e 's/ /-/g'
}

# Function to get normalized directory names under ./src/
get_available_simulations() {
    for dir in ./src/*; do
        if [ -d "$dir" ]; then
            echo "$(normalize_word "$(basename "$dir")")"
        fi
    done
}

# Function to parse a Makefile and extract the TARGET
get_target_file() {
    make_file="$1"
    target=$(grep -E '^TARGET\s*=' "$make_file" | sed -e 's/TARGET\s*=\s*//g')
    echo "$target"
}

echo "Available simulations:"
echo "$(get_available_simulations)"

read -p "Enter simulation name: " sim_name

denormalize_name=$(denormalize_word "$sim_name")
sim_dir="./src/$denormalize_name"

if [ -d "$sim_dir" ]; then
    # Compile the simulation
    make_file="$sim_dir/Makefile"
    echo "Compiling simulation $sim_name"
    echo "=============================="
    
    if make -C "$sim_dir"; then
        # Move ./src/$denormalize_name/$target to ./build/$target
        target=$(get_target_file "$make_file")
        
        if [ -f "$sim_dir/$target" ]; then
            mkdir -p ./build
            mv "$sim_dir/$target" "./build/$target"
        else
            echo "Error: Target $target not found"
        fi
        
        echo "=============================="
        
        # Run the simulation
        read -p "Run simulation $sim_name? [y/n]: " run_sim
        if [ "$run_sim" == "y" ]; then
            echo "Running simulation $sim_name"
            echo "=============================="
            "./build/$target"
            echo "Simulation $sim_name ran successfully"
        else
            echo "Simulation $sim_name not run"
        fi
    else
        echo "Error: Compilation failed for simulation $sim_name"
    fi
else
    echo "Error: Simulation $sim_name not found"
fi
