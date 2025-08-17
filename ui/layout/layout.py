from dash import html
import dash_bootstrap_components as dbc

from layout.background_telemetry import telemetry_bridge_block
from layout.camera_controls import camera_controls_block
from layout.current_trajectory import current_trajectory_block
from layout.header import header_block
from layout.log_output import log_output_block
from layout.move_controls import move_controls_block
from layout.ota_update import ota_update_block
from layout.pid_graph import pid_graph_block
from layout.status_info import status_block
from layout.telem_rate_control import telem_rate_control_block
from layout.trajectory_planner import trajectory_planner_block

layout = html.Div(
    [
        header_block,
        telemetry_bridge_block(),
        dbc.Container(
            dbc.Row(
                [
                    dbc.Col(
                        [
                            status_block(),
                            telem_rate_control_block(),
                            ota_update_block(),
                            log_output_block()
                        ],
                        width=3
                    ),
                    dbc.Col(
                        [
                            trajectory_planner_block(),
                            current_trajectory_block(),
                            pid_graph_block()
                        ],
                        width=6
                    ),
                    dbc.Col(
                        [
                            move_controls_block(),
                            camera_controls_block()
                        ],
                        width=3
                    ),
                ],
                className="gy-4"
            ),
            fluid=True,
            className="mt-4"
        )
    ]
)
