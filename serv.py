import socket
import threading

def PingHandler(s):
	while (1):
		data = s.recv(1024)
		if (data.decode() == 'PING'):
			s.send(b"PONG")

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

s.bind(('', 5555))
s.listen(1)

conn, addr = s.accept()
t = threading.Thread(target=PingHandler, args=(conn, ))
t.start()

while (1):
	conn.send(input("msg: ").encode())
