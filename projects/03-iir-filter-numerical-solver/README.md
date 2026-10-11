# Project 03: IIR Digital Filter Stability & Numerical Root-Finding

## 📌 Overview
Design and algorithmic analysis of continuous and discrete-time filters. This project focuses on high-precision numerical extraction of complex poles and verification of Bounded-Input Bounded-Output (BIBO) stability for higher-order analog and digital filters.

---

## 🔬 Mathematical Methodology

### 1. Aberth-Ehrlich Multi-Root Finding Algorithm
For a high-order polynomial $P(s) = \sum_{k=0}^n a_k s^k$, traditional iterative Newton-Raphson solvers suffer from divergence near closely spaced complex-conjugate root pairs. The **Aberth-Ehrlich method** applies simultaneous electrostatics-inspired multi-root iteration:

$$s_i^{(k+1)} = s_i^{(k)} - \frac{\frac{P(s_i^{(k)})}{P'(s_i^{(k)})}}{1 - \frac{P(s_i^{(k)})}{P'(s_i^{(k)})} \sum_{j \neq i} \frac{1}{s_i^{(k)} - s_j^{(k)}}}$$

This guaranteed rapid cubic convergence to extract all poles of a **5th-order Bessel low-pass filter** with minimal rounding error.

### 2. Bilinear Transform & Stability Verification
The continuous transfer function $H(s)$ is mapped into the discrete $z$-domain using the Bilinear Transformation:

$$s = \frac{2}{T_s} \frac{1 - z^{-1}}{1 + z^{-1}}$$

Poles were evaluated against the unit circle constraint:

$$\max_i |z_i| < 1 \implies \text{BIBO Stable}$$

### 3. Sensor Noise Mitigation
The resulting IIR filter was benchmarked against synthetic sensor telemetry corrupted by:
* 50 Hz power-line interference.
* High-frequency broadband Gaussian noise.
The recovered signal preserved phase linearity and smooth time-domain group delay characteristics typical of Bessel profiles.
