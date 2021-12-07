# BoAT System Requirements


## Introduction

### Overview
This article describes the system requirements for BoAT Framework (C language version) on cellular modules. BoAT is an SDK that runs on the module's application processor. For the cellular module with an OpenCPU, BoAT is linked and called by the application as a library. For cellular modules without OpenCPU, BoAT APIs will be wrapped into AT commands to work properly in host MCUs.

### Abbreviated Terms
|Term   |Explanation                                            |
|:----- |:----------------------------------------------------- |
|BoAT   |Blockchain of AI Things                                |
|SDK    |Software Development Kit                               |
|API    |Application Programming Interface                      |
|MCU    |Microcontroller Unit                                   |
|RTOS   |Real Time Operating System                             |
|TRNG   |True Random Number Generator                           |
|CSPRNG |Cryptographically Secure Pseudo-Random Number Generator|
|RTC    |Real Time Clock                                        |
|NTP    |Network Time Protocol                                  |
|HTTP   |Hyper Text Transfer Protocol                           |
|HTTPs  |Hyper Text Transfer Protocol Secure                    |
|CoAP   |Constrained Application Protocol                       |
|MQTT   |Message Queuing Telemetry Transport                    |
|TCP    |Transmission Control Protocol                          |
|TEE    |Trusted Execution Environment                          |
|TA     |Trusted Application                                    |
|ECDSA  |Elliptic Curve Digital Signature Algorithm             |
|SHA2   |Secure Hash Algorithm 2                                |


## Part 1- Storage Requirements

To enable Ethereum/PlatONE/FISCO BCOS capabilities, the storage requirements of the C-language BoAT Framework (excluding static or shared dependent libraries such as the C library) are:
- Flash (code and read-only data): about 210kB
- Flash (non-volatile RW data): hundreds of bytes
- RAM (global variables, heap, stack): about 10kB

To enable HyperLedger Fabric capability, the storage requirements of the C-language BoAT Framework (excluding static or shared dependent libraries such as the C library) are:
- Flash (code and read-only data): about 520kB
- Flash (non-volatile RW data): thousands of bytes
- RAM (global variables, heap, stack): about 30kB


The above does not include the system libraries that the BoAT Framework (C language version) depends on. The exact values may vary with different blockchain protocols.

## Part 2- Process Capacity Requirements

In the Ethereum blockchain, the BoAT Framework (C language version) takes about 1 second (excluding network communication time) to complete the cryptographic operations for a blockchain transaction or smart contract call, running on an ARM Cortex M4 at around 100MHz. The exact time can vary with different blockchain protocols.

The exact computing performance requirements depend on the application's power consumption and latency requirements calling (porting in) the BoAT Framework SDK. BoAT itself has no special requirements.

## Part 3- Operating System and Communication Requirements 

There are no special requirements concerning the operating system. Generally, BoAT Framework (C language version) can be ported over most operating systems (e.g., Linux, various RTOS), as long as the following capabilities (below) are supported: 

1. Supports dynamic memory allocation/release. 
2. Supports mutual exclusion (mutex) protection mechanism.
3. Supports thread suspension for a specified duration (optional). BoAT cannot support "timeout" or "polling" functions without the support of the "thread suspension" function, while other functions have no such dependency. 
4. From the candidate random number generators list, select to support at least one type in the order of priority: <br>
   (1) TRNG, true random number generator (requires hardware support) <br>
   (2) CSPRNG, a cryptographically secure pseudo-random number generator. For Linux, this capability can be provided by the OpenSSL library <br>
   (3) PRNG, a (non-cryptographically secure) pseudo-random number generator <br>

5. Depending on a high to low minimum priority, at least one of the following types of times below is supported (for the pseudo-random number seed and the time in the data and log): <br>
   (1) RTC time that can be consistent with the real-time through protocols such as NTP <br>
   (2) RTC time needs to be set manually <br>
   (3) Tick since power on <br>

6. According to priority from high to low, at least one of the following communication protocols is supported: <br>
   (1) HTTP / HTTPS (for Linux, this capability can be provided by the curl library) <br>
   (2) CoAP <br>
   (3) MQTT <br>
   (4) TCP <br>

The communication protocol follows the operator's requirements if the device can only connect to a specific operator or service provider (such as OneNET, OceanConnect).

7. Supports commonly used standard C library functions such as memcpy(), strcpy(), snprintf()(optional for I/O-related functions such as printf()).

8. If the cellular IoT Module utilizes a Linux operating system, it should support adb or similar login mechanism and have root privileges during debugging.

## Part 4- TEE and Remote Attestation (optional)

### TEE

If the application processor of the cellular module supports the TEE (Trusted Execution Environment), the TEE can be used to protect sensitive data such as keys, signatures as well as sensitive processes like encryption. The TEE should support at least the following capabilities:

1. Allowing customers to develop a TA (Trusted Application). For example, some necessary keys are offered.
2. Supporting Secure Boot, fuse, etc.
3. Support for secure storage

### Remote Attestation

Remote attestation is a mechanism that uses the Root of Trust embedded in the chip to provide signature services for device data, and may probe device operating environment characteristic information. Remote attestation can help service providers remotely authenticate the authenticity of equipment. If the module's chip supports remote authentication, it should support at least the following capabilities:

1. Support for signing given data on the device, and verifying the signature on the remote server
2. If the TEE is supported, the data should be signed in TEE (optional)


## Part 5- Cryptography Hardware Acceleration (optional)

If the hardware supports cryptographic hardware acceleration, utilizing this technology will improve the performance of cryptographic operations.

For Ethereum/PlatONE/FISCO BCOS, BoAT needs at least the following cryptographic operations:
1. Elliptic curve signature ECDSA (secp256k1 curve)
2. Keccak-256 (Keccak-256 is a variant of SHA3-256)

For HyperLedger Fabric, BoAT needs at least the following cryptographic operations:
1. Elliptic curve signature ECDSA (secp256r1 curve)
2. SHA2-256


