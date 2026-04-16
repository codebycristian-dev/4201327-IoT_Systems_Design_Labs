# Design & Decision Record (DDR)
**GreenField Technologies | IoT Systems Design**

**Team Members:**
1. ____________________
2. ____________________

---

## 1. System Overview

*   **System Type:** [ ] Component (Lab 1-2) | [ ] System (Lab 3-6) | [ ] Environment (Lab 7-8)
*   **Description:**

---

## 2. Lab Log & Stakeholder Summaries

### Lab 1: RF Characterization
*   **To Samuel (Architect):**
*   **To Edwin (Ops):**

### Lab 2: 6LoWPAN
*   **To Samuel:**

### Lab 3: Thread & CoAP
*   **To Daniela (Customer):**

### Lab 4: Sensors & Control
*   **To Edwin:**

### Lab 5: Border Router
*   **To Daniela:**

### Lab 6: Security
*   **To Edward (Security):**

### Lab 7: Dashboard
*   **To Gustavo (Product):**

### Lab 8: Final Integration
*   **To All:**

---

## 3. Architecture Decision Records (ADRs)

**ADR-___: [Title]**
*   **Context:**
*   **Decision:**
*   **Rationale:**
*   **Status:** [ ] Proposed | [ ] Accepted | [ ] Deprecated

---

## 4. ISO/IEC 30141 Mapping

### Domain Mapping

| Component | ISO Domain | Justification |
|-----------|------------|---------------|
|           |            |               |

### Component Capabilities

| Capability Category | Subcategory | Component/Feature | Active/Latent | Lab Introduced |
|---------------------|-------------|-------------------|---------------|----------------|
|                     |             |                   |               |                |

---

## 5. First Principles Reflections

**Lab 1:**
1.
2.

**Lab 2:**
1.

...

---

## 6. Performance Baselines

| Metric | Target | Measured | Status |
|--------|--------|----------|--------|
| Lab 1: Max Range | > 20m | ___ m | [ ] Pass |
| Lab 2: Healing Time | < 120s | ___ s | [ ] Pass |
| Lab 3: CoAP Latency | < 200ms| ___ ms | [ ] Pass |
| Lab 4: Poll Latency | < 5s | ___ s | [ ] Pass |
| Lab 6: DTLS Time | < 3s | ___ s | [ ] Pass |

---

## 7. Ethics & Sustainability Checklist

*   [ ] **Lab 1:** Verified interference doesn't disrupt neighbors.
*   [ ] **Lab 4:** Data collection minimized (Privacy).
*   [ ] **Lab 5:** System works locally without cloud (Sustainability).
*   [ ] **Lab 6:** Encryption enabled (Privacy).
*   [ ] **Lab 8:** End-of-Life plan considered.

---

## 8. Viewpoint Analysis

| Viewpoint | Labs Addressed | Key Concerns Documented |
|-----------|----------------|-------------------------|
| Foundational |            |                         |
| Business |               |                         |
| Usage |                  |                         |
| Functional |             |                         |
| Trustworthiness |        |                         |
| Construction |           |                         |

---

## 9. Trustworthiness Audit (Lab 6+)

| Characteristic | Addressed? | How | Gaps |
|----------------|------------|-----|------|
| Availability |            |     |      |
| Confidentiality |         |     |      |
| Integrity |               |     |      |
| Reliability |             |     |      |
| Resilience |              |     |      |
| Safety |                  |     |      |
| Compliance |              |     |      |

---

## 10. Construction Viewpoint - IoT System Pattern (Lab 8)

| Pattern Element | Category | Your System |
|-----------------|----------|-------------|
| IoT System |            |             |
| IoT Components |         |             |
| Digital Network |        |             |
| IoT Devices |            |             |
| Primary Capability (observation) | |   |
| Primary Capability (control) | |       |
| Secondary Capability (processing) | |  |
| Secondary Capability (transferring) | | |
| Secondary Capability (storage) | |     |
| Interface (network) |    |             |
| Interface (human UI) |   |             |
| Interface (application) | |            |
| Supplemental (security) | |            |
| Supplemental (orchestration) | |       |
| Supplemental (management) | |          |