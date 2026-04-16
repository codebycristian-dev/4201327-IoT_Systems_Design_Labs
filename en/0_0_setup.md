# Environment Setup - IoT Course with ESP32-C6

This guide will take you step-by-step through setting up the complete development environment needed for the course using Windows native and VS Code graphical tools with the ESP-IDF extension. The setup is divided into three main parts:

1. **Prerequisites** - Installing Python and Git on Windows
2. **IDE and ESP-IDF** - Installing VS Code, ESP-IDF extension, and framework installation
3. **Workspace setup** - Configuring the workspace based on this repository

---

## Part 1: Prerequisites on Windows

### 1.1) Install Python 3.8 or higher

Download and install Python from [python.org](https://www.python.org/downloads/). Make sure to check the "Add Python to PATH" option during installation.

Verify the installation:
```cmd
python --version
pip --version
```

### 1.2) Install Git

Download and install Git from [git-scm.com](https://git-scm.com/download/win).

Verify the installation:
```cmd
git --version
```

> **Linux/macOS Users**: Please refer to the [official Espressif Installation Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32c6/get-started/linux-macos-setup.html) for detailed setup instructions.

## Part 2: IDE and ESP-IDF

### 2.1) Install and configure VS Code

1. Install [VS Code](https://code.visualstudio.com/download).
2. Install the [ESP-IDF](https://marketplace.visualstudio.com/items?itemName=espressif.esp-idf-extension) extension from the VS Code marketplace.

### 2.2) Install ESP-IDF through the extension

The ESP-IDF extension can install and configure ESP-IDF automatically:

1. Open VS Code.
2. Press `Ctrl+Shift+P` to open the command palette.
3. Search for and run `ESP-IDF: Configure ESP-IDF Extension`.
4. Select "Express", then version v5.5.1.
5. Choose the installation location (default is fine).
6. Press the "Install" button and wait for the installation to complete.
7. Choose `esp32c6` as the default target.

---

## Part 3: Workspace Setup based on this repository

### 3.1) Create a fork of the repository

To have your own copy of the repository where you can save your changes and submissions:

1. Go to the original repository: [https://github.com/saacifuentesmu/4201327-IoT_Systems_Design_Labs](https://github.com/saacifuentesmu/4201327-IoT_Systems_Design_Labs)
2. Click the **"Fork"** button in the upper right corner
3. Select your GitHub account as the fork destination
4. Wait for the fork to complete

### 3.2) Clone your fork

```cmd
# Clone your fork (choose a convenient location, e.g. Documents)
cd %USERPROFILE%\Documents
git clone https://github.com/<YOUR_USERNAME>/4201327-IoT_Systems_Design_Labs.git
cd 4201327-IoT_Systems_Design_Labs
```

> **Note:** Replace `<YOUR_USERNAME>` with your GitHub username.

---
---

## Quick Start with ESP-IDF Extension

For each lab, you can use the graphical interface of the VS Code ESP-IDF extension:

1. Open VS Code → **Command Palette** (`Ctrl+Shift+P`) → **ESP-IDF: Show Examples** → select the appropriate example (see lab instructions for the specific example, e.g. `esp-idf/examples/openthread/ot_cli`)
2. **ESP-IDF: Set Target** → `esp32c6`
3. **ESP-IDF SDK Configuration Editor** (GUI) to adjust lab-specific configurations when indicated in the steps.
4. Use **Build ESP-IDF Project**, **Flash Device**, and **Monitor Device** from the IDE toolbar.

---

## Setup Complete ✅

Once all steps are completed, you will have:
- ✅ Python 3.8+ installed
- ✅ Git installed
- ✅ ESP-IDF v5.5.1 installed and configured
- ✅ Course repository cloned
- ✅ VS Code with ESP-IDF extension

## Daily Workflow

To work on the course each day:

```cmd
# 1. Connect the ESP32-C6 to the USB port
# (Drivers install automatically or manually as in Part 2.4)

# 2. Open VS Code in your fork directory
code %USERPROFILE%\Documents\4201327-IoT_Systems_Design_Labs

# 3. Work with the labs using the ESP-IDF extension GUI
# ... follow the corresponding lab instructions
```

---

## Smoke Test: Hello World (Quick Validation)

Before the first formal lab, perform this minimal test to confirm that ESP-IDF works correctly in your environment using the VS Code GUI.

### Build and flash the Hello World example

1. Open VS Code in your fork directory `4201327-IoT_Systems_Design_Labs`.
2. Press `Ctrl+Shift+P` to open the command palette.
3. Search for and run `ESP-IDF: New Project` → select `{YOUR_IDF_PATH}` → select `ESP-IDF Examples` → select `get-started/blink`.
4. In the ESP-IDF toolbar, click **ESP-IDF: Set Target** and select `esp32c6`.
5. Click **ESP-IDF: Build Project**.
6. Connect the ESP32-C6 and click **ESP-IDF: Flash Device** (UART).
7. Click **ESP-IDF: Monitor Device**.

You should see the "LED ON/OFF" messages repeatedly in the console and the LED blinking, confirming that the environment is configured correctly.


---

