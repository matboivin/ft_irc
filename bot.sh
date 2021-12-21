#!/bin/bash

send_message()
{
	printf "%s\r\n" "$@"
}

get_random_quote()
{
	response=$(curl -s 'https://api.quotable.io/random')
	formatted=$(echo "$response" | tr '"' '\n')
	content=$(echo "$formatted" | grep -A2 content | sed '3q;d')
	author=$(echo "$formatted" | grep -A2 author | sed '3q;d')
	echo "$content - $author"
}

PASSWORD="pass"
NICK="quotes_bot"
USER="irccat 8 x : irccat"
HOST="irc.w3.org"
PORT="6667"
CHANNEL="#random_quotes"
SLEEP_TIME=5

bot()
{
	send_message "PASS $PASSWORD"
	send_message "NICK $NICK"
	send_message "USER $USER"
	send_message "JOIN $CHANNEL"
	while true; do
		sleep "$SLEEP_TIME"
		quote=$(get_random_quote)
		send_message "PRIVMSG $CHANNEL :$quote"
	done
	
}

run()
{
	while true;do
		bot | nc localhost 6667
	done
}

run