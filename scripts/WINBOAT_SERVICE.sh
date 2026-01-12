#!/bin/bash
if [ "$(docker inspect -f '{{.State.Running}}' winboat-vessel 2>/dev/null)" != "true" ]; then
    echo "Starting Windows Compatibility Layer..."
    docker start winboat-vessel
    sleep 15
fi

winboat --run "$1"
