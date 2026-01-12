#!/bin/bash
# Copyright (C) 2026 Macaulay Smith
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License.


if ! netstat -tn | grep :3389 | grep ESTABLISHED > /dev/null; then
    docker stop winboat-vessel
fi
