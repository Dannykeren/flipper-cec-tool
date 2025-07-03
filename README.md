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
flipper-cec-tool/
├── .github/workflows/    # Automated builds
├── src/                  # Source code
│   ├── cec_test_tool.c  # Main application
│   ├── application.fam  # App manifest
│   └── icons/           # App icons
├── docs/                # Documentation
├── README.md           # This file
└── CHANGELOG.md        # Version history

### Building Locally
```bash
# Clone Momentum firmware
git clone --recursive https://github.com/Next-Hack/Momentum-Firmware.git
cd Momentum-Firmware

# Copy app
cp -r ../src applications/external/cec_test_tool/

# Build
./fbt fap_cec_test_tool
Contributing

Fork the repository
Create a feature branch
Make your changes
Test thoroughly
Submit a pull request

📋 Changelog
See CHANGELOG.md for detailed version history.
🐛 Bug Reports
Found a bug? Please create an issue with:

Flipper Zero firmware version
TV/device model
Steps to reproduce
Expected vs actual behavior

📄 License
This project is licensed under the MIT License - see the LICENSE file for details.
🙏 Acknowledgments

Flipper Zero Team for the amazing platform
Momentum Firmware Team for enhanced features
HDMI CEC specification contributors
Community testers and contributors

📞 Support

📚 Documentation
💬 Discussions
🐛 Issues


⭐ If this tool helped you, please consider giving it a star! ⭐
