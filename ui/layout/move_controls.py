from dash import html
import dash_bootstrap_components as dbc

def move_controls_block():
    return dbc.Card(
        [
            dbc.CardHeader("Move Controls"),
            dbc.CardBody(
                [
                    html.Div(
                        [
                            html.Div(), dbc.Button("↑", id="btn-move-up"), html.Div(),
                            dbc.Button("←", id="btn-move-left"), html.Div(), dbc.Button("→", id="btn-move-right"),
                            html.Div(), dbc.Button("↓", id="btn-move-down"), html.Div(),
                        ],
                        className="d-grid gap-2",
                        style={"gridTemplateColumns": "1fr 1fr 1fr",
                               "marginLeft": "20%",
                               "marginRight": "20%"}
                    ),
                    dbc.Input(id="move-degrees", type="number", placeholder="Degrees", className="mt-3"),
                ]
            )
        ],
        className="mb-3"
    )
