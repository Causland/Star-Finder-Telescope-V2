import dash_bootstrap_components as dbc

def trajectory_planner_block() -> dbc.Card:
    return dbc.Card(
        [
            dbc.CardHeader("Trajectory Planner"),
            dbc.CardBody(
                [
                    dbc.ButtonGroup(
                        [
                            dbc.Button("Search", id="btn-search"),
                            dbc.Button("Add", id="btn-add"),
                            dbc.Button("Remove", id="btn-remove"),
                            dbc.Button("Edit", id="btn-edit"),
                        ],
                        className="mb-3"
                    ),
                    dbc.Textarea(id="planner-list", placeholder="Planned targets...", style={"height": "180px"})
                ]
            )
        ],
        className="mb-3"
    )