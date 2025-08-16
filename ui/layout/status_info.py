from dash import html, dcc, callback, Input, Output
import dash_bootstrap_components as dbc

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

GRAY_DOT_STYLE = {
    "width": "10px",
    "height": "10px",
    "borderRadius": "50%",
    "background": "#6c757d",  # Bootstrap gray
    "display": "inline-block",
    "marginRight": "8px"
}

def status_block() -> dbc.Card:
    return dbc.Card(
        [
            dbc.CardHeader("Status"),
            dbc.CardBody(
                [
                    html.Div(
                        [
                            html.Span(id="telemetry-indicator-dot", style=GRAY_DOT_STYLE),
                            html.Span(id="telemetry-indicator-text", children="Telemetry: waiting", className="text-muted"),
                            dcc.Interval(id="telemetry-indicator-watchdog", interval=1000, n_intervals=0, disabled=True)
                        ],
                        className="mb-4"
                    ),
                    dbc.ListGroup(
                        [
                            dbc.ListGroupItem(
                                [
                                    html.Span("Uptime"),
                                    html.Span(id="status-uptime", className="fw-bold")
                                ],
                                className="d-flex justify-content-between"
                            ),
                            dbc.ListGroupItem(
                                [
                                    html.Span("Version #"),
                                    html.Span(id="status-version", className="fw-bold")
                                ],
                                className="d-flex justify-content-between"
                            ),
                            dbc.ListGroupItem(
                                [
                                    html.Span("Free Heap"),
                                    html.Span(id="status-free-heap", className="fw-bold")
                                ],
                                className="d-flex justify-content-between"
                            ),
                            dbc.ListGroupItem(
                                [
                                    html.Span("Commands Received"),
                                    html.Span(id="status-messages", className="fw-bold")
                                ],
                                className="d-flex justify-content-between"
                            ),
                            dbc.ListGroupItem(
                                [
                                    html.Span("GPS Latitude"),
                                    html.Span(id="status-gpslat", className="fw-bold")
                                ],
                                className="d-flex justify-content-between"
                            ),
                            dbc.ListGroupItem(
                                [
                                    html.Span("GPS Longitude"),
                                    html.Span(id="status-gpslon", className="fw-bold")
                                ],
                                className="d-flex justify-content-between"
                            ),
                            dbc.ListGroupItem(
                                [
                                    html.Span("GPS Altitude"),
                                    html.Span(id="status-gpsalt", className="fw-bold")
                                ],
                                className="d-flex justify-content-between"
                            ),
                        ],
                        flush=True
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
