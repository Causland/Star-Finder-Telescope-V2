from dash import html, callback, Input, State
import dash_bootstrap_components as dbc
from models.commands import pack_telem_rate
from services.network_constants import WIFI_USER_ADDR, WIFI_TELESCOPE_ADDR, WIFI_CMD_PORT
import socket

def telem_rate_control_block() -> dbc.Card:
    return dbc.Card(
        [
            dbc.CardHeader("Telemetry Rate Control"),
            dbc.CardBody(
                [
                    html.Div(
                        [
                            dbc.InputGroup(
                                [
                                    dbc.Input(id="telem-rate-control-value", type="number", placeholder="Rate (ms)"),
                                    dbc.Button("Update", id="telem-rate-control-btn", style={"width": "15%"}),
                                ],
                                className="gap-2",
                            )
                        ]
                    )
                ]
            )
        ],
        className="mb-3"
    )

@callback(Input("telem-rate-control-btn", "n_clicks"),
          State("telem-rate-control-value", "value"),
    prevent_initial_call=True          
)
def send_telem_rate_command(_, rate: int):
    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    udp_socket.bind((WIFI_USER_ADDR, WIFI_CMD_PORT))

    packet = pack_telem_rate(rate)
    udp_socket.sendto(packet, (WIFI_TELESCOPE_ADDR, WIFI_CMD_PORT))

    udp_socket.close()
