#!/usr/bin/python3

# Before running this wrapper, please provide the key and the certificate. You can
# generate them using the following:
#
# openssl req -x509 -newkey rsa:2048 -keyout key.pem -out cert.pem -days 365

from http.server import HTTPServer, BaseHTTPRequestHandler
import ssl

class KeaCARequestHandler(BaseHTTPRequestHandler):

    def do_GET(self):
        self.send_response(200)
        self.end_headers()
        self.wfile.write(b'Hello, world!')

    def do_POST(self):
        content_length = int(self.headers['Content-Length'])
        body = self.rfile.read(content_length)
        self.send_response(200)
        self.end_headers()
        response = BytesIO()
        response.write(b'This is POST request. ')
        response.write(b'Received: ')
        response.write(body)
        self.wfile.write(response.getvalue())


httpd = HTTPServer(('localhost', 4443), KeaCARequestHandler)

httpd.socket = ssl.wrap_socket (httpd.socket, 
        keyfile="./kea-ca.key", 
        certfile='./kea-ca.cert', server_side=True)

httpd.serve_forever()
