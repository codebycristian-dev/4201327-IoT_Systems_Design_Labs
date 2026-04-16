"""
IoT Systems Design - Lab Dashboard (MQTT Version)
Application and Service Domain (ASD) Server
Subscribes to ESP32 telemetry via MQTT, publishes control commands.
"""

from flask import Flask, render_template_string, request, jsonify
import paho.mqtt.client as mqtt
import json
import threading
import logging

app = Flask(__name__)
log = logging.getLogger('werkzeug')
log.setLevel(logging.ERROR)

# --- MQTT Configuration ---
MQTT_BROKER = "localhost"
MQTT_PORT = 1883
TOPIC_SENSOR = "iot/sensor"
TOPIC_CONTROL = "iot/control"

# Shared state: latest sensor reading (protected by lock)
latest_sensor_data = {"error": "Waiting for first message..."}
sensor_lock = threading.Lock()

# --- MQTT Client Setup ---

def on_connect(client, userdata, flags, reason_code, properties):
    print(f"[MQTT] Connected to broker (rc={reason_code})")
    client.subscribe(TOPIC_SENSOR, qos=0)
    print(f"[MQTT] Subscribed to: {TOPIC_SENSOR}")

def on_message(client, userdata, msg):
    global latest_sensor_data
    try:
        data = json.loads(msg.payload.decode())
        with sensor_lock:
            latest_sensor_data = data
    except json.JSONDecodeError:
        with sensor_lock:
            latest_sensor_data = {"raw": msg.payload.decode()}

mqtt_client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
mqtt_client.on_connect = on_connect
mqtt_client.on_message = on_message

# --- Frontend User Domain (UD) ---
HTML_TEMPLATE = '''
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>IoT Lab: MQTT Dashboard</title>
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <style>
        body { font-family: system-ui, sans-serif; background-color: #f4f4f9; padding: 2rem; max-width: 800px; margin: 0 auto; color: #333; }
        .card { background: white; padding: 1.5rem; border-radius: 8px; box-shadow: 0 4px 6px rgba(0,0,0,0.1); margin-bottom: 1.5rem; }
        .btn-group { display: flex; gap: 1rem; margin-top: 1rem; }
        .btn { flex: 1; padding: 1rem; font-size: 1rem; border: none; border-radius: 4px; cursor: pointer; color: white; font-weight: bold; }
        .btn-on { background-color: #28a745; }
        .btn-off { background-color: #dc3545; }
        .status { font-family: monospace; font-size: 0.9rem; color: #666; }
        .badge { background: #e9ecef; padding: 0.25rem 0.5rem; border-radius: 4px; font-size: 0.85rem; font-family: monospace; }
        .proto-note { background: #fff3cd; padding: 0.75rem 1rem; border-radius: 4px; font-size: 0.85rem; border-left: 4px solid #ffc107; margin-bottom: 1.5rem; }
    </style>
</head>
<body>
    <h2>IoT Systems Design Lab: MQTT Implementation</h2>
    <p class="status">Broker: <span class="badge">{{ broker }}:{{ port }}</span> (MQTT/TCP)</p>

    <div class="proto-note">
        <strong>Protocol:</strong> Unlike the HTTP dashboard, this page does not poll the ESP32 directly.
        Data arrives via MQTT broker &mdash; the ESP32 <em>pushes</em> readings to <code>{{ topic_sensor }}</code>,
        and this dashboard subscribes to that topic.
    </div>

    <div class="card">
        <h3>Sensing Capability (MQTT SUB: {{ topic_sensor }})</h3>
        <canvas id="telemetryChart" height="100"></canvas>
        <p class="status" id="conn-status">Waiting for data from broker...</p>
    </div>

    <div class="card">
        <h3>Actuating Capability (MQTT PUB: {{ topic_control }})</h3>
        <div class="btn-group">
            <button class="btn btn-on" onclick="controlLight(1)">Turn ON</button>
            <button class="btn btn-off" onclick="controlLight(0)">Turn OFF</button>
        </div>
    </div>

    <script>
        const ctx = document.getElementById('telemetryChart').getContext('2d');
        const chart = new Chart(ctx, {
            type: 'line',
            data: {
                labels: [],
                datasets: [{
                    label: 'Temperature (C)',
                    borderColor: '#0056b3',
                    data: [],
                    fill: false,
                    tension: 0.1
                }]
            },
            options: { animation: false, scales: { y: { beginAtZero: false } } }
        });

        function updateChart(temp) {
            const time = new Date().toLocaleTimeString();
            chart.data.labels.push(time);
            chart.data.datasets[0].data.push(temp);
            if (chart.data.labels.length > 20) {
                chart.data.labels.shift();
                chart.data.datasets[0].data.shift();
            }
            chart.update();
        }

        function fetchSensorData() {
            fetch('/api/sensor')
                .then(res => res.json())
                .then(data => {
                    const status = document.getElementById('conn-status');
                    if (data.error) {
                        status.innerText = "Waiting: " + data.error;
                        status.style.color = "orange";
                    } else if (data.temperature !== undefined) {
                        status.innerText = "Receiving MQTT messages. Live data stream active.";
                        status.style.color = "green";
                        updateChart(data.temperature);
                    }
                });
        }

        function controlLight(state) {
            fetch('/api/control', {
                method: 'POST',
                headers: {'Content-Type': 'application/json'},
                body: JSON.stringify({state: state})
            }).then(res => res.json()).then(data => {
                if(data.status !== 'ok') alert("Failed to publish MQTT command.");
            });
        }

        // Poll the Flask backend for the latest MQTT message
        // (The Flask backend holds the latest value received from the broker)
        setInterval(fetchSensorData, 1500);
    </script>
</body>
</html>
'''

# --- Flask Routes ---
@app.route('/')
def index():
    return render_template_string(HTML_TEMPLATE,
        broker=MQTT_BROKER, port=MQTT_PORT,
        topic_sensor=TOPIC_SENSOR, topic_control=TOPIC_CONTROL)

@app.route('/api/sensor', methods=['GET'])
def api_sensor():
    with sensor_lock:
        return jsonify(latest_sensor_data)

@app.route('/api/control', methods=['POST'])
def api_control():
    state = request.json.get('state', 0)
    payload = json.dumps({"state": state})
    result = mqtt_client.publish(TOPIC_CONTROL, payload, qos=1)
    if result.rc == mqtt.MQTT_ERR_SUCCESS:
        return jsonify({'status': 'ok'}), 200
    return jsonify({'status': 'error', 'message': 'MQTT publish failed'}), 502

if __name__ == '__main__':
    # Connect MQTT client in a background thread
    mqtt_client.connect(MQTT_BROKER, MQTT_PORT, keepalive=60)
    mqtt_client.loop_start()

    print(f"[*] MQTT Dashboard running.")
    print(f"[*] Broker: {MQTT_BROKER}:{MQTT_PORT}")
    print(f"[*] Subscribed to: {TOPIC_SENSOR}")
    print(f"[*] Publishing control to: {TOPIC_CONTROL}")
    app.run(host='0.0.0.0', port=5000)
