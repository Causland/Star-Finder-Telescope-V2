from dash import dcc
import dash_bootstrap_components as dbc
import plotly.graph_objects as go

def pid_graph_block() -> dbc.Card:
    return dbc.Card(
        [
            dbc.CardHeader("PID Controller"),
            dbc.CardBody(
                [
                    dcc.Graph(
                        id="pid-graph",
                        config={"displayModeBar": False},
                        style={"height": "30%"},
                    ),
                    dcc.Store(id="pid-data-store",
                              data={"time": [],
                                    "curr_az": [],
                                    "curr_el": [],
                                    "speed_az": [],
                                    "target_az": [],
                                    "target_el": []}
                    )
                ]
            )
        ],
        className="mb-3"
    )

def update_pid_graph(data: dict) -> go.Figure:
    fig = go.Figure()
    
    fig.add_trace(go.Scatter(
        x=data["time"],
        y=data["curr_az"],
        mode="lines",
        name="Current Azimuth"
    ))
    fig.add_trace(go.Scatter(
        x=data["time"],
        y=data["curr_el"],
        mode="lines",
        name="Current Elevation"
    ))
    fig.add_trace(go.Scatter(
        x=data["time"],
        y=data["target_az"],
        mode="lines",
        name="Target Azimuth"
    ))
    fig.add_trace(go.Scatter(
        x=data["time"],
        y=data["target_el"],
        mode="lines",
        name="Target Elevation"
    ))
    fig.add_trace(go.Scatter(
        x=data["time"],
        y=data["speed_az"],
        mode="lines",
        name="Speed Azimuth"
    ))

    fig.update_layout(
        margin=dict(t=10, b=30, l=40, r=10),
        xaxis_title="Time (s)",
        yaxis_title="PID Output",
        template="plotly_white"
    )
    return fig
