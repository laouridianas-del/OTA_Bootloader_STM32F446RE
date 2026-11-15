# STM32F446RE – Custom OTA Bootloader

**A Custom Embedded Bootloader for Firmware Validation, Flash Memory Management, and OTA-Style Firmware Updates.**

## 1. Project Overview

This project focuses on the design and implementation of a custom bootloader for the **STM32F446RE (ARM Cortex-M4)** microcontroller.

The main objective is to develop a firmware update mechanism that allows the microcontroller to install a new application without requiring direct reprogramming through an external debugger.

The bootloader is responsible for validating the installed application, managing internal Flash memory, handling firmware installation, and transferring execution to the application when the firmware is valid.

The application validation mechanism relies on three main parameters:

- **Magic Number:** Identifies a valid application image.
- **Application Size:** Defines the size of the firmware image.
- **CRC:** Verifies the integrity of the application image.

The project is developed in two main stages:

1. **Basic Bootloader:** Application validation, Flash memory management, and bootloader-to-application handover.
2. **OTA-Style Firmware Update:** Update request handling, firmware installation from an Intel HEX image, and validation of the updated application.

## 2. Key Features

- Custom bootloader implementation for STM32F446RE.
- Application validation using a dedicated Flash memory header.
- Magic number verification.
- Application size verification.
- CRC-based firmware integrity verification.
- Internal Flash memory programming and management.
- OTA update request handling.
- Software reset mechanism.
- Bootloader-to-application handover.
- Firmware installation from an Intel HEX image.
- Application metadata management.
- Separation between bootloader, application header, and application firmware.

## 3. Hardware Platform

| Component | Description |
|---|---|
| Microcontroller | STM32F446RE |
| Development Board | STM32 Nucleo-F446RE |
| Processor Core | ARM Cortex-M4 |
| Internal Flash | 512 KB |
| User Interface | On-board User Push Button |
| Debugging Interface | ST-LINK |

## 4. System Architecture

The firmware architecture is divided into two main software components.

### 4.1 Bootloader

The bootloader is the first software component executed after a system reset.

Its responsibilities include:

- Reading application metadata.
- Validating the installed application.
- Checking the OTA update request flag.
- Installing a new firmware image.
- Managing internal Flash memory.
- Updating application metadata.
- Transferring execution to the application.

### 4.2 Application

The application contains the main firmware functionality.

Its responsibilities include:

- Executing the main application.
- Managing application metadata.
- Requesting a firmware update when necessary.
- Setting the OTA update flag.
- Triggering a software reset to return execution to the bootloader.

## 5. Flash Memory Architecture

The STM32F446RE provides **512 KB of internal Flash memory**, with an address range starting at `0x08000000`.

The Flash memory is logically divided into three regions to support the bootloader architecture.

### 5.1 Flash Memory Map

| Memory Region | Start Address | End Address | Size |
|---|---|---|---:|
| Bootloader | `0x08000000` | `0x08007FFF` | 32 KB |
| Application Header | `0x08008000` | `0x0800BFFF` | 16 KB |
| Application Firmware | `0x0800C000` | `0x0807FFFF` | 464 KB |
| **Total** | `0x08000000` | `0x0807FFFF` | **512 KB** |

### 5.2 Flash Memory Diagram

```text
       STM32F446RE INTERNAL FLASH
              TOTAL: 512 KB

       0x08000000
             |
             v
    +-------------------------+
    |                         |
    |       BOOTLOADER        |
    |                         |
    |          32 KB          |
    |                         |
    |       0x08000000         |
    |       0x08007FFF         |
    +-------------------------+
    |                         |
    |    APPLICATION HEADER  |
    |                         |
    |          16 KB          |
    |                         |
    |       0x08008000         |
    |       0x0800BFFF         |
    +-------------------------+
    |                         |
    |                         |
    |    APPLICATION         |
    |       FIRMWARE          |
    |                         |
    |                         |
    |         464 KB          |
    |                         |
    |       0x0800C000         |
    |       0x0807FFFF         |
    |                         |
    +-------------------------+
```

### 5.3 Linker Script Configuration

The memory regions are configured through dedicated linker scripts.

**Bootloader linker configuration:**

```c
FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 32K
```

**Application linker configuration:**

```c
APP_HEADER (rx) : ORIGIN = 0x08008000, LENGTH = 16K

FLASH (rx)      : ORIGIN = 0x0800C000, LENGTH = 464K
```

This configuration separates the bootloader, application metadata, and application firmware into independent logical memory regions.

## 6. Flash Sector Organization

The STM32F446RE belongs to the STM32F446 family and provides 512 KB of internal Flash memory.

Flash memory is organized into sectors, which are the minimum units that can be erased independently.

The Flash erase operation must be performed according to the physical sector boundaries defined by the microcontroller.

### Sector Management in This Project

The logical memory allocation is:

| Region | Address Range | Purpose |
|---|---|---|
| Bootloader | `0x08000000–0x08007FFF` | Bootloader code |
| Application Header | `0x08008000–0x0800BFFF` | Application metadata |
| Application | `0x0800C000–0x0807FFFF` | Application firmware |

The following considerations apply:

- The bootloader region must be protected during application updates.
- The application header must be managed carefully to preserve firmware metadata.
- Only the physical Flash sectors containing the application image should be erased during firmware installation.
- Flash erase operations must follow the actual sector boundaries of the STM32F446RE.

**Important:** The linker memory regions describe the logical allocation. They do not define independent Flash erase units. The Flash erase routine must use the sector organization specified in the STM32F446 reference manual.

## 7. Application Header

The application header is stored in a dedicated Flash memory region starting at:

```c
#define APP_HEADER_ADDRESS  0x08008000
```

It contains the metadata required by the bootloader to validate the application.

### Header Parameters

| Parameter | Description |
|---|---|
| Magic Number | Identifies a valid application image. |
| Application Size | Stores the application image size. |
| CRC | Stores the expected CRC value. |
| OTA Flag | Indicates whether a firmware update has been requested. |

The bootloader reads this information during startup to determine whether the installed application is valid.

## 8. Bootloader Operation

After a system reset, the bootloader performs the following operations:

1. Reads the application header.
2. Checks the magic number.
3. Retrieves the application size.
4. Calculates the application CRC.
5. Compares the calculated CRC with the expected CRC.
6. Determines whether the application is valid.

If the application passes all validation checks, the bootloader transfers execution to the application.

If validation fails, the bootloader prevents execution of the invalid application.

## 9. OTA Firmware Update Workflow

The OTA-style update mechanism is initiated by the application.

In the current implementation, the on-board user push button is used to simulate a firmware update request, while the new firmware is supplied through an Intel HEX image.

### 9.1 Update Sequence

**Step 1 – Update Request**

The user presses the on-board push button.

The application detects the request and sets the OTA flag in the application header.

**Step 2 – Software Reset**

The application triggers a software reset, allowing the bootloader to take control.

**Step 3 – OTA Request Detection**

After restarting, the bootloader reads the application header and checks the OTA flag.

If the flag is set, the bootloader starts the firmware update procedure.

**Step 4 – OTA Flag Handling**

The bootloader clears the OTA flag to prevent the same request from being processed repeatedly.

**Step 5 – Firmware Installation**

The bootloader reads the new firmware image from the provided Intel HEX file and programs the new application into the designated Flash memory region.

**Step 6 – Firmware Validation**

The bootloader validates the new firmware using:

- Magic number.
- Application size.
- CRC.

**Step 7 – Application Header Update**

After successful validation, the bootloader updates the application header with the new firmware metadata.

**Step 8 – Application Execution**

Finally, the bootloader transfers execution to the newly installed application.

### 9.2 OTA Workflow Diagram

```text
             APPLICATION
                  |
                  v
          User Button Pressed
                  |
                  v
             Set OTA Flag
                  |
                  v
            Software Reset
                  |
                  v
              BOOTLOADER
                  |
                  v
            Check OTA Flag
                  |
             +----+----+
             |         |
            Yes        No
             |         |
             v         v
       Clear OTA Flag  Validate
             |         Existing App
             v             |
       Read HEX Image       v
             |         Jump to App
             v
       Program Flash
             |
             v
      Validate New Firmware
       (Magic, Size, CRC)
             |
        +----+----+
        |         |
      Valid     Invalid
        |         |
        v         v
   Update Header  Reject Image
        |
        v
   Jump to New App
```

## 10. Firmware Integrity Verification

Firmware integrity is an important part of the bootloader implementation.

The project uses CRC verification to detect corruption in the firmware image.

The validation process consists of:

1. Reading the expected CRC from the firmware metadata.
2. Calculating the CRC over the application image.
3. Comparing the calculated CRC with the stored CRC.
4. Accepting or rejecting the firmware based on the result.

The combination of the magic number, application size, and CRC provides a basic mechanism for checking firmware validity.

**Note:** CRC detects accidental corruption but does not provide cryptographic authentication or protection against malicious firmware modification.

## 11. Bootloader-to-Application Handover

Once the application has been validated, the bootloader transfers execution to the application firmware.

This operation requires correctly configuring the Cortex-M4 execution environment, including:

- Loading the application's initial Main Stack Pointer (MSP).
- Setting the vector table offset to the application vector table.
- Loading the application's reset handler address.
- Transferring execution to the application reset handler.

The application firmware must be linked to its designated Flash address:

```c
0x0800C000
```

## 12. Development Environment

| Tool / Technology | Purpose |
|---|---|
| STM32CubeIDE | Embedded software development |
| STM32CubeMX | Microcontroller configuration |
| Embedded C | Firmware implementation |
| ARM Cortex-M4 | Processor architecture |
| STM32 HAL | Hardware abstraction |
| GNU ARM Toolchain | Compilation and linking |
| ST-LINK | Programming and debugging |

## 13. Project Structure

```text
OTA_Bootloader_STM32F446RE/
│
├── Application1/
│   ├── Core/
│   ├── Drivers/
│   ├── *.ioc
│   └── Application source files
│
├── Bootloader1/
│   ├── Core/
│   ├── Drivers/
│   ├── *.ioc
│   └── Bootloader source files
│
└── README.md
```

*Note: Adjust this structure to match the actual repository contents.*

## 14. Future Improvements

Potential improvements to extend the functionality and reliability of this project include:

- Implementing firmware transfer through UART, USB, Ethernet, or wireless communication.
- Adding digital signature verification for firmware authenticity.
- Implementing an application rollback mechanism.
- Improving Flash memory protection.
- Adding robust error handling and update status reporting.
- Supporting power-failure recovery during firmware installation.
- Implementing a more advanced firmware image format.
- Adding secure boot functionality.

## 15. Learning Objectives

This project provides practical experience with:

- Custom embedded bootloader development.
- STM32 startup and reset sequences.
- ARM Cortex-M4 architecture.
- Internal Flash memory organization.
- Linker script configuration.
- Flash sector erase and programming.
- Firmware image validation.
- CRC implementation.
- Application vector table relocation.
- Bootloader-to-application handover.
- Embedded firmware update architecture.
  
<img width="900" height="1600" alt="image" src="https://github.com/user-attachments/assets/818cb62c-5c14-4b9a-8e03-830d5e563144" />


<img width="1600" height="167" alt="image" src="https://github.com/user-attachments/assets/36f7ea8f-a5c5-4f74-8695-8e6aa8235ff8" />


## 16. Author

**Project:** STM32F446RE Custom OTA Bootloader

**GitHub:** [laouridianas-del](https://github.com/laouridianas-del)

**Repository:** [OTA_Bootloader_STM32F446RE](https://github.com/laouridianas-del/OTA_Bootloader_STM32F446RE)

---

*Developed as an embedded systems project focused on custom bootloader architecture, firmware integrity verification, Flash memory management, and OTA-style firmware updates.*


