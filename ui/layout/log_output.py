from dash import html, callback, Output, Input, State, no_update, dcc
import dash_bootstrap_components as dbc
from dash_extensions import WebSocket
import json
from services.network_constants import QUART_WEB_PORT, LOGS_WEBSOCKET_PATH

def log_output_block() -> dbc.Card:
    return dbc.Card(
        [
            dbc.CardHeader("Telescope Logs"),
            dbc.CardBody(
                [
                    dbc.Textarea(id="telescope-logs",
                                 placeholder="No logs", 
                                 style={"height": "400px"},
                                 readOnly=True)
                ]
            ),
            WebSocket(id="ws-logs", url=f"ws://localhost:{QUART_WEB_PORT}{LOGS_WEBSOCKET_PATH}"),
            dcc.Store(id="logs-data-store",
                      data={"logs": ""})
        ],
        className="mb-3"
    )

@callback(Output("telescope-logs", "value"), 
          Output("logs-data-store", "data"),
          Input("ws-logs", "message"),
          State("logs-data-store", "data"),
    prevent_initial_call=True
)
def update_log_display(message: dict, log_data: dict):
    if not message:
        return no_update, no_update
    
    data = json.loads(message["data"])

    log_data["logs"] += data["log_data"]

    return log_data["logs"], log_data