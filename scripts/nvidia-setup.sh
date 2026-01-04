#!/bin/bash
set -e  # Exit on error
# Copyright (C) 2026 Macaulay Smith
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License.
# Ensure we have the detective tool


#FULLY AI WRITTEN TO REDUCE TIME

if ! command -v nvidia-detect &> /dev/null; then
    echo "🔍 Installing nvidia-detect..."
    apt update && apt install -y nvidia-detect
fi

# Check what NVIDIA driver Debian recommends
DRV=$(nvidia-detect | grep nvidia-driver | awk '{print $NF}')

if [ ! -z "$DRV" ]; then
    echo "🏎️ NVIDIA Hardware detected. Installing $DRV..."
    apt update
    apt install -y "$DRV"
else
    echo "✅ No NVIDIA card found or already configured."
fi
