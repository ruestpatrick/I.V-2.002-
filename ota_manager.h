// ota_manager.h
// Over-The-Air Update Manager for Toyota Multimedia System

#ifndef TOYOTA_OTA_MANAGER_H
#define TOYOTA_OTA_MANAGER_H

#include <string>
#include <memory>
#include <functional>
#include <vector>
#include <atomic>
#include <chrono>

namespace Toyota {
namespace OTA {

enum class UpdateChannel {
    Stable,
    Beta,
    Development
};

enum class UpdateState {
    Idle,
    CheckingForUpdates,
    UpdateAvailable,
    Downloading,
    DownloadComplete,
    Installing,
    InstallComplete,
    VerificationFailed,
    RollbackRequired,
    Error
};

struct UpdateInfo {
    std::string version;
    std::string releaseNotes;
    uint64_t packageSize;
    std::string downloadUrl;
    std::string signature;
    std::string checksum;
    UpdateChannel channel;
    std::chrono::system_clock::time_point releaseDate;
    bool isMandatory;
    bool isDeltaUpdate;
};

struct UpdateProgress {
    UpdateState state;
    float progressPercentage;
    uint64_t bytesDownloaded;
    uint64_t totalBytes;
    std::string currentOperation;
    std::chrono::seconds estimatedTimeRemaining;
};

class OTAManager {
public:
    using ProgressCallback = std::function<void(const UpdateProgress&)>;
    using CompletionCallback = std::function<void(bool success, const std::string& message)>;
    
    OTAManager();
    ~OTAManager();
    
    // Prevent copying
    OTAManager(const OTAManager&) = delete;
    OTAManager& operator=(const OTAManager&) = delete;
    
    /**
     * @brief Initialize the OTA manager
     * @param serverUrl The OTA server URL
     * @param vehicleId Unique vehicle identifier
     * @param channel Update channel to subscribe to
     * @return true if initialization successful
     */
    bool initialize(const std::string& serverUrl, 
                   const std::string& vehicleId,
                   UpdateChannel channel = UpdateChannel::Stable);
    
    /**
     * @brief Check for available updates
     * @param callback Called when check completes
     */
    void checkForUpdates(CompletionCallback callback);
    
    /**
     * @brief Get information about available update
     * @return UpdateInfo if update available, nullptr otherwise
     */
    std::unique_ptr<UpdateInfo> getAvailableUpdate() const;
    
    /**
     * @brief Download the available update
     * @param progressCallback Called periodically with download progress
     * @param completionCallback Called when download completes
     */
    void downloadUpdate(ProgressCallback progressCallback,
                       CompletionCallback completionCallback);
    
    /**
     * @brief Install the downloaded update
     * @param progressCallback Called periodically with installation progress
     * @param completionCallback Called when installation completes
     */
    void installUpdate(ProgressCallback progressCallback,
                      CompletionCallback completionCallback);
    
    /**
     * @brief Cancel ongoing download or installation
     */
    void cancelOperation();
    
    /**
     * @brief Verify update package integrity
     * @return true if package is valid and signed correctly
     */
    bool verifyUpdatePackage(const std::string& packagePath);
    
    /**
     * @brief Rollback to previous version
     * @return true if rollback successful
     */
    bool rollbackToPreviousVersion();
    
    /**
     * @brief Get current software version
     */
    std::string getCurrentVersion() const;
    
    /**
     * @brief Get update channel
     */
    UpdateChannel getUpdateChannel() const;
    
    /**
     * @brief Set update channel
     */
    void setUpdateChannel(UpdateChannel channel);
    
    /**
     * @brief Check if conditions are suitable for update
     * @return true if safe to update (parked, battery OK, etc.)
     */
    bool areUpdateConditionsMet() const;
    
    /**
     * @brief Get update history
     */
    std::vector<UpdateInfo> getUpdateHistory() const;
    
    /**
     * @brief Report telemetry to cloud
     */
    void reportTelemetry(const std::string& event, const std::string& data);

private:
    class Implementation;
    std::unique_ptr<Implementation> impl_;
    
    // Internal methods
    bool downloadChunk(const std::string& url, uint64_t offset, uint64_t size);
    bool verifySignature(const std::string& packagePath, const std::string& signature);
    bool checkBatteryLevel() const;
    bool checkStorageSpace() const;
    bool checkNetworkConnection() const;
    bool isVehicleParked() const;
    bool switchBootPartition();
    void notifyUpdateResult(bool success, const std::string& version);
};

/**
 * @brief Secure boot verification
 */
class SecureBoot {
public:
    static bool verifyBootChain();
    static bool verifyKernelSignature();
    static bool verifySystemPartition();
    static std::string getTPMAttestation();
};

/**
 * @brief A/B partition manager
 */
class PartitionManager {
public:
    enum class Partition {
        A,
        B
    };
    
    static Partition getCurrentPartition();
    static Partition getInactivePartition();
    static bool setBootPartition(Partition partition);
    static bool markPartitionAsGood(Partition partition);
    static bool markPartitionAsBad(Partition partition);
    static uint32_t getBootAttempts();
    static void resetBootAttempts();
};

/**
 * @brief Update package handler
 */
class UpdatePackage {
public:
    UpdatePackage(const std::string& packagePath);
    
    bool open();
    bool verify();
    bool extract(const std::string& destinationPath);
    UpdateInfo getMetadata() const;
    std::vector<std::string> getFileList() const;
    
private:
    std::string packagePath_;
    std::unique_ptr<class PackageImpl> impl_;
};

} // namespace OTA
} // namespace Toyota

#endif // TOYOTA_OTA_MANAGER_H
