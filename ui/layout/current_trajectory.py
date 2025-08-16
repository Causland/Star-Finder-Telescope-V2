from dash import html
import dash_bootstrap_components as dbc

def current_trajectory_block() -> dbc.Card:
    return dbc.Card(
        [
            dbc.CardHeader("Current Trajectory"),
            dbc.CardBody(
                html.Div(id="current-trajectory-list", style={"height": "160px", "overflowY": "auto"})
            )
        ],
        className="mb-3"
    )
