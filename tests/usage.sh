#!/bin/bash

#import functions
. ./tests/common.sh

#Verify that the --help option works properly
assert_exec "$EXE --help" \
"usage: ./ircserv [host:port_network:password_network] <port> <password>" "" 0

#Verify that empty arguments throw an error and display the usage message
assert_exec "$EXE" \
"usage: ./ircserv [host:port_network:password_network] <port> <password>" "Not enough arguments" 1
