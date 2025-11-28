# Toyota Corolla 2026 - Project Structure

```
toyota-corolla-2026-multimedia/
│
├── README.md                          # Main project documentation
├── CMakeLists.txt                     # Top-level CMake configuration
├── .gitlab-ci.yml                     # CI/CD pipeline configuration
├── .gitignore                         # Git ignore rules
├── package.json                       # Project metadata and scripts
│
├── src/                               # Source code
│   ├── core/                          # Core system services
│   │   ├── system_controller.cpp      # Main system controller
│   │   ├── configuration_manager.cpp  # Configuration management
│   │   ├── telemetry_manager.cpp      # Telemetry and analytics
│   │   └── security_manager.cpp       # Security and encryption
│   │
│   ├── hmi/                           # Human-Machine Interface (Qt/QML)
│   │   ├── Main.qml                   # Main UI entry point
│   │   ├── HomeScreen.qml             # Home screen
│   │   ├── NavigationScreen.qml       # Navigation interface
│   │   ├── MediaScreen.qml            # Media player interface
│   │   ├── PhoneScreen.qml            # Phone/Bluetooth interface
│   │   ├── SettingsScreen.qml         # Settings screen
│   │   ├── main.cpp                   # C++ entry point
│   │   ├── application.cpp            # Application logic
│   │   └── qml.qrc                    # Qt resources
│   │
│   ├── connectivity/                  # Connectivity services
│   │   ├── bluetooth_manager.cpp      # Bluetooth stack
│   │   ├── wifi_manager.cpp           # WiFi management
│   │   ├── carplay_integration.cpp    # Apple CarPlay
│   │   └── android_auto_integration.cpp # Android Auto
│   │
│   ├── navigation/                    # Navigation system
│   │   ├── navigation_engine.cpp      # Core navigation
│   │   ├── map_renderer.cpp           # Map rendering
│   │   ├── route_planner.cpp          # Route calculation
│   │   └── cloud_sync.cpp             # Cloud-based features
│   │
│   ├── media/                         # Media playback
│   │   ├── audio_player.cpp           # Audio playback
│   │   ├── video_player.cpp           # Video playback
│   │   ├── radio_tuner.cpp            # AM/FM/HD radio
│   │   └── media_library.cpp          # Media database
│   │
│   ├── vehicle-interface/             # Vehicle integration
│   │   ├── can_interface.cpp          # CAN bus communication
│   │   ├── vehicle_state.cpp          # Vehicle state management
│   │   └── diagnostic_interface.cpp   # OBD-II diagnostics
│   │
│   └── ota/                           # Over-the-Air updates
│       ├── ota_manager.h              # OTA manager header
│       ├── ota_manager.cpp            # OTA implementation
│       ├── secure_boot.cpp            # Secure boot verification
│       ├── partition_manager.cpp      # A/B partition management
│       └── update_package.cpp         # Update package handler
│
├── deployment/                        # Deployment configurations
│   ├── yocto/                         # Yocto build system
│   │   ├── conf/
│   │   │   ├── layer.conf             # Layer configuration
│   │   │   ├── local.conf             # Local build settings
│   │   │   └── bblayers.conf          # Layer dependencies
│   │   └── recipes/                   # Custom recipes
│   │       ├── multimedia/
│   │       ├── kernel/
│   │       └── bootloader/
│   │
│   ├── docker/                        # Development containers
│   │   ├── docker-compose.yml         # Docker compose config
│   │   ├── Dockerfile.hmi             # HMI dev container
│   │   ├── Dockerfile.services        # Services dev container
│   │   └── mqtt/
│   │       └── mosquitto.conf         # MQTT configuration
│   │
│   ├── ota-packages/                  # OTA deployment packages
│   │   ├── create-ota-package.sh      # Package creation script
│   │   ├── sign-package.sh            # Signing script
│   │   └── metadata/                  # Package metadata
│   │
│   ├── ci-cd/                         # CI/CD configurations
│   │   ├── gitlab-runner.yml          # Runner configuration
│   │   └── build-agents/              # Build agent configs
│   │
│   └── scripts/                       # Deployment scripts
│       ├── deploy-ota.sh              # OTA deployment script
│       ├── deploy-to-staging.sh       # Staging deployment
│       ├── rollback.sh                # Rollback script
│       ├── expand-rollout.sh          # Gradual rollout
│       ├── monitor-deployment.sh      # Monitoring script
│       └── health-check.sh            # Health verification
│
├── tests/                             # Test suites
│   ├── unit/                          # Unit tests
│   │   ├── test_ota_manager.cpp
│   │   ├── test_configuration.cpp
│   │   ├── test_security.cpp
│   │   └── run-all-tests.sh
│   │
│   ├── integration/                   # Integration tests
│   │   ├── test_vehicle_integration.py
│   │   ├── test_connectivity.py
│   │   └── pytest.ini
│   │
│   └── hardware-in-loop/              # HIL tests
│       ├── run-hil-tests.sh
│       ├── scenarios/
│       └── results/
│
├── docs/                              # Documentation
│   ├── architecture/                  # Architecture docs
│   │   ├── system-overview.md
│   │   └── component-diagrams.md
│   │
│   ├── api/                           # API documentation
│   │   ├── rest-api.yaml
│   │   └── internal-apis.md
│   │
│   └── deployment/                    # Deployment docs
│       ├── DEPLOYMENT_ARCHITECTURE.md # Deployment guide
│       ├── ota-manual.md
│       └── troubleshooting.md
│
└── tools/                             # Development tools
    ├── build-scripts/                 # Build automation
    │   ├── build.sh                   # Main build script
    │   ├── setup-dev-env.sh           # Environment setup
    │   └── cross-compile.sh           # Cross-compilation
    │
    └── testing/                       # Testing utilities
        ├── can-simulator.py           # CAN bus simulator
        ├── ota-test-server.py         # Mock OTA server
        └── coverage.sh                # Coverage reporting
```

## Key Files Explained

### Build and Configuration
- **CMakeLists.txt**: Main build configuration for the entire system
- **.gitlab-ci.yml**: Complete CI/CD pipeline with staging and production deployment
- **package.json**: Project metadata and npm-style scripts for convenience

### Source Code
- **src/ota/ota_manager.h**: Complete OTA update system with A/B partitioning
- **src/hmi/Main.qml**: Qt/QML-based user interface entry point

### Deployment
- **deployment/docker/docker-compose.yml**: Full development environment with all services
- **deployment/yocto/conf/layer.conf**: Yocto layer configuration for building Linux image
- **deployment/scripts/deploy-ota.sh**: Production OTA deployment script with safety checks

### Documentation
- **README.md**: Comprehensive project overview and quick start guide
- **docs/deployment/DEPLOYMENT_ARCHITECTURE.md**: Detailed deployment architecture and processes

## Technology Stack Summary

### Operating System
- **Base**: Automotive Grade Linux (AGL) / Yocto Project
- **Kernel**: Linux 6.1 LTS
- **Init System**: systemd

### Application Framework
- **UI**: Qt 6.5.3 with QML
- **Backend**: C++17
- **Build System**: CMake 3.20+

### Key Libraries
- **Graphics**: OpenGL ES 3.0, Wayland
- **Multimedia**: GStreamer 1.0, PulseAudio
- **Networking**: Qt Network, libcurl
- **Security**: OpenSSL 3.0, TPM 2.0

### Development Tools
- **CI/CD**: GitLab CI/CD
- **Containerization**: Docker
- **Testing**: Google Test, pytest
- **Code Quality**: MISRA C++, Coverity, SonarQube

### Deployment
- **OTA**: Custom solution with A/B partitioning
- **Packaging**: DEB, RPM
- **Distribution**: Gradual rollout with canary releases
- **Monitoring**: Prometheus, Grafana

## Development Workflow

1. **Local Development**: Use Docker containers for isolated development
2. **Code Commit**: Push to GitLab repository
3. **CI Pipeline**: Automated build, test, and security scanning
4. **Staging**: Deploy to test fleet for validation
5. **Production**: Gradual OTA rollout to customer vehicles
6. **Monitoring**: Real-time telemetry and analytics
7. **Rollback**: Automatic or manual rollback if issues detected

## Quick Start Commands

```bash
# Setup development environment
./tools/build-scripts/setup-dev-env.sh

# Build the system
npm run build

# Run tests
npm run test

# Start development environment
npm run dev

# Deploy to staging
npm run deploy:staging

# Deploy OTA update to production
npm run deploy:production -- --version 2026.1.0.0
```

---
Created: November 2025
Version: 1.0
