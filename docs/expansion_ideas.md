# Expansion Ideas

Fun ideas that use the TRNG.

## Hardware:

0. TRNG Characterization
    - Determine actual throughput of this TRNG... (This would be rlly helpful lol)
1. Trojaned TRNG (XOR Tap, Expose to SC, Condition Based Activation)
    - How to get the trojan on the FPGA? (MITM FTP, Bitstream vs HDL etc.)
    - What results can be derived from the trojan? (Lots of papers on this!)
    - What systems could detect/prevent the trojan? (Secure boot, internal FPGA validation?, etc.)
2. Cryptographic Acceleration (AES Cyphers, throughput comparisons, etc.)
3. Hashing to condition the TRNG (SHA-256 is most common!)
4. Fault injection attacks (Clock injection, poke around Quartus's fault injection tool, temperature, power supply, etc.)
5. Side channel analysis. (Power analysis would be cool to try)

## Software:

1. Compare ECDSA effectiveness with good TRNG vs broken TRNG as the nonce (& just learn more about EC).
2. Active TRNG effectiveness monitor.
3. Recreate NSA Dual_EC Backdoor.
