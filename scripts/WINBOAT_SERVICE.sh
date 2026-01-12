#!/bin/bash
# Copyright (C) 2026 Macaulay Smith
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License.
# Ensure we have the detective tool

if [ "$(docker inspect -f '{{.State.Running}}' winboat-vessel 2>/dev/null)" != "true" ]; then
    echo "Starting Windows Compatibility Layer..."
    docker start winboat-vessel
    sleep 15
fi

winboat --run "$1"
