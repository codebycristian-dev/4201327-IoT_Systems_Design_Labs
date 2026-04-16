# Networking Fundamentals & IoT Architecture

**GreenField Technologies - Onboarding Module**

**Phase**: Team Alignment

**Topic**: OSI vs. ISO 30141, IPv6, and IoT First Principles

---

## 1. Project Context: Why this module?

**From:** Eng. Samuel Cifuentes (Senior Architect)

**To:** New IoT Systems Engineering Team

**Subject:** Knowledge Alignment - Networking Fundamentals

Welcome to the team! Before we start building our precision agriculture mesh network, we need to ensure everyone is aligned on the fundamental technologies we use.

**Don't worry if some concepts are new**—this module is designed to bridge gaps in networking knowledge. We'll start with basics (what is the Internet?) and build up to IoT-specific concepts (IPv6, mesh networks, constrained devices).

📖 **Tip**: Keep the [glossary.md](glossary.md) open in another tab—it explains every technical term in plain English.

**Objectives**:
1.  **Architecture**: Understand the difference between *Communication Models* (OSI) and *System Models* (ISO/IEC 30141).
2.  **IPv6 Deep Dive**: Our production system uses Thread (IPv6-based). You must understand why we don't use IPv4.
3.  **IoT Constraints**: Understand the "Triangle of Pain" (Energy, Memory, Bandwidth) that drives our decisions.

---

## 2. Two Different Lenses: OSI vs ISO/IEC 30141

When building IoT systems, you need **TWO different perspectives**:

| Framework | Question | Metaphor |
|-----------|----------|----------|
| **OSI Model** | "How does data move?" | Postal system (how mail flows through sorting centers) |
| **ISO/IEC 30141** | "How is the system organized?" | City map (where buildings are located) |

**Both are essential** - like needing both a recipe (OSI) and a kitchen layout (ISO 30141) to cook well.

---

### 2.1 OSI Model: The Protocol Stack (Vertical)

**Example: Your BLE sensor sending "23.5°C"**

```
┌─────────────────────────────────────┐
│ L7: Application                     │ ← Your code: "Send temperature: 23.5°C"
├─────────────────────────────────────┤
│ L4: Transport (UDP)                 │ ← Break into packets
├─────────────────────────────────────┤
│ L3: Network (IPv6)                  │ ← Add destination address
├─────────────────────────────────────┤
│ L2: Data Link (BLE)                 │ ← BLE connection
├─────────────────────────────────────┤
│ L1: Physical (2.4 GHz Radio)        │ ← The radio you studied yesterday!
└─────────────────────────────────────┘
```

**OSI helps you**: Choose protocols (CoAP vs MQTT), debug (which layer failed?), optimize bandwidth.

**OSI doesn't tell you**: Where sensors go, who manages the system, how cloud storage works.

---

### 2.2 ISO/IEC 30141: System Architecture (Horizontal)

**Same sensor, different view:**

```
┌────────────────────────────────────────────────────────────┐
│  PED        →    SCD      →   Network  →  Application     │
│  (Physical)     (Devices)     (Comms)     (Services)       │
│                                                            │
│  [Soil]    ←  [Sensor]  →  [Gateway] →  [Cloud/Dashboard] │
│   23°C         reads        bridges       stores/displays  │
└────────────────────────────────────────────────────────────┘
```

**ISO 30141 helps you**: Organize your DDR, communicate with stakeholders, design system boundaries.

**ISO 30141 doesn't tell you**: Which wireless protocol, how packets are formatted, encryption algorithms.

---

### 2.3 Quick Comparison

| Aspect | OSI Model | ISO/IEC 30141 |
|--------|-----------|---------------|
| **Direction** | Vertical (layers stacked) | Horizontal (domains side-by-side) |
| **Answers** | "How does data move?" | "How is the system organized?" |
| **Example** | "BLE uses PHY → Link → GATT" | "Sensor is in SCD, Dashboard is in UD" |
| **In your DDR** | Justify protocol choices | Map components to domains |

**Remember**:
- **OSI** = Layers (1, 2, 3, 4, 7)
- **ISO 30141** = Domains (PED, SCD, ASD, OMD, UD, RAID)

**You'll use BOTH in Labs 1-8!**

📖 **See [glossary](glossary.md)** for full explanations of layers and domains.

---

## 3. Networking Recap - The Foundation

### What is the Internet?

At its core, the Internet is a global system of interconnected computer networks that use the standard Internet Protocol Suite (TCP/IP). It is a **network of networks**. See [What is internet?](https://www.geeksforgeeks.org/computer-science-fundamentals/what-is-internet-definition-uses-working-advantages-and-disadvantages/) and [¿Qué es Internet?](https://youtu.be/GkA5WOeLWbM)

* **Packet Switching**: Data is broken into small chunks (packets), sent independently, and reassembled at the destination.

### The TCP/IP Model (Simplified for IoT)

For our IoT work, we care about these interactions:

1. **Application**: CoAP / MQTT. *This maps to the Operations Domain in ISO 30141.*
2. **Transport**: UDP (Fast, fire-and-forget). *Preferred for battery-powered nodes.*
3. **Network**: IPv6 (Addressing and Routing). *Getting packets from A to B.*
4. **Link/Physical**: 802.15.4. *Low power radio.*

---

## 4. IPv4 vs IPv6 - The Critical Shift

Traditionally, the internet ran on **IPv4**.

### IPv4 Examples & Analysis

When you run `ip a` on your laptop, you might see: `inet 192.168.1.11/24`.

* **The Problem:** This is a **Private IP**. It only exists inside your house.
* **The Workaround (NAT):** If you want to send data to Google, your router has to "fake" the packet. It erases your IP and writes its own Public IP.
* **The Consequence:** A server in Japan cannot initiate a connection to your device because your device has no public identity.

**Visualizing the IPv4 Packet Flow (The "Post Office" method):**

```mermaid
sequenceDiagram
    participant Laptop as Laptop (192.168.1.11)
    participant Router as Router (NAT)
    participant Google as Google Server (142.250.x.x)

    Note over Laptop, Router: INSIDE HOUSE (Private)
    Laptop->>Router: Src: 192.168.1.11 <br/> Dest: Google
    
    Note over Router: Router STOPS the packet. <br/> Erases "192.168.1.11". <br/> Writes "181.x.x.x" (Public IP).
    
    Note over Router, Google: PUBLIC INTERNET
    Router->>Google: Src: 181.x.x.x <br/> Dest: Google
    
    Note over Google: Google thinks the Router <br/> sent the message.
    Google-->>Router: Reply to 181.x.x.x
    
    Note over Router: Router looks up memory table. <br/> "Oh, this was for the Laptop."
    Router-->>Laptop: Forward to 192.168.1.11

```

### Other IPv4 Examples

* **Class A private range**: 10.0.0.1 to 10.255.255.255
* **Class B private range**: 172.16.0.1 to 172.31.255.255
* **Class C private range**: 192.168.0.1 to 192.168.255.255
* **Real-world examples**:
  - 104.244.42.129 (Twitter.com)
  - 151.101.65.140 (Reddit.com)
  - 108.174.10.10 (LinkedIn.com)



### IPv6 Examples & Analysis

**IPv6** uses 128-bit addresses, giving us **340 undecillion** addresses (that's 2<sup>128</sup> ≈ 340,282,366,920,938,463,463,374,607,431,768,211,456).

**In practical terms**: Enough to give every grain of sand on Earth its own IP address—and still have plenty left over!

When you run `ip a`, you will see multiple addresses. Here is how to read them:

1. **Global Unicast (The Goal)**: `2800:e2:807f:f60d...`

* **Meaning:** This is a **Public** address (LACNIC/LatAm region).
* **Capability:** You can talk directly to Japan. No NAT required.

2. **Link-Local (The Internal)**: `fe80::ec9b...`

* **Meaning:** This is for "in-room" talk only. It effectively replaces the Ethernet cable.

**Visualizing the IPv6 Packet Flow (End-to-End):**

```mermaid
sequenceDiagram
    participant Laptop as Laptop (2800:e2... Public)
    participant Router as Router (Gateway)
    participant Google as Google Server (2001:4860...)

    Note over Laptop, Router: INSIDE HOUSE
    Laptop->>Router: Src: 2800:e2... <br/> Dest: 2001:4860...
    
    Note over Router: Router DOES NOTHING to the header. <br/> Just forwards the packet.
    
    Note over Router, Google: PUBLIC INTERNET
    Router->>Google: Src: 2800:e2... <br/> Dest: 2001:4860...
    
    Note over Google: Google sees your Laptop's <br/> ACTUAL address.
    Google-->>Laptop: Reply directly to 2800:e2...

```

**Key Advantages for GreenField:**

1. **No NAT Needed**: Every sensor node has a globally unique public IP.
2. **Auto-configuration (SLAAC)**: Devices generate their own addresses when they join the network.
3. **Efficiency**: Simplified header format for faster processing.

| Feature | IPv4 | IPv6 |
| --- | --- | --- |
| Address Length | 32-bit | 128-bit |
| Example | `192.168.1.50` | `2800:e2:807f::1` |
| Config | DHCP (Manual/Server) | SLAAC (Automatic) |
| Broadcast | Yes (Noisy) | No (Uses Multicast - Efficient) |

---

## 5. Class Exercise: The Human Local Network

**Objective:** Create a real LAN, identify the Gateway, and transfer data between devices without using the Cloud.

**Why:** University networks often use "Client Isolation" which blocks peer-to-peer IoT traffic. To bypass this, we use a **Phone Hotspot** as our Router.

### Phase 1: The Setup (Infrastructure)

1. **Router:** Connect to the Instructor's Samsung Phone Hotspot.
2. **Verify:** This phone is now your **Default Gateway** and **DHCP Server**.

### Phase 2: Discovery (Layer 3)

Find your place on the network map.

```bash
# 1. Find your IP address
ip a # or ipconfig on Windows
# Look for an address like 192.168.x.x or 10.x.x.x

# 2. Find your Gateway (The Router's IP)
ip route # or route show on Windows
# Look for the IP after "default via" (e.g., 10.121.201.198)

```

### Phase 3: Connectivity Test (Ping)

Verify you can reach the infrastructure.

```bash
# Replace with the Gateway IP you found in step 2
ping 10.121.201.198

```

*Note the time (latency). High latency (>100ms) indicates network congestion or power-saving modes.*

### Phase 4: The "IoT" Service (Layer 7)

We will simulate an IoT Sensor serving data.

1. **Server Node (You):**
```bash
echo "<h1>Hello from IoT Node</h1>" > index.html
python3 -m http.server 8000

```


2. **Client Node (Partner):** Open browser to `http://[Your_IP]:8000`

---

## 6. The "Constraint" Triangle (Why IoT is Different)

**Key insight**: IoT engineering is the art of working within limits.

Unlike traditional software development (where you can add more RAM or CPU), IoT devices operate under strict constraints. This fundamentally changes how we design systems.

```mermaid
graph TD
    E[Energy <br/> Batteries must last years] --- M[Memory <br/> KiloBytes, not GigaBytes]
    M --- B[Bandwidth <br/> Low throughput, high latency]
    B --- E

```

### The Three Constraints

1. **Energy** ⚡
   - **Your laptop**: Plugged into wall power, can run 24/7
   - **IoT sensor**: Tiny battery that must last months or years
   - **Impact**: Devices sleep 99% of the time, waking only to take measurements

2. **Memory** 💾
   - **Your laptop**: 8-16 GB RAM, 500+ GB storage
   - **ESP32-C6**: 512 KB RAM, 4 MB flash storage
   - **Impact**: Code must be optimized for size, can't use heavy libraries

3. **Bandwidth** 📡
   - **Your laptop**: WiFi at 100+ Mbps, can stream 4K video
   - **IoT sensor**: 250 kbps radio, sends tiny packets
   - **Impact**: We send bytes (temperature reading: "23.5°C"), not multimedia

**Example**: This is why we use CoAP instead of HTTP, CBOR instead of JSON, and UDP instead of TCP—every byte and every millisecond of radio time costs battery power.

---

## 7. The Official Definition (ISO/IEC 30141)

Now that we understand the networking and the constraints, let's define what we are actually building. The **ISO/IEC 30141:2024** standard provides a clear framework for our work.

### Core Definition

> "Fundamental to IoT are devices that interact with the physical world. Sensors collect the information about the physical world, while actuators can act upon the physical world."

These field devices are **connected to the digital world through network connections**, creating a bridge between physical reality and digital systems. See [¿Qué es IoT?](https://youtu.be/u5iho36snrc)

### Why this matters

The standard highlights that IoT is an essential enabler for advanced computing areas that we will touch upon in this course, including **digital twins**, **artificial intelligence**, and **big data**.

---

## 8. Review & References

**Key Reference Text:**

* Herrero, Rolando. *Practical Internet of Things Networking: Understanding IoT Layered Architecture*. Springer, 2023.

**Self-Check Questions:**

1. Why is the ISO 30141 "Sensing Domain" physically separate from the "Application Domain"?
2. Why can't we just give every sensor a static IPv4 address?
3. What is the difference between a "Link-Local" IPv6 address and a "Global" address?
4. Why does the ESP32-C6 need to "sleep" instead of staying connected to WiFi all the time?

---

### Navigation
[< Back to Setup](0_setup.md) | [Next: Project Scenario >](1_project_scenario.md)
