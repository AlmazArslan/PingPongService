import socket
import threading
from time import sleep

def PingHandler(s):
	while (1):
		data = s.recv(1024).decode()

		print(data)
		if (data == 'PING'):
			print("Send pong")
			s.send(b"PONG")

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

s.bind(('', 5555))
s.listen(1)

conn, addr = s.accept()
print(conn)
t = threading.Thread(target=PingHandler, args=(conn, ))
t.start()

while (1):
	a = 20
# 	sleep(1)
	conn.send(input("msg: ").encode())
