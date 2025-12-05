# 🔒 FPGA True Random Number Generator (TRNG)

## Project Purpose

- [Full Report](docs/trng.md) 
 
- A custom IP core was implemented on the **DE10-Nano** to generate random numbers on a 32-bit register, and potential exploits were explored. This 32-bit memory-mapped register was exposed to the HPS for software interfacing.

## Overview

- **Platform:** DE10-Nano FPGA (Cyclone V SoC)
- **Communication:** HPS-FPGA interface, NFS and TFTP Servers over Ethernet  
- **Development Tools:** Quartus Prime, OpenSSL, Linux for HPS  
- **Userspace Languages:** C, Python, Lua
- **Kernel Drivers:** Custom drivers for hardware interaction and communication between HPS and FPGA  
- **IP Cores:** Includes custom FPGA IP cores developed in VHDL

## Concepts Explored:
- NIST STS Tests
- Metastability Sampling vs Ring Oscillator TRNGs
- Von-Neumann Correction/Whitened LFSR Post-Mixing
- AES-128-CBC Encryption/Decryption with OpenSSL

## Acknowledgements

- Repository template provided by Trevor Vannoy of MSU Bozeman for the purposes of **EELE 467**.

