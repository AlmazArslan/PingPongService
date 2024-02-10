import socket
import time
import threading

PONG_RESPONE = True

def PingHandler(s):
	while (1):
		data = s.recv(1024).decode()
		print(data)
		if (data == 'PING\r\n' and PONG_RESPONE):
			s.send(b"PONG\r\n")

		if (data == 'CLOSE_SESSEOION\r\n'):
			s.close()
			exit(0)

		time.sleep(0.1)

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

s.bind(('', 5555))
s.listen(1)

conn, addr = s.accept()
t = threading.Thread(target=PingHandler, args=(conn, ), daemon=True)
t.start()

while (1):
	conn.send((input("msg: ") + '\r\n').encode())
