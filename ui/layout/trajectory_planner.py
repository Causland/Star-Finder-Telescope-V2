import dash_bootstrap_components as dbc
from dash import html

def trajectory_planner_block():
    return dbc.Card(
        [
            dbc.CardHeader("Trajectory Planner"),
            dbc.CardBody(
                [
                    dbc.ButtonGroup(
                        [
                            dbc.Button("Search", id="btn-search", color="primary"),
                            dbc.Button("Add", id="btn-add", color="success"),
                            dbc.Button("Remove", id="btn-remove", color="danger"),
                            dbc.Button("Edit", id="btn-edit", color="warning"),
                        ],
                        className="mb-3"
                    ),
                    dbc.Textarea(id="planner-list", placeholder="Planned targets...", style={"height": "180px"})
                ]
            )
        ],
        className="mb-3"
    )