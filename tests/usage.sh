#!/bin/bash

#import functions
. ./tests/common.sh

#Verify that the --help option works properly
assert_exec "$EXE --help" \
"usage: ./ircserv [host:port_network:password_network] <port> <password>" "" 0
