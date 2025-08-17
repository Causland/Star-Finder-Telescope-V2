from dash import html, dcc
import dash_bootstrap_components as dbc

def ota_update_block() -> dbc.Card:
    return dbc.Card(
        [
            dbc.CardHeader("OTA Update"),
            dbc.CardBody(
                [
                    html.Div(
                        [
                            dcc.Upload(id="ota-update-file",
                                       children=dbc.InputGroup(
                                            [
                                                dbc.Input(id="ota-update-file-name", type="text", placeholder="Path to update file"),
                                                dbc.Button("Set File", id="ota-update-set-btn"),
                                            ],
                                            className="gap-3"
                                        ),
                                       multiple=False,
                                       className="mb-4"),
                            dbc.Button("Start Update", id="start-ota-btn")
                        ]
                    )
                ]
            )
        ],
        className="mb-3"
    )    

