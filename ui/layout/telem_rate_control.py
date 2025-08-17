from dash import html
import dash_bootstrap_components as dbc

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