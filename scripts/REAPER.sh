#!/bin/bash
if ! netstat -tn | grep :3389 | grep ESTABLISHED > /dev/null; then
    docker stop winboat-vessel
fi
