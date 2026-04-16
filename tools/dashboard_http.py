"""
IoT Systems Design - Lab Dashboard (Wi-Fi / HTTP Version)
Application and Service Domain (ASD) Server
"""

from flask import Flask, render_template_string, request, jsonify
import requests
import logging

app = Flask(__name__)
log = logging.getLogger('werkzeug')
log.setLevel(logging.ERROR)

# --- Network Configuration ---
# replace this with the IPv4 address their ESP32 gets from the Wi-Fi router
ESP32_IP = "192.168.1.100"  

def get_sensor_data():
    """Sensing Capability: Polls the ESP32 via HTTP GET."""
    try:
        # Assuming the ESP32 serves JSON at this endpoint
        response = requests.get(f"http://{ESP32_IP}/api/sensor", timeout=2)
        if response.status_code == 200:
            return response.json()
        return {"error": f"HTTP {response.status_code}"}
    except requests.exceptions.RequestException:
        return {"error": "Node Unreachable"}

def control_light(state):
    """Actuating Capability: Sends command to ESP32 via HTTP POST."""
    try:
        payload = {"state": state}
        response = requests.post(f"http://{ESP32_IP}/api/control", json=payload, timeout=2)
        return response.status_code == 200
    except requests.exceptions.RequestException:
        return False

# --- Frontend User Domain (UD) ---
HTML_TEMPLATE = '''
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>IoT Lab: Wi-Fi Dashboard</title>
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <style>
        body { font-family: system-ui, sans-serif; background-color: #f4f4f9; padding: 2rem; max-width: 800px; margin: 0 auto; color: #333; }
        .card { background: white; padding: 1.5rem; border-radius: 8px; box-shadow: 0 4px 6px rgba(0,0,0,0.1); margin-bottom: 1.5rem; }
        .btn-group { display: flex; gap: 1rem; margin-top: 1rem; }
        .btn { flex: 1; padding: 1rem; font-size: 1rem; border: none; border-radius: 4px; cursor: pointer; color: white; font-weight: bold; }
        .btn-on { background-color: #28a745; }
        .btn-off { background-color: #dc3545; }
        .status { font-family: monospace; font-size: 0.9rem; color: #666; }
    </style>
</head>
<body>
    <h2>IoT Systems Design Lab: Minimal Implementation</h2>
    <p class="status">Target Node: {{ esp_ip }} (Wi-Fi/HTTP)</p>

    <div class="card">
        <h3>Sensing Capability (Live Telemetry)</h3>
        <canvas id="telemetryChart" height="100"></canvas>
        <p class="status" id="conn-status">Waiting for data...</p>
    </div>

    <div class="card">
        <h3>Actuating Capability (LED Control)</h3>
        <div class="btn-group">
            <button class="btn btn-on" onclick="controlLight(1)">Turn ON</button>
            <button class="btn btn-off" onclick="controlLight(0)">Turn OFF</button>
        </div>
    </div>

    <script>
        // Initialize Chart.js
        const ctx = document.getElementById('telemetryChart').getContext('2d');
        const chart = new Chart(ctx, {
            type: 'line',
            data: {
                labels: [],
                datasets: [{
                    label: 'Temperature (°C)',
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
            if (chart.data.labels.length > 20) { // Keep last 20 data points
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
                        status.innerText = "Error: " + data.error;
                        status.style.color = "red";
                    } else if (data.temperature) {
                        status.innerText = "Connected. Live data stream active.";
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
                if(data.status !== 'ok') alert("Failed to route command to ESP32.");
            });
        }

        // Poll every 1.5 seconds
        setInterval(fetchSensorData, 1500);
    </script>
</body>
</html>
'''

# --- Flask Routes ---
@app.route('/')
def index():
    return render_template_string(HTML_TEMPLATE, esp_ip=ESP32_IP)

@app.route('/api/sensor', methods=['GET'])
def api_sensor():
    return jsonify(get_sensor_data())

@app.route('/api/control', methods=['POST'])
def api_control():
    state = request.json.get('state', 0)
    if control_light(state):
        return jsonify({'status': 'ok'}), 200
    return jsonify({'status': 'error'}), 502

if __name__ == '__main__':
    print(f"[*] Dashboard running. Target ESP32 IP: {ESP32_IP}")
    app.run(host='0.0.0.0', port=5000)