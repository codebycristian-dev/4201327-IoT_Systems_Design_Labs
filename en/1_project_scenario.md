# GreenField Technologies: Project Briefing

**Document Type**: Project Context & Stakeholder Guide

**Audience**: IoT Systems Engineers (Students)

**Purpose**: Provide realistic development scenario for ISO/IEC 30141-aligned learning

---

## 📌 TL;DR (Quick Summary)

**Your role**: You're a Junior IoT Systems Engineer at GreenField Technologies (a fictional startup)

**Your project**: Build a wireless sensor network for small farms to monitor soil conditions

**Your deliverables**: Professional engineering documentation (DDRs and ADRs), not traditional lab reports

**Your mentor**: Eng. Samuel Cifuentes, who expects you to understand the "why" behind your design choices

**Why this matters**: This scenario gives you real-world context for the technical work you'll do in Labs 1-8.

---

## 1. Company Background

**GreenField Technologies** is a fictional agricultural technology startup founded in 2025. The company's mission is to make precision agriculture accessible to small and medium-scale farms (5-50 hectares) through affordable IoT sensor networks.

### Company Profile
- **Founded**: 2025
- **Market**: Small-scale sustainable farms in temperate climates
- **Competitors**: Established players focus on large industrial farms; GreenField targets the underserved small farm market

### Core Values
1. **Affordability** - Solutions must cost <$50/node to be competitive
2. **Simplicity** - Farmers shouldn't need IT expertise to deploy systems
3. **Sustainability** - Low power consumption, multi-year battery life
4. **Resilience** - Systems must work in remote areas with limited connectivity

---

## 2. The Product: SoilSense Network

### Product Vision
A wireless mesh network of environmental sensors that helps farmers optimize irrigation, reduce water waste, and improve crop yields through data-driven decisions.

### Target Customer Profile
- **Farm size**: 5-50 hectares
- **Crops**: Vegetables, orchards, small-scale grains
- **Tech comfort**: Basic smartphone usage
- **Pain points**:
  - Over-irrigation wastes water and money
  - Under-irrigation reduces yields
  - No real-time visibility into soil conditions
  - Weather stations don't reflect microclimate variations

### Minimum Viable Product (MVP) Requirements
1. **10-node sensor network** monitoring soil moisture and temperature
2. **Self-forming mesh** - farmers just power on devices
3. **3-month battery life** on coin cells
5. **Mobile dashboard** showing current conditions and trends
6. **Alert system** for irrigation scheduling

---

## 3. Your Role: IoT Systems Engineer

### Position Description
You are a **Junior IoT Systems Engineer** on the product development team.

**In simple terms**: You design and build the sensor network—from programming the devices to documenting your architectural decisions. You're not just writing code; you're making design choices and explaining *why* you made them.

**Team Structure:**
- **Labs 1-6**: Work in **pairs** (2 students)
- **Labs 7-8**: Merge into **teams of 4** (2 pairs combine) for system integration

### Your Manager
**Eng. Samuel Cifuentes** (Senior IoT Architect)
- Electronics Engineer, IoT Expert
- Reviews your Architecture Decision Records (ADRs)
- Provides technical guidance during weekly check-ins
- Expects ISO/IEC 30141-aligned documentation
- Values first-principles understanding over "copy-paste" solutions

### Your Team
**Your Partner** (Labs 1-6) / **Your Team** (Labs 7-8)
- **Labs 1-6**: You work with ONE partner
  - Share 3× ESP32-C6 boards
  - Maintain one shared DDR
  - Alternate driver/navigator roles each week

- **Labs 7-8**: Two pairs merge into a team of 4
  - Build larger mesh network (6+ nodes)
  - Integrate different subsystems
  - Combine DDRs into system-level architecture

### Other Teams
**Fellow engineering teams** - Other student pairs/teams in the class
- Share knowledge during code reviews
- Collaborate on infrastructure (border router, cloud integration)
- Compare performance benchmarks

### Your Primary Mentor

**Eng. Samuel Cifuentes** (Senior IoT Architect) - Your technical reviewer
- **What he reviews**: Your Architecture Decision Records (ADRs) and Design Decision Record (DDR)
- **What he cares about**: Understanding the "why" behind your choices, not just the "how"
- **His style**: Mentoring but rigorous—expects professional engineering documentation
- **His favorite question**: "Which ISO domain does this belong to?"

**In practice**: After each lab, you'll update your DDR and Samuel will provide feedback during class sessions. Think of him as your technical manager who wants you to succeed but expects quality work.

### Other Stakeholders (You'll Meet Them in Later Labs)

As your project progresses, you'll need to consider different perspectives from other team members at GreenField Technologies. You'll be introduced to them when their expertise becomes relevant:

- **Gustavo** (Product Owner) - Cares about cost and customer value
- **Edwin** (Field Operations Lead) - Cares about deployment and reliability
- **Edward** (Security Lead) - Cares about data protection
- **Daniela** (Pilot Customer) - The farmer who will actually use your system

**Why introduce them gradually?** In real engineering projects, you don't interact with all stakeholders at once. You'll learn to think from their perspectives as their concerns become relevant to your work.

<details>
<summary><b>📋 Full Stakeholder Reference (Click to expand)</b></summary>

| Stakeholder | Role | Primary Concern | When They Matter Most |
|-------------|------|-----------------|----------------------|
| **Samuel** | Senior Architect | Technical correctness, ISO alignment | Every lab |
| **Gustavo** | Product Owner | Customer value, cost targets | Labs 1, 4, 7-8 |
| **Edwin** | Field Operations Lead | Deployment ease, reliability | Labs 5-8 |
| **Edward** | Security Lead | Data privacy, secure communication | Labs 6-8 |
| **Daniela** | Customer (Pilot Farmer) | Ease of use, actionable insights | Labs 7-8 |

</details>

---

## 4. Project Phases (Mapped to Labs)

### Phase 1: Feasibility & Prototyping (Labs 1-2)

**In plain English**: Does the hardware actually work for farms? How far can devices talk to each other?

**Context**: The hardware team selected ESP32-C6. You need to validate it works in real farm environments.

**Your Tasks**:
- Test the radio: How far can it transmit? What blocks the signal?
- Determine spacing: How far apart can we place sensors on a farm?
- Measure power use: Will batteries last long enough?

**Questions You'll Answer**:
- Samuel: "What's the link budget? Show me the RF propagation model." *(Translation: Prove mathematically that the signal is strong enough)*
- Gustavo: "Can we achieve 100m range between nodes with this hardware?" *(Translation: Will this be good enough for farmers?)*
- Edwin: "What happens if a farmer places a node behind a metal barn?" *(Translation: What are the failure modes?)*

**Deliverables**:
- DDR section documenting your radio testing results
- ADR: "Why we chose channel 15 for mesh operation" (your first decision record!)

**New concepts you'll learn**: RF characterization, link budgets, IEEE 802.15.4 → See [glossary.md](glossary.md)

---

### Phase 2: Network Architecture (Labs 3-4)

**In plain English**: How do devices talk to each other and send data efficiently?

**Context**: Hardware is validated. Now design the mesh network topology and application protocol.

**Your Tasks**:
- Build the mesh network: Devices that relay messages for each other
- Design the data format: How sensors report measurements
- Test resilience: What happens when a device fails?

**Questions You'll Answer**:
- Samuel: "How does the system behave when a router node fails?" *(Translation: Prove your network is resilient)*
- Gustavo: "What's the worst-case latency from sensor to gateway?" *(Translation: Is it fast enough for real-time monitoring?)*
- Edwin: "How long does it take for the network to recover after power loss?" *(Translation: Will farmers get frustrated waiting?)*

**Key Decision Points**:
- ADR: "CoAP vs MQTT for constrained devices" (you'll learn why CoAP wins for battery-powered sensors)
- ADR: "Polling vs push for sensor updates" (should gateway ask for data, or should sensors send it automatically?)

**Deliverables**:
- DDR section with diagrams showing how data flows through your system
- Performance measurements: Network healing time, message latency
- ADRs explaining your protocol choices

**New concepts you'll learn**: Thread mesh, CoAP, CBOR → See [glossary.md](glossary.md)

---

### Phase 3: Integration & Security (Labs 5-6)

**In plain English**: Connecting your sensor network to the Internet and making it secure

**Context**: Pilot customer identified. Need cloud integration and data protection before field deployment.

**Your Tasks**:
- Build the gateway: Connect your mesh network to WiFi and the Internet
- Add encryption: Protect sensor data from attackers
- Design setup process: How do farmers configure devices without technical expertise?

**Questions You'll Answer**:
- Edward: "How do we prevent sensor spoofing attacks?" *(Translation: Can an attacker inject fake data?)*
- Gustavo: "What's the cost of encryption on battery life?" *(Translation: Does security hurt our 3-month battery goal?)*
- Daniela (Farmer): "Do I need to configure WiFi credentials on every sensor?" *(Translation: Is this easy enough for non-technical users?)*

**Critical Concerns**:
- Edwin: "Farmers can't be expected to enter 32-character passwords. How do we make provisioning simple?"
- Edward: "We need GDPR compliance for EU customers. Where is data stored?"

**Deliverables**:
- DDR sections on security and gateway architecture
- Threat model: What attacks are possible and how you mitigate them
- ADR: "Pre-shared keys vs certificate-based authentication"
- Provisioning workflow: Step-by-step setup process

**New concepts you'll learn**: Border routers, DTLS encryption, provisioning, STRIDE → See [glossary.md](glossary.md)

---

### Phase 4: Deployment Readiness (Labs 7-8)

**In plain English**: Putting it all together—does the complete system work?

**Context**: Pilot deployment scheduled in 4 weeks. Final integration and validation required.

**Your Tasks**:
- Build the dashboard: User interface showing sensor data in real-time
- Optimize power: Make batteries last 3+ months
- System testing: Verify everything works together
- Create deployment guide: Instructions for installing sensors on real farms

**Questions You'll Answer**:
- Gustavo: "What's the total bill-of-materials cost per node?" *(Translation: Can we afford to manufacture this?)*
- Edwin: "What's our deployment checklist? What can go wrong in the field?" *(Translation: How do we troubleshoot problems?)*
- Daniela: "Can I see real-time soil moisture on my phone?" *(Translation: Does this actually help me farm better?)*
- Samuel: "Provide a complete system architecture diagram with all six ISO domains" *(Translation: Show me the big picture)*

**Final Deliverable**: **System Integration Report**
- Complete architecture: All components working together
- All six ISO viewpoints: Analyzed and documented
- Performance verification: Meets all targets (battery life, range, latency, etc.)
- Deployment guide: Step-by-step instructions for field installation

**New concepts you'll learn**: Dashboards, system integration, deployment planning → See [glossary.md](glossary.md)

---

## 5. Communication Norms

### Weekly "Design Reviews" (Lab Sessions)
- Present your progress to Samuel (instructor)
- Discuss architectural tradeoffs
- Receive feedback on ADRs and DDRs

### Documentation Standards
Samuel expects **professional engineering documentation**, not academic reports:
- Use ISO/IEC 30141 vocabulary
- Justify decisions with quantitative analysis when possible
- Acknowledge tradeoffs explicitly
- Reference first principles (physics, protocol RFCs, security best practices)

### Cross-Functional Communication
Different stakeholders care about different aspects of your design:

**To Samuel (Architect)**: Technical depth, correctness, standards alignment
- "The mesh routing algorithm uses Trickle timers (RFC 6206) to minimize control traffic..."

**To Gustavo (Product)**: Business impact, cost, performance
- "By using deep sleep, we extended battery life from 6 weeks to 14 weeks, meeting the 3-month target with margin..."

**To Edwin (Operations)**: Reliability, troubleshooting, deployment
- "If a node fails to join the network, check these 3 things in this order..."

**To Edward (Security)**: Threat mitigation, compliance
- "We use AES-128-CCM (aligned with Thread spec) providing both confidentiality and authenticity..."

---

## 6. Success Criteria

### Technical Excellence
- ✅ All performance baselines met (see [References](references.md))
- ✅ System passes integration testing
- ✅ Documentation complete and ISO-aligned

### Professional Growth
- ✅ Can articulate design decisions from multiple viewpoints
- ✅ Understands first-principles "why" behind protocols
- ✅ Produces industry-quality ADRs and DDRs

### Stakeholder Satisfaction
- ✅ Samuel approves your architecture
- ✅ Gustavo confirms product requirements met
- ✅ Edwin has clear deployment procedures
- ✅ Edward validates security implementation

---

## 7. Real-World Grounding

While GreenField Technologies is fictional, the scenario is grounded in reality:

### Realistic Constraints
- **Cost targets** based on actual IoT module pricing
- **Battery life calculations** using real ESP32-C6 power profiles
- **Network performance** validated against 802.15.4 physics
- **Security requirements** aligned with GDPR and industry standards

### Authentic Stakeholder Conflicts
- **Product vs Engineering**: "Can we ship without full encryption to save cost?"
- **Operations vs Engineering**: "Your 'elegant' design requires 5 CLI commands to provision each node!"
- **Security vs Usability**: "Pre-shared keys are secure but farmers will write them on sticky notes..."

### Professional Practices
- ADRs document decisions **before** implementation (not post-hoc justification)
- Baselines define "done" (not subjective assessment)
- Viewpoint analysis ensures all stakeholders are considered

---

## 8. Using This Project Context

- **Write DDRs to Samuel**, addressing his technical concerns
- **Consider all stakeholders** when making design decisions
- **Use professional language**: "I selected CoAP because..." vs "I had to use CoAP for the assignment"
- **Build a portfolio**: These documents showcase your work to future employers

---

## 8b. Business Viewpoint Exercise (ISO/IEC 30141 Section 6.3)

The Business Viewpoint addresses *why* a system exists before engineers decide *how* to build it. ISO/IEC 30141 Table 3 defines three core concerns for this viewpoint. Your task is to answer each one in the context of GreenField Technologies.

**In-class discussion**: Write 200-300 words total covering all three questions below.

### Concern 1 — "How to leverage the various capabilities of an IoT system to provide value for a business?"

GreenField's value proposition is reducing water waste by 30% while maintaining crop yield. Consider how the sensor network creates this value. What is the path from raw data (soil moisture, weather) through insights (analytics, thresholds) to action (valve control, alerts to Daniela's field teams)? Where in that chain does Gustavo's business actually capture value?

### Concern 2 — "How to use IoT for innovative new business models?"

GreenField currently sells hardware (nodes, gateways) to farm operators. But what if they sold "irrigation-as-a-service" instead? Think about what that shift would mean for the system architecture: Who owns the data? What uptime SLAs would Gustavo need to guarantee? How does subscription billing change the requirements for reliability and remote management?

### Concern 3 — "How do characteristics of an IoT system influence business and system owner?"

ISO/IEC 30141 lists emergent characteristics of IoT systems, including scalability, composability, interoperability, and others. Pick two characteristics and explain how they affect Gustavo's business decisions. For example: scalability means GreenField can start with 10-node pilot deployments and grow to 500-node farms without redesigning the core platform. That directly impacts pricing strategy and sales approach.

### Why this matters

The Business Viewpoint is often skipped by engineers, but it determines which technical tradeoffs matter.

Understanding *why* the system is being built prepares you for Labs 1-8, where you will decide *how* to build it. A design choice that looks optimal in isolation (e.g., choosing the lowest-power radio) may be wrong if it conflicts with the business model (e.g., the service model requires real-time dashboards that need higher bandwidth).

---

## 9. Stakeholder Profiles (Detailed)

### Eng. Samuel - Senior IoT Architect
- **Background**: Electronics Engineer, IoT Expert with extensive experience in embedded systems and wireless networks
- **Personality**: Mentoring but rigorous, values learning over perfection
- **Pet peeves**: Hand-waving explanations, copy-paste without understanding
- **Catch phrase**: "Show me the numbers" / "Which domain does this belong to?"

**What he looks for in your DDRs**:
- Correct use of ISO/IEC 30141 terminology
- Quantitative justification (not "seems faster")
- Explicit acknowledgment of tradeoffs
- Evidence of first-principles thinking

---

### Gustavo - Product Owner
- **Background**: MBA, former farmer-turned-entrepreneur
- **Personality**: Optimistic, customer-obsessed, pragmatic about tradeoffs
- **Pet peeves**: Over-engineering, missed deadlines, vague estimates
- **Catch phrase**: "Will farmers pay for this feature?"

**What he cares about**:
- Cost (BOM, development time)
- Customer-facing features
- Time to market
- Competitive differentiation

---

### Edwin - Field Operations Lead
- **Background**: Agricultural engineer, manages pilot deployments
- **Personality**: Detail-oriented, risk-averse, field-tested wisdom
- **Pet peeves**: Solutions that work "in the lab" but fail in the field
- **Catch phrase**: "How do I troubleshoot this at 6 AM in a muddy field?"

**What she cares about**:
- Deployment simplicity
- Failure modes and diagnostics
- Environmental robustness
- Maintenance burden

---

### Edward - Security Lead
- **Background**: Cybersecurity, previously at industrial control systems company
- **Personality**: Skeptical, threat-model driven, pragmatic about risk
- **Pet peeves**: "Security through obscurity", unpatched vulnerabilities
- **Catch phrase**: "What's the attack surface?"

**What he cares about**:
- Data confidentiality and integrity
- Device authentication
- Compliance (GDPR, agricultural data regulations)
- Update mechanisms for vulnerability patches

---

### Daniela - Pilot Customer (Vegetable Farm)
- **Background**: Third-generation farmer, 20-hectare organic vegetable operation
- **Personality**: Practical, tech-curious but not tech-savvy, budget-conscious
- **Pet peeves**: Complicated setup, data without actionable insights
- **Catch phrase**: "Will this actually help me grow better tomatoes?"

**What she cares about**:
- Ease of installation (no IT background)
- Clear, actionable information
- Return on investment
- Reliability (can't babysit technology during harvest season)

---

**End of Project Briefing**

**Next Steps**: Review [README.md](README.md) for how this scenario integrates with the 8-lab curriculum.
