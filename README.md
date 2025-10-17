# ⚡ DC–DC Boost Converter

**Course Project:** EE256 – Power Electronics
**Under:** Dr. Vijay A. S.
**Duration:** Mar 2024 – Apr 2024

A hybrid **hardware–software implementation of a DC–DC Boost Converter**, developed as part of the EE256 Power Electronics course.

The project demonstrates analog and digital control techniques for voltage regulation and compares experimental results with **MATLAB simulations**.
---

## 📌 Project Overview

The **DC–DC Boost Converter** steps up a low-level DC input voltage to a higher output voltage using switch-mode control.

This project was executed in two distinct phases, moving from analog open-loop control to a fully digital closed-loop system:

---

### Phase 1: Hardware PWM Control (TL494 IC)

* **Implementation:** Implemented the boost converter on a breadboard using a **TL494 PWM controller** with a manually adjustable duty cycle.
* **Analysis:** Analyzed converter behavior in **Continuous Conduction Mode (CCM)** and **Discontinuous Conduction Mode (DCM)**.

---

### Phase 2: Digital Closed-Loop Control (Arduino Uno)

* **Closed-Loop System:** Replaced the analog PWM generator with an **Arduino Uno**, implementing closed-loop voltage regulation via **ADC feedback** and software-controlled PWM.
* **Protection Logic:** Added **auto-cutoff** and protection logic for under-voltage and charge-completion conditions.

---

### Phase 3: MATLAB Validation

* **Simulation:** Simulated the Arduino-based control loop and converter circuit within **MATLAB Simulink**.
* **Verification:** Validated the output voltage regulation and transient response against experimental data collected from the hardware prototype.

---
## ⚙️ Key Features

The implemented DC–DC Boost Converter includes the following core functionalities:

* **Closed-Loop Regulation:** Achieves precise output voltage control through **real-time feedback** of the output voltage, which is used to dynamically adjust the **PWM duty cycle** via the Arduino Uno.
* **Mode Adaptation:** Features advanced logic for **automatic switching** between buck and boost operational modes, adapting based on the instantaneous input and desired output voltage levels.
* **Safety Logic:** Enhanced system safety via **auto-cutoff** functionality, triggered by either low input voltage detection or the detection of charge-completion in the load (e.g., battery charging).
* **MATLAB Verification:** The simulation model in MATLAB confirmed **consistent regulation characteristics** and a stable **transient response** that accurately matched the performance observed on the physical hardware.

---
## 🧰 Tools & Technologies
Hardware: **TL494 PWM IC**, **Arduino Uno**, **MOSFET Driver**, **Inductor**, **Diode**, **Capacitor**
Software: **Arduino IDE (C++)**, **MATLAB Simulink**
Key Concepts: **PWM Control**, **ADC Feedback**, **Closed-Loop Regulation**, **CCM/DCM Analysis**

---

## 📊 Results & Observations

The project successfully demonstrated the core functionalities across both hardware implementations and simulation environments:

* **TL494 Implementation:** Successfully verified the expected **CCM (Continuous Conduction Mode) and DCM (Discontinuous Conduction Mode) waveforms** and confirmed the theoretical voltage-boost relationship with respect to the manually adjusted **duty cycle**.
* **Arduino Implementation:** Achieved **automatic voltage regulation** for the output, maintaining the voltage reliably within the set operational thresholds (e.g., **11 V–13 V range**).
* **MATLAB Simulation:** The simulated output voltage and transient behavior (such as startup and response to load changes) **closely matched** the experimental data and performance recorded during the Arduino hardware testing.

---

## 🚀 Future Work

* **Advanced Control:** Implement full **PID (Proportional-Integral-Derivative) digital control** to achieve significantly finer and more stable output voltage regulation compared to the current implementation.
* **Performance Scaling:** Port the existing control design and code to **higher-performance microcontrollers** (such as the **ESP32** or **STM32**) to leverage faster processing speeds and dedicated PWM hardware for more efficient switching and complex control algorithms.
