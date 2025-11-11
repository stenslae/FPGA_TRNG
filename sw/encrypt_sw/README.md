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

- `-v`
  Print the IV, key, and ciphertext.

- `-f FILE`  
  Specify a text file with any content.

## Building

1. Ensure the FPGA is booted with the appropriate .rbf file, and you have access to the kernel.

2. Install all OpenSSL dependencies:

```bash
sudo studio apt install
sudo dpkg --add-architecture armhf
sudo apt-get update
sudo apt install openssl
sudo apt-get install libssl-dev:armhf
```

3. In this directory, run make to get the executable for the ARM CPU using the following:

```bash
make CC=arm-linux-gnueabihf-gcc
```

3. Copy this compiled file into the mounted NFS Kernel's home using the following:

```bash
cp encrypt /srv/nfs/de10nano/nfs-kernel-server/home/soc/
```

4. In the SoC's ARM CPU, follow the [Usage Guide](#usage) to run!

5. To test the encryption, take the `ciphertext.bin`, `key.hex`, and `iv.hex` outputs and run the following.

```bash
openssl enc -aes-128-cbc -d -in ciphertext.bin -out decrypt.txt -K $(cat key.hex) -iv $(cat iv.hex) -p
```