import threading
from collections import defaultdict, deque

import paho.mqtt.client as mqtt
from flask import Flask, jsonify
from flask_socketio import SocketIO
import json

# ── config ────────────────────────────────────────────────────
MQTT_BROKER = "localhost"
MQTT_PORT   = 1883
TOPICS      = [
    "iot/sensors/bme680",
    "iot/sensors/ultrasonic",
    "iot/sensors/pms",
    "iot/sensors/rfid",
]

app = Flask(__name__, static_folder="static", static_url_path="")
sio = SocketIO(app, cors_allowed_origins="*", async_mode="threading")

history: dict[str, deque] = defaultdict(lambda: deque(maxlen=200))

# ── MQTT ──────────────────────────────────────────────────────
def on_connect(client, userdata, flags, reason_code, properties=None):
    print(f"[MQTT] Connected")
    for t in TOPICS:
        client.subscribe(t)
        print(f"[MQTT] Subscribed to {t}")

def on_message(client, userdata, msg):
    try:
        data = json.loads(msg.payload.decode())
    except Exception as e:
        print(f"[MQTT] Parse error: {e}")
        return

    sensor = msg.topic.split("/")[-1]
    data["sensor"] = sensor

    history[sensor].append(data)
    sio.emit("update", data)
    print(f"[MQTT] {sensor}: {data}")

def mqtt_thread():
    client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
    client.on_connect = on_connect
    client.on_message = on_message
    while True:
        try:
            client.connect(MQTT_BROKER, MQTT_PORT)
            client.loop_forever()
        except Exception as e:
            print(f"[MQTT] Error: {e} — retrying in 5s")
            import time; time.sleep(5)

# ── HTTP ──────────────────────────────────────────────────────
@app.route("/")
def index():
    return app.send_static_file("dashboard.html")

@app.route("/history/<sensor>")
def get_history(sensor):
    return jsonify({"data": list(history.get(sensor, []))})

# ── main ──────────────────────────────────────────────────────
if __name__ == "__main__":
    threading.Thread(target=mqtt_thread, daemon=True).start()
    print("[*] Server running on http://192.168.4.1:5000")
    sio.run(app, host="0.0.0.0", port=5000, debug=False, allow_unsafe_werkzeug=True)