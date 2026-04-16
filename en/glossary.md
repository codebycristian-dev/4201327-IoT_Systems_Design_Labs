# IoT Systems Design - Glossary

**Purpose**: Quick reference for terminology used throughout the course. Bookmark this page!

**How to use**: When you see an unfamiliar term in labs or readings, search this page (Ctrl+F / Cmd+F).

---

## Table of Contents
- [Core IoT Concepts](#core-iot-concepts)
- [Architecture & Documentation](#architecture--documentation)
- [Networking & Protocols](#networking--protocols)
- [Hardware & Radio](#hardware--radio)
- [Security](#security)
- [Course-Specific Terms](#course-specific-terms)

---

## Core IoT Concepts

### IoT (Internet of Things)
Devices with sensors or actuators that connect to the internet to collect or act on data.
- **Example**: A soil moisture sensor that sends data to your phone

### Sensor
A device that measures something in the physical world (temperature, moisture, light, etc.)
- **Example**: The DHT22 sensor in your lab kit measures temperature and humidity

### Actuator
A device that performs an action in the physical world (motor, valve, LED, etc.)
- **Example**: A valve that opens to water plants when soil is dry

### Constrained Device
An IoT device with limited resources: battery power, memory (RAM), processing speed, or network bandwidth
- **Why it matters**: You can't run the same software on a sensor node that you'd run on a laptop

### Edge Device / Node
A device at the "edge" of the network (where sensors/actuators interact with the physical world)
- **In this course**: Your ESP32-C6 boards are edge devices

### Gateway / Border Router
A device that connects your IoT network to the Internet
- **Analogy**: Like a translator between two groups speaking different languages

---

## Architecture & Documentation

### ISO/IEC 30141:2024
An international standard that defines how to organize and describe IoT systems
- **Why we use it**: Provides a common language for engineers worldwide
- **Think of it as**: A blueprint language for IoT systems

### Functional Domains (ISO 30141)
Six main areas of responsibility in an IoT system:
1. **PED** (Physical Entity Domain) - The real-world things being sensed/controlled
2. **SCD** (Sensing & Controlling Domain) - Sensors, actuators, and gateways
3. **ASD** (Application & Service Domain) - Software that processes data
4. **OMD** (Operation & Management Domain) - Monitoring and maintaining the system
5. **UD** (User Domain) - Interfaces for end users
6. **RAID** (Resource Access & Interchange Domain) - Security, APIs, data exchange

### Viewpoints (ISO 30141)
Six different perspectives for analyzing an IoT system:
- **Foundational**: What is this system?
- **Business**: Why are we building it?
- **Usage**: Who uses it and how?
- **Functional**: What does it do?
- **Trustworthiness**: How is it secure and reliable?
- **Construction**: How do we build it?

### DDR (Design Decision Record)
A living document that tracks your architectural decisions throughout the course
- **Format**: One DDR per team, updated each lab
- **Contains**: Design decisions, performance measurements, ISO domain mappings

### ADR (Architecture Decision Record)
A standalone document explaining a major technical decision
- **Format**: One ADR per significant decision
- **Contains**: Problem, alternatives considered, decision made, consequences
- **Example**: "ADR-001: Why we chose CoAP instead of MQTT"

### First Principles
Understanding the fundamental "why" behind a technology, not just the "how"
- **Example**: Understanding *why* IPv6 is needed (address exhaustion) vs just knowing *how* to configure it

---

## Networking & Protocols

### IPv6 (Internet Protocol version 6)
The addressing system that gives every device on the internet a unique address
- **Address length**: 128 bits (vs IPv4's 32 bits)
- **Example**: `2001:db8::1`
- **Why IoT needs it**: Billions of IoT devices need unique addresses

### IPv4 (Internet Protocol version 4)
The older addressing system (still widely used)
- **Address length**: 32 bits
- **Example**: `192.168.1.1`
- **Problem**: Only ~4 billion addresses (not enough for IoT)

### 6LoWPAN (IPv6 over Low-Power Wireless Personal Area Networks)
A way to run IPv6 on constrained wireless networks
- **What it does**: Compresses IPv6 headers to fit in small radio packets
- **Why we need it**: Regular IPv6 packets are too large for low-power radios

### Mesh Network
A network where devices relay messages for each other (no central hub required)
- **Benefit**: More reliable—if one path fails, messages find another route
- **In this course**: Thread creates a mesh network

### Thread
A low-power wireless networking protocol designed for IoT
- **Based on**: IPv6, IEEE 802.15.4 radio
- **Key feature**: Self-forming mesh network
- **Used in**: Smart homes (Google Nest, Apple HomeKit)

### CoAP (Constrained Application Protocol)
A lightweight protocol for sending/receiving data on constrained devices
- **Think of it as**: HTTP's smaller cousin for IoT
- **Runs on**: UDP (not TCP)
- **Why not HTTP**: HTTP is too heavy for battery-powered sensors

### CBOR (Concise Binary Object Representation)
A compact way to encode data for transmission
- **Think of it as**: JSON's more efficient cousin
- **Why not JSON**: JSON wastes bandwidth with text encoding

### MQTT (Message Queuing Telemetry Transport)
Another IoT protocol (alternative to CoAP)
- **Architecture**: Publish/subscribe with a central broker
- **When to use**: Cloud-connected devices with reliable power
- **Why we use CoAP instead**: No central broker needed, works better for mesh networks

### UDP (User Datagram Protocol)
A simple transport protocol that sends data without guarantees
- **Characteristics**: Fast, fire-and-forget, no connection setup
- **Used by**: CoAP, DNS
- **Contrast with TCP**: TCP guarantees delivery but adds overhead

### TCP (Transmission Control Protocol)
A transport protocol that guarantees reliable delivery
- **Characteristics**: Slower, maintains connection state
- **Used by**: HTTP, MQTT
- **Not ideal for**: Battery-powered IoT devices

### NAT (Network Address Translation)
A technique routers use to share one public IP among many private devices
- **Problem it solves**: IPv4 address shortage
- **Problem it creates**: Devices behind NAT can't be reached from the internet
- **IPv6 eliminates**: No NAT needed with enough addresses

### Link Budget
Calculation to determine if a wireless signal is strong enough to reach its destination
- **Factors**: Transmit power, antenna gain, distance, obstacles, receiver sensitivity
- **Lab 1 focus**: Measuring real-world link budgets

---

## Hardware & Radio

### ESP32-C6
The microcontroller you'll use in this course
- **Features**: RISC-V processor, WiFi, Bluetooth, IEEE 802.15.4 radio
- **RAM**: 512 KB (tiny compared to your laptop's gigabytes!)
- **Why this chip**: Supports Thread networking

### IEEE 802.15.4
The radio standard used by Thread, Zigbee, and other low-power networks
- **Frequency**: 2.4 GHz (same as WiFi, but different protocol)
- **Range**: ~10-100m depending on obstacles
- **Data rate**: 250 kbps (slow compared to WiFi)

### RF (Radio Frequency)
Electromagnetic waves used for wireless communication
- **In this course**: 2.4 GHz (IEEE 802.15.4)

### RSSI (Received Signal Strength Indicator)
A measurement of how strong a wireless signal is at the receiver
- **Units**: dBm (decibel-milliwatts)
- **Example**: -30 dBm is strong, -90 dBm is weak
- **Lab 1**: You'll measure RSSI to understand radio performance

### Transmit Power
How much power a radio uses to send signals
- **Units**: dBm
- **Tradeoff**: Higher power = longer range but shorter battery life

### Antenna Gain
How well an antenna focuses radio energy in a particular direction
- **Units**: dBi (decibels relative to isotropic)
- **Higher gain**: More directional, longer range

### Path Loss
How much signal strength is lost as radio waves travel through space
- **Factors**: Distance, obstacles (walls, trees), frequency
- **Formula**: Free-space path loss ∝ distance²

---

## Security

### Encryption
Scrambling data so only authorized parties can read it
- **In this course**: AES-128-CCM for Thread network traffic

### DTLS (Datagram Transport Layer Security)
Encryption protocol for UDP-based communication
- **Used by**: CoAP
- **Like**: TLS (used by HTTPS) but for UDP instead of TCP

### AES-128-CCM
Encryption algorithm used by Thread
- **128**: Key length in bits
- **CCM**: Combines encryption with authentication (ensures data isn't tampered)

### PSK (Pre-Shared Key)
A password that both devices know in advance
- **Used for**: Setting up encrypted connections
- **Challenge**: How do you securely give devices the PSK?

### Provisioning
The process of giving a device its initial configuration (WiFi password, encryption keys, etc.)
- **Challenge in IoT**: Doing this securely and simply (farmers shouldn't need to type 32-character passwords)

### OTA (Over-The-Air) Update
Updating device firmware wirelessly (without physically connecting to it)
- **Why critical**: You can't visit every sensor in a farm to update software

### STRIDE Threat Model
A framework for identifying security threats:
- **S**poofing, **T**ampering, **R**epudiation, **I**nformation disclosure, **D**enial of service, **E**levation of privilege

---

## Course-Specific Terms

### GreenField Technologies
The fictional company you work for in this course
- **Product**: SoilSense Network (soil monitoring for small farms)
- **Your role**: Junior IoT Systems Engineer

### Stakeholders (GreenField)
The people who care about different aspects of your design:
- **Samuel** (Senior Architect) - Technical correctness, ISO alignment
- **Gustavo** (Product Owner) - Cost, customer value
- **Edwin** (Field Operations) - Deployment, reliability
- **Edward** (Security Lead) - Data protection, compliance
- **Daniela** (Pilot Customer) - Ease of use, actionable insights

### Performance Baselines
Specific measurable targets your system must meet
- **Example**: "Mesh network must heal within 60 seconds of node failure"
- **Found in**: [references.md](references.md)

### SOPs (Standard Operating Procedures)
Step-by-step implementation guides for each lab
- **Location**: [labs/sops/](labs/sops/)
- **When to use**: When you need detailed technical instructions

---

## Common Abbreviations

| Abbreviation | Full Term | Meaning |
|--------------|-----------|---------|
| **IoT** | Internet of Things | Connected devices with sensors/actuators |
| **RF** | Radio Frequency | Wireless communication signals |
| **RSSI** | Received Signal Strength Indicator | Measure of signal strength |
| **dBm** | Decibel-milliwatts | Unit for radio power |
| **UDP** | User Datagram Protocol | Fast, connectionless transport |
| **TCP** | Transmission Control Protocol | Reliable, connection-based transport |
| **CoAP** | Constrained Application Protocol | Lightweight HTTP for IoT |
| **CBOR** | Concise Binary Object Representation | Compact data format |
| **MQTT** | Message Queuing Telemetry Transport | Publish/subscribe IoT protocol |
| **PSK** | Pre-Shared Key | Password known by both devices |
| **OTA** | Over-The-Air | Wireless firmware updates |
| **DTLS** | Datagram TLS | Encryption for UDP |
| **ADR** | Architecture Decision Record | Document explaining a design decision |
| **DDR** | Design Decision Record | Living document of all decisions |

---

## Quick Comparisons

### CoAP vs HTTP
| Feature | CoAP | HTTP |
|---------|------|------|
| Transport | UDP | TCP |
| Overhead | Low | High |
| Use case | Battery devices | Web servers |

### Thread vs WiFi
| Feature | Thread | WiFi |
|---------|--------|------|
| Power | Very low | High |
| Range | 10-100m | 50-150m |
| Data rate | 250 kbps | 1+ Gbps |
| Topology | Mesh | Star (router-centric) |
| Best for | Sensors | Phones, laptops |

### IPv4 vs IPv6
| Feature | IPv4 | IPv6 |
|---------|------|------|
| Address length | 32 bits | 128 bits |
| Total addresses | ~4 billion | ~340 undecillion |
| NAT required | Yes | No |
| Auto-config | DHCP | SLAAC (built-in) |

---

## When to Look Things Up

- **Week 0**: Start with "Core IoT Concepts" and "Networking & Protocols"
- **Lab 1**: Focus on "Hardware & Radio" terms (RF, RSSI, link budget)
- **Lab 2**: Review "Networking & Protocols" (IPv6, 6LoWPAN)
- **Lab 3+**: Add "Architecture & Documentation" (ISO domains, ADRs)
- **Lab 6**: Study "Security" section

---

## Still Confused?

1. **Check**: [5_theory_foundations.md](5_theory_foundations.md) for detailed explanations
2. **Ask**: Your instructor during lab sessions
3. **Search**: The [references.md](references.md) quick reference guide
4. **Discuss**: With your lab partner or classmates

---

**Remember**: You don't need to memorize all these terms upfront. Use this glossary as a reference when you encounter unfamiliar words in the labs. By Week 8, these will feel natural!
