from dash import html, callback, Output, Input, State, no_update
from dash_extensions import WebSocket
import json
from layout.pid_graph import update_pid_graph
from services.network_constants import QUART_WEBSOCKET_PORT, TELEMETRY_WEBSOCKET_PATH

def telemetry_bridge_block() -> html.Div:
    return html.Div(
        WebSocket(id="ws-telem", url=f"ws://localhost:{QUART_WEBSOCKET_PORT}{TELEMETRY_WEBSOCKET_PATH}"),
        style={"display": "none"}
    )

@callback(Output("status-uptime", "children"), 
          Output("status-version", "children"),
          Output("status-free-heap", "children"),
          Output("status-messages", "children"),
          Output("status-gpslat", "children"),
          Output("status-gpslon", "children"),
          Output("status-gpsalt", "children"),
          Output("current-trajectory-list", "children"),
          Output("pid-data-store", "data"),
          Output("pid-graph", "figure"),
          Output("telemetry-indicator-watchdog", "n_intervals"),
          Output("telemetry-indicator-watchdog", "disabled"),
          Input("ws-telem", "message"),
          State("pid-data-store", "data"),
    prevent_initial_call=True
)
def update_telem_display(message: dict, pid_data: dict):
    if not message:
        return "0", "N/A", "0", "0", "0.0", "0.0", "0.0", \
               no_update, pid_data, no_update, no_update, no_update
    
    data = json.loads(message["data"])

    pid_data["time"].append(data["time"])
    pid_data["curr_az"].append(data["curr_az"])
    pid_data["curr_el"].append(data["curr_el"])
    pid_data["speed_az"].append(data["speed_az"])
    pid_data["target_az"].append(data["target_az"])
    pid_data["target_el"].append(data["target_el"])

    # Limit to 1000 entries
    MAX_ENTRIES = 1000
    for k in pid_data:
        pid_data[k] = pid_data[k][-MAX_ENTRIES:]

    # Get updated graph figure
    pid_figure = update_pid_graph(pid_data)

    return f"{data["time"]:.3f}s", f"{data["version"]}", \
           f"{data["free_heap"]} bytes", f"{data["cmds_received"]}", \
           f"{data["gps_lat"]:.4f}" if data["gps_lat"] != 0.0 else "N/A", \
           f"{data["gps_lon"]:.4f}" if data["gps_lon"] != 0.0 else "N/A", \
           f"{data["gps_alt"]:.2f}m" if data["gps_alt"] != 0.0 else "N/A", \
           f"Running: {data["traj_running"]}, Time to next: {data["time_to_next"]:.2f}s, \
            Entry: {data["traj_current_entry"]}/{data["traj_num_entries"]}", \
           pid_data, pid_figure, \
           0, False