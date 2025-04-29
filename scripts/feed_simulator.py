import socket
import time
import random
import uuid

HOST = "127.0.0.1"
PORT = 9000

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
s.listen(1)
print(f"Waiting for connection on {HOST}:{PORT}...")

conn, addr = s.accept()
print(f"Connected by {addr}")

# Keep track of added orders
added_order_ids = set()

while True:
    try:
        if conn.fileno() == -1:
            print("Connection closed. Waiting for new connection...")
            conn, addr = s.accept()
            print(f"Connected by {addr}")

        # Decide action
        if not added_order_ids:
            action = "ADD"
        else:
            action = random.choices(['ADD', 'MODIFY', 'CANCEL'], weights=[0.6, 0.3, 0.1])[0]

        if action == "ADD":
            order_id = str(uuid.uuid4())[:8]
            added_order_ids.add(order_id)
            price = round(random.uniform(100.00, 102.00), 2)
            size = random.randint(1, 10)
            side = random.choice(['B', 'S'])
            message = f"ADD,{side},{price},{size},{order_id}"

        elif action == "MODIFY":
            order_id = random.choice(list(added_order_ids))
            price = round(random.uniform(100.00, 102.00), 2)
            size = random.randint(1, 10)
            side = random.choice(['B', 'S'])
            message = f"MODIFY,{side},{price},{size},{order_id}"

        elif action == "CANCEL":
            order_id = random.choice(list(added_order_ids))
            price = round(random.uniform(100.00, 102.00), 2)
            side = random.choice(['B', 'S'])
            message = f"CANCEL,{side},{price},0,{order_id}"
            added_order_ids.remove(order_id)  # Remove after cancel

        # Send message
        conn.sendall((message + "\n").encode())
        print("Sent:", message)

        time.sleep(random.uniform(0.1, 0.5))

    except (BrokenPipeError, ConnectionResetError):
        print("Client disconnected. Waiting for new connection...")
        conn.close()
        conn, addr = s.accept()
        print(f"Connected by {addr}")

    except Exception as e:
        print(f"Unexpected error: {e}")
        conn.close()
        conn, addr = s.accept()
        print(f"Connected by {addr}")
