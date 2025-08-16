import dash_bootstrap_components as dbc
from dash import html

def camera_controls_block():
    return dbc.Card(
        [
            dbc.CardHeader("Camera Controls"),
            dbc.CardBody(
                [
                    dbc.InputGroup(
                        [
                            dbc.Button("←", id="btn-cam-left"),
                            dbc.Button("→", id="btn-cam-right"),
                            dbc.Input(id="camera-degrees", type="number", placeholder="Degrees"),
                        ],
                        className="mb-3"
                    ),
                    dbc.Button("Take Photo", id="btn-take-photo", color="info"),
                    html.Div(
                        html.Img(id="camera-image", src=None, style={"width": "100%", "objectFit": "contain"}),
                        id="camera-image-box",
                        className="mt-3",
                        style={"height": "220px", "background": "#343a40", "border": "1px dashed #6c757d"}
                    )
                ]
            )
        ],
        className="mb-3"
    )
