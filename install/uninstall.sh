#!/bin/bash

source activate venv
echo "y" | pip uninstall dumbpy
echo "y" | pip uninstall numc
source deactivate
