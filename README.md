# Flipper Zero CEC Test Tool

[![Build Status](https://github.com/YOUR_USERNAME/flipper-cec-tool/workflows/Build%20Flipper%20CEC%20Tool/badge.svg)](https://github.com/YOUR_USERNAME/flipper-cec-tool/actions)
[![Release](https://img.shields.io/github/v/release/YOUR_USERNAME/flipper-cec-tool)](https://github.com/YOUR_USERNAME/flipper-cec-tool/releases)
[![Downloads](https://img.shields.io/github/downloads/YOUR_USERNAME/flipper-cec-tool/total)](https://github.com/YOUR_USERNAME/flipper-cec-tool/releases)

A comprehensive HDMI CEC (Consumer Electronics Control) test tool for Flipper Zero with Video Game Module support.

## ✨ Features

- 🎮 **Device Discovery** - Find all CEC devices on your HDMI network
- 📤 **Send CEC Commands** - Custom command transmission
- 📥 **Receive CEC Commands** - Monitor CEC bus traffic in real-time
- 🔌 **Power Control** - Turn devices on/off remotely
- 🔊 **Volume Control** - Adjust audio volume via CEC
- 📋 **Command Log** - View history of sent/received commands
- ⚙️ **Settings** - Configure CEC address and options
- 🎯 **Compatible** with Momentum firmware v010

## 🚀 Quick Start

### Prerequisites
- Flipper Zero with [Momentum firmware v010](https://github.com/Next-Hack/Momentum-Firmware)
- Video Game Module for Flipper Zero
- HDMI cable
- TV/device with CEC support

### Installation

1. **Download the latest release**
   - Go to [Releases](https://github.com/YOUR_USERNAME/flipper-cec-tool/releases)
   - Download `cec_test_tool.fap`

2. **Install on Flipper Zero**
   - Connect Flipper Zero to computer
   - Open qFlipper
   - Navigate to `/ext/apps/GPIO/`
   - Copy `cec_test_tool.fap` to this folder

3. **Hardware Setup**
   - Attach Video Game Module to Flipper Zero
   - Connect HDMI cable from module to TV
   - Enable CEC in TV settings (see [TV Settings](#tv-settings))

4. **Run the App**
   - On Flipper: Applications → GPIO → CEC Test Tool
   - Start with "Device Discovery"

## 📺 TV Settings

Enable CEC in your TV settings. Look for:
- **Samsung**: Anynet+ (HDMI-CEC)
- **Sony**: BRAVIA Sync
- **LG**: SimpLink
- **Panasonic**: VIERA Link
- **Toshiba**: CE-Link

## 🎯 Usage

### Basic Testing
1. **Device Discovery** - Find connected devices
2. **Power Control** - Test on/off functionality
3. **Volume Control** - Test volume adjustment
4. **View Log** - Check command history

### Advanced Testing
1. **Send Custom Commands** - Test specific CEC opcodes
2. **Receive Commands** - Monitor CEC bus traffic
3. **Settings** - Configure your CEC address

### Common CEC Commands
- `0x82` - Active Source
- `0x9D` - Inactive Source
- `0x8F` - Give Device Power Status
- `0x90` - Report Power Status
- `0x36` - Standby
- `0x44` - User Control Pressed
- `0x45` - User Control Released

## 🔧 Development

### Project Structure
