# Lab 6: Security & Trustworthiness
> **Technical Guide:** [SOP-06: Security & OTA](sops/sop06_security_ota.md)

**GreenField Technologies - SoilSense Project**
**Phase:** Hardening
**Duration:** 3 hours
**ISO Domains:** RAID (Resource Access), Trustworthiness (Cross-cutting)

---

## 1. Project Context

### Your Mission This Week

**From:** Edward (Security Lead)
**To:** Firmware Team
**Subject:** VULNERABILITY DETECTED

I ran a penetration test on the Pilot Farm network. **It failed.**

I was able to:
1.  Sniff the air and read the exact temperature values (Privacy violation).
2.  Inject a fake packet telling the irrigation valve to "OPEN", flooding Daniela's greenhouse.

**This is a critical stop-ship issue.**
We need **Defense in Depth**. You must implement **DTLS (Datagram Transport Layer Security)** to encrypt the CoAP links.

— Edward

### Stakeholders Counting On You

| Stakeholder | Their Question | How This Lab Helps |
|---|---|---|
| **Edward (Security)** | "Can an attacker inject fake commands?" | DTLS provides Integrity and Authentication. |
| **Daniela (Farmer)** | "Is my farm data private?" | Encryption ensures Confidentiality. |
| **ISO 30141 Auditor** | "Have you addressed Trustworthiness?" | You are implementing the **Trustworthiness Viewpoint**. |

---

## ISO/IEC 30141 Context

### Visual Domain Mapping

```mermaid
graph TD
    subgraph Trustworthiness [Trustworthiness Viewpoint]
        Confidentiality[Encryption (DTLS)]
        Integrity[Message Integrity]
        Availability[DoS Protection]
    end
    subgraph RAID [Communication]
        Client --"Encrypted Channel (DTLS)"--> Server
    end
    Confidentiality -.-> Client
    Integrity -.-> Server
    
    style Trustworthiness fill:#f9f,stroke:#333,stroke-width:2px
    style RAID fill:#bbf,stroke:#333,stroke-width:2px
```

### Trustworthiness Characteristics (ISO/IEC 30141 Section 6.6)

The standard defines seven characteristics that collectively define a trustworthy IoT system. Use this table as a checklist for your DTLS implementation.

| Characteristic | Standard Definition | How DTLS Addresses It | What to Test |
|---|---|---|---|
| **Availability** | System performs its function when required | Graceful degradation if DTLS handshake fails; connection retry logic | Disable the server mid-handshake. Does the client recover? |
| **Confidentiality** | Information is not disclosed to unauthorised entities | DTLS encrypts CoAP payloads; eavesdropper sees only ciphertext | Capture packets with Wireshark. Confirm payload is unreadable. |
| **Integrity** | Data has not been altered or destroyed in an unauthorised manner | DTLS MAC detects any tampering with message contents | Flip a byte in a captured DTLS record. Does the receiver reject it? |
| **Reliability** | System performs consistently under stated conditions | DTLS retransmission compensates for lossy Thread links | Introduce artificial packet loss. Do messages still arrive? |
| **Resilience** | System can withstand and recover from adverse events | Key compromise recovery; ability to rotate PSKs without reflashing | Compromise a PSK. How fast can you rotate to a new one? |
| **Safety** | No unacceptable risk of harm to persons or environment | Authentication prevents fake actuator commands (e.g. "OPEN valve") | Send an unauthenticated "OPEN valve" command. Is it rejected? |
| **Compliance** | Adherence to applicable legislation and regulations | GDPR Art. 32 requires encryption of personal data in transit | Document which data qualifies as personal and confirm it is encrypted. |

> **Note:** The standard treats trustworthiness as a cross-cutting concern across all domains, not a separate domain. Your STRIDE threat model (Task A) maps to these characteristics.

> **For your DDR:** Walk through each row. Document which characteristics your implementation addresses and which remain gaps. A gap is acceptable if you justify the risk; an undocumented gap is not.

---

## 2. Theory Preamble (15 min)
*Reference: [Theory Foundations](../5_theory_foundations.md) > Lab 6: Security & Trustworthiness*

* **STRIDE Model:** Spoofing, Tampering, Repudiation, Info Disclosure, DoS, Elevation of Privilege.
* **The Cost of Security:** A DTLS handshake requires ~6 flights of packets. It is "expensive" in energy. We must minimize how often we handshake.

> **In other stacks:** LoRaWAN encrypts at the network level (AES-128 built into the protocol) — no application-layer DTLS needed. Zigbee uses a network-wide key (weaker model). WiFi/MQTT systems typically use TLS (TCP-based). The Trustworthiness characteristics (confidentiality, integrity, etc.) apply regardless of mechanism.

---

## 3. Execution Tasks

### Task A: The Threat Model (Paper Exercise)
Before coding, go to your DDR. Fill out the **STRIDE Table**.
* *Scenario:* "Attacker with a laptop standing 50m from the greenhouse."
* *Identify:* 3 specific threats.

### Task B: DTLS Implementation (Pre-Shared Key)
Upgrade your CoAP server to **CoAPS** (Port 5684).
* **Key Management:** Use a hardcoded PSK (for lab only). In production, this would be in the Secure Element.
* **Test:** Attempt to read data with a standard CoAP client (Should timeout/fail).
* **Test:** Read data with a CoAPS client and the correct Key (Should succeed).

### Task C: Packet Sniffing
Use the Sniffer.
* **Observe:** The payload should now be "Encrypted Application Data" (Gibberish).

---

## 4. Deliverables (Update your DDR)

* **Threat Model:** The completed STRIDE table.
* **Performance Check:** Measure the time to complete a DTLS handshake. Is it `< 3 seconds`?
* **ADR-006 (Encryption):** Rationale for using Pre-Shared Keys vs. Certificates (Constraint: Flash size and complexity).
* **Trustworthiness Audit:** For each of the 7 ISO/IEC 30141 trustworthiness characteristics, document whether your implementation addresses it and identify any gaps.

---

## 5. Ethics Connection: Privacy Through Security

*Reference: [4_ethics_sustainability.md](../4_ethics_sustainability.md)*

Security is the technical foundation of privacy. Your DTLS implementation protects Daniela's data from:

| Threat | Without DTLS | With DTLS |
|--------|-------------|-----------|
| **Eavesdropping** | Competitor reads soil moisture data | Encrypted - unreadable |
| **Tampering** | Attacker sends fake "irrigate" command | Authenticated - rejected |
| **Data inference** | Patterns reveal farm operations | Protected during transit |

### Reflection Questions (for your DDR Section 11)

1. **Data minimization**: Now that we encrypt data, should we also reduce *what* we collect?
2. **Regulatory compliance**: How does DTLS help us meet GDPR's "security of processing" requirement?
3. **User transparency**: Does Daniela know her data is encrypted? Should she?

**Remember**: Encryption protects data *in transit*. Consider: Is Daniela's data also encrypted *at rest* on the dashboard server?

---

## Grading Rubric (Total: 100 points)

### Technical Execution (40 points)
* [ ] CoAPS (DTLS) server running on port 5684 (15 pts)
* [ ] Handshake successful with PSK (10 pts)
* [ ] Packet sniffing verifies payload is encrypted (15 pts)

### ISO/IEC 30141 Alignment (30 points)
* [ ] Trustworthiness Viewpoint: all 7 characteristics audited (10 pts)
* [ ] Threat Model (STRIDE) completed (10 pts)
* [ ] Gaps identified and documented (10 pts)

### Analysis (20 points)
* [ ] ADR-006 (Encryption) justification (10 pts)
* [ ] Handshake latency measurement (10 pts)

### Ethics Checkpoint (Mandatory Pass/Fail)
* [ ] **Privacy**: Encryption enabled (Privacy protection).
* [ ] **Transparency**: Did you document the security limitations (e.g., PSK management) so stakeholders are aware?
