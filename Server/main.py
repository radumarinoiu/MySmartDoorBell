import socketserver


class DoorBellTCPHandler(socketserver.BaseRequestHandler):
    def handle(self) -> None:
        data = self.request.recv(8192).strip()
        print("{} sent: {}".format(self.client_address[0], data))


if __name__ == '__main__':
    HOST, PORT = "0.0.0.0", 3000
    server = socketserver.TCPServer((HOST, PORT), DoorBellTCPHandler)
    server.serve_forever()
