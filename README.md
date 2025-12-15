# 🖥️ MiniOS - Custom Operating System with VMware

<div align="center">

![OS](https://img.shields.io/badge/OS-MiniOS%20v1.0-brightgreen?style=for-the-badge)
![Platform](https://img.shields.io/badge/Platform-VMware-607078?style=for-the-badge&logo=vmware)
![Arch](https://img.shields.io/badge/Architecture-x86%2032--bit-blue?style=for-the-badge)
![Language](https://img.shields.io/badge/Language-C%20%2B%20Assembly-orange?style=for-the-badge&logo=c)
![Bootloader](https://img.shields.io/badge/Bootloader-GRUB-red?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Fully%20Functional-success?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge)

</div>

---

## 📋 Table of Contents

- [Project Overview](#-project-overview)
- [Features](#-features)
- [System Architecture](#-system-architecture)
- [Technologies Used](#-technologies-used)
- [Prerequisites](#-prerequisites)
- [Installation & Build](#-installation--build)
- [Running in VMware](#-running-in-vmware)
- [Commands Reference](#-commands-reference)
- [Project Structure](#-project-structure)
- [Team Members](#-team-members)
- [Screenshots](#-screenshots)
- [Contributing](#-contributing)
- [License](#-license)

---

## 🎯 Project Overview

**MiniOS** is a custom-built operating system developed from scratch that runs independently on VMware. This educational OS project demonstrates core operating system concepts including process scheduling, virtual memory management, and hardware interaction without relying on any existing OS framework.

The project implements a complete boot-to-shell experience with an interactive command-line interface, multiple CPU scheduling algorithms, and a virtual memory paging system with LRU page replacement.

###  Academic Project
- **Course:** Operating Systems
- **Institution:** Air University
- **Duration:** 1 Weeks (6 Days)
- **Type:** Educational Operating System

---

##  Features

###  Core Components

#### 1. **Custom Bootloader**
- GRUB multiboot-compliant bootloader
- 32-bit x86 architecture support
- Stack initialization and kernel loading
- Boot menu with multiple entries

#### 2. **Operating System Kernel**
- Written in C language
- Direct hardware control (no OS dependencies)
- VGA text mode display driver (80x25 characters)
- Keyboard input driver with scan code conversion
- I/O port communication (inb/outb)
- 16-color support with custom color schemes

#### 3. **Interactive Shell**
- Command-line interface with colored prompt
- Command parser with argument tokenization
- 12+ builtin commands
- Real-time user input handling
- Backspace support and input validation
- Error handling with user-friendly messages

#### 4. **CPU Scheduler (3 Algorithms)**
- **FCFS (First Come First Serve):** Non-preemptive, arrival-order execution
- **Round Robin:** Time-sliced preemptive scheduling with configurable quantum
- **Priority Scheduling:** Priority-based process selection
- Process Control Block (PCB) with state management
- Process states: NEW, READY, RUNNING, TERMINATED
- Real-time process statistics (waiting time, turnaround time)

#### 5. **Virtual Memory Management**
- Paging system with 4KB page size
- 16 physical frames for demonstration
- Page table per process
- LRU (Least Recently Used) page replacement algorithm
- Page fault handling
- Memory statistics (hit rate, fault rate)
- Frame allocation table visualization

#### 6. **Build System**
- Automated Makefile compilation
- Linker script for memory layout
- Bootable ISO generation with grub-mkrescue
- Clean and rebuild targets

---

##  System Architecture

```
┌─────────────────────────────────────────┐
│         USER INTERACTION                │
│   (Types commands in shell)             │
└─────────────────────────────────────────┘
                 ↓ ↑
┌─────────────────────────────────────────┐
│           SHELL (shell.c)               │
│  • Command parsing                      │
│  • Builtin command execution            │
│  • Input/output handling                │
└─────────────────────────────────────────┘
                 ↓ ↑
┌─────────────────────────────────────────┐
│         KERNEL (kernel.c)               │
│  • VGA display driver                   │
│  • Keyboard input driver                │
│  • I/O port management                  │
│  • Color management                     │
└─────────────────────────────────────────┘
      ↓              ↓              ↓
┌──────────┐  ┌──────────┐  ┌──────────┐
│SCHEDULER │  │  MEMORY  │  │  OTHER   │
│(sched.c) │  │(memory.c)│  │ MODULES  │
│          │  │          │  │          │
│• FCFS    │  │• Paging  │  │• Future  │
│• RR      │  │• Frames  │  │  modules │
│• Priority│  │• LRU     │  │          │
└──────────┘  └──────────┘  └──────────┘
                 ↓
┌─────────────────────────────────────────┐
│      HARDWARE (VMware Virtual)          │
│  • Virtual CPU                          │
│  • Virtual RAM                          │
│  • Virtual Keyboard                     │
│  • Virtual VGA Display                  │
└─────────────────────────────────────────┘
```

---

##  Technologies Used

### Programming Languages
| Language | Purpose | Lines of Code |
|----------|---------|---------------|
| **C** | Kernel, scheduler, memory manager, shell | ~1,200 |
| **Assembly (NASM)** | Bootloader | ~25 |
| **Makefile** | Build automation | ~50 |

### Tools & Software
| Tool | Version | Purpose |
|------|---------|---------|
| **GCC** | 11.4.0+ | C compiler (32-bit) |
| **NASM** | 2.15+ | Assembly compiler |
| **LD** | GNU Linker | Object file linking |
| **GRUB** | 2.06+ | Bootloader |
| **xorriso** | 1.5.4+ | ISO image creation |
| **VMware Workstation Pro** | 17.0+ | Testing platform |
| **Ubuntu Linux** | 22.04 LTS | Development environment |
| **Git** | 2.34+ | Version control |

### Standards & Specifications
- **Multiboot Specification v1** - GRUB compatibility
- **VGA Text Mode** - 80x25 character display
- **x86 Protected Mode** - 32-bit operation
- **ELF Format** - Executable binary format

---

##  Prerequisites

### Development Machine
- **Operating System:** Ubuntu 22.04 LTS (or WSL2 with Ubuntu)
- **RAM:** 4 GB minimum
- **Disk Space:** 2 GB free space
- **Virtualization:** VMware Workstation Pro / Player

### Required Packages (Ubuntu)
```bash
sudo apt update && sudo apt upgrade -y
sudo apt install -y build-essential gcc-multilib git make nasm
sudo apt install -y grub-pc-bin xorriso mtools
sudo apt install -y gcc-multilib libc6-dev-i386
```

### Verify Installation
```bash
gcc --version       # Should show GCC 11.4.0+
nasm -version       # Should show NASM 2.15+
grub-mkrescue --version  # Should show GRUB 2.06+
```

---

##  Installation & Build

### 1. Clone Repository
```bash
git clone https://github.com/yourusername/minios.git
cd minios
```

### 2. Project Structure Setup
```bash
mkdir -p src asm grub iso/boot/grub build docs
```

### 3. Build the OS
```bash
make clean    # Clean previous builds
make          # Compile and create ISO
```

### Expected Output
```
mkdir -p build
nasm -f elf32 asm/boot.asm -o build/boot.o
gcc -m32 -ffreestanding ... -c src/kernel.c -o build/kernel.o
gcc -m32 -ffreestanding ... -c src/shell.c -o build/shell.o
gcc -m32 -ffreestanding ... -c src/scheduler.c -o build/scheduler.o
gcc -m32 -ffreestanding ... -c src/memory.c -o build/memory.o
ld -m elf_i386 -T linker.ld -o build/kernel.bin ...
grub-mkrescue -o minios.iso iso
====================================
 ISO created: minios.iso
Load this file in VMware to test!
====================================
```

### 4. Verify Build
```bash
ls -lh minios.iso
# Should show ~3-4 MB file

file build/kernel.bin
# Should show: ELF 32-bit LSB executable
```

---

##  Running in VMware

### Method 1: Create New Virtual Machine

#### Step 1: Create VM
1. Open **VMware Workstation Pro**
2. **File** → **New Virtual Machine**
3. Select **"Typical"** configuration
4. Choose **"I will install the operating system later"**
5. **Guest OS:** Other → **Other (32-bit)**
6. **Name:** MiniOS
7. **Disk Size:** 1 GB (won't be used)
8. Click **Finish**

#### Step 2: Configure VM
1. Select VM → **Edit virtual machine settings**
2. **CD/DVD (IDE):**
   - ☑ Connected
   - ☑ Connect at power on
   - ⦿ Use ISO image file
   - Browse to `minios.iso`
3. **Memory:** 128 MB (sufficient)
4. Click **OK**

#### Step 3: Boot
1. Select VM
2. Click **Power on this virtual machine**
3. GRUB menu appears → Press **Enter** on "MiniOS v1.0"
4. System boots → Shell prompt appears

### Method 2: Quick Test (Command Line)
```bash
# Using QEMU (alternative to VMware for testing)
qemu-system-i386 -cdrom minios.iso -m 128M
```

---

## Commands Reference

### System Commands
| Command | Description | Example |
|---------|-------------|---------|
| `help` | Show all available commands | `help` |
| `clear` | Clear screen | `clear` |
| `echo <text>` | Print text to screen | `echo Hello MiniOS!` |

### Process Management
| Command | Description | Example |
|---------|-------------|---------|
| `ps` | List all processes | `ps` |
| `run <burst> <priority>` | Create new process | `run 10 5` |
| `kill <pid>` | Terminate process | `kill 3` |

### Scheduler Control
| Command | Description | Example |
|---------|-------------|---------|
| `scheduler mode <algorithm>` | Set scheduling mode | `scheduler mode rr` |
| `scheduler quantum <n>` | Set time quantum (RR only) | `scheduler quantum 4` |
| `scheduler tick` | Execute one scheduling cycle | `scheduler tick` |

**Available Modes:**
- `fcfs` - First Come First Serve
- `rr` - Round Robin
- `priority` - Priority-based

### Memory Management
| Command | Description | Example |
|---------|-------------|---------|
| `meminfo` | Show memory statistics | `meminfo` |
| `frames` | Display frame allocation table | `frames` |
| `allocpages <pid> <count>` | Allocate pages to process | `allocpages 1 8` |
| `access <pid> <page>` | Access a page (triggers fault/hit) | `access 1 5` |

---

##  Project Structure

```
minios/
├── asm/
│   └── boot.asm              # Bootloader assembly code
├── src/
│   ├── kernel.h              # Kernel function declarations
│   ├── kernel.c              # Main kernel implementation
│   ├── shell.h               # Shell interface
│   ├── shell.c               # Command parser & builtins
│   ├── scheduler.h           # Scheduler interface
│   ├── scheduler.c           # CPU scheduling algorithms
│   ├── memory.h              # Memory management interface
│   └── memory.c              # Paging & LRU implementation
├── grub/
│   └── grub.cfg              # GRUB boot configuration
├── iso/
│   └── boot/
│       ├── grub/
│       │   └── grub.cfg      # (copied during build)
│       └── kernel.bin        # (generated during build)
├── build/                    # Compiled object files
│   ├── boot.o
│   ├── kernel.o
│   ├── shell.o
│   ├── scheduler.o
│   ├── memory.o
│   └── kernel.bin
├── docs/                     # Documentation
│   ├── VIVA_GUIDE.md
│   └── ARCHITECTURE.md
├── Makefile                  # Build automation
├── linker.ld                 # Linker script
├── minios.iso                # Bootable ISO (generated)
└── README.md                 # This file
```

---

## Team Members

<table>
<tr>
<td align="center">
<img src="https://via.placeholder.com/100" width="100px;" alt="Saad Khan"/><br />
<sub><b>Saad Khan</b></sub><br />
<i>Team Leader</i><br />
<sub>Kernel • Scheduler • Shell</sub>
<sub>Memory • Build System • Bootloader</sub>
</td>
</tr>
</table>

### Contributions

#### Saad Khan (Leader)
-  Implemented kernel core (VGA driver, keyboard driver)
-  Developed all 3 CPU scheduling algorithms (FCFS, RR, Priority)
-  Created interactive shell with command parsing
-  Designed color system and UI
-  Implemented virtual memory management with paging
-  Developed LRU page replacement algorithm
-  Created build system (Makefile, linker script)
-  Wrote assembly bootloader

---

##  Screenshots

### Boot Screen
```
  _____ _____ _____ _____  _____  _____ 
 |     |     |   | |     ||     ||   __|
 | | | |-   -| | | |  |  ||   --||__   |
 |_|_|_|_____|_|___|_____||_____||_____|

        >> MiniOS v1.0 - VMware Edition 
           Made with <3 by Your Team
  =============================================

  [*] Booting MiniOS...
      [####################] 100%

  [*] Initializing subsystems...
      [OK] Scheduler initialized
      [OK] Memory manager initialized

  +-------------------------------------------+
  |  Welcome to MiniOS Interactive Shell!   |
  |  Type 'help' to see available commands  |
  |  Have fun exploring! ^_^                |
  +-------------------------------------------+

minios > _
```

### Process Listing (ps command)
```
  ===============================================
              Process Status Table
  ===============================================
  Scheduler Mode: Round-Robin (quantum=4)

  +-----+----------+------+-------+--------+------+
  | PID |  State   | Prio | Burst | Remain | Wait |
  +-----+----------+------+-------+--------+------+
  |  1  | RUN      |  5   |  10   |    6   |  0   |
  |  2  | READY    |  3   |   8   |    8   |  4   |
  |  3  | READY    |  7   |  12   |   12   |  4   |
  +-----+----------+------+-------+--------+------+
       Total: 3 process(es)
```

### Memory Information (meminfo command)
```
  ===============================================
              Memory Information
  ===============================================

  Page size: 4096 bytes
  Total frames: 16
  Used frames: 8 [########--------]
  Free frames: 8

  Statistics:
    * Page faults: 12
    * Page hits: 38
    * Hit rate: 76%
```

---

##  Learning Outcomes

This project demonstrates understanding of:

### Operating System Concepts
- Boot process and bootloader operation
- Kernel architecture and design
- Hardware abstraction (VGA, keyboard)
- Process management and PCB structure
- CPU scheduling algorithms
- Virtual memory and paging
- Page replacement algorithms (LRU)
- Memory management

### Systems Programming
- Low-level C programming
- Assembly language (x86)
- Direct hardware access
- Memory-mapped I/O
- I/O port communication
- Linker scripts and memory layout
- Build automation with Make

### Software Engineering
- Modular code organization
- Header file design
- Version control with Git
- Documentation
- Team collaboration
- Project planning and execution

---

##  Future Enhancements

### Planned Features
- [ ] File system implementation (FAT32)
- [ ] User space / kernel space separation
- [ ] System call interface
- [ ] Multi-threading support
- [ ] Timer interrupts for preemptive scheduling
- [ ] Network stack (basic TCP/IP)
- [ ] Graphical user interface (GUI)
- [ ] Additional scheduling algorithms (MLFQ, CFS)
- [ ] Demand paging
- [ ] Disk I/O operations

### Possible Extensions
- [ ] Multi-core CPU support
- [ ] Memory protection (paging with protection bits)
- [ ] Inter-process communication (IPC)
- [ ] Device drivers (SATA, USB)
- [ ] Shell scripting support
- [ ] Package manager

---

##  Contributing

Contributions are welcome! Please follow these guidelines:

### How to Contribute
1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

### Coding Standards
- Follow existing code style
- Add comments for complex logic
- Update documentation for new features
- Test thoroughly in VMware


---

##  Contact

**Saad Khan** - Team Leader  
📧 Email: saadkhan@example.com  
🔗 GitHub: [@saadkhan](https://github.com/saadoxyz)

**Project Repository:** [https://github.com/yourusername/minios](https://github.com/saadoxyz/minios)

---

## Acknowledgments

- **GRUB Developers** - For the excellent bootloader
- **OSDev Community** - For invaluable resources and documentation
- **VMware** - For providing robust virtualization platform
- **Our Professor** - For guidance and project requirements
- **Open Source Community** - For tools and libraries

---

## References & Resources

- [OSDev Wiki](https://wiki.osdev.org/) - Comprehensive OS development resource
- [Intel x86 Manual](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html) - CPU architecture documentation
- [GRUB Manual](https://www.gnu.org/software/grub/manual/) - Bootloader documentation
- [Bare Bones Tutorial](https://wiki.osdev.org/Bare_Bones) - Getting started with OS development
- [James Molloy's Tutorial](http://www.jamesmolloy.co.uk/tutorial_html/) - Kernel development guide

---

<div align="center">

**Star this repository if you found it helpful!**

**Made by Saad Khan**

![Build Status](https://img.shields.io/badge/build-passing-brightgreen?style=flat-square)
![Version](https://img.shields.io/badge/version-1.0.0-blue?style=flat-square)
![Stars](https://img.shields.io/github/stars/yourusername/minios?style=flat-square)
![Forks](https://img.shields.io/github/forks/yourusername/minios?style=flat-square)

</div>
