#!/bin/bash

NICK="quotes_bot"
USER="irccat 8 x : irccat"
SLEEP_TIME=3.4

send_message()
{
	printf "%s\r\n" "$@"
}

get_random_quote()
{
	local response=$(curl -s 'https://api.quotable.io/random')
	local formatted=$(echo "$response" | tr '"' '\n')
	local content=$(echo "$formatted" | grep -A2 content | sed '3q;d')
	local author=$(echo "$formatted" | grep -A2 author | sed '3q;d')
	echo "$content - $author"
}

bot()
{

	local password="$1"
	local channel="$2"
	if ! (echo "$channel" | grep -q '#'); then
		channel="#$channel"
	fi
	send_message "PASS $password"
	send_message "NICK $NICK"
	send_message "USER $USER"
	send_message "JOIN $channel"
	while true; do
		sleep "$SLEEP_TIME"
		quote=$(get_random_quote)
		send_message "PRIVMSG $channel :$quote"
	done
	
}

run()
{
	if [ "$#" -ne 4 ]; then
		echo "Usage: $0 <host> <port> <password> <channel>"
		exit 1
	fi

	local host="$1"
	local port="$2"
	shift 2
	while true;do
		bot $@ | nc -q 0 "$host" "$port"
	done
}

run $@