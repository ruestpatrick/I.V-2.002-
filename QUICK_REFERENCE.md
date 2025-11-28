# Toyota Corolla 2026 Multimedia - Quick Reference Card

## 🎯 Project Type
**Modern Automotive Infotainment System with OTA Updates**

## 💻 Core Technologies
- **OS**: Automotive Grade Linux (Yocto)
- **UI**: Qt 6.5.3 (QML)
- **Backend**: C++17
- **Build**: CMake + Yocto/BitBake
- **CI/CD**: GitLab
- **Containers**: Docker

## 📦 Main Components

| Component | Location | Description |
|-----------|----------|-------------|
| UI | `src/hmi/` | Qt/QML interface |
| OTA | `src/ota/` | Update system |
| Services | `src/core/` | Core services |
| Vehicle | `src/vehicle-interface/` | CAN bus |
| Connectivity | `src/connectivity/` | CarPlay/Android Auto |

## 🚀 Essential Commands

```bash
# Setup
./tools/build-scripts/setup-dev-env.sh

# Build
npm run build                  # Everything
npm run build:hmi              # UI only
npm run build:services         # Services only

# Test
npm run test                   # All tests
npm run test:unit              # Unit tests
npm run test:integration       # Integration

# Development
npm run dev                    # Start containers
npm run simulator              # Vehicle simulator

# Deploy
npm run deploy:staging         # To staging
./deployment/scripts/deploy-ota.sh --version 2026.1.0.0
```

## 📋 OTA Deployment Process

```
1. Build & Test (CI/CD)
2. Create Package
3. Sign with RSA-4096
4. Deploy to Staging (3-5 days)
5. Canary Release (1% - 24h)
6. Gradual Rollout:
   - 10% @ 6 hours
   - 25% @ 12 hours  
   - 50% @ 24 hours
   - 100% @ 48 hours
7. Monitor & Rollback if needed
```

## 🔒 Security Features
✅ Secure Boot Chain
✅ RSA-4096 Code Signing
✅ TPM 2.0 Attestation
✅ A/B Partitions
✅ TLS 1.3 Updates
✅ Automatic Rollback

## 📁 Key Files

| File | Purpose |
|------|---------|
| `README.md` | Project overview |
| `GETTING_STARTED.md` | Getting started guide |
| `PROJECT_STRUCTURE.md` | Directory structure |
| `.gitlab-ci.yml` | CI/CD pipeline |
| `CMakeLists.txt` | Build config |
| `deployment/scripts/deploy-ota.sh` | OTA deployment |
| `docs/deployment/DEPLOYMENT_ARCHITECTURE.md` | Architecture |

## 🎓 Use Cases
- **Learning**: Modern automotive software architecture
- **Reference**: OTA update implementation
- **Template**: Starting point for similar systems
- **Demo**: Enterprise deployment practices

## 📊 Deployment Stats
- **Build Time**: ~4 hours (full Yocto)
- **Test Duration**: ~8 hours (with HIL)
- **Rollout Time**: ~2 weeks (to 100%)
- **Success Target**: 95%+ update success rate

## 🛠️ Development Tools
- Docker containers for isolated dev
- CAN bus simulator
- Vehicle simulator
- Mock OTA server
- Hardware-in-Loop test benches

## ⚡ Quick Test

```bash
# Start all services
docker-compose -f deployment/docker/docker-compose.yml up -d

# Check services
docker-compose ps

# View logs
docker-compose logs -f vehicle-simulator

# Stop all
docker-compose down
```

## 🔗 Simulated URLs
- OTA Server: `ota.toyota.cloud`
- Staging: `staging-multimedia.toyota.internal`
- Docs: `docs.toyota.internal/multimedia-api`
- Wiki: `wiki.toyota.internal/multimedia/corolla-2026`

## 📈 Compliance
- ISO 26262 (Functional Safety)
- ISO/SAE 21434 (Cybersecurity)
- MISRA C++ 2008
- AUTOSAR Adaptive Platform

## 🎯 Project Highlights
✅ Complete source code structure
✅ Production CI/CD pipeline  
✅ Full OTA implementation
✅ Docker dev environment
✅ Yocto build system
✅ Comprehensive docs
✅ Security by design
✅ Enterprise deployment patterns

---

**Version**: 2026.1.0.0
**Status**: Production Ready (Simulated)
**Created**: November 2025
