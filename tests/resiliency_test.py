#!/usr/bin/python3

#https://docs.python.org/3/howto/sockets.html
from socket import socket, AF_INET, SOCK_STREAM
from sys import argv
import subprocess
from time import sleep
import atexit

def cleanup():
	proc.kill()

atexit.register(cleanup)

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
	print("[KO] ircserv died")
	exit(1)

if result_of_check != 0:
	print(f"[KO] Connection to {address}:{port} failed")
	#kill proc
	proc.kill()
	exit (1)

s1 = socket(AF_INET, SOCK_STREAM)
result_of_check = s1.connect_ex((address, port))
#check that the connection is established
if proc.poll() is not None:
	print("[KO] ircserv died")
	exit(1)

if result_of_check != 0:
	print(f"[KO] Connection to {address}:{port} failed")
	#kill proc
	proc.kill()
	exit (1)

#send NICK
s.send(b"NICK test\n")
#terminate s1
s1.close()
sleep(1)		
#assert s is still alive
if (s.fileno() == -1):
	print("[KO] s is not alive")
	exit(1)

s.send(b"EXIT\n")
sleep(1)
#check proc is still alive
if proc.poll() is not None:
	print("[KO] ircserv died")
	exit(1)

#reconnect s
s1 = socket(AF_INET, SOCK_STREAM)
result_of_check = s1.connect_ex((address, port))
#check that the connection is established
if proc.poll() is not None:
	print("[KO] ircserv died")
	exit(1)

if result_of_check != 0:
	print(f"[KO] Connection to {address}:{port} failed")
	#kill proc
	proc.kill()
	exit (1)

print("[OK]")
