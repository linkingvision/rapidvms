#!/usr/bin/env bash

start_server() {
    until ./gen/Release/firebird/bin/firebird; do
        echo "Firebird crashed with exit code $?.  Restarting..." >&2
    done
}

dummy_output() {
    while true; do
        sleep 9m
        echo "dummy output"
    done
}

skip_tests() {
    SKIP=skip.txt
    echo bugs.core_0870 >> $SKIP
}

set -e
#set -x
eval "$1"
