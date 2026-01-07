# 🏆 A Centralized Cooperation Model for Disaster Recovery

[![IEEE ANTS](https://img.shields.io/badge/IEEE%20ANTS-2025-blue.svg)](https://ants2025.ieee-ants.org/)
[![Award](https://img.shields.io/badge/Award-Best%20Short%20Paper-gold.svg)](#)
[![Status](https://img.shields.io/badge/Status-Published-green.svg)](#)

> **Winner of the Best Short Paper Award** at the IEEE International Conference on Advanced Networks and Telecommunications Systems (IEEE ANTS 2025), Delhi, India.

![Best Paper Award Certificate](WhatsApp%20Image%202025-12-18%20at%2016.10.18.jpeg)

---

## ⚡ Overview

When disasters strike, cloud service restoration is often hindered by a lack of cooperation. Network Carriers and Datacenter Providers (DCPs) face a dilemma: they need to cooperate to restore services, but they hesitate to share sensitive resource data due to confidentiality and competition.

We propose a **Provider Neutral Exchange (PNE)**—a neutral orchestration entity—powered by a **Multi-Objective Integer Linear Programming (ILP)** model. Where PNE acts as a trusted middleman, ensuring confidentiality while optimizing recovery.

## 🚀 Key Features

* **Trust-Aware Architecture:** Uses a neutral entity (PNE) to mediate between competing stakeholders.
* **Multi-Objective Optimization:** Our ILP model doesn't just "fix" connections; it solves for three conflicting goals:
    1.  **Maximize** restored cloud services.
    2.  **Minimize** restoration time.
    3.  **Minimize** operational costs.
* **Criticality First:** Prioritizes critical connections to ensure essential services are back online immediately.
* **Superior Performance:** Outperforms traditional heuristic strategies in simulated disaster scenarios.

## 🛠️ The Logic

* **Algorithm:** Integer Linear Programming (ILP)
* **Problem Type:** Demand-Resource Matching with Confidentiality constraints
* **Stack:** (Add your specific solver/language here, e.g., Python/Gurobi, CPLEX, Julia)

## 📚 Authors

**Ignazio Maria Castrignano**, Subhadeep Sahoo, Sifat Ferdousi, Sugang Xu, Yusuke Hirota, Massimo Tornatore, Yoshinari Awaji, Biswanath Mukherjee.

*Politecnico di Milano (Italy), University of California, Davis (USA), NICT (Japan).*

## 📄 Citation

If you would like to cite the paper:

```bibtex
@inproceedings{ants2025disaster,
  title={A Centralized Cooperation Model among Datacenter Providers and Carriers for Disaster Recovery},
  author={Castrignano, Ignazio Maria and et al.},
  booktitle={2025 IEEE International Conference on Advanced Networks and Telecommunications Systems (ANTS)},
  year={2025},
  organization={IEEE}
}
