# True Random Number Generator

## Overview

This is a HPS-to-FPGA system for a **True Random Number Generator (TRNG).** Random numbers, with an estimated effective bitrate >60kHz, are generated and rolled into the 32-bit memory-mapped register.

## Address Map Table

| **Register #**  | **Offset** | **Name** | **Physical Address**  |
| :--- | :---: | :---: | ---: |
| 00 | 0x000000 | TRNG Numbers | 0xFF200000 |

## System Overview

1. XOR Chains of 8 **Ring Oscillators** are the primary source of entropy. 
2. **MUlti Ring-Oscillator (MUROs)** were built, where multiple ring oscilators are sampled after a divided count of ring oscillator.
3. The output of the MUROs was XORed together.
4. **Von-Neumann Correction** discards biased bits.
5. **Linear Feedback Shift Register (LFSR)** provides whitening where the ring oscillator bits are the provided "seed" and the LFSR generates a new value in its register via XOR masking with a 32-bit polynomial: `x^32 + x^22 + x^17 + x^16 + 1`

**TRNG.vhd Structure**

![Circuit Diagram](../../docs/assets/trng_circuit_diagram.png)

**MURO.vhd Structure**

![Ring Oscillator Diagram](../../docs/assets/muro_circuit_diagram.png)
