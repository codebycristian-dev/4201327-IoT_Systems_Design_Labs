# Course References

This document contains:
1. **Quick Reference Guides** - CoAP, Thread, CBOR, ESP-IDF cheat sheets
2. **Performance Baselines** - Expected metrics and troubleshooting for each lab

Bookmark this page for quick lookup during labs!

---

# Part 1: Quick Reference Guides

Cheat sheets for common tools and protocols used throughout the course.

---

## CoAP Quick Reference

### CoAP Message Types

| Type | Code | Description | Use Case |
|------|------|-------------|----------|
| **CON** | 0 | Confirmable | Reliable delivery required (actuator commands) |
| **NON** | 1 | Non-confirmable | Best-effort delivery (periodic telemetry) |
| **ACK** | 2 | Acknowledgment | Response to CON message |
| **RST** | 3 | Reset | Error, reject message |

### CoAP Methods

| Method | Code | Description | Idempotent? | Cacheable? |
|--------|------|-------------|-------------|------------|
| **GET** | 0.01 | Retrieve resource | ✅ Yes | ✅ Yes |
| **POST** | 0.02 | Create resource | ❌ No | ❌ No |
| **PUT** | 0.03 | Update/create resource | ✅ Yes | ❌ No |
| **DELETE** | 0.04 | Delete resource | ✅ Yes | ❌ No |

### CoAP Response Codes

| Code | Description | HTTP Equivalent |
|------|-------------|-----------------|
| **2.01** | Created | 201 Created |
| **2.02** | Deleted | 204 No Content |
| **2.03** | Valid | 304 Not Modified |
| **2.04** | Changed | 204 No Content |
| **2.05** | Content | 200 OK |
| **4.00** | Bad Request | 400 Bad Request |
| **4.04** | Not Found | 404 Not Found |
| **4.05** | Method Not Allowed | 405 Method Not Allowed |
| **5.00** | Internal Server Error | 500 Internal Server Error |

### CoAP Command Line (libcoap)

```bash
# GET request (CON)
coap-client -m get coap://[fd00::1234]/sensors/temp

# GET request (NON)
coap-client -m get -N coap://[fd00::1234]/sensors/temp

# PUT request
coap-client -m put coap://[fd00::1234]/actuators/led -e '{"state":"on"}'

# Observe resource (subscribe)
coap-client -m get -s 60 coap://[fd00::1234]/sensors/temp

# With DTLS (secure)
coap-client -m get coaps://[fd00::1234]/sensors/temp \
  -u username -k password

# Custom options
coap-client -m get coap://[fd00::1234]/sensors/temp \
  -O 6,0x01  # Accept: application/json
```

### CoAP URI Structure

```
coap://[host]:[port]/[path]?[query]

Example:
coap://[fd00::1234]:5683/sensors/temp?unit=celsius

Components:
- Scheme: coap:// or coaps:// (secure)
- Host: IPv6 address (in brackets)
- Port: 5683 (coap) or 5684 (coaps)
- Path: Resource identifier (URI-Path options)
- Query: Optional parameters (URI-Query options)
```

### Common Content-Format Values

| Code | Media Type | Description |
|------|------------|-------------|
| **0** | text/plain | Plain text |
| **40** | application/link-format | CoRE Link Format |
| **41** | application/xml | XML |
| **42** | application/octet-stream | Binary data |
| **47** | application/exi | Efficient XML |
| **50** | application/json | JSON |
| **60** | application/cbor | CBOR (binary JSON) |

---

## Thread CLI Quick Reference

### Network Formation

```bash
# Initialize Thread stack
> dataset init new
> dataset commit active
> ifconfig up
> thread start

# Check status
> state
leader  # or router, child, detached

# Get network info
> dataset active
Active Timestamp: 1
Channel: 15
Channel Mask: 0x07fff800
Ext PAN ID: 1122334455667788
Mesh Local Prefix: fd12:3456:7890::/64
Network Key: 00112233445566778899aabbccddeeff
Network Name: OpenThreadDemo
PAN ID: 0x1234
PSKc: 3aa55f91ca47d1e4e71a08cb35e91591
Security Policy: 672 onrc

# Join existing network
> dataset networkkey 00112233445566778899aabbccddeeff
> dataset panid 0x1234
> dataset channel 15
> dataset commit active
> ifconfig up
> thread start
```

### Network Management

```bash
# Show all routers
> router table
| ID | RLOC16 | Next Hop | Path Cost | LQ In | LQ Out | Age | Extended MAC     |
+----|--------|----------|-----------|-------|--------|-----|------------------+
| 1  | 0x0400 | 1        | 0         | 3     | 3      | 0   | 1234567890abcdef |

# Show all children
> child table
| ID  | RLOC16 | Timeout    | Age | LQ In | C_VN | R | D | N | Ver | CSL | Mode |
+-----+--------+------------+-----+-------+------+---+---+---+-----+-----+------+
|   1 | 0x0401 |        240 |  12 |     3 |  131 | 1 | 1 | 1 |   3 |   0 | rdn  |

# Get IPv6 addresses
> ipaddr
fd12:3456:7890::1
fe80::1234:5678:90ab:cdef

# Ping another device
> ping fd12:3456:7890::5678
16 bytes from fd12:3456:7890::5678: icmp_seq=1 hlim=64 time=24ms

# Set TX power
> txpower -10
Done
> txpower
-10 dBm

# Change channel
> channel 20
Done

# Get RSSI
> rssi
-45 dBm
```

### Diagnostics

```bash
# Show counters
> counters
mac
    TxTotal: 100
    TxUnicast: 80
    TxBroadcast: 20
    RxTotal: 150
    RxUnicast: 120
    RxBroadcast: 30
    TxErrCca: 5
    RxErrNoFrame: 2

# Reset counters
> counters reset

# Get link quality
> linkquality 1
Link Quality In: 3 (excellent)
Link Quality Out: 3 (excellent)

# Get network data
> netdata show
Prefixes:
  fd12:3456:7890::/64 paros med 0400
Routes:
  fd00::/64 s med 0400
Services:
```

### CoAP Server (Thread CLI)

```bash
# Start CoAP server
> coap start

# Register resource
> coap resource test-resource
Done

# Send CoAP response (when client requests)
> coap set testing123
Done

# Send CoAP GET
> coap get fd12:3456:7890::5678 test-resource
Received CoAP response: testing123
```

---

## ESP-IDF Quick Reference

### Project Structure

```
my_project/
├── CMakeLists.txt              # Top-level CMake config
├── sdkconfig                   # Project configuration
├── sdkconfig.defaults          # Default config overrides
├── main/
│   ├── CMakeLists.txt          # Main component CMake
│   ├── main.c                  # Application entry point
│   └── Kconfig.projbuild       # Project-specific config
├── components/                 # Custom components
│   └── my_component/
│       ├── CMakeLists.txt
│       ├── component.mk
│       ├── my_component.c
│       └── include/
│           └── my_component.h
└── build/                      # Build output (generated)
```

### Build & Flash Commands

```bash
# Configure project (interactive menu)
idf.py menuconfig

# Build project
idf.py build

# Flash to device
idf.py -p /dev/ttyUSB0 flash

# Monitor serial output
idf.py -p /dev/ttyUSB0 monitor

# Build + Flash + Monitor (all-in-one)
idf.py -p /dev/ttyUSB0 flash monitor

# Clean build
idf.py fullclean

# Erase flash
idf.py -p /dev/ttyUSB0 erase-flash

# Set target (ESP32-C6)
idf.py set-target esp32c6
```

### Common ESP-IDF APIs

#### Logging

```c
#include "esp_log.h"

static const char *TAG = "MY_APP";

ESP_LOGI(TAG, "Info message: %d", value);
ESP_LOGW(TAG, "Warning message");
ESP_LOGE(TAG, "Error message");
ESP_LOGD(TAG, "Debug message (only if log level >= DEBUG)");
ESP_LOGV(TAG, "Verbose message");

// Set log level
esp_log_level_set(TAG, ESP_LOG_DEBUG);
```

#### GPIO

```c
#include "driver/gpio.h"

// Configure GPIO as output
gpio_config_t io_conf = {
    .pin_bit_mask = (1ULL << GPIO_NUM_2),
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE,
};
gpio_config(&io_conf);

// Set GPIO level
gpio_set_level(GPIO_NUM_2, 1);  // High
gpio_set_level(GPIO_NUM_2, 0);  // Low

// Read GPIO
int level = gpio_get_level(GPIO_NUM_2);
```

#### FreeRTOS Tasks

```c
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void my_task(void *pvParameters) {
    while (1) {
        ESP_LOGI(TAG, "Task running");
        vTaskDelay(pdMS_TO_TICKS(1000));  // Delay 1 second
    }
}

// Create task
xTaskCreate(my_task, "my_task", 4096, NULL, 5, NULL);
//          function  name      stack  param priority handle
```

#### NVS (Non-Volatile Storage)

```c
#include "nvs_flash.h"
#include "nvs.h"

// Initialize NVS
esp_err_t ret = nvs_flash_init();
if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
}

// Open NVS namespace
nvs_handle_t my_handle;
nvs_open("storage", NVS_READWRITE, &my_handle);

// Write integer
int32_t value = 42;
nvs_set_i32(my_handle, "my_key", value);
nvs_commit(my_handle);

// Read integer
int32_t read_value;
nvs_get_i32(my_handle, "my_key", &read_value);

// Close
nvs_close(my_handle);
```

### OpenThread ESP-IDF Integration

#### Basic OpenThread Setup

```c
#include "esp_openthread.h"
#include "esp_openthread_netif_glue.h"
#include "openthread/instance.h"
#include "openthread/thread.h"

// Initialize OpenThread
esp_openthread_platform_config_t config = {
    .radio_config = ESP_OPENTHREAD_DEFAULT_RADIO_CONFIG(),
    .host_config = ESP_OPENTHREAD_DEFAULT_HOST_CONFIG(),
    .port_config = ESP_OPENTHREAD_DEFAULT_PORT_CONFIG(),
};

esp_netif_config_t cfg = ESP_NETIF_DEFAULT_OPENTHREAD();
esp_netif_t *openthread_netif = esp_netif_new(&cfg);

esp_openthread_init(&config);
esp_openthread_launch_mainloop();

// Get OpenThread instance
otInstance *instance = esp_openthread_get_instance();

// Start Thread network
otThreadSetEnabled(instance, true);
```

#### OpenThread CoAP Server

```c
#include "openthread/coap.h"

static void coap_request_handler(void *context, otMessage *message,
                                   const otMessageInfo *message_info) {
    otError error = OT_ERROR_NONE;
    otMessage *response_message = NULL;
    otCoapCode response_code = OT_COAP_CODE_CHANGED;

    // Create response
    response_message = otCoapNewMessage(context, NULL);
    if (response_message == NULL) {
        return;
    }

    otCoapMessageInit(response_message, OT_COAP_TYPE_ACKNOWLEDGMENT,
                      response_code);
    otCoapMessageSetToken(response_message, otCoapMessageGetToken(message),
                          otCoapMessageGetTokenLength(message));
    otCoapMessageSetMessageId(response_message,
                              otCoapMessageGetMessageId(message));

    // Set payload
    const char *payload = "{\"status\":\"ok\"}";
    otMessageAppend(response_message, payload, strlen(payload));

    // Send response
    otCoapSendResponse(context, response_message, message_info);
}

// Register CoAP resource
static otCoapResource resource = {
    .mUriPath = "sensors/temp",
    .mHandler = coap_request_handler,
    .mContext = NULL,
    .mNext = NULL,
};

otCoapAddResource(instance, &resource);
otCoapStart(instance, OT_DEFAULT_COAP_PORT);
```

---

## Common Troubleshooting

### Thread Network Issues

| Symptom | Possible Cause | Solution |
|---------|----------------|----------|
| **Device won't join network** | Wrong network key | Verify `dataset networkkey` matches |
| | Wrong PAN ID | Verify `dataset panid` matches |
| | Wrong channel | Verify `dataset channel` matches |
| | Commissioner not running | Enable commissioning on leader |
| **High packet loss** | Interference (WiFi on same channel) | Change Thread channel: `channel 20` |
| | Low RSSI (<-85 dBm) | Move devices closer or increase TX power |
| | Too many hops (>3) | Add more routers to reduce depth |
| **Slow network** | Too many devices on one router | Balance load, add more routers |
| | High latency mesh | Reduce hop count, optimize topology |
| **Partition** | RF interference blocking link | Improve placement, use different channel |
| | Router failure | Wait for partition merge (~60s) |

### CoAP Issues

| Symptom | Possible Cause | Solution |
|---------|----------------|----------|
| **Timeout** | Device unreachable | Check `ping` to verify connectivity |
| | CoAP server not running | Verify `coap start` on device |
| | Firewall blocking | Check Border Router firewall rules |
| **4.04 Not Found** | Wrong URI path | Verify resource path with `coap resource` |
| | Resource not registered | Check server registered resource |
| **5.00 Internal Error** | Server crash | Check device logs with `idf.py monitor` |
| | Out of memory | Reduce payload size, increase heap |

### ESP-IDF Build Issues

| Symptom | Possible Cause | Solution |
|---------|----------------|----------|
| **CMake Error** | Wrong ESP-IDF version | Check `idf.py --version`, install v5.1+ |
| | Missing dependencies | Run `idf.py install` |
| **Flash Failed** | Wrong USB port | Check `ls /dev/ttyUSB*` or Device Manager |
| | Insufficient permissions | Linux: `sudo usermod -a -G dialout $USER` |
| | Bootloader mode issue | Hold BOOT button while powering on |
| **Out of Memory** | Stack overflow | Increase task stack size in `xTaskCreate` |
| | Heap fragmentation | Use `heap_caps_get_free_size()` to monitor |

---

## Performance Targets (Quick Check)

| Metric | Target | Measurement Command |
|--------|--------|---------------------|
| **RSSI @ 10m** | > -70 dBm | `ot-cli> rssi` |
| **Packet Loss** | < 5% | Send 100 pings, count losses |
| **CoAP Latency (1 hop)** | < 100 ms | `time coap-client -m get ...` |
| **CoAP Latency (3 hops)** | < 300 ms | Measure end-to-end with timestamps |
| **Mesh Convergence** | < 60 s | Disconnect router, time until routes update |
| **OTA Update** | < 5 min | 500 KB firmware @ 250 kbps ≈ 16s + overhead |
| **Current (Active TX)** | ~30-50 mA | Measure with ammeter or power profiler |
| **Current (Deep Sleep)** | < 5 µA | Configure deep sleep, measure current |

---

## Useful Links

### Documentation
- **Thread Specification**: https://www.threadgroup.org/support#specifications
- **CoAP RFC 7252**: https://datatracker.ietf.org/doc/html/rfc7252
- **CoAP Observe RFC 7641**: https://datatracker.ietf.org/doc/html/rfc7641
- **ESP-IDF Programming Guide**: https://docs.espressif.com/projects/esp-idf/
- **ESP32-C6 Technical Reference**: https://www.espressif.com/sites/default/files/documentation/esp32-c6_technical_reference_manual_en.pdf
- **ISO/IEC 30141:2024**: (Course materials, official standard)

### Tools
- **Wireshark (802.15.4)**: https://www.wireshark.org/
- **libcoap**: https://github.com/obgm/libcoap
- **OpenThread**: https://github.com/openthread/openthread
- **ESP-IDF**: https://github.com/espressif/esp-idf

---

## Course Helper Scripts

The `tools/` directory contains Python utilities for testing and development. These are used in the implementation guides.

### Prerequisites

```bash
# Install Python dependencies
pip install aiocoap flask
```

### Available Tools

#### 1. CoAP Client (`tools/coap_client.py`)

Simple CoAP client for interacting with your Thread nodes.

```bash
# GET sensor data
python tools/coap_client.py --host fd00::1234 get /sensor

# PUT to control LED
python tools/coap_client.py --host fd00::1234 put /light 1

# Observe sensor (subscribe to updates)
python tools/coap_client.py --host fd00::1234 observe /sensor
```

**Used in**: Labs 4, 5, 8

---

#### 2. Dashboard Server (`tools/dashboard_http.py`)

Flask-based web dashboard for monitoring sensor data.

```bash
# Configure the node IP in the script first
python tools/dashboard_http.py
# Open http://localhost:5000 in your browser
```

**Features**:
- Real-time sensor polling
- LED control toggle
- Historical data display

**Used in**: Lab 4, 7

---

#### 3. OTA Server (`tools/ota_server.py`)

HTTP server for firmware over-the-air updates.

```bash
# Start OTA server
python tools/ota_server.py

# Server listens on port 8080 by default
# Place firmware.bin in the same directory
```

**Used in**: Lab 6

---

#### 4. End-to-End Test Suite (`tools/test_e2e.py`)

Automated testing script for system validation.

```bash
# Test single node
python tools/test_e2e.py fd00::1234

# Test with border router
python tools/test_e2e.py fd00::1234 fd00::100
```

**Features**:
- Connectivity tests
- Latency measurements
- Error injection (invalid endpoints, bad payloads)
- Success/failure metrics

**Used in**: Lab 8

---

**Pro Tips**:
1. **Save frequent commands**: Create shell aliases for common `idf.py` and `coap-client` commands
2. **Use tab completion**: Thread CLI supports tab completion for commands
3. **Log to file**: `idf.py monitor | tee log.txt` saves serial output
4. **Bookmark this page**: Print or keep open for quick reference during labs
# Performance Baselines and Troubleshooting Guide

## Purpose

This document provides:
1. **Performance baselines**: Expected metrics for each lab
2. **Measurement procedures**: How to verify your system meets targets
3. **Troubleshooting guide**: Common issues and solutions

---

## Lab 1: IEEE 802.15.4 Physical Layer

### Expected Performance

| Metric | Target | Typical Range | Measurement Method |
|--------|--------|---------------|---------------------|
| **RSSI @ 1m** | > -40 dBm | -30 to -50 dBm | `ot-cli> rssi` |
| **RSSI @ 10m** | > -70 dBm | -60 to -75 dBm | `ot-cli> rssi` |
| **RSSI @ 50m (outdoor)** | > -90 dBm | -85 to -95 dBm | `ot-cli> rssi` |
| **Packet Error Rate** | < 5% | 1-5% | Send 100 frames, count losses |
| **TX Power** | Configurable | -24 to +20 dBm | `ot-cli> txpower` |
| **Link Margin @ 10m** | > 20 dB | 20-40 dB | RSSI - Sensitivity (-100 dBm) |

### Measurement Procedures

#### 1. RSSI Measurement
```bash
# On Device A
> dataset init new
> dataset commit active
> ifconfig up
> thread start

# Wait until device becomes leader
> state
leader

# On Device B (at measured distance)
> dataset networkkey <same as Device A>
> dataset panid <same as Device A>
> dataset channel <same as Device A>
> dataset commit active
> ifconfig up
> thread start

# After joining network
> rssi
-65 dBm

# Get detailed neighbor info
> neighbor table
```

#### 2. Packet Error Rate (PER) Test
```bash
# On receiving device
> counters reset

# On sending device, send 100 pings
for i in {1..100}; do
  ot-cli> ping <receiver_ipv6_address>
  sleep 0.1
done

# On receiving device, check counters
> counters
RxTotal: 97
RxErrNoFrame: 3

PER = 3 / 100 = 3% ✅
```

### Troubleshooting

| Problem | Symptoms | Causes | Solutions |
|---------|----------|---------|-----------|
| **Low RSSI** | RSSI < -85 dBm at close range | Antenna disconnected, low TX power | Check antenna connection, increase TX power: `txpower 20` |
| **High PER** | >10% packet loss | Interference, obstacles | Change channel: `channel 20`, move closer, remove obstacles |
| **Cannot detect device** | No neighbor shown | Wrong PAN ID/network key | Verify `dataset` parameters match |
| **Intermittent connection** | RSSI fluctuates wildly | RF interference (WiFi, microwave) | Use spectrum analyzer, change channel |

### Performance Degradation Factors

```
Free-space RSSI: -65 dBm

With obstacles:
- Single drywall: -5 to -10 dB  → -70 to -75 dBm
- Concrete wall: -10 to -20 dB  → -75 to -85 dBm
- Metal barrier: -20 to -40 dB  → -85 to -105 dBm (likely no link)
- Human body: -3 to -5 dB       → -68 to -70 dBm

With interference:
- WiFi on same channel: +5 to +15 dB noise floor
- Bluetooth devices: +3 to +10 dB noise floor
- Microwave oven: +20 to +40 dB noise (severe)
```

---

## Lab 2: 6LoWPAN and IPv6

### Expected Performance

| Metric | Target | Typical Range | Measurement Method |
|--------|--------|---------------|---------------------|
| **Header Compression Ratio** | > 60% | 60-80% | Compare uncompressed vs compressed packet size |
| **IPv6 Ping Latency** | < 50 ms | 20-50 ms | `ping6` to neighbor |
| **Fragmentation Overhead** | < 10% | 5-10% | Calculate (fragment headers / total payload) |
| **MTU Efficiency** | > 70% | 70-85% | (Payload / Total packet size) |

### Measurement Procedures

#### 1. Header Compression Verification
```bash
# Capture traffic with Wireshark
# Filter: wpan

# Look for IPHC-compressed packets
# Frame format:
#   - Dispatch byte: 0x61 (IPHC compressed)
#   - IPHC encoding: 2-3 bytes
#   - Payload: variable

# Uncompressed IPv6 would be:
#   40 bytes (IPv6) + 8 bytes (UDP) = 48 bytes overhead

# IPHC compressed:
#   2-3 bytes (IPHC) + 4 bytes (UDP compressed) = 6-7 bytes

# Compression ratio = (48 - 7) / 48 = 85% ✅
```

#### 2. Ping Latency
```bash
# From one device to another on same Thread network
> ping fd12:3456:7890::5678
16 bytes from fd12:3456:7890::5678: icmp_seq=1 hlim=64 time=24ms
16 bytes from fd12:3456:7890::5678: icmp_seq=2 hlim=64 time=21ms
16 bytes from fd12:3456:7890::5678: icmp_seq=3 hlim=64 time=28ms

Average: 24ms ✅ (< 50ms target)
```

### Troubleshooting

| Problem | Symptoms | Causes | Solutions |
|---------|----------|---------|-----------|
| **Fragmentation** | Large packets split | Payload > 100 bytes | Reduce payload size, use CBOR instead of JSON |
| **Address not reachable** | Ping fails | Routing issue, device sleeping | Check `netdata show`, verify device is awake |
| **Duplicate Address** | IPv6 conflict | Two devices with same IID | Check `ipaddr`, verify unique MAC addresses |

---

## Lab 3: Thread Mesh Networking

### Expected Performance

| Metric | Target | Typical Range | Measurement Method |
|--------|--------|---------------|---------------------|
| **Mesh Convergence Time** | < 60 s | 30-60 s | Time from device join to routes established |
| **Latency (1 hop)** | < 50 ms | 20-50 ms | Ping neighboring router |
| **Latency (3 hops)** | < 150 ms | 100-200 ms | Ping device 3 hops away |
| **Partition Recovery Time** | < 120 s | 60-120 s | Time from link failure to partition merge |
| **Max Network Size** | 32 routers | 10-32 routers | Check `router table` |
| **Leader Election Time** | < 30 s | 10-30 s | Time from leader failure to new leader elected |

### Measurement Procedures

#### 1. Mesh Convergence Test
```bash
# Device A (already running as leader)
> state
leader

# Device B (new device joining)
> dataset networkkey <same>
> dataset commit active
> ifconfig up
> thread start

# Time how long until routing works
time_start=$(date +%s)

# Wait until device becomes router/child
while true; do
  state=$(ot-cli> state)
  if [[ "$state" == "router" ]] || [[ "$state" == "child" ]]; then
    break
  fi
  sleep 1
done

time_end=$(date +%s)
convergence_time=$((time_end - time_start))

echo "Convergence time: ${convergence_time}s"
# Target: < 60s
```

#### 2. Multi-Hop Latency Test
```bash
# Topology: A ← → B ← → C ← → D

# From Device A, ping Device D (3 hops)
> ping fd12:3456:7890::D
16 bytes from fd12:3456:7890::D: icmp_seq=1 time=142ms
16 bytes from fd12:3456:7890::D: icmp_seq=2 time=128ms
16 bytes from fd12:3456:7890::D: icmp_seq=3 time=156ms

Average: 142ms ✅ (< 150ms target for 3 hops)
```

#### 3. Partition Recovery Test
```bash
# Setup: 2 routers connected (A ← → B)

# On Router A: Disable radio to simulate link failure
> ifconfig down

# Wait for B to detect partition (MLE timeout ~60s)
# On Router B, check router table
> router table
# Router A should disappear after timeout

# On Router A: Re-enable radio
> ifconfig up

# Time until networks merge
# Both devices should eventually show same partition ID
> partitionid
12345678  # Should match on both devices after merge
```

### Troubleshooting

| Problem | Symptoms | Causes | Solutions |
|---------|----------|---------|-----------|
| **High latency** | Ping > 200 ms | Too many hops, high network load | Add routers to reduce hop count, reduce traffic |
| **Devices won't merge** | Separate partition IDs | Different network parameters | Verify channel, PAN ID, network key match |
| **Router table full** | Cannot add more routers | >32 routers | Thread limit reached, segment network or upgrade to REED (not typically needed) |
| **Constant re-parenting** | Device switches parent frequently | Unstable link quality | Improve RF environment, increase TX power |

### Network Topology Recommendations

```
❌ Poor Topology (star, single point of failure):
           [Leader]
         /    |    \
    [R1]    [R2]    [R3]
    / | \   / | \   / | \
  ED  ED ED ED ED ED ED ED

✅ Good Topology (mesh, redundant paths):
    [Leader] ← → [R1] ← → [R2]
        ↕           ↕        ↕
      [R3]  ← →   [R4] ← → [R5]
        ↕           ↕        ↕
       ED          ED       ED

Benefits:
- Multiple paths between any two points
- Link failure doesn't isolate devices
- Balanced load across routers
```

---

## Lab 4: CoAP Application Protocol

### Expected Performance

| Metric | Target | Typical Range | Measurement Method |
|--------|--------|---------------|---------------------|
| **CoAP Latency (1 hop)** | < 100 ms | 50-100 ms | `time coap-client -m get ...` |
| **CoAP Latency (3 hops)** | < 300 ms | 150-300 ms | Same, with 3-hop topology |
| **Throughput** | > 50 req/s | 50-100 req/s | Stress test with multiple clients |
| **Observe Notification Delay** | < 500 ms | 200-500 ms | Timestamp sensor reading vs dashboard receipt |
| **Packet Loss (CON messages)** | < 3% | 1-3% | Retry count in CoAP logs |
| **Memory per Resource** | < 500 bytes | 300-500 bytes | Heap profiling |

### Measurement Procedures

#### 1. CoAP Latency Test
```bash
# Single request timing
time coap-client -m get coap://[fd00::1234]:5683/sensors/temp

# Output:
v:1 t:ACK c:2.05 i:1234 {} [ ] :: 23.5
real    0m0.082s  # 82 ms ✅

# Batch test (100 requests)
for i in {1..100}; do
  time coap-client -m get coap://[fd00::1234]:5683/sensors/temp 2>&1 | grep real
done | awk '{sum+=$2; n++} END {print sum/n}'

# Average latency: 87ms ✅
```

#### 2. Throughput Stress Test
```bash
# Use Apache Bench (ab) with CoAP proxy
# Or custom script with libcoap

#!/bin/bash
start_time=$(date +%s%3N)
success=0
failure=0

for i in {1..1000}; do
  if coap-client -m get coap://[fd00::1234]:5683/sensors/temp -B 1 > /dev/null 2>&1; then
    ((success++))
  else
    ((failure++))
  fi
done

end_time=$(date +%s%3N)
duration=$((end_time - start_time))
throughput=$((success * 1000 / duration))

echo "Throughput: ${throughput} req/s"
echo "Success: ${success}, Failure: ${failure}"
echo "Packet loss: $((failure * 100 / 1000))%"
```

#### 3. Observe Notification Delay
```bash
# On device, add timestamp to CoAP payload
void send_sensor_reading() {
    uint32_t timestamp_ms = esp_timer_get_time() / 1000;
    char payload[100];
    snprintf(payload, sizeof(payload),
             "{\"temp\":23.5,\"timestamp\":%u}", timestamp_ms);
    // Send CoAP notification
}

# On dashboard, calculate delay
received_time_ms = Date.now();
delay = received_time_ms - payload.timestamp;
console.log(`Observe delay: ${delay} ms`);

# Target: < 500 ms
```

### Troubleshooting

| Problem | Symptoms | Causes | Solutions |
|---------|----------|---------|-----------|
| **Timeout** | No response after 30s | Server crash, network issue | Check server logs, verify ping works, increase timeout |
| **High latency** | > 500 ms per request | Network congestion, too many hops | Reduce hop count, throttle request rate |
| **Observe stops** | No notifications after initial | Server forgot subscription | Implement keep-alive, re-subscribe on timeout |
| **4.04 Not Found** | Resource doesn't exist | Wrong URI path | Check `coap resource` on server, verify path |
| **Memory leak** | Heap decreases over time | CoAP messages not freed | Check `otMessageFree()` called for every message |

---

## Lab 5: Thread Border Router

### Expected Performance

| Metric | Target | Typical Range | Measurement Method |
|--------|--------|---------------|---------------------|
| **NAT64 Latency** | < 20 ms | 10-20 ms | Compare ping with/without NAT64 |
| **Routing Throughput** | > 200 kbps | 150-250 kbps | iperf3 through Border Router |
| **Concurrent Connections** | > 50 | 50-100 | Stress test with multiple clients |
| **Failover Time (dual BR)** | < 10 s | 5-10 s | Disconnect primary BR, time until secondary takes over |
| **WiFi ↔ Thread Bridge Latency** | < 50 ms | 30-50 ms | Ping from WiFi client to Thread device |

### Measurement Procedures

#### 1. NAT64 Latency Test
```bash
# From Thread device, ping IPv4 address (via NAT64)
> ping 8.8.8.8
# This gets translated to IPv6: 64:ff9b::808:808

# Measure latency
16 bytes from 64:ff9b::808:808: icmp_seq=1 time=42ms

# For comparison, ping direct IPv6 address
> ping 2001:4860:4860::8888
16 bytes from 2001:4860:4860::8888: icmp_seq=1 time=38ms

# NAT64 overhead: 42 - 38 = 4ms ✅
```

#### 2. Border Router Health Check
```bash
# On Border Router, check status
otbr-agent[1234]: Running
wpan0: UP, fd00::1

# Check routing table
ip -6 route
fd00::/64 dev wpan0 proto kernel metric 256
default via fe80::1 dev wlan0 metric 1024

# Check NAT64 prefix
cat /proc/net/if_inet6 | grep "64:ff9b"
64ff9b0000000000000000000000000 ...
```

### Troubleshooting

| Problem | Symptoms | Causes | Solutions |
|---------|----------|---------|-----------|
| **Thread mesh isolated** | Devices can ping each other but not internet | Border Router down, WiFi disconnected | Check BR status, verify WiFi connection |
| **NAT64 not working** | Cannot reach IPv4 addresses | NAT64 module disabled | Check `ip6tables`, verify NAT64 prefix configured |
| **High packet loss** | > 10% loss through BR | BR overloaded | Reduce traffic, add second Border Router for load balancing |
| **Cannot reach Thread device from WiFi** | Firewall blocking | Check iptables rules, add FORWARD rule |

---

## Lab 6: Security and OTA Updates

### Expected Performance

| Metric | Target | Typical Range | Measurement Method |
|--------|--------|---------------|---------------------|
| **RSA Signature Verification** | < 20 ms | 10-20 ms | Measure with esp_timer |
| **AES Encryption (1 KB)** | < 1 ms | 0.5-1 ms | Measure with esp_timer |
| **OTA Download (500 KB)** | < 5 min | 2-5 min | Time from start to reboot |
| **Secure Boot Verification** | < 50 ms | 30-50 ms | Measure boot time with/without secure boot |
| **DTLS Handshake** | < 500 ms | 300-500 ms | Measure with Wireshark |
| **Flash Write (1 KB)** | < 5 ms | 3-5 ms | Measure with esp_timer |

### Measurement Procedures

#### 1. Cryptographic Performance Test
```c
#include "esp_timer.h"
#include "mbedtls/rsa.h"
#include "mbedtls/aes.h"

// RSA signature verification
uint8_t signature[256];  // RSA-2048 signature
uint8_t hash[32];        // SHA-256 hash

int64_t start = esp_timer_get_time();
int ret = mbedtls_rsa_pkcs1_verify(&rsa, NULL, NULL,
                                    MBEDTLS_RSA_PUBLIC,
                                    MBEDTLS_MD_SHA256,
                                    32, hash, signature);
int64_t end = esp_timer_get_time();

ESP_LOGI(TAG, "RSA verify time: %lld ms", (end - start) / 1000);
// Target: < 20 ms

// AES encryption
uint8_t plaintext[1024];
uint8_t ciphertext[1024];
mbedtls_aes_context aes;

start = esp_timer_get_time();
mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, 1024,
                      iv, plaintext, ciphertext);
end = esp_timer_get_time();

ESP_LOGI(TAG, "AES encrypt (1KB): %lld us", end - start);
// Target: < 1 ms (1000 us)
```

#### 2. OTA Update Time Measurement
```bash
# On device, enable OTA logging
idf.py menuconfig
# Component config → App Update → Log level → Debug

# Start OTA update
curl -X POST http://192.168.1.100/ota -F "file=@firmware.bin"

# Check device logs
[OTA] Starting OTA update...
[OTA] Download progress: 10% (50 KB / 500 KB)
[OTA] Download progress: 50% (250 KB / 500 KB)
[OTA] Download progress: 100% (500 KB / 500 KB)
[OTA] Verifying signature... OK (15 ms)
[OTA] Writing to flash... OK (2.3 s)
[OTA] Rebooting...

Total time: ~180 seconds (3 minutes) ✅
```

### Troubleshooting

| Problem | Symptoms | Causes | Solutions |
|---------|----------|---------|-----------|
| **Signature verification failed** | OTA rejected | Unsigned firmware, wrong key | Verify firmware signed with correct private key |
| **OTA timeout** | Download incomplete after 10 min | Network congestion, low bandwidth | Reduce chunk size, retry failed chunks |
| **Boot loop after OTA** | Device reboots repeatedly | Corrupted firmware | MCUboot should rollback automatically after 3 failed boots |
| **Secure boot disabled** | Warning in logs | eFuse not burned | Burn secure boot eFuse (irreversible!) |

---

## Lab 7: Dashboard and Data Visualization

### Expected Performance

| Metric | Target | Typical Range | Measurement Method |
|--------|--------|---------------|---------------------|
| **Dashboard Load Time** | < 2 s | 1-2 s | Browser DevTools Network tab |
| **WebSocket Latency** | < 100 ms | 50-100 ms | Timestamp message sent vs received |
| **UI Update Rate** | > 1 Hz | 1-10 Hz | Count updates per second in console |
| **Sensor-to-Dashboard Latency** | < 500 ms | 200-500 ms | End-to-end timestamp (sensor → dashboard) |
| **Concurrent Users** | > 10 | 10-50 | Stress test with multiple browsers |

### Measurement Procedures

#### 1. End-to-End Latency Test
```javascript
// On device (CoAP server), add timestamp to payload
uint32_t sensor_timestamp_ms = esp_timer_get_time() / 1000;
snprintf(payload, sizeof(payload),
         "{\"temp\":23.5,\"ts\":%u}", sensor_timestamp_ms);

// On dashboard (JavaScript)
websocket.onmessage = (event) => {
  const data = JSON.parse(event.data);
  const dashboard_timestamp_ms = Date.now();
  const latency_ms = dashboard_timestamp_ms - data.ts;
  console.log(`End-to-end latency: ${latency_ms} ms`);
};

// Target: < 500 ms
```

#### 2. Dashboard Performance Profiling
```javascript
// Browser DevTools → Performance tab
// Record page load, look for:
// - DOMContentLoaded: < 1s
// - Load event: < 2s
// - Time to Interactive: < 2.5s

// WebSocket message frequency
let message_count = 0;
setInterval(() => {
  console.log(`Messages/sec: ${message_count}`);
  message_count = 0;
}, 1000);

websocket.onmessage = () => {
  message_count++;
};
```

### Troubleshooting

| Problem | Symptoms | Causes | Solutions |
|---------|----------|---------|-----------|
| **Dashboard not updating** | Stale data | WebSocket disconnected | Check connection state, implement reconnect logic |
| **High latency** | > 1s delay | Network congestion, slow server | Optimize queries, add caching, use CDN for static assets |
| **Browser crash** | Tab becomes unresponsive | Memory leak, too many DOM elements | Limit history (e.g., last 1000 points), use virtual scrolling |
| **WebSocket connection refused** | 403 Forbidden | Authentication failed | Check session token, verify CORS headers |

---

## Lab 8: End-to-End Integration

### Expected Performance (System-Level)

| Metric | Target | Typical Range | Measurement Method |
|--------|--------|---------------|---------------------|
| **End-to-End Latency (Sensor → Dashboard)** | < 500 ms | 200-500 ms | Timestamp at each stage |
| **System Availability** | > 99% | 99-99.9% | Uptime monitoring over 7 days |
| **Throughput (100 devices)** | < 50% utilization | 30-50% | Calculate total bandwidth usage |
| **MTBF (Mean Time Between Failures)** | > 168 hours | 168-720 hours | Track failures during testing |
| **MTTR (Mean Time To Recover)** | < 60 s | 30-60 s | Measure recovery from simulated failures |

### Measurement Procedures

#### 1. End-to-End Latency Breakdown
```
Component                        Latency       % of Total
────────────────────────────────────────────────────────
1. Sensor reading (ADC)          2 ms          1%
2. Format CoAP notification      1 ms          0.5%
3. Thread mesh (3 hops)          60 ms         30%
4. Border Router processing      5 ms          2.5%
5. Server processing             5 ms          2.5%
6. WebSocket transmission        5 ms          2.5%
7. Browser rendering             10 ms         5%
────────────────────────────────────────────────────────
Total                            88 ms         ✅

Breakdown shows Thread mesh is bottleneck (30%).
Optimization: Reduce hops or use faster channel.
```

#### 2. System Availability Test
```bash
# Run for 7 days (168 hours)
# Monitor with external service (e.g., UptimeRobot) or custom script

#!/bin/bash
start_time=$(date +%s)
downtime=0

while true; do
  if ! curl -s http://192.168.1.100/health > /dev/null; then
    # System down
    downtime=$((downtime + 1))
  fi
  sleep 60  # Check every minute
done

# After 7 days
uptime_minutes=$((168 * 60))
availability=$(echo "scale=4; ($uptime_minutes - $downtime) / $uptime_minutes * 100" | bc)
echo "Availability: ${availability}%"

# Target: > 99% (< 1.68 hours downtime per week)
```

### Troubleshooting

| Problem | Symptoms | Causes | Solutions |
|---------|----------|---------|-----------|
| **Cascading failure** | Multiple components fail together | Single point of failure | Add redundancy (e.g., dual Border Routers) |
| **Data loss** | Sensor readings missing | Buffer overflow, network partition | Implement buffering, increase buffer size |
| **Performance degradation** | Latency increases over time | Memory leak, resource exhaustion | Profile memory, identify leak, fix |
| **Inconsistent state** | Dashboard shows different data than device | Synchronization issue | Implement version numbers, conflict resolution |

---

## General Troubleshooting Tips

### 1. Always Check the Basics
- [ ] Power supply (3.3V, sufficient current)
- [ ] USB cable (data cable, not charge-only)
- [ ] Antenna connected
- [ ] Correct firmware flashed

### 2. Use Logging Effectively
```c
// Set appropriate log levels
esp_log_level_set("*", ESP_LOG_INFO);
esp_log_level_set("OT", ESP_LOG_DEBUG);  // Detailed OpenThread logs
esp_log_level_set("COAP", ESP_LOG_DEBUG);

// Add timing logs
ESP_LOGI(TAG, "Operation started");
int64_t start = esp_timer_get_time();
// ... operation ...
ESP_LOGI(TAG, "Operation took %lld ms", (esp_timer_get_time() - start) / 1000);
```

### 3. Wireshark Packet Capture
```bash
# On Border Router or sniffer device
# Capture 802.15.4 traffic
tcpdump -i wpan0 -w capture.pcap

# Analyze in Wireshark
# Filters:
#   wpan             - All 802.15.4 traffic
#   coap             - CoAP messages
#   icmpv6           - IPv6 pings
#   wpan.src64 == 0x1234567890abcdef  - Specific device
```

### 4. Performance Profiling
```c
// Use ESP-IDF heap tracing
#include "esp_heap_trace.h"

heap_trace_init_standalone(trace_records, NUM_RECORDS);
heap_trace_start(HEAP_TRACE_LEAKS);

// ... run application ...

heap_trace_stop();
heap_trace_dump();  // Show all allocations

// Use FreeRTOS task stats
vTaskList(task_stats_buffer);
ESP_LOGI(TAG, "Task stats:\n%s", task_stats_buffer);
```

---

## Quick Reference: Performance Targets Summary

| Lab | Key Metric | Target |
|-----|------------|--------|
| **Lab 1** | RSSI @ 10m | > -70 dBm |
| | Packet Error Rate | < 5% |
| **Lab 2** | Ping Latency | < 50 ms |
| | Header Compression | > 60% |
| **Lab 3** | Mesh Convergence | < 60 s |
| | Latency (3 hops) | < 150 ms |
| **Lab 4** | CoAP Latency (1 hop) | < 100 ms |
| | Throughput | > 50 req/s |
| **Lab 5** | NAT64 Overhead | < 20 ms |
| | Bridge Latency | < 50 ms |
| **Lab 6** | RSA Verify | < 20 ms |
| | OTA Update | < 5 min |
| **Lab 7** | Dashboard Load | < 2 s |
| | Sensor → Dashboard | < 500 ms |
| **Lab 8** | System Availability | > 99% |
| | MTTR | < 60 s |

---

**Usage**: Refer to this guide throughout the course to:
1. Verify your system meets performance targets
2. Diagnose issues when metrics are out of range
3. Optimize bottlenecks identified during testing
4. Document performance in your DDR

**Next**: Use these baselines in your DDR Section 10 ([DDR Template](3_deliverables_template.md)).
