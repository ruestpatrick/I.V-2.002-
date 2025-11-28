// Main.qml - Toyota Corolla 2026 Multimedia HMI
// Entry point for the user interface

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Toyota.Multimedia 1.0

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 1920
    height: 1080
    title: "Toyota Multimedia System"
    color: "#000000"
    
    // Global theme settings
    property color primaryColor: "#EB0A1E"  // Toyota Red
    property color secondaryColor: "#1F1F1F"
    property color textColor: "#FFFFFF"
    property color accentColor: "#00A0E9"   // Toyota Blue
    
    // System state
    property bool isDarkMode: true
    property bool isVehicleMoving: VehicleInterface.speed > 5
    property int currentView: 0  // 0: Home, 1: Navigation, 2: Media, 3: Phone, 4: Settings
    
    // Boot animation
    BootAnimation {
        id: bootAnimation
        anchors.fill: parent
        visible: SystemController.bootInProgress
        z: 1000
    }
    
    // Main content area
    StackLayout {
        id: contentStack
        anchors.fill: parent
        currentIndex: mainWindow.currentView
        
        // Home screen
        HomeScreen {
            id: homeScreen
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
        
        // Navigation screen
        NavigationScreen {
            id: navigationScreen
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
        
        // Media player screen
        MediaScreen {
            id: mediaScreen
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
        
        // Phone screen
        PhoneScreen {
            id: phoneScreen
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
        
        // Settings screen
        SettingsScreen {
            id: settingsScreen
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
    
    // Bottom navigation bar
    NavigationBar {
        id: navigationBar
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: 100
        currentIndex: mainWindow.currentView
        
        onNavigationRequested: function(index) {
            mainWindow.currentView = index
        }
    }
    
    // Status bar (top)
    StatusBar {
        id: statusBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 60
        
        // Display vehicle status, time, connectivity
        vehicleSpeed: VehicleInterface.speed
        fuelLevel: VehicleInterface.fuelLevel
        isConnected: ConnectivityManager.isConnected
        currentTime: Qt.formatTime(new Date(), "hh:mm")
    }
    
    // Popup notifications
    NotificationManager {
        id: notificationManager
        anchors.top: statusBar.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 20
        width: parent.width * 0.4
    }
    
    // CarPlay/Android Auto overlay
    SmartphoneProjection {
        id: smartphoneProjection
        anchors.fill: parent
        visible: ConnectivityManager.projectionActive
        z: 500
        
        onExitRequested: {
            ConnectivityManager.exitProjection()
        }
    }
    
    // Voice assistant overlay
    VoiceAssistant {
        id: voiceAssistant
        anchors.fill: parent
        visible: VoiceController.isActive
        z: 600
    }
    
    // Update notification
    UpdateNotification {
        id: updateNotification
        anchors.centerIn: parent
        visible: OTAManager.updateAvailable
        z: 700
        
        updateVersion: OTAManager.availableVersion
        updateSize: OTAManager.updateSize
        
        onAccepted: {
            OTAManager.downloadUpdate()
        }
        
        onRejected: {
            OTAManager.dismissUpdate()
        }
    }
    
    // System connections
    Connections {
        target: VehicleInterface
        
        function onIgnitionStateChanged() {
            if (VehicleInterface.ignitionState === VehicleInterface.Off) {
                SystemController.initiateShutdown()
            }
        }
        
        function onSpeedChanged() {
            // Disable certain features while moving for safety
            if (isVehicleMoving) {
                // Lock out text input, complex menus, etc.
                SystemController.enableDrivingMode()
            } else {
                SystemController.disableDrivingMode()
            }
        }
    }
    
    Connections {
        target: OTAManager
        
        function onUpdateDownloaded() {
            notificationManager.showNotification(
                "Update Ready",
                "A system update is ready to install. Install now or schedule for later?",
                NotificationManager.Info
            )
        }
        
        function onUpdateFailed(error) {
            notificationManager.showNotification(
                "Update Failed",
                error,
                NotificationManager.Error
            )
        }
    }
    
    // Performance monitoring
    Component.onCompleted: {
        console.log("Toyota Multimedia System initialized")
        console.log("Version:", SystemController.softwareVersion)
        console.log("Boot time:", SystemController.bootTime, "ms")
        
        // Start telemetry
        TelemetryManager.startSession()
    }
    
    Component.onDestruction: {
        TelemetryManager.endSession()
    }
}
