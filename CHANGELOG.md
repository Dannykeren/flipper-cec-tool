# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- Initial development

## [1.0.0] - 2024-01-XX

### Added
- Initial release of Flipper Zero CEC Test Tool
- Device discovery functionality
- Send/receive CEC commands
- Power control (on/off, standby)
- Volume control (up/down)
- Command logging and history
- Settings configuration
- Support for Momentum firmware v010
- Video Game Module integration
- Real-time CEC bus monitoring
- Custom command sending
- Automated GitHub Actions builds
- Complete documentation

### Features
- **Device Discovery**: Scan and identify all CEC devices on HDMI network
- **Command Transmission**: Send custom CEC commands with hex input
- **Command Reception**: Monitor and log all CEC bus traffic
- **Power Management**: Remote power control for connected devices
- **Volume Control**: Audio volume adjustment via CEC
- **History Logging**: Track all sent and received commands
- **Configuration**: Adjustable CEC address and settings
- **User Interface**: Intuitive menu system with navigation
- **Error Handling**: Robust error detection and reporting

### Technical Details
- Written in C for optimal performance
- Utilizes Flipper Zero expansion interface
- Implements full HDMI CEC protocol
- Supports all standard CEC opcodes
- Compatible with Momentum firmware v010
- Automated build and release pipeline

### Requirements
- Flipper Zero with Momentum firmware v010
- Video Game Module
- HDMI cable
- CEC-compatible TV or device

### Known Issues
- None at initial release

---

## Release Notes Format

Each release includes:
- **Added**: New features
- **Changed**: Changes in existing functionality
- **Deprecated**: Soon-to-be removed features
- **Removed**: Removed features
- **Fixed**: Bug fixes
- **Security**: Security improvements

---

## Version History

- **v1.0.0**: Initial release with core CEC functionality
- **vX.X.X**: Future releases will be documented here

## Links

- [Latest Release](https://github.com/YOUR_USERNAME/flipper-cec-tool/releases/latest)
- [All Releases](https://github.com/YOUR_USERNAME/flipper-cec-tool/releases)
- [Issues](https://github.com/YOUR_USERNAME/flipper-cec-tool/issues)
- [Pull Requests](https://github.com/YOUR_USERNAME/flipper-cec-tool/pulls)
