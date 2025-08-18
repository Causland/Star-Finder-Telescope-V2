from dash import html, dcc, callback, callback_context, Input, Output, no_update
import dash_bootstrap_components as dbc
from models.commands import pack_ota_update
import os
from services.network_constants import WIFI_OTA_PORT, WIFI_CMD_PORT, WIFI_USER_ADDR, WIFI_TELESCOPE_ADDR
from services.ota_server import ota_server
import socket

def ota_update_block() -> dbc.Card:
    return dbc.Card(
        [
            dbc.CardHeader("OTA Update"),
            dbc.CardBody(
                [
                    html.Div(
                        [
                            html.Div(children=dbc.Input(id="ota-update-file-name", type="text", placeholder="Path to telescope.bin"), className="mb-1"),
                            html.Div(children=dbc.Input(id="ota-update-ca-cert", type="text", placeholder="Path to ca_cert.pem"), className="mb-1"),
                            html.Div(children=dbc.Input(id="ota-update-ca-key", type="text", placeholder="Path to ca_key.pem"), className="mb-1"),     
                        ],
                        className="mb-3"
                    ),
                    dbc.Button("Start Update", id="start-ota-btn", disabled=True, className="mb-1"),
                    dbc.Alert(id="update-alert", is_open=False, color="danger", dismissable=True, duration=5000),
                    dcc.Interval(id="update-complete-check", interval=1000, n_intervals=0, disabled=True)
                ]
            )
        ],
        className="mb-3"
    )    

@callback(
    Output("ota-update-file-name", "valid"),
    Output("ota-update-file-name", "invalid"),
    Output("ota-update-ca-cert", "valid"),
    Output("ota-update-ca-cert", "invalid"),
    Output("ota-update-ca-key", "valid"),
    Output("ota-update-ca-key", "invalid"),
    Output("start-ota-btn", "disabled"),
    Output("update-alert", "is_open"),
    Output("update-alert", "children"),
    Output("update-complete-check", "disabled"),
    Output("update-complete-check", "n_intervals"),
    Output("start-ota-btn", "children"),
    Input("ota-update-file-name", "value"),
    Input("ota-update-ca-cert", "value"),
    Input("ota-update-ca-key", "value"),
    Input("start-ota-btn", "n_clicks"),
    Input("update-complete-check", "n_intervals"),
    prevent_initial_call=True
)
def ota_update_handler(file_path, cert_path, key_path, _, n_intervals):
    triggered = callback_context.triggered[0]["prop_id"].split(".")[0]

    # Input validation
    file_valid, file_invalid = False, True
    cert_valid, cert_invalid = False, True
    key_valid, key_invalid = False, True

    if file_path and os.path.exists(file_path) and os.path.isfile(file_path) and os.path.basename(file_path) == "telescope.bin":
        file_valid, file_invalid = True, False
    if cert_path and os.path.exists(cert_path) and os.path.isfile(cert_path) and os.path.basename(cert_path) == "ca_cert.pem":
        cert_valid, cert_invalid = True, False
    if key_path and os.path.exists(key_path) and os.path.isfile(key_path) and os.path.basename(key_path) == "ca_key.pem":
        key_valid, key_invalid = True, False

    inputs_valid = file_valid and cert_valid and key_valid
    start_disabled = not inputs_valid

    # Update logic
    alert_open = False
    alert_msg = ""
    interval_disabled = no_update
    interval_count = no_update
    button_value = no_update

    if triggered == "start-ota-btn" and inputs_valid:
        if ota_server.server_running():
            alert_open = True
            alert_msg = "⚠️ Update already in progress!"
        else:
            ota_server.start_server(
                os.path.dirname(file_path),
                WIFI_USER_ADDR,
                WIFI_OTA_PORT,
                cert_path,
                key_path
            )

            udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            udp_socket.bind((WIFI_USER_ADDR, WIFI_CMD_PORT))
            packet = pack_ota_update(os.path.basename(file_path))
            udp_socket.sendto(packet, (WIFI_TELESCOPE_ADDR, WIFI_CMD_PORT))

            interval_disabled = False
            interval_count = 0
            button_value = "Updating..."
            start_disabled = True

    elif triggered == "update-complete-check":
        if not ota_server.server_running() or n_intervals > 10:
            if (n_intervals > 10):
                # The update timed out
                ota_server.stop_server()

            interval_disabled = True
            button_value = "Start Update"
            start_disabled = False

    return (
        file_valid, file_invalid,
        cert_valid, cert_invalid,
        key_valid, key_invalid,
        start_disabled,
        alert_open, alert_msg,
        interval_disabled, interval_count,
        button_value
    )