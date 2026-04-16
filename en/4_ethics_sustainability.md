# Ethics & Sustainability in IoT Systems
**Document Type**: Professional Practice Guide
**Audience**: IoT Systems Engineers (Students)
**Purpose**: Integrate ethical considerations into technical decision-making

---

## Why This Matters

> "Just because we *can* collect data doesn't mean we *should*."

As IoT engineers, you're not just writing firmware—you're building systems that observe, measure, and sometimes control the physical world. These systems affect real people, communities, and environments. **Every design decision has ethical implications.**

This document covers three interconnected concerns:
1. **Privacy** - Protecting the people whose data you collect
2. **Security** - Preventing harm from system misuse
3. **Sustainability** - Designing for long-term environmental impact

---

## 1. Privacy by Design

### The Challenge

IoT devices collect **continuous, granular data** about physical spaces and human behavior:

| Device Type | Data Collected | Privacy Risk |
|-------------|----------------|--------------|
| Smart thermostats | Temperature patterns | Infer when you're home/away |
| Security cameras | Video, audio | Surveillance, routine tracking |
| Wearables | Heart rate, location | Health inference, stalking |
| **SoilSense (Your Project)** | Environmental sensors | Could infer farm operations, schedule |

Even "harmless" environmental data can reveal sensitive patterns. Daniela's soil moisture readings might tell competitors when she irrigates, what crops she grows, or whether her farm is thriving.

### Key Privacy Risks

| Risk | Example | Consequence |
|------|---------|-------------|
| **Surveillance** | Data stored indefinitely | Loss of privacy, potential abuse |
| **Inference** | Temperature patterns reveal occupancy | Security vulnerability |
| **Data breach** | Poorly secured cloud database | Personal data leaked |
| **Function creep** | "Monitoring" → targeted ads → insurance pricing | Loss of autonomy |

### Principles: Privacy by Design

**1. Minimize Data Collection**
- Collect only what you need for the stated purpose
- Aggregate early: Send "average moisture" not minute-by-minute readings
- Local processing: Keep raw data on-device when possible

**2. User Control & Transparency**
- Users know what data is collected
- Users can access, export, and delete their data
- Opt-out options exist without breaking core functionality

**3. Security = Privacy**
- Encryption in transit (DTLS) and at rest
- Strong authentication
- Regular security updates (your Lab 6 work!)

### GreenField Application

**Questions to ask during design:**
- [ ] Does Daniela know exactly what data SoilSense collects?
- [ ] Can Daniela export her data and take it to a competitor?
- [ ] If GreenField shuts down, can Daniela still use her sensors?
- [ ] Who else has access to Daniela's farm data? Under what circumstances?

---

## 2. Regulatory Compliance

### GDPR (European Union)

If GreenField expands to the EU, you must comply with GDPR:

| Right | Implication for IoT |
|-------|---------------------|
| **Right to access** | Users can request all data collected about them |
| **Right to erasure** | "Right to be forgotten" - delete user data on request |
| **Data minimization** | Collect only necessary data |
| **Purpose limitation** | Data used only for stated purpose |
| **Penalties** | Up to 4% of global revenue |

### Colombia (Ley 1581 de 2012)

Relevant for GreenField's initial market:

| Principle | Requirement |
|-----------|-------------|
| **Habeas data** | Right to know what data is collected |
| **Prior consent** | Users must explicitly authorize data use |
| **Purpose limitation** | Data used only for stated purpose |
| **Security** | Adequate measures to protect data |

**Regulatory body**: SIC (Superintendencia de Industria y Comercio)

### Design Implications

Your firmware decisions affect legal compliance:

| Decision | Compliance Impact |
|----------|-------------------|
| Cloud vs local storage | Where is data stored? Which jurisdiction? |
| Data retention period | How long before automatic deletion? |
| Third-party integrations | Who has access? Under what terms? |
| Logging level | Are you storing more than necessary? |

---

## 3. Sustainability & E-Waste

### The Scale of the Problem

- **50 million tons** of e-waste generated globally per year
- IoT devices often have **short lifespans** (2-5 years) vs. traditional electronics (10+ years)
- **17%** of e-waste properly recycled globally
- Many IoT devices become e-waste when cloud services shut down

### Contributors to IoT E-Waste

| Factor | Problem | Example |
|--------|---------|---------|
| **Cloud dependency** | Device bricks when service ends | Nest Revolv, many startup IoT products |
| **Firmware abandonment** | No updates after 2 years | Security vulnerabilities, incompatibility |
| **Non-repairable design** | Glued/soldered components | Cannot replace battery |
| **Planned obsolescence** | New version every 2 years | Old devices unsupported |

### Sustainable Design Principles

**1. Design for Longevity**

| Principle | GreenField Application |
|-----------|------------------------|
| Local-first operation | Border Router approach - works without internet |
| Open protocols | Thread/CoAP are standards, not proprietary |
| Modular hardware | Replaceable batteries, standard connectors |
| Repairable | Documentation available for field repair |

**2. Right to Repair**

| Practice | Implementation |
|----------|----------------|
| Accessible documentation | Schematics, repair guides |
| Standard parts | Off-the-shelf components |
| Long update support | Commit to 5+ year firmware support |
| Open firmware | Consider open-sourcing after product EOL |

**3. End-of-Life Planning**

| Consideration | Plan |
|---------------|------|
| Recyclability | Use materials that can be separated |
| Take-back program | Accept old devices for proper disposal |
| Repurposing | Device can be reprogrammed for different use |

**Your ESP32-C6 is fully reprogrammable** - after this course, it becomes a new project, not e-waste.

---

## 4. Ethical Decision Framework

When facing design decisions, ask these questions:

### Data Ethics Checklist

**Collection**
- [ ] Do I *need* this data to provide value, or is it "nice to have"?
- [ ] Can I achieve the same functionality with less data?
- [ ] Am I collecting data at the right granularity?

**Storage**
- [ ] How long do I need to store this data?
- [ ] Where is the data stored? Who can access it?
- [ ] What happens to the data if GreenField is acquired?

**User Agency**
- [ ] Can users opt out without breaking core functionality?
- [ ] Do users understand what data is collected and why?
- [ ] Can users delete their data completely?

**Security**
- [ ] What happens if this device is hacked?
- [ ] How do I handle vulnerabilities discovered after deployment?
- [ ] Is data encrypted in transit and at rest?

**Longevity**
- [ ] Will this device work if our cloud service shuts down?
- [ ] Can users repair or upgrade this device?
- [ ] What is our end-of-life plan?

---

## 5. Case Studies

### Case Study 1: Ring Doorbells (Privacy Failure)

**What happened**: Amazon Ring entered partnerships with police departments, giving law enforcement access to camera footage without warrants or user notification.

**Privacy violation**: Users unaware their footage was shared with police.

**Lesson learned**: Third-party data access must be transparent. Users should explicitly consent to each data-sharing relationship.

**GreenField application**: If agricultural agencies want access to SoilSense data, how do we ensure Daniela explicitly consents?

---

### Case Study 2: Apple HomeKit (Privacy Success)

**Design choices**:
- All automation runs locally on HomePod/AppleTV (no cloud required)
- End-to-end encrypted camera feeds
- Users can disable features and delete data
- No advertising business model

**Lesson learned**: Privacy-first architecture is possible, but requires intentional design effort and business model alignment.

**GreenField application**: Can SoilSense operate fully locally, with cloud as optional enhancement?

---

### Case Study 3: Revolv Smart Home Hub (Sustainability Failure)

**What happened**: Google/Nest acquired Revolv in 2014, then shut down the cloud service in 2016. All Revolv hubs became non-functional.

**Impact**: Customers paid $300 for a device that lasted 2 years, then became e-waste.

**Lesson learned**: Cloud-dependent devices have inherent sustainability risks.

**GreenField application**: What happens to SoilSense devices if GreenField fails? Our local-first Border Router design is a sustainability feature.

---

## 6. GreenField Ethics Scenarios

### Scenario A: Data Access Request

**Situation**: A large agricultural conglomerate offers to pay GreenField for aggregated soil data from all farms in a region.

**Ethical tensions**:
- Revenue could help GreenField survive and improve the product
- Daniela never consented to her data being sold
- Aggregated data might still reveal individual farm information

**Questions for discussion**:
1. Is "anonymized" data truly anonymous when farms have known locations?
2. Should Daniela receive a share of revenue from her data?
3. What consent mechanism would make this acceptable?

---

### Scenario B: Law Enforcement Request

**Situation**: Police investigating illegal water usage request SoilSense data showing irrigation patterns for farms in a drought-restricted area.

**Ethical tensions**:
- Complying might help enforce important environmental regulations
- Daniela uses her data for farm management, not to report to authorities
- Could create chilling effect where farmers avoid using sensors

**Questions for discussion**:
1. Under what circumstances should GreenField comply?
2. Should Daniela be notified of the request?
3. How does this affect user trust in IoT systems?

---

### Scenario C: Feature Creep

**Situation**: Marketing suggests adding microphone to SoilSense "for ambient temperature calibration" but also enabling voice commands later.

**Ethical tensions**:
- Voice commands could be genuinely useful for farmers
- Microphone in agricultural setting could record workers, neighbors
- "Later features" often never materialize; privacy risk is immediate

**Questions for discussion**:
1. How do we evaluate features where privacy cost is immediate but benefit is hypothetical?
2. Should there be a "no surveillance" hardware guarantee (no mic, no camera)?
3. How does this affect user trust?

---

## 7. Integration with Your Labs

Ethics isn't a separate module—it's woven through your technical work:

| Lab | Ethical Dimension |
|-----|-------------------|
| **Lab 1-2** (RF) | Radio range determines where data can be sniffed |
| **Lab 3** (CoAP) | Data format affects what's exposed in traffic analysis |
| **Lab 4** (Sensors) | What data do we collect? At what granularity? |
| **Lab 5** (Border Router) | Local-first design enables operation without cloud |
| **Lab 6** (Security) | Encryption protects privacy; threat model considers attackers |
| **Lab 7** (Dashboard) | User interface determines transparency and control |
| **Lab 8** (Integration) | System-level view of all ethical considerations |

### DDR Integration

In your **Design Decision Record**, include ethical considerations:

**Section 4.5 (Stakeholder Summary - Edward)**: Security and privacy implications of your design choices.

**Section 5 (First Principles)**: Why your data collection approach is proportionate to the functionality provided.

**ADRs**: When making major decisions (e.g., cloud vs. local storage), document ethical tradeoffs alongside technical ones.

---

## 8. Professional Commitment

As IoT engineers at GreenField, commit to:

1. **Question data collection** - Always ask "Do we need this?"
2. **Design for privacy** - Encryption, local processing, user control
3. **Build for longevity** - Reprogrammable, repairable, open protocols
4. **Consider externalities** - E-waste, surveillance, power dynamics
5. **Stay informed** - Laws change, vulnerabilities emerge
6. **Advocate internally** - Push back on features that compromise user trust

**Remember**: Every design decision has ethical implications. Be intentional.

---

## 9. Resources

### Standards & Regulations
- **ISO/IEC 27701** - Privacy Information Management
- **GDPR** - General Data Protection Regulation (EU)
- **Ley 1581 de 2012** - Colombian Data Protection
- **IEEE 7000** - Model Process for Addressing Ethical Concerns

### Further Reading
- "The Age of Surveillance Capitalism" by Shoshana Zuboff
- "Privacy's Blueprint" by Woodrow Hartzog
- EFF (Electronic Frontier Foundation) - eff.org

### Colombian Resources
- **SIC** (Superintendencia de Industria y Comercio) - Data protection guidelines
- **MinTIC** - Technology and communications ministry

---

## Discussion Questions

Use these for team discussions or design reviews:

1. Should IoT devices have a "privacy mode" by default (opt-in to features) or "full functionality" by default (opt-out)?

2. Who is responsible when an IoT device causes harm (e.g., hacked sensor triggers over-irrigation)? Manufacturer? User? Attacker?

3. Is it ethical to design devices with planned obsolescence if it enables lower cost for consumers?

4. Should farmers own their data, or is it fair for the platform that collected it to have some rights?

5. How do we balance innovation (which requires experimentation) with the precautionary principle (avoid harm)?

---

**Next Steps**: Apply these principles in your lab work. Document ethical considerations in your DDR alongside technical decisions.

---

_This document supports ISO/IEC 30141:2024 Trustworthiness Viewpoint implementation._
