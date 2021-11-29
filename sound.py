#record computer sound
import pyaudio
import wave
import time
import os
import sys

#import socket
import socket

#record sound and stream it to a listening socket

def stream_sound():
	#create listening socket
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	sock.bind(('', 8500))
	sock.listen(1)
	conn, addr = sock.accept()
	print('connected:', addr)
	#create an audio object
	p = pyaudio.PyAudio()
	#open stream
	stream = p.open(format =
					p.get_format_from_width(WIDTH),
					channels = CHANNELS,
					rate = RATE,
					output = True)
	#read data
	data = conn.recv(CHUNK)
	while data:
		stream.write(data)
		data = conn.recv(CHUNK)
	#stop stream
	stream.stop_stream()
	stream.close()
	#close PyAudio
	p.terminate()

