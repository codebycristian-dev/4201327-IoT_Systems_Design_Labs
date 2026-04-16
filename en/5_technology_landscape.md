# Technology Landscape: IoT Protocol Stacks
**Document Type**: Instructor Preamble (~40 min)
**Audience**: IoT Systems Design Students
**Purpose**: Position Thread/CoAP within the broader IoT ecosystem and show that ISO/IEC 30141 applies regardless of stack choice

---

## 1. Introduction

The ISO/IEC 30141 reference architecture is technology-agnostic. Its six viewpoints and functional domains describe *what* an IoT system must do, not *how* it does it. Whether you use Thread, LoRaWAN, BLE Mesh, or WiFi, the same architectural questions apply: Where does data originate? How is it secured? Who manages the devices?

This guide maps the major IoT stacks against each other so you understand **why** we chose Thread/CoAP and what tradeoffs that implies.

---

## 2. The IoT Protocol Stack

Every IoT system maps onto a layered model. The layers below correspond to ISO/IEC 30141 Clause 6 (Functional Viewpoint), where each layer implements functions within one or more functional domains.

```
┌─────────────────────────────────────────────┐
│  Cloud / Management    (OMD, UD)            │  dashboards, analytics, device management
├─────────────────────────────────────────────┤
│  Application Protocol  (ASD, RAID)          │  CoAP, MQTT, HTTP, ZCL
├─────────────────────────────────────────────┤
│  Encoding              (ASD)                │  CBOR, JSON, Protobuf, binary
├─────────────────────────────────────────────┤
│  Security              (RAID)               │  DTLS, TLS, AES-128, BLE pairing
├─────────────────────────────────────────────┤
│  Transport             (SCD)                │  UDP, TCP
├─────────────────────────────────────────────┤
│  Network               (SCD)               │  IPv6/Thread, LoRaWAN, Zigbee, BLE Mesh
├─────────────────────────────────────────────┤
│  Link                  (PED, SCD)           │  802.15.4, LoRa, BLE, 802.11
├─────────────────────────────────────────────┤
│  Physical              (PED)                │  2.4 GHz, sub-GHz, 5 GHz
└─────────────────────────────────────────────┘
```

**Key**: PED = Physical Entity Domain, SCD = Service & Communication Domain, ASD = Application & Service Domain, RAID = Resource Access & Interchange Domain, OMD = Operation & Management Domain, UD = User Domain.

---

## 3. Alternative Stacks Comparison

| Aspect | Thread/CoAP (this course) | LoRaWAN/MQTT | BLE Mesh/HTTP | Zigbee/MQTT | WiFi/MQTT |
|---|---|---|---|---|---|
| **Physical/Link** | IEEE 802.15.4 (2.4 GHz) | LoRa (sub-GHz) | BLE 5.0 (2.4 GHz) | IEEE 802.15.4 (2.4 GHz) | IEEE 802.11 (2.4/5 GHz) |
| **Network** | Thread (IPv6 mesh) | LoRaWAN (star-of-stars) | BLE Mesh (flood) | Zigbee (mesh/tree) | TCP/IP (star) |
| **Transport** | UDP | UDP | TCP | UDP | TCP |
| **Application** | CoAP (RESTful) | MQTT (pub/sub) | HTTP/REST | MQTT or ZCL | MQTT or HTTP |
| **Encoding** | CBOR (binary) | JSON or Protobuf | JSON | ZCL binary | JSON |
| **Security** | DTLS 1.2 | AES-128 (LoRaWAN spec) | BLE pairing + TLS | AES-128 (Zigbee NWK) | TLS 1.2/1.3 |
| **Topology** | Mesh (self-healing) | Star-of-stars | Mesh (flood relay) | Mesh (tree/cluster) | Star (AP-centric) |
| **Range** | ~100 m | ~10 km | ~30 m | ~100 m | ~50 m |
| **Power** | Low (months on battery) | Very low (years) | Medium (weeks-months) | Low (months) | High (days) |
| **IPv6 native?** | Yes | No (gateway required) | No | No | Yes |
| **ISO 30141 PED** | Sensors/actuators + radio | Sensors + LoRa radio | Sensors + BLE radio | Sensors + radio | Sensors + WiFi radio |
| **ISO 30141 SCD** | Thread mesh routing | Network server routing | BLE relay nodes | Zigbee coordinator | WiFi AP routing |
| **Best for** | Building/campus mesh | Rural, wide-area, agriculture | Wearables, consumer | Home automation, lighting | Always-powered devices |

All five stacks implement the same ISO/IEC 30141 functional domains. The table rows above Clause 6's Functional Viewpoint; the architecture analysis is identical across stacks.

---

## 4. ADR-000: Why Thread/CoAP for This Course

**Status**: Accepted

### Context

We need a protocol stack for teaching IoT systems design to university students using affordable hardware. The stack must illustrate the full ISO/IEC 30141 architecture while being practical for 6 lab sessions.

### Decision

Thread/CoAP on ESP32-C6 (IEEE 802.15.4), with CBOR encoding and DTLS security.

### Rationale

| Criterion | Thread/CoAP advantage |
|---|---|
| **IPv6 native** | Students learn real networking: addresses, routing, multicast. No proprietary addressing schemes. |
| **Mesh topology** | Demonstrates self-healing and self-organization, key emergent characteristics (ISO 30141 Clause 8). |
| **CoAP (RESTful)** | Same GET/PUT/POST/DELETE model as HTTP. Students transfer existing web knowledge to constrained devices. |
| **CBOR encoding** | Binary encoding teaches efficiency tradeoffs vs. JSON. Directly comparable in labs. |
| **Open standard** | IETF RFCs (CoAP: 7252, CBOR: 8949, Thread: IEEE 802.15.4 + IETF). No vendor lock-in. |
| **ESP32-C6** | ~$8 per board, Thread-certified, well-documented ESP-IDF support, USB-C serial. |

### Tradeoffs Acknowledged

- **Not wide-area**: LoRaWAN would be better for real agricultural deployments at scale (~10 km vs ~100 m).
- **Not consumer-dominant**: Zigbee and BLE dominate installed smart-home base. Thread/Matter adoption is growing but smaller.
- **UDP complexity**: Students must handle unreliable transport (retries, confirmable messages) that TCP abstracts away.
- **Tooling maturity**: WiFi/MQTT has far more tutorials, libraries, and Stack Overflow answers.

### Consequences

Students gain transferable architectural thinking. Swapping Thread/CoAP for any other stack changes implementation details but not the ISO viewpoint analysis, deliverable structure, or design methodology.

---

## 5. How the Architecture Stays Constant

If you replaced Thread/CoAP with LoRaWAN/MQTT tomorrow, every lab's ISO viewpoint analysis would remain valid. The six functional domains still apply. The DDR structure does not change. Only the implementation rows in your deliverables shift.

| Lab | Architectural concern (constant) | Thread/CoAP detail | LoRaWAN/MQTT detail |
|---|---|---|---|
| Lab 1 | Proximity Networking (PED, SCD) | 802.15.4 at 100 m | LoRa at 10 km |
| Lab 2 | Network Formation (SCD) | Thread mesh, router roles | LoRaWAN join, OTAA |
| Lab 3 | Application Protocol (ASD) | CoAP observe, CBOR | MQTT subscribe, JSON |
| Lab 4 | Cloud Integration (UD, OMD) | CoAP-to-cloud bridge | MQTT broker direct |
| Lab 5 | Multi-domain System (all domains) | Thread border router | LoRaWAN network server |
| Lab 6 | Trustworthiness (RAID) | DTLS + mesh key rotation | AES-128 + AppKey |

The ISO/IEC 30141 viewpoints ask the same questions regardless of stack. That is the point of a reference architecture.

---

## 6. Emerging Technologies

| Technology | What it is | ISO 30141 relevance |
|---|---|---|
| **Matter** | Application layer built *on top of* Thread, WiFi, and Ethernet. Unifies smart-home interoperability. | Standardises the ASD and RAID across multiple SCD transports. |
| **Wi-SUN** | IEEE 802.15.4g mesh for smart city and utility networks. Ranges to several km. | Extends the PED/SCD scope to city-scale infrastructure. |
| **NB-IoT / LTE-M** | Cellular IoT using licensed spectrum. Carrier-managed, no gateway needed. | Moves SCD responsibility to telecom operators; simplifies OMD. |
| **DECT NR+** | ETSI mesh standard (1.9 GHz) for massive IoT. Up to 1 million devices/km^2. | New PED/SCD option for dense industrial deployments. |

All four fit cleanly into ISO/IEC 30141. The framework was designed to outlast any single technology generation.

---

*This document supports the 40-minute technology landscape preamble. For protocol-level detail, see [Theory Foundations](5_theory_foundations.md). For the ISO architecture itself, see [ISO/IEC 30141 Guide](2_iso_architecture.md).*
