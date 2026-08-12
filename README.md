# hsmctl

A command line interface tool for Hardware Security Module key management and audit logging. 

Built on the [TROPIC01](https://www.tropicsquare.com/tropic01) secure element, hsmctl provides hardware backed key lifecycle management through key generation, reading, and erasure, with a persistent audit trail of every operation. Private keys are generated inside the secure element chip and never leave the hardware.

## Hardware Requirements

- **Raspberry Pi** - available at [Raspberry Pi Foundation](https://www.raspberrypi.com/)
- **TROPIC01 Raspberry Pi Shield** - available at [Tropic Square](https://www.tropicsquare.com/order-devboard-form)

The TROPIC01 shield mounts directly onto the Raspberry Pi GPIO header and communicates over SPI. You can get started with the TROPIC01 with the tutorials provided [here](https://tropicsquare.github.io/libtropic/latest/tutorials/linux/spi/).

## Dependencies

SQLite3 is required and must be installed before building:

```bash
sudo apt install libsqlite3-dev
```

## Building

```bash
git clone https://github.com/yourname/hsmctl
cd hsmctl
mkdir build && cd build
cmake ..
ninja
```

## Command Reference

### status

Check if the TROPIC01 secure element is connected and responding.

```bash
hsmctl status
hsmctl status --help
```

### generate-key

Generate an ECC key pair in the specified slot. The private key is generated inside the hardware and never leaves the secure element. The public key can be retrieved with `read-key`.

```bash
hsmctl generate-key --slot <0-31>
hsmctl generate-key --slot <0-31> --curve <ed25519|p256>
hsmctl generate-key --help
```

### read-key

Read back the public key from a slot.

```bash
hsmctl read-key --slot <0-31>
hsmctl read-key --help
```

### erase-key

Erase the key stored in the specified slot.

```bash
hsmctl erase-key --slot <0-31>
hsmctl erase-key --help
```

### logs

Display the audit log of all operations performed. 

```bash
hsmctl logs
hsmctl logs --help
```

The audit log persists across sessions in `~/.hsmctl/audit.db`, presenting the user with an output similar to the one below:

```
Audit Log
--------------------------------------------------------------------
Timestamp             Operation      Result    Options
--------------------------------------------------------------------
2026-08-11 20:18:41   status         SUCCESS
2026-08-11 20:18:44   generate-key   SUCCESS   slot=1  curve=ed25519
2026-08-11 20:18:47   read-key       SUCCESS   slot=1
2026-08-11 20:18:49   erase-key      SUCCESS   slot=1
--------------------------------------------------------------------
4 operations logged
```

## Roadmap

**Coming soon:** `list-keys`, `sign`, and `verify` commands.

**Longer term:** multi-device support for additional HSM hardware.
