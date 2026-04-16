# SOP-07: Telemetry & Dashboards

> **Main Lab Guide:** [Lab 7: Operations & Observability](../lab7.md)
> **ISO Domains:** OMD (Operations & Management Domain), UD (User Domain)
> **GreenField Context:** Detecting "Silent Failures" before crops die

## Objectives
Implement the **Operations & Management Domain (OMD)** by reporting device health (Battery, RSSI, Uptime). Visualize this data to detect "Silent Failures."

## Code Implementation

### 2.1 The "Piggyback" Strategy (Optimization)
Instead of sending a separate packet for health (wasting energy), add health data to the existing sensor packet.

**Modify `hnd_temp_get` in `coap_demo.c`:**
```c
// Initialize CBOR Map
CborEncoder map;
cbor_encoder_create_map(&encoder, &map, CborIndefiniteLength);

// Inside the CBOR map creation...
cbor_encode_text_stringz(&map, "t");
cbor_encode_float(&map, current_temp);

// ADD TELEMETRY HERE:
cbor_encode_text_stringz(&map, "rssi");
cbor_encode_int(&map, otPlatRadioGetRssi(instance)); 

cbor_encode_text_stringz(&map, "batt");
cbor_encode_int(&map, 3100); // Mock battery in mV (or read ADC)

cbor_encode_text_stringz(&map, "up");
cbor_encode_int(&map, esp_timer_get_time() / 1000000); // Uptime in sec

cbor_encoder_close_container(&encoder, &map);
```

### 2.2 The Diagnostic Script (Python Dashboard)
Use the provided script in `tools/dashboard_http.py`. This acts as the "Cloud Server."

**Setup:**
1. Create a virtual environment (recommended):
   ```bash
   python -m venv .venv
   source .venv/bin/activate  # Linux/Mac
   # .venv\Scripts\activate   # Windows
   ```
2. Install dependencies:
   ```bash
   pip install aiocoap cbor2 matplotlib
   ```

**Usage:**
```bash
python tools/dashboard_http.py
```
*Note: Ensure you update the IPv6 address in the script to match your device.*

## Verification

1. Walk Test: Run the Python script. Pick up the sensor node and walk away from the Border Router.

2. Observation: Watch the rssi value drop in real-time on your screen.

3. Fail Condition: Determine at what RSSI the "Observe" stream stops updating.
