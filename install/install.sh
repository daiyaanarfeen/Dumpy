#!/bin/bash

if [ ! -d ~/.conda/envs/venv/ ]; then 
    echo "y" | conda create --name venv
    echo "y" | conda install -n venv pip
    echo "y" | conda install -n venv scipy
    echo "y" | conda install -n venv numpy
fi

source activate venv
pip install .
echo ""
echo ""
echo "==============="
echo "Testing Speedup"
echo "==============="
python testing/speedup.py
source deactivate
