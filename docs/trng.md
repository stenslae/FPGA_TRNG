# True Random Number Generator (TRNG) Report

## Table of Contents

1. [Introduction](#introduction)
   - [Project Goals](#project-goals)
   - [TRNG Background](#trng-background)

2. [System Architecture](#system-architecture)
   - [VHDL Structure](#vhdl-structure)
   - [Ring Oscillators and MUROs](#ring-oscillators-and-muros)

3. [Randomness Testing](#randomness-testing)
   - [Test Suite Overview](#test-suite-overview)
   - [Results Summary](#results-summary)

4. [Takeaways](#takeaways)
   
5. [References](#references)


## Introduction

### Project Goals

- Software PRNGs are cool. But analog noise is cooler. With this project, a **true random number generator (TRNG)** was implemented on an SoC FPGA. The randomness quality was evaluated using **NIST SP 800-22** standards.

### TRNG Background

- **True Random Number Generators** derive randomness from physical processes, while PRNGs are deterministic and use math.
- This system's primary entropy source is **ring oscillators**, which exploit **thermal noise and jitter in electronics** for unpredictable bit generation.
- These generated bits often contain **bias or correlation**, so post-processing ensures uniform randomness.
- **Von-Neumann correction** removes bias by discarding consecutive identical bits.
- **LFSR-based whitening** spreads entropy across the output bits by using linear feedback.

## System Architecture

### VHDL Structure

![Circuit Diagram](assets/trng_circuit_diagram.png)

- `trng_avalon.vhd` - HPS-to-Fabric interface to expose 32-bit read only register.
  - `trng.vhd` — Generates the TRNG bitstream by using four MUROs XORed together, then doing V-N correction, then putting the bit in a 32-bit LFSR with the polynomial `x^32 + x^22 + x^17 + x^16 + 1`.
    - `muro.vhd` — MURO module structure.
        - `ring_oscillator_async.vhd` - provides an async clock source.
        - `ring_oscillator.vhd` - oscillators with a D-FF triggered by the oscillator-derived clock.

### Ring Oscillators and MUROs

- **8 Ring Oscillators** with 7 inverters each were XORed together to serve as the primary entropy source.
- **MURO (Multi-Ring Oscillator):** seven oscillators sampled with an oscillator-derived divided clock.
- **Theory of Operation:** Ring oscillators are ideally perfect "clocks." But, due to phsyical variations and analog noise, each ring oscillator has a slight timing jitter. When multiple ring oscillators are simultaneously-sampled after a few clock cycles, they will be slightly offset due to accumulated jitter.

![Ring Oscillator Diagram](assets/muro_circuit_diagram.png)

![Image Provided by OpenTRNG.com](assets/muro_timing_diagram.png)

## Randomness Testing

### Test Suite Overview

- [Full Testing Procedure](../sw/testing)

- NIST SP 800-22 statistical test suite (V2.1.2) was used for validation. It verifies the frequency of `1` and `0` bits are equal and checks for correlation, repeatability, and a possible pattern that can be derived from the "random" bitstream. Essentially, the STS will break your heart if it's not statistically random according to federal standards.

- The TRNG passed the NIST SP 800-22 tests! For the final test, a sample size of 100 bitstreams with 500k bits each was used (50M bits total).

### Results Summary

- **Observations:** The FPGA True Random Number Generator is statistically random according to NIST SP 800-22 V2.1.2 Criteria!

| **Test**  | **P-Value** | **Score** | **Pass/Fail?** |
| :--- | :---: | :---: | ---: |
| Frequency | 0.017912 | 98/100 | Pass |
| Block Frequency | 0.851383 | 99/100 | Pass |
| Cumulative Sums (ave) | 0.547965 | 97.5/100 | Pass |
| Runs | 0.834308 | 100/100 | Pass |
| Longest Run | 0.883171 | 100/100 | Pass |
| Rank | 0.236810 | 97/100 | Pass |
| FFT | 0.40119 | 99/100 | Pass |
| Non Overlapping Template (ave) | 0.494875 | 99/100 | Pass |
| Overlapping Template (ave) | 0.851383 | 99/100 | Pass |
| Universal | 0.441199 | 99/100 | Pass |
| Approximate Entropy | 0.678686 | 98/100 | Pass |
| Random Excursions (ave) | 0.239909 | 38.5/39 | Pass |
| Random Excursions Variant (ave) | 0.204264 | 38.7/39 | Pass |
| Serial (ave) | 0.368002 | 99/100 | Pass |
| Linear Complexity | 0.834308 | 100/100 | Pass |

> Note: A score of 96/100 is required to pass the given test (36 for random excursions).

## Takeaways

By using more ring oscillators, better entropy can be derived, but this increases FPGA resources usage. Additionally, ring oscillators are very fragile in certain conditions. For example, ring oscillators in close proximity to each other will experience

Additionally, the sampling frequency of the 


## Resources

- Kubíček, Jiří. "Data Whitening Used in RF." Kubicek Blog, 2024.
- Le, Jin, et al. "An efficient and stable composed entropy extraction method for FPGA-based RO PUF." IEICE Electronics Express, 2020.
- NIST 800-22 Statistical Test Suite.
- OpenTRNG.com
- Sklavos, Nicolas, et al. *Hardware Security and Trust.* Springer, 2017.
- Torii, Naoya, et al. "Implementation and Evaluation of Ring Oscillator-based True Random Number Generator." SOKA University, 2022.
- Varchola, Michal and Milos Drutarovsky. "New High Entropy Element for FPGA Based True Random Number Generators." Technical University of Kosice, 2010.
