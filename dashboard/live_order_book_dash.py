import dash
from dash import dcc, html, dash_table
from dash.dependencies import Output, Input
import websocket
import json
import threading
import time

# Shared state for order book data
order_book = {
    "bids": [],
    "asks": []
}

# WebSocket client logic
def websocket_listener():
    while True:
        try:
            ws = websocket.create_connection("ws://localhost:9001")
            print("Connected to WebSocket server")

            while True:
                # Send "snapshot" request every second
                ws.send("snapshot")
                data = ws.recv()
                book = json.loads(data)
                order_book["bids"] = sorted(book.get("bids", []), key=lambda x: -x["price"])[:10]  # Top 10 bids
                order_book["asks"] = sorted(book.get("asks", []), key=lambda x: x["price"])[:10]   # Top 10 asks
                time.sleep(1)

        except Exception as e:
            print(f"WebSocket error: {e}")
            time.sleep(2)  # Retry after delay

# Start WebSocket client in background thread
threading.Thread(target=websocket_listener, daemon=True).start()

# Dash app
app = dash.Dash(__name__)
app.title = "Live Order Book"

app.layout = html.Div([
    html.H1("Live Order Book", style={'textAlign': 'center'}),
    html.Div([
        html.Div([
            html.H2("Bids"),
            dash_table.DataTable(
                id='bids-table',
                columns=[
                    {"name": "Price", "id": "price"},
                    {"name": "Quantity", "id": "quantity"},
                    {"name": "ID", "id": "id"},
                ],
                style_table={'height': '400px', 'overflowY': 'auto'},
                style_cell={'textAlign': 'center'},
            )
        ], className="six columns"),

        html.Div([
            html.H2("Asks"),
            dash_table.DataTable(
                id='asks-table',
                columns=[
                    {"name": "Price", "id": "price"},
                    {"name": "Quantity", "id": "quantity"},
                    {"name": "ID", "id": "id"},
                ],
                style_table={'height': '400px', 'overflowY': 'auto'},
                style_cell={'textAlign': 'center'},
            )
        ], className="six columns"),
    ], className="row"),

    dcc.Interval(id="interval-component", interval=1000, n_intervals=0)
])

@app.callback(
    Output("bids-table", "data"),
    Output("asks-table", "data"),
    Input("interval-component", "n_intervals")
)
def update_tables(n):
    return order_book["bids"], order_book["asks"]

if __name__ == "__main__":
    app.run(debug=True)
