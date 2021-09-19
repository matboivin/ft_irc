#!/usr/bin/python3

#https://docs.python.org/3/howto/sockets.html
from socket import socket, AF_INET, SOCK_STREAM
from sys import argv
import subprocess
from time import sleep

bind_address = argv[1]
address = argv[2]
port = int(argv[3])

#launch ./ircserv
proc = subprocess.Popen(["./ircserv", bind_address, argv[3], ""])
sleep(1)

# create an INET, STREAMing socket
s = socket(AF_INET, SOCK_STREAM)
# now connect to the web server on port 80 - the normal http port
result_of_check = s.connect_ex((address, port))

#check that the connection is established
if proc.poll() is not None:
	print("ircserv died")
	exit(1)

if result_of_check == 0:
	print(f"Connection to {address}:{port} established")
	#kill proc
	proc.kill()
	exit (0)
else:
	print(f"Connection to {address}:{port} failed")
	#kill proc
	proc.kill()
	exit (1)
