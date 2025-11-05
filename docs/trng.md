# True Random Number Generator (TRNG) in an FPGA

## Overview

A system was implemented on a DE10-Nano to generate true random numbers on a 32 bit register, and potential exploits were explored. This 32 bit memory-mapped register was then exposed to the HPS for software interfacing.

## System Diagram

## Implementation

## Randomness Testing

## Software Applications

### Encrypting/decrypting files in the userspace
- [Project overview](../sw/encrypt_sw/README.md)
- **Summary:** Takes file and uses a random key to encrypt the file using AES-128-CBC from OpenSSL. Outputs the key, iv, and ciphertext into separate files.

## Exploits

### Fault Injection Attacks
- [Project overview](../attacks/fault_injection/trng/README.md)
- **Summary:**

### Side Channel Attacks
- [Project overview](../attacks/side_channel/trng/README.md)
- **Summary:**

## Acknowledgements

- [Encrypting files](#encryptingdecrypting-files-in-the-userspace) and the [hardware](#implementation) were developed by Emma Stensland at MSU Bozeman for the purposes of EELE 467.
- The driver for the TRNG was developed by Luke Connoly at MSU Bozeman for the purposes of EELE 467.
- See license and [Main Overview](../README.md) for repository acknowledgements.
