import dash_bootstrap_components as dbc
from dash import html, dcc, callback, Input, Output

RED_DOT_STYLE = {
    "width": "10px",
    "height": "10px",
    "borderRadius": "50%",
    "background": "#dc3545",  # Bootstrap red
    "display": "inline-block",
    "marginRight": "8px"
}

GREEN_DOT_STYLE = {
    "width": "10px",
    "height": "10px",
    "borderRadius": "50%",
    "background": "#198754",  # Bootstrap green
    "display": "inline-block",
    "marginRight": "8px"
}

def status_block():
    return dbc.Card(
        [
            dbc.CardHeader("Status"),
            dbc.CardBody(
                [
                    dbc.ListGroup(
                        [
                            dbc.ListGroupItem(
                                [
                                    html.Span("Uptime", className="text-muted"),
                                    html.Span(id="status-uptime", className="fw-bold")
                                ],
                                className="d-flex justify-content-between"
                            ),
                            dbc.ListGroupItem(
                                [
                                    html.Span("Version #", className="text-muted"),
                                    html.Span(id="status-version", className="fw-bold")
                                ],
                                className="d-flex justify-content-between"
                            ),
                            dbc.ListGroupItem(
                                [
                                    html.Span("Free Heap", className="text-muted"),
                                    html.Span(id="status-free-heap", className="fw-bold")
                                ],
                                className="d-flex justify-content-between"
                            ),
                            dbc.ListGroupItem(
                                [
                                    html.Span("Commands Received", className="text-muted"),
                                    html.Span(id="status-messages", className="fw-bold")
                                ],
                                className="d-flex justify-content-between"
                            ),
                            dbc.ListGroupItem(
                                [
                                    html.Span("GPS Latitude", className="text-muted"),
                                    html.Span(id="status-gpslat", className="fw-bold")
                                ],
                                className="d-flex justify-content-between"
                            ),
                            dbc.ListGroupItem(
                                [
                                    html.Span("GPS Longitude", className="text-muted"),
                                    html.Span(id="status-gpslon", className="fw-bold")
                                ],
                                className="d-flex justify-content-between"
                            ),
                            dbc.ListGroupItem(
                                [
                                    html.Span("GPS Altitude", className="text-muted"),
                                    html.Span(id="status-gpsalt", className="fw-bold")
                                ],
                                className="d-flex justify-content-between"
                            ),
                        ],
                        flush=True
                    ),
                    html.Div(
                        [
                            html.Span(id="telemetry-indicator-dot", style=RED_DOT_STYLE),
                            html.Span(id="telemetry-indicator-text", children="Telemetry: inactive", className="text-muted"),
                            dcc.Interval(id="telemetry-indicator-watchdog", interval=1000, n_intervals=0)
                        ],
                        className="mt-3"
                    )
                ]
            )
        ],
        className="mb-3"
    )

@callback(
    Output("telemetry-indicator-dot", "style"),
    Output("telemetry-indicator-text", "children"),
    Output("telemetry-indicator-text", "className"),
    Input("telemetry-indicator-watchdog", "n_intervals"),
    prevent_initial_call=True
)
def update_indicator(n_intervals):
    if n_intervals > 5:
        return RED_DOT_STYLE, "Telemetry: inactive", "text-danger"
    else:
        return GREEN_DOT_STYLE, "Telemetry: active", "text-success"
