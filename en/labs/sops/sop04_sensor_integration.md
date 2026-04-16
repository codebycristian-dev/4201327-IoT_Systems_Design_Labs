# SOP-04: Sensor Integration and Dashboards

> **Main Lab Guide:** [Lab 4: Reliability & Downlink Control](../lab4.md)
> **ISO Domains:** SCD (Sensing & Controlling), ASD (Application & Service)
> **GreenField Context:** Implementing reliable valve control for irrigation system

## Objectives
- Integrate a physical sensor (I2C) or structured mock into the Thread network.
- Expose readings via CoAP JSON.
- Manage energy (sleep between samples) and measure duty cycle.
- Build a minimal dashboard (Flask or chosen stack) for reading + `/light` control.

## Context
This implementation guide provides step-by-step technical instructions for sensor integration and dashboard creation. It complements the [main lab guide](../lab4.md) which covers the reliable downlink problem (sleeping devices) and poll period trade-offs.

## Pedagogical Order
1. Driver / mock sensor.
2. Stable `/sensor` JSON endpoint.
3. Basic PM (sleep) and timestamp logs.
4. Collector service (polling or existing observe) → store history.
5. Dashboard: table + latest value + toggle.

## Project Setup

### 1. Continue with the Lab 3 Project

This lab continues developing the project started in Lab 3. Make sure you have the `lab03` project (or equivalent) open in VS Code.

### 2. Add Base CoAP Code + Physical/Mock Structured Sensor + Basic PM

**CoAP base same as Lab 3**, then add structured sensor and power management.

**Add sensor structures** in `main/coap_demo.c` (after defines):
```c
typedef struct {
    float temperature;
    float humidity;
    uint32_t timestamp;
    uint32_t sequence;
} sensor_data_t;

static sensor_data_t current_sensor_data = {0};
```

**Mock sensor function** (simulates I2C, replace with real driver if physical sensor available):
```c
static void read_sensor_mock(sensor_data_t *data) {
    // Simulate I2C sensor reading (e.g.: SHT30)
    data->temperature = 20.0f + (esp_random() % 100) / 10.0f; // 20-30°C
    data->humidity = 40.0f + (esp_random() % 400) / 10.0f;    // 40-80%
    data->timestamp = esp_log_timestamp();
    data->sequence++;
}
```

**Modify `handle_sensor`** to use structured data:
```c
static void handle_sensor(coap_context_t *ctx, coap_resource_t *resource,
                           coap_session_t *session, coap_pdu_t *request,
                           coap_binary_t *token, coap_string_t *query,
                           coap_pdu_t *response)
{
    char payload[128];
    size_t len;

    if (request->code != COAP_REQUEST_GET) {
        response->code = COAP_RESPONSE_CODE_METHOD_NOT_ALLOWED;
        return;
    }

    // Read sensor before responding
    read_sensor_mock(&current_sensor_data);

    len = snprintf(payload, sizeof(payload),
                    "{\"temp\":%.1f,\"hum\":%.1f,\"ts\":%u,\"seq\":%u}",
                    current_sensor_data.temperature,
                    current_sensor_data.humidity,
                    current_sensor_data.timestamp,
                    current_sensor_data.sequence);

    response->code = COAP_RESPONSE_CODE_CONTENT;
    coap_add_data_blocked_response(resource, session, request, response,
                                    token, COAP_MEDIATYPE_APPLICATION_JSON, 0,
                                    len, (const uint8_t *)payload);
}
```

**Add PM task** in `main/coap_demo.c`:
```c
static void sensor_pm_task(void *pvParameters)
{
    while (1) {
        ESP_LOGI(TAG, "Sensor reading active");
        read_sensor_mock(&current_sensor_data);

        // Sleep between samples (30s for demo, adjust according to duty cycle)
        ESP_LOGI(TAG, "Entering light sleep for 30s");
        esp_sleep_enable_timer_wakeup(30 * 1000000); // 30s
        esp_light_sleep_start();

        ESP_LOGI(TAG, "Woke up from sleep");
    }
}

void start_sensor_pm(void)
{
    xTaskCreate(sensor_pm_task, "sensor_pm", 2048, NULL, 4, NULL);
}
```

**Call `start_sensor_pm()`** in `main/main.c` after `start_coap_server()`.

### 3. Configure Settings for PM

**Update `sdkconfig`** (add to base configurations):
```bash
# Power Management
CONFIG_PM_ENABLE=y
CONFIG_PM_DFS_INIT_AUTO=y
CONFIG_PM_LIGHT_SLEEP=y

# I2C (if using real sensor)
CONFIG_I2C_ENABLED=y
```

**Build and flash:**
```bash
idf.py set-target esp32c6
idf.py build
idf.py flash
idf.py monitor
```

### 4. Build Minimal Dashboard

**Create simple dashboard with Flask** (on the host PC):

**Install dependencies:**
```bash
pip install flask requests
```

**Use the dashboard from `tools/dashboard_http.py`** (already included in the repository).

**Configure the node IP** in `tools/dashboard_http.py`:
```python
NODE_IP = "fd11:22:33:0:0:0:0:1"  # Replace with actual node IP
```

**Run dashboard:**
```bash
python tools/dashboard_http.py
```

Open http://localhost:5000 in browser to see dashboard with real-time data and light control.

## Deliverables
- Functional `/sensor` JSON endpoint with structured data (temp, hum, ts, seq)
- Power management logs (sleep/wake cycles)
- Functional web dashboard with sensor polling and light control
- Duty cycle measurements and estimated consumption
