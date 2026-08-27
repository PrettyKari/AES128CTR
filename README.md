# AES128CTR
An implementation of AES128CTR by PrettyKari


# AESCrypt

A lightweight C implementation of **AES-128 in CTR mode**, designed with a modular architecture supporting software AES and hardware-accelerated AES-NI backends.

The project is written in C with a focus on low-level implementation details, platform-specific functionality, explicit memory handling, and a clean separation between cryptographic primitives, backend selection, container handling, and platform APIs.

> **Status: Work in Progress**
>
> The core project structure and implementation are in place, but the project is **not yet considered complete**. The existing test cases still require debugging and validation before the implementation can be considered fully verified.

---

## Features

* **AES-128** block cipher implementation
* **AES-128-CTR** mode
* Software AES backend
* **AES-NI** hardware-accelerated backend
* Runtime CPU capability detection
* Automatic backend dispatch
* Cryptographically secure nonce generation
* Platform-specific random generation

  * Linux
  * Windows
* AES-128 key management
* Secure memory clearing for sensitive cryptographic material
* Encrypted file container format
* Separate reader/writer/container layers
* Cross-platform build system using **CMake**
* GCC support on Linux
* MinGW support on Windows
* Modular internal architecture with public and private APIs

---

## Architecture

The project is divided into several layers:

```text
include/aescrypt/
        |
        v
   Public API
        |
        v
src/
├── aes/
│   ├── AES-128 core
│   ├── Software backend
│   ├── AES-NI backend
│   └── CTR mode
│
├── backend/
│   ├── CPU detection
│   └── Backend dispatch
│
├── crypto/
│   ├── Key management
│   ├── Nonce generation
│   └── Random generation
│
├── container/
│   ├── Container format
│   ├── Reader
│   └── Writer
│
├── io/
│   └── File I/O
│
└── platform/
    ├── Linux
    │   ├── CPU support
    │   └── Random generation
    │
    ├── Windows
    │   ├── CPU support
    │   └── Random generation
    │
    └── Platform API
```

The platform layer provides a common API while keeping operating-system-specific functionality isolated from the rest of the implementation.

The backend layer detects available CPU capabilities and selects the appropriate AES implementation.

---

## Cryptographic Design

### AES-128

The project uses AES with a fixed:

* Key size: **128 bits / 16 bytes**
* Block size: **128 bits / 16 bytes**
* Number of rounds: **10**
* Expanded key schedule: **11 round keys / 176 bytes**

The software implementation contains the AES substitution box and the required transformation helpers internally.

### CTR Mode

AES-128-CTR is used for streaming encryption.

The counter block is structured as:

```text
+----------------+----------------+
|     Nonce      |    Counter     |
|     8 bytes    |    8 bytes     |
+----------------+----------------+
        128-bit AES input block
```

The nonce is generated independently and must not be reused with the same AES key.

CTR mode does not require padding and allows plaintext of arbitrary length to be processed.

---

## Backend Selection

The AES implementation supports two backends:

```text
                 CPU capability
                       |
                       v
                +--------------+
                | Backend      |
                | Detection    |
                +--------------+
                   /        \
                  /          \
                 v            v
          AES-NI backend   Software backend
```

When AES-NI is available, the hardware-accelerated implementation can be selected. Otherwise, the software implementation provides a portable fallback.

Platform-specific CPU detection is isolated under:

```text
src/platform/
```

while backend selection itself is handled under:

```text
src/backend/
```

This keeps platform detection separate from cryptographic backend dispatch.

---

## Platform Support

### Linux

The Linux implementation uses the Linux-specific platform APIs for:

* CPU capability detection
* Cryptographically secure random generation

The primary compiler/toolchain target is:

```text
GCC
```

### Windows

The Windows implementation provides:

* CPU capability detection
* Cryptographically secure random generation

The primary compiler/toolchain target is:

```text
MinGW
```

MSVC may also be used where required by the build environment.

---

## Building

The project uses CMake.

### Linux

```bash
mkdir build
cd build
cmake ..
make -j$(nproc)
```

### Windows with MinGW

From a MinGW environment:

```bash
mkdir build
cd build
cmake ..
mingw32-make
```

The exact CMake generator can be selected depending on the installed toolchain.

---

## Project Structure

```text
.
├── CMakeLists.txt
├── DESIGN.md
├── LICENSE
├── README.md
│
├── docs/
│
├── examples/
│   └── main.c
│
├── include/
│   └── aescrypt/
│       ├── aes.h
│       ├── backend.h
│       ├── error.h
│       ├── file.h
│       ├── key.h
│       ├── nonce.h
│       └── version.h
│
├── src/
│   ├── aes/
│   ├── backend/
│   ├── container/
│   ├── crypto/
│   ├── io/
│   └── platform/
│
└── tests/
    ├── test_aes.c
    ├── test_backend.c
    ├── test_container.c
    ├── test_ctr.c
    └── test_roundtrip.c
```

---

## Testing

The repository contains tests covering the major components:

* AES
* Backend detection and dispatch
* CTR mode
* Container functionality
* Encryption/decryption round trips

However, **the current test suite still requires debugging**.

The implementation should therefore be considered **work in progress** until the existing tests have been fully debugged and the cryptographic behavior has been independently validated against known-good test vectors.

---

## Security Considerations

This project is primarily an implementation and engineering exercise and should **not currently be treated as production-grade cryptographic software**.

In particular:

* The test suite still requires debugging.
* Cryptographic behavior requires additional validation.
* The container format requires further review.
* Side-channel resistance has not been comprehensively established.
* Production deployment should use a mature, independently audited cryptographic library instead.

The project is intended to demonstrate the implementation and organization of low-level cryptographic software rather than replace established cryptographic libraries.

---

## AI-Assisted Documentation

AI assistance was used **only for source-code documentation/comments**.

The comments were developed to make the source code more self-documenting and to explain interfaces, responsibilities, and implementation details.

---

## Author

**prettykari**

GitHub: **prettykari**

---

## License

This project is licensed under the **MIT License**.

See [`LICENSE`](LICENSE) for the complete license text.

---

## Disclaimer

This software is provided for educational and research purposes.

Use established, audited cryptographic libraries for security-critical or production systems.
