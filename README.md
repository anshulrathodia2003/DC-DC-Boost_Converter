# ⚡ DC-DC Boost Converter

**Course Project:** EE256  
**Under:** Dr. Vijay A. S.  
**Duration:** Mar 2024 – Apr 2024  

A hardware and software implementation of a **DC-DC Boost Converter** designed as part of the EE256 course project.  
This project demonstrates the design, implementation, and analysis of a boost converter in both **hardware (breadboard prototype)** and **software (MATLAB Simulink)** domains.

---

## 📌 Project Overview

The DC-DC Boost Converter is a type of **switch-mode power supply** that steps up (boosts) a lower input DC voltage to a higher output DC voltage.  
Our project involved:

- Implementing the boost converter on a breadboard.
- Designing and building a **Pulse Width Modulator (PWM)** using the TL494 IC.
- Simulating and analyzing the converter in **MATLAB Simulink**.
- Studying the converter behavior under **Continuous Conduction Mode (CCM)** and **Discontinuous Conduction Mode (DCM)**.

---

## ⚡ Key Features

- **Hardware Implementation**
  - Breadboard prototype using inductor, diode, MOSFET, capacitor, and load resistor.
  - PWM generation using **TL494 IC** for gate control.
  - Measurement of input/output voltages and duty cycle control.

- **Software Simulation**
  - MATLAB Simulink model replicating the hardware circuit.
  - Parameter tuning for duty cycle, switching frequency, and load.
  - Observation of inductor current, output voltage ripple, and efficiency.

- **Waveform Analysis**
  - Compared simulation and experimental results.
  - Analyzed waveforms for both CCM and DCM modes.

---

## 🛠️ Hardware Components Used

| Component           | Description / Rating |
|--------------------|---------------------|
| **TL494 IC**       | PWM controller for gate drive |
| **Inductor (L)**   | 220 µH (approx.) |
| **MOSFET**         | IRF540N (N-channel) |
| **Diode**          | Schottky Diode (Fast Recovery) |
| **Capacitor (C)**  | 470 µF, 25V |
| **Load Resistor**  | 10 Ω |
| **Power Supply**   | 5V–12V DC |

---

## 🖥️ Software Used

- **MATLAB Simulink** – for converter modeling and waveform analysis  
- **LTSpice / Multisim (Optional)** – for pre-verification of design parameters  
- **Digital Oscilloscope** – for waveform observation  

---

## ⚙️ Working Principle

The converter works in two phases:

1. **Switch ON:** Current flows through the inductor, storing energy in its magnetic field.  
2. **Switch OFF:** Inductor releases stored energy, boosting the voltage across the load.

The output voltage \( V_{out} \) is given by:

\[
V_{out} = \frac{V_{in}}{1 - D}
\]

Where **D** is the duty cycle of the PWM signal.

---

## 📊 Results & Observations

- **Boosted Output:** Successfully stepped up input voltage (e.g., 5V → ~9V at 60% duty cycle).  
- **Waveform Validation:** Observed expected current rise during switch ON and voltage boost during switch OFF.  
- **CCM vs DCM:** Verified theoretical predictions — CCM showed continuous current flow, whereas DCM exhibited zero current intervals.  

---

## 📈 Future Improvements

- Replace TL494 with a microcontroller-based PWM generator (Arduino / STM32) for better flexibility.
- Improve efficiency using low-RDS(on) MOSFETs.
- Develop a PCB for a more robust and compact prototype.

---

## 📜 License

This project was created as part of an academic course and is intended for educational purposes.  
Feel free to use and modify it for learning and non-commercial applications.

