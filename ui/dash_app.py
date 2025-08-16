import dash
import dash_bootstrap_components as dbc
from layout.layout import layout
from services.network_constants import DASH_WEB_PORT

dash_app = dash.Dash(
    __name__,
    external_stylesheets=[dbc.themes.COSMO],
    suppress_callback_exceptions=True,
    title="Star Finder Telescope"
)

dash_app.layout = layout

if __name__ == "__main__":
    dash_app.run(debug=True, port=DASH_WEB_PORT)
