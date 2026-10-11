# Mohamed Ahmed Galal



<p align="center">
  <b>Electronics & Communications Engineering Undergraduate</b><br>
  Egypt-Japan University of Science and Technology (E-JUST) | CGPA: 3.44<br>
  📍 Alexandria, Egypt &nbsp;|&nbsp; 
  <a href="mailto:mohamed.galal.eng12@gmail.com">mohamed.galal.eng12@gmail.com</a> &nbsp;|&nbsp; 
  <a href="https://www.linkedin.com/in/mohamed-ahmed-galal-49a585332/">LinkedIn Profile</a> &nbsp;|&nbsp; 
  <a href="https://github.com/Mohamed-Ahmed-44">GitHub Profile</a>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Verilog-HDL-blue?style=for-the-badge&logo=cpu" alt="Verilog" />
  <img src="https://img.shields.io/badge/SystemVerilog-Verification-purple?style=for-the-badge&logo=microchip" alt="SystemVerilog" />
  <img src="https://img.shields.io/badge/ESP32-IoT%20Firmware-green?style=for-the-badge&logo=espressif" alt="ESP32" />
  <img src="https://img.shields.io/badge/MATLAB-DSP%20%26%20Math-orange?style=for-the-badge&logo=mathworks" alt="MATLAB" />
  <img src="https://img.shields.io/badge/Altium-PCB%20Design-brightgreen?style=for-the-badge&logo=altium-designer" alt="Altium" />
  <img src="https://img.shields.io/badge/Python-Scripting-yellow?style=for-the-badge&logo=python" alt="Python" />
</p>

---

## 📌 About Me

I am an Electronics and Communications Engineering student at the **Egypt-Japan University of Science and Technology (E-JUST)** with a strong background in:
- **Digital Integrated Circuit Design & Verification (RTL)**: Verilog HDL, SystemVerilog testbenches, FSM design, synchronous FIFO architectures, ALU modeling.
- **Embedded Systems & IoT**: ESP32, sensor instrumentation (ACS712, DS18B20, pyranometer), cloud telemetry integration (Firebase RTDB).
- **Digital Signal Processing (DSP) & Numerical Analysis**: Filter design, MATLAB Aberth-Ehrlich complex root-finding algorithms, bilinear transformations, stability analysis.
- **PCB Design & Hardware Engineering**: Schematic capture, PCB routing, component selection, manufacturing and assembly rules (trained at the **Egyptian Space Agency - EgSA**).

---

## 📁 Repository Structure

```
├── assets/
│   ├── images/                # Architecture diagrams, waveform snapshots, certificates
│   └── docs/                  # Curriculum Vitae & supplementary documentation
├── projects/
│   ├── 01-synchronous-fifo-and-digital-design/
│   │   ├── rtl/               # Verilog / SystemVerilog hardware modules (NTI labs & final project)
│   │   └── docs/              # FIFO block diagram & architecture spec PDF
│   ├── 02-iot-solar-monitoring-system/
│   │   ├── firmware/          # ESP32 C++ firmware with WiFi and Firebase integration
│   │   └── analysis/          # Sensor data acquisition, MATLAB analysis, and reports
│   ├── 03-iir-filter-numerical-solver/
│   │   └── matlab/            # 5th-order Bessel filter & Aberth-Ehrlich root-solver
│   └── 04-egsa-pcb-design-workshop/
│       └── README.md          # Egyptian Space Agency PCB engineering training review
└── README.md
```

---

## 🚀 Featured Engineering Projects

### 1. Synchronous FIFO Memory & Verilog Digital Design Suite
> **NTI (National Telecommunication Institute) Training & Capstone Project**  
> *Skills: Verilog HDL, SystemVerilog Verification, Digital Logic Design, FSM, Synthesis*

<p align="center">
  <img src="assets/images/fifo_architecture.png" alt="Synchronous FIFO Architecture" width="85%" />
</p>

* **Synchronous FIFO Core** ([`synchronous_fifo_top.v`](projects/01-synchronous-fifo-and-digital-design/rtl/synchronous_fifo_top.v)):
  * Configurable depth and width parameterization.
  * Robust pointer management (`write_pointer`, `read_pointer`) with synchronous status flag logic (`full`, `empty`, `almost_full`, `almost_empty`).
  * Circular dual-port RAM core preventing overflow and underflow read hazards.
* **Arithmetic Logic Unit (ALU)** ([`alu.v`](projects/01-synchronous-fifo-and-digital-design/rtl/alu.v)):
  * 8-bit parameterized ALU supporting arithmetic, logic, and comparison operations with zero-flag generation (`a_zero`).
* **Subsystem Integration** ([`ram_sipo_piso_alu_system.v`](projects/01-synchronous-fifo-and-digital-design/rtl/ram_sipo_piso_alu_system.v)):
  * Integrated RAM block, Serial-In Parallel-Out (SIPO), Parallel-In Serial-Out (PISO) shift registers, and ALU data pipelines.
* **Sequential Detectors & Control FSMs**:
  * Mealy sequence detectors with both overlapping ([`seq_detector_mealy_overlap.v`](projects/01-synchronous-fifo-and-digital-design/rtl/seq_detector_mealy_overlap.v)) and non-overlapping ([`seq_detector_mealy_nonoverlap.v`](projects/01-synchronous-fifo-and-digital-design/rtl/seq_detector_mealy_nonoverlap.v)) states.
  * Rising-edge pulse detector (`rising_edge_detector.v`) and stream parity generator (`stream_parity_generator.v`).
  * Configurable clock divider and LED light-chaser module (`light_chaser.v`).
* **SystemVerilog Verification** ([`verification_tb.sv`](projects/01-synchronous-fifo-and-digital-design/rtl/verification_tb.sv)):
  * Testbench environment applying automated stimulus and assertions to check RTL functionality.

📁 **[Explore RTL Source Files & Documentation →](projects/01-synchronous-fifo-and-digital-design/)**

---

### 2. IoT-Based Solar Panel Real-Time Performance Monitoring System
> *Skills: ESP32 Microcontroller, C++, FreeRTOS, Firebase RTDB, Sensor Signal Conditioning*

* **Hardware Instrumentation**:
  * Built real-time data acquisition circuits for photovoltaic panels using **ACS712** current sensor, precision resistor voltage divider, **DS18B20** temperature sensor, and solar irradiance sensors.
  * Implemented 200-sample zero-current baseline calibration to eliminate noise and DC offset drift.
* **Firmware Architecture** ([`solar_dashboard.ino`](projects/02-iot-solar-monitoring-system/firmware/solar_dashboard.ino)):
  * Computes actual versus expected power output based on real-time solar irradiance and temperature coefficients.
  * Connects over WiFi with secure authentication to **Google Firebase Realtime Database (RTDB)** for cloud streaming and diagnostics.
  * Embeds an internal web server for local telemetry monitoring without external cloud dependencies.
* **Signal Analysis & Validation** ([`signal_analysis.m`](projects/02-iot-solar-monitoring-system/analysis/signal_analysis.m)):
  * Processed sampled telemetry in MATLAB with FFT analysis and resampling to detect efficiency degradation and shading anomalies.

📁 **[Explore Firmware & Analysis Scripts →](projects/02-iot-solar-monitoring-system/)**

---

### 3. IIR Digital Filter Stability & Numerical Root-Finding
> *Skills: MATLAB, Complex Analysis, Digital Signal Processing, Filter Synthesis*

* Engineered a **MATLAB Aberth-Ehrlich numerical solver** capable of extracting complex poles and zeros of high-order systems (specifically a 5th-order Bessel filter) with rapid multi-root convergence.
* Formulated the **Bilinear Transform** mapping from the continuous s-domain to the discrete z-domain, mathematically verifying bounded-input bounded-output (BIBO) stability within the unit circle $|z| < 1$.
* Demonstrated sensor noise suppression by isolating micro-volt low-frequency signals from severe high-frequency interference and 50/60 Hz power-line hum.

📁 **[Explore Filter Implementation →](projects/03-iir-filter-numerical-solver/)**

---

### 4. Spacecraft Electronics & PCB Design Engineering Workshop
> **EgSA — Egyptian Space Agency (New Cairo, Egypt)**  
> *Skills: Altium Designer, Schematic Capture, High-Reliability PCB Routing, Design for Manufacturing (DFM)*

Completed intensive professional hands-on training with Egyptian Space Agency aerospace engineers:
* **Fundamentals & Component Selection**: Thermal characteristics, tolerance budgeting, space-grade and industrial electronic components.
* **Schematic Architecture**: Strict design rule checks (DRC), netlabel hierarchies, and power rail filtering.
* **PCB Layout & Routing**: High-speed trace impedance considerations, ground plane integrity, decoupling capacitor placement, and parasitic reduction.
* **Manufacturing & Assembly (DFM/DFA)**: Gerber file generation, drill tables, solder mask tolerances, testing, board bring-up, and hardware debugging.

📁 **[Explore Workshop Overview & Technical Notes →](projects/04-egsa-pcb-design-workshop/)**

---

## 🛠️ Technical Competencies

| Domain | Technologies & Tools |
| :--- | :--- |
| **HDL & IC Design** | Verilog, SystemVerilog, ModelSim / QuestaSim, Vivado, FSM Architecture, FIFO, ALU |
| **Embedded & IoT** | ESP32, Arduino C++, FreeRTOS, Firebase RTDB, I2C, SPI, UART, ADC Calibration |
| **Hardware & PCB** | Altium Designer, LTspice, Schematics, Multilayer PCB Routing, DFM/DFA |
| **Mathematics & DSP** | MATLAB, Python, Numerical Methods, FFT, Digital Filters, Control Theory |
| **Languages** | English (B2 Professional), Arabic (Native) |

---

## 🎓 Education & Certifications

* **B.Sc. in Electronics and Communications Engineering**  
  *Egypt-Japan University of Science and Technology (E-JUST)* | Alexandria, Egypt  
  *Expected Graduation: June 2028* | **CGPA: 3.44 / 4.00**  
  *Relevant Coursework*: Digital Logic Design, Signals and Systems, Probability & Statistics, Numerical Analysis, Microprocessors & Microcontrollers.
* **PCB Design Training Workshop Certificate**  
  *Egyptian Space Agency (EgSA)* (Jan 2026 – Feb 2026)
* **Digital Design & Verification Training**  
  *National Telecommunication Institute (NTI)*

---

## 📬 Contact & Links

* 📧 Email: [mohamed.galal.eng12@gmail.com](mailto:mohamed.galal.eng12@gmail.com)
* 📱 Phone: +20 122 567 2116
* 💼 LinkedIn: [Mohamed Ahmed Galal](https://www.linkedin.com/in/mohamed-ahmed-galal-49a585332/)
* 🐙 GitHub: [@Mohamed-Ahmed-44](https://github.com/Mohamed-Ahmed-44)
* 📄 Full Curriculum Vitae: [Download CV (DOCX)](assets/docs/Mohamed_Ahmed_Galal_CV.docx)
