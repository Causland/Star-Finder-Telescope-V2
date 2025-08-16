import dash_bootstrap_components as dbc
from dash import html

def current_trajectory_block():
    return dbc.Card(
        [
            dbc.CardHeader("Current Trajectory"),
            dbc.CardBody(
                html.Div(id="current-trajectory-list", style={"height": "160px", "overflowY": "auto"})
            )
        ],
        className="mb-3"
    )
