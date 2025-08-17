from dash import html
import dash_bootstrap_components as dbc

def camera_controls_block() -> dbc.Card:
    return dbc.Card(
        [
            dbc.CardHeader("Camera Controls"),
            dbc.CardBody(
                [
                    html.Div(
                        [
                            dbc.InputGroup(
                                [
                                    dbc.Button("←", id="btn-cam-left", style={"width": "15%"}),
                                    dbc.Button("→", id="btn-cam-right", style={"width": "15%"}),
                                ],
                                className="gap-4 justify-content-center mb-1",
                            ),
                            dbc.Input(id="camera-degrees", type="number", placeholder="Degrees")
                        ],
                        className="mb-5"
                    ),
                    dbc.Button("Take Photo", id="btn-take-photo", className="mb-1"),
                    html.Div(
                        html.Img(id="camera-image", src=None, style={"width": "100%", "objectFit": "contain"}),
                        id="camera-image-box",
                        style={"height": "300px", "background": "#343a40", "border": "1px dashed #6c757d"}
                    )
                ]
            )
        ],
        className="mb-3"
    )
