#!/bin/bash
# Test replies

# import functions
. ./tests/common.sh

BIND_ADDR="0.0.0.0"
PORT="2727"

# run server in background
./ircserv $BIND_ADDR $PORT "" &
# store PID
SERV_PID=$!
sleep 1

# compare outputs

# wrong commands
assert_exec "echo foo | nc -q 1 $BIND_ADDR $PORT" ":irc.42.fr 421 foo :Unknown command" "" 0
assert_exec "echo 123 | nc -q 1 $BIND_ADDR $PORT" "" "" 0

# wrong nicknames
assert_exec "echo NICK -meow | nc -q 1 $BIND_ADDR $PORT" ":irc.42.fr 432 -meow :Erroneous nickname" "" 0
assert_exec "echo NICK 0meow | nc -q 1 $BIND_ADDR $PORT" ":irc.42.fr 432 0meow :Erroneous nickname" "" 0
assert_exec "echo NICK meooowwwww | nc -q 1 $BIND_ADDR $PORT" ":irc.42.fr 432 meooowwwww :Erroneous nickname" "" 0
# valid nicknames
assert_exec "echo NICK me0w | nc -q 1 $BIND_ADDR $PORT" "" "" 0
assert_exec 'echo NICK \`me0w | nc -q 1 $BIND_ADDR $PORT' "" "" 0
assert_exec "echo NICK [me0w] | nc -q 1 $BIND_ADDR $PORT" "" "" 0
assert_exec "echo NICK meow-ow | nc -q 1 $BIND_ADDR $PORT" "" "" 0

# assert_exec "echo QUIT | nc -q 1 $BIND_ADDR $PORT" "" "" 0

kill $SERV_PID
