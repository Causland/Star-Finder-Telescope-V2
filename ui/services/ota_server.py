from functools import partial
import http.server
import os
import ssl
import threading

class OneShotHandler(http.server.SimpleHTTPRequestHandler):
    def __init__(self, *args, callback=None, **kwargs):
        self._callback = callback
        super().__init__(*args, **kwargs)

    def log_message(self, format, *args) -> None:
        print("Log: %s - [%s] %s" % (
            self.client_address[0],
            self.log_date_time_string(),
            format % args))

    def do_GET(self) -> None:
        print(f"\n📥 Incoming GET request from {self.client_address}")
        print(f"🌐 Path: {self.path}")
        print("📨 Headers:")
        for header, value in self.headers.items():
            print(f"   {header}: {value}")
        
        super().do_GET()  # Handles the actual response
        
        print(f"✅ Sent file: {self.path}\n")

        threading.Thread(target=self._callback).start()

    def send_response(self, code, message=None) -> None:
        print(f"🔁 Response Code: {code} {message if message else ''}")
        super().send_response(code, message)

class OtaServer:
    def __init__(self):
        self._httpd = None
        self._thread = None

    def start_server(self, ota_image_dir: str, server_ip: str, server_port: int,
                     server_file: str, key_file: str) -> None:
        os.chdir(ota_image_dir)

        handler = partial(OneShotHandler, callback=self.stop_server)
        self._httpd = http.server.HTTPServer((server_ip, server_port), handler)

        ssl_context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
        ssl_context.load_cert_chain(certfile=server_file, keyfile=key_file)

        self._httpd.socket = ssl_context.wrap_socket(self._httpd.socket, server_side=True)

        self._thread = threading.Thread(target=self._httpd.serve_forever, daemon=True)
        self._thread.start()

    def stop_server(self) -> None:
        if self._httpd:
            self._httpd.shutdown()
            self._thread.join()
            self._httpd = None
            self._thread = None

    def server_running(self) -> bool:
        return self._httpd != None

ota_server = OtaServer()