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

| Command | Description | Options |
|---------|-------------|---------|
| `status` | Check if the secure element is connected | — |
| `logs` | Display the full audit log | — |
| `generate-key` | Generate an ECC key pair in a hardware slot | `--slot <0-31>` `--curve <ed25519\|p256>` |
| `read-key` | Read back the public key from a slot | `--slot <0-31>` |
| `erase-key` | Erase the key stored in a slot | `--slot <0-31>` |

### Quick start

```bash
# Confirm the secure element is connected
hsmctl status

# Generate a key in slot 0 (Ed25519 by default)
hsmctl generate-key --slot 0

# Generate a P-256 key in slot 1 (NIST P-256)
hsmctl generate-key --slot 1 --curve p256

# Read back a public key
hsmctl read-key --slot 0

# Erase a key
hsmctl erase-key --slot 0

# View the audit log
hsmctl logs
```

Every operation is logged automatically to `~/.hsmctl/audit.db`. Running the quick start commands above produces:

```
Audit Log
---------------------------------------------------------------------------
Timestamp             Operation      Result    Options             
---------------------------------------------------------------------------
2026-08-14 21:24:21   status         SUCCESS                       
2026-08-14 21:24:42   generate-key   SUCCESS   slot=0  curve=ed25519
2026-08-14 21:24:55   generate-key   SUCCESS   slot=1  curve=p256  
2026-08-14 21:25:09   read-key       SUCCESS   slot=0              
2026-08-14 21:25:23   erase-key      SUCCESS   slot=0              
---------------------------------------------------------------------------
5 operations logged
```

## Roadmap

**Coming soon:** `list-keys`, `sign`, and `verify` commands.

**Longer term:** multi-device support for additional HSM hardware.
