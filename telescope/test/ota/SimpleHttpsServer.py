import http.server
import os
import ssl
import sys

def start_https_server(ota_image_dir: str, server_ip: str, server_port: int,
                       server_file: str, key_file: str) -> None:
    os.chdir(ota_image_dir)

    httpd = http.server.HTTPServer((server_ip, server_port), http.server.SimpleHTTPRequestHandler)

    ssl_context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
    ssl_context.load_cert_chain(certfile=server_file, keyfile=key_file)

    httpd.socket = ssl_context.wrap_socket(httpd.socket, server_side=True)
    httpd.serve_forever()

if __name__ == '__main__':
    if len(sys.argv) > 4:
        # Usage: SimpleHttpsServer.py <image_dir> <server_ip> <server_port> <cert_dir>
        this_dir = os.getcwd()
        bin_dir = os.path.join(this_dir, sys.argv[1])
        ip = sys.argv[2]
        port = int(sys.argv[3])
        cert_dir = os.path.join(this_dir, sys.argv[4])
        print(f'Starting HTTPS server at "https://{ip}:{port} with:\r\n bin_dir={bin_dir}\r\n cert_dir={cert_dir}"')
        start_https_server(
            bin_dir,
            ip,
            port,
            server_file=os.path.join(cert_dir, 'ca_cert.pem'),
            key_file=os.path.join(cert_dir, 'ca_key.pem'),
        )
