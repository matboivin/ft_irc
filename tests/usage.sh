#!/bin/bash

#import functions
. ./tests/common.sh

#Verify that the --help option works properly
assert_exec "$EXE --help" \
"usage: ./ircserv <port> <password>" "" 0
