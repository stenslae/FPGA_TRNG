# Software Item 1: Encrypt

## Usage

```bash
./encrypt [-f FILE]
```

### Description
Takes file and uses a random key to encrypt the file in AES-128. Outputs the key and encrypted data into separate files.

### Arguments
- `-h`  
  Show this help message and exit.

- `-f FILE`  
  Specify a text file with any content.

## Building

1. Ensure the FPGA is set up as described in.

2. In this directory, compile `encrypt.c` for the ARM CPU using the following:

```bash
arm-linux-gnueabihf-gcc -o encrypt -Wall encrypt_sw.c -lssl -lcrypto
```

3. Copy this compiled file into the mounted NFS Kernel `~` using the following:

```bash
cp encrypt /srv/nfs/de10nano/nfs-kernel-server/home/soc/
```

4. In the SoC's ARM CPU, follow the [Usage Guide](#usage) to run!