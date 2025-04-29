import dash
from dash import dcc, html, dash_table
from dash.dependencies import Output, Input
import websocket
import json
import threading
import time
from collections import defaultdict

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
                ws.send("snapshot")
                data = ws.recv()
                book = json.loads(data)
                order_book["bids"] = sorted(book.get("bids", []), key=lambda x: -x["price"])
                order_book["asks"] = sorted(book.get("asks", []), key=lambda x: x["price"])
                time.sleep(1)

        except Exception as e:
            print(f"WebSocket error: {e}")
            time.sleep(2)

# Start WebSocket client in background thread
threading.Thread(target=websocket_listener, daemon=True).start()

# Dash app
app = dash.Dash(__name__)
app.title = "Live Order Book"

app.layout = html.Div([
    html.H1("Live Order Book", style={'textAlign': 'center'}),
    
    dcc.Tabs(id="tabs", value='level-1', children=[
        dcc.Tab(label='Level 1 (Top of Book)', value='level-1'),
        dcc.Tab(label='Level 2 (Market Depth)', value='level-2'),
        dcc.Tab(label='Level 3 (Full Orders)', value='level-3'),
    ]),
    
    html.Div(id='tabs-content'),
    
    dcc.Interval(id="interval-component", interval=1000, n_intervals=0)
])

@app.callback(
    Output('tabs-content', 'children'),
    Input('tabs', 'value'),
    Input('interval-component', 'n_intervals')
)
def render_content(selected_tab, n):
    bids = order_book["bids"]
    asks = order_book["asks"]

    if selected_tab == 'level-1':
        bids_display = bids[:1]
        asks_display = asks[:1]

    elif selected_tab == 'level-2':
        def aggregate_by_price(entries):
            summary = defaultdict(int)
            for entry in entries:
                summary[entry["price"]] += entry["quantity"]
            return [{"price": price, "quantity": qty} for price, qty in sorted(summary.items(), reverse=(entries == bids))]

        bids_display = aggregate_by_price(bids)[:10]
        asks_display = aggregate_by_price(asks)[:10]

    elif selected_tab == 'level-3':
        bids_display = bids
        asks_display = asks

    else:
        bids_display = []
        asks_display = []

    return html.Div([
        html.Div([
            html.H2("Bids"),
            dash_table.DataTable(
                columns=[
                    {"name": "Price", "id": "price"},
                    {"name": "Quantity", "id": "quantity"},
                ] if selected_tab != 'level-3' else [
                    {"name": "Price", "id": "price"},
                    {"name": "Quantity", "id": "quantity"},
                    {"name": "ID", "id": "id"},
                ],
                data=bids_display,
                style_table={'height': '400px', 'overflowY': 'auto'},
                style_cell={'textAlign': 'center'},
            )
        ], className="six columns"),

        html.Div([
            html.H2("Asks"),
            dash_table.DataTable(
                columns=[
                    {"name": "Price", "id": "price"},
                    {"name": "Quantity", "id": "quantity"},
                ] if selected_tab != 'level-3' else [
                    {"name": "Price", "id": "price"},
                    {"name": "Quantity", "id": "quantity"},
                    {"name": "ID", "id": "id"},
                ],
                data=asks_display,
                style_table={'height': '400px', 'overflowY': 'auto'},
                style_cell={'textAlign': 'center'},
            )
        ], className="six columns"),
    ], className="row")
if __name__ == "__main__":
    app.run(debug=True)
