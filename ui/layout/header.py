import dash_bootstrap_components as dbc
from dash import html, dcc, Output, Input, callback
import socket, ipaddress

APP_VERSION = "v0.0.0"

header_block = dbc.Navbar(
    html.Div(
        [
            html.Div("GUI Client IP: —", id="ip-address", className="text-white fw-semibold", style={
                "flex": "1", "textAlign": "left", "paddingLeft": "20px", "fontSize": "18px"
            }),
            html.Div("Star Finder Telescope", className="text-white fw-bold", style={
                "flex": "1", "textAlign": "center", "fontSize": "20px"
            }),
            html.Div(f"{APP_VERSION}", id="gui-version", className="text-white fw-semibold", style={
                "flex": "1", "textAlign": "right", "paddingRight": "20px", "fontSize": "18px"
            }),
            dcc.Interval(id="interval-ip-scan", interval=5000, n_intervals=0),
        ],
        style={"display": "flex", "width": "100%", "alignItems": "center"}
    ),
    color="dark",
    dark=True,
    sticky="top"
)

def list_local_ipv4_addresses():
    ips = set()
    try:
        host = socket.gethostname()
        for info in socket.getaddrinfo(host, None, family=socket.AF_INET):
            ip = info[4][0]
            if ip and not ip.startswith(("127.", "169.254.", "0.")):
                ips.add(ip)
    except Exception:
        pass
    return ips

def select_telescope_ip():
    NETWORK_CANDIDATE = ipaddress.ip_network("192.168.4.0/24")
    
    for ip in sorted(list_local_ipv4_addresses(), key=lambda ip: [int(x) for x in ip.split(".")]):
        try:
            if ipaddress.ip_address(ip) in NETWORK_CANDIDATE:
                return ip
        except ValueError:
            continue
    return None

@callback(Output("ip-address", "children"), Input("interval-ip-scan", "n_intervals"))
def update_ip_label(_):
    ip = select_telescope_ip()
    return html.Div([
        html.Span("GUI Client IP:", style={"marginRight": "20px"}),
        html.Span(ip or "Not Connected")
    ])
