import socket

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind(('localhost', 20000))
server_socket.listen(0)
client_socket, addr = server_socket.accept()
data = client_socket.recv(20001)
print(data)

