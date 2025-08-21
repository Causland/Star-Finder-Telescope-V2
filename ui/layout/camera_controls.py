from dash import html, Input, Output, State, callback, no_update
import dash_bootstrap_components as dbc
from dash_extensions import WebSocket
import json
from models.commands import pack_focus_manual, pack_control_camera_photo
from services.network_constants import WIFI_USER_ADDR, WIFI_TELESCOPE_ADDR, WIFI_CMD_PORT, QUART_WEB_PORT, CAMERA_WEBSOCKET_PATH
import socket

def camera_controls_block() -> dbc.Card:
    return dbc.Card(
        [
            dbc.CardHeader("Camera Controls"),
            dbc.CardBody(
                [
                    html.Div(
                        [
                            dbc.InputGroup(
                                [
                                    dbc.Button("←", id="btn-cam-left", style={"width": "15%"}),
                                    dbc.Button("→", id="btn-cam-right", style={"width": "15%"}),
                                ],
                                className="gap-4 justify-content-center mb-1",
                            ),
                            dbc.Input(id="camera-degrees", type="number", placeholder="Degrees")
                        ],
                        className="mb-5"
                    ),
                    dbc.Button("Take Photo", id="btn-take-photo", className="mb-1"),
                    html.Div(
                        html.Img(id="camera-image", src=None, style={"width": "100%", "height": "auto", "objectFit": "contain"}),
                        id="camera-image-box",
                        style={"height": "300px", "background": "#343a40", "border": "1px dashed #6c757d"}
                    ),
                    WebSocket(id="ws-camera", url=f"ws://localhost:{QUART_WEB_PORT}{CAMERA_WEBSOCKET_PATH}"),
                ]
            )
        ],
        className="mb-3"
    )

@callback(Input("btn-cam-left", "n_clicks"),
          State("camera-degrees", "value"),
    prevent_initial_call=True
)
def move_left(_, deg: float):
    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    udp_socket.bind((WIFI_USER_ADDR, WIFI_CMD_PORT))

    packet = pack_focus_manual(deg)
    udp_socket.sendto(packet, (WIFI_TELESCOPE_ADDR, WIFI_CMD_PORT))

    udp_socket.close()

@callback(Input("btn-cam-right", "n_clicks"),
          State("camera-degrees", "value"),
    prevent_initial_call=True
)
def move_right(_, deg: float):
    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    udp_socket.bind((WIFI_USER_ADDR, WIFI_CMD_PORT))

    packet = pack_focus_manual(-1 * deg)
    udp_socket.sendto(packet, (WIFI_TELESCOPE_ADDR, WIFI_CMD_PORT))

    udp_socket.close()

@callback(Input("btn-take-photo", "n_clicks"),
    prevent_initial_call=True
)
def take_photo(_):
    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    udp_socket.bind((WIFI_USER_ADDR, WIFI_CMD_PORT))

    packet = pack_control_camera_photo()
    udp_socket.sendto(packet, (WIFI_TELESCOPE_ADDR, WIFI_CMD_PORT))

    udp_socket.close()

@callback(Output("camera-image", "src"),
          Input("ws-camera", "message"),
    prevent_initial_call=True
)
def update_camera_image(message: dict):
    if not message:
        return no_update

    data = json.loads(message["data"])
    
    return data["image"]