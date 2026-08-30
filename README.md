# hsmctl

A command line interface tool for Hardware Security Module key management and audit logging. 

Built on the [TROPIC01](https://www.tropicsquare.com/tropic01) secure element, hsmctl provides hardware backed key lifecycle management through key generation, reading, and erasure, with a persistent audit trail of every operation. Private keys are generated inside the secure element chip and never leave the hardware.

## Hardware Requirements

The following hardware is required in order to use hsmctl: 

- Raspberry Pi - available at [Raspberry Pi Foundation](https://www.raspberrypi.com/)

- TROPIC01 Raspberry Pi Shield - available at [Tropic Square](https://www.tropicsquare.com/order-devboard-form)

The TROPIC01 shield mounts directly onto the Raspberry Pi GPIO header and communicates over SPI. You can get started with the TROPIC01 with the tutorials provided [here](https://tropicsquare.github.io/libtropic/latest/tutorials/linux/spi/).

## Dependencies

```bash
sudo apt install git
sudo apt install build-essential
sudo apt install cmake
sudo apt install libsqlite3-dev
sudo apt install libssl-dev
```

## Building and installing

```bash
git clone https://github.com/TuckerLucas/hsmctl
cd hsmctl
mkdir build && cd build
cmake ..
make
sudo make install
```

## Command Reference

| Command | Description | Usage |
|---------|-------------|---------|
| `status` | Check if the secure element is connected | — |
| `logs` | Display the full audit log | — |
| `generate-key` | Generate an ECC key pair in a hardware slot | `--slot <0-31>` `[--curve <ed25519\|p256>]` |
| `erase-key` | Erase the key stored in a slot | `--slot <0-31>` |
| `read-key` | Read back the public key from a slot | `--slot <0-31>` |
| `list-keys` | List all public keys stored on the HSM | `[--verbose]` |
| `sign` | Sign data or a file using a hardware backed key | `--slot <0-31>` `<--data <data> \| --file <path>>` |

### Quick start

```bash
# Confirm the secure element is connected
hsmctl status

# Generate an ECC key pair in slot 0 (Ed25519 by default)
hsmctl generate-key --slot 0

# Generate a NIST P-256 ECC key pair in slot 1
hsmctl generate-key --slot 1 --curve p256

# Generate an Ed25519 ECC key pair in slot 29
hsmctl generate-key --slot 29 --curve ed25519

# Read back the public key from slot 0
hsmctl read-key --slot 0

# Erase the key from slot 0
hsmctl erase-key --slot 0

# List all public keys stored on the HSM
hsmctl list-keys

# Sign a message using the key in slot 29
hsmctl sign --slot 29 --data "hello"

# Create a sample file to sign 
echo "hello hsmctl" > sample.txt

# Sign the file using the key in slot 29
hsmctl sign --slot 29 --file sample.txt

# View the audit log
hsmctl logs
```

The `list-keys` operation presents a list of all public keys stored on the HSM, producing an output similar to the following:

```
------------------------------------------------------------
Slot    Curve     Public Key
------------------------------------------------------------
1       P-256     bf98aad487c19154...f3b8a20a
29      Ed25519   f11fe3f0c136961f...b720f27a
------------------------------------------------------------
2 key(s) found
```

All operations are logged automatically to `~/.hsmctl/audit.db` and can be retrieved using `logs`. Running the quick start commands above produces:

```
Audit Log
---------------------------------------------------------------------------
Timestamp             Operation      Result    Options             
---------------------------------------------------------------------------
2026-08-18 10:40:35   status         SUCCESS                       
2026-08-18 10:40:47   generate-key   SUCCESS   slot=0  curve=ed25519
2026-08-18 10:41:00   generate-key   SUCCESS   slot=1  curve=p256  
2026-08-18 10:41:23   generate-key   SUCCESS   slot=29 curve=ed25519
2026-08-18 10:41:49   read-key       SUCCESS   slot=0              
2026-08-18 10:42:08   erase-key      SUCCESS   slot=0              
2026-08-18 10:42:28   list-keys      SUCCESS
2026-08-18 10:42:45   sign           SUCCESS   slot=29 type=data   
2026-08-18 10:43:01   sign           SUCCESS   slot=29 type=file path=sample.txt               
---------------------------------------------------------------------------
9 operations logged
```

## Roadmap

**Coming soon:** `verify` operation.

**Longer term:** multi-device support for additional HSM hardware.
