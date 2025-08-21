from dash import html, Input, State, callback
import dash_bootstrap_components as dbc
from models.commands import pack_move_base_servos
from services.network_constants import WIFI_USER_ADDR, WIFI_TELESCOPE_ADDR, WIFI_CMD_PORT
import socket

def move_controls_block():
    return dbc.Card(
        [
            dbc.CardHeader("Move Controls"),
            dbc.CardBody(
                [
                    html.Div(
                        [
                            html.Div(), dbc.Button("↑", id="btn-move-up"), html.Div(),
                            dbc.Button("←", id="btn-move-left"), html.Div(), dbc.Button("→", id="btn-move-right"),
                            html.Div(), dbc.Button("↓", id="btn-move-down"), html.Div(),
                        ],
                        className="d-grid gap-2",
                        style={"gridTemplateColumns": "1fr 1fr 1fr",
                               "marginLeft": "20%",
                               "marginRight": "20%"}
                    ),
                    dbc.Input(id="move-degrees", type="number", placeholder="Degrees", className="mt-3"),
                ]
            )
        ],
        className="mb-3"
    )

@callback(Input("btn-move-up", "n_clicks"),
          State("move-degrees", "value"),
          State("pid-data-store", "data"),
    prevent_initial_call=True
)
def move_up(_, deg: float, pid_data: dict):
    target_az = pid_data["target_az"][-1]
    curr_el = pid_data["curr_el"][-1]

    target_el = curr_el - deg

    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    udp_socket.bind((WIFI_USER_ADDR, WIFI_CMD_PORT))

    packet = pack_move_base_servos(target_az, target_el)
    udp_socket.sendto(packet, (WIFI_TELESCOPE_ADDR, WIFI_CMD_PORT))

    udp_socket.close()

@callback(Input("btn-move-down", "n_clicks"),
          State("move-degrees", "value"),
          State("pid-data-store", "data"),
    prevent_initial_call=True
)
def move_down(_, deg: float, pid_data: dict):
    target_az = pid_data["target_az"][-1]
    curr_el = pid_data["curr_el"][-1]

    target_el = curr_el + deg

    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    udp_socket.bind((WIFI_USER_ADDR, WIFI_CMD_PORT))

    packet = pack_move_base_servos(target_az, target_el)
    udp_socket.sendto(packet, (WIFI_TELESCOPE_ADDR, WIFI_CMD_PORT))

    udp_socket.close()

@callback(Input("btn-move-left", "n_clicks"),
          State("move-degrees", "value"),
          State("pid-data-store", "data"),
    prevent_initial_call=True
)
def move_left(_, deg: float, pid_data: dict):
    curr_az = pid_data["curr_az"][-1]
    target_el = pid_data["target_el"][-1]

    target_az = curr_az - deg

    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    udp_socket.bind((WIFI_USER_ADDR, WIFI_CMD_PORT))

    packet = pack_move_base_servos(target_az, target_el)
    udp_socket.sendto(packet, (WIFI_TELESCOPE_ADDR, WIFI_CMD_PORT))

    udp_socket.close()

@callback(Input("btn-move-right", "n_clicks"),
          State("move-degrees", "value"),
          State("pid-data-store", "data"),
    prevent_initial_call=True
)
def move_right(_, deg: float, pid_data: dict):
    curr_az = pid_data["curr_az"][-1]
    target_el = pid_data["target_el"][-1]

    target_az = curr_az + deg

    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    udp_socket.bind((WIFI_USER_ADDR, WIFI_CMD_PORT))

    packet = pack_move_base_servos(target_az, target_el)
    udp_socket.sendto(packet, (WIFI_TELESCOPE_ADDR, WIFI_CMD_PORT))

    udp_socket.close()