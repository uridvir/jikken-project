class MainManager {
public:
    enum Message {
        CameraSetupComplete, CameraOnlyMode, NormalQuit, HarshWarningQuit, LockAllCameraControls, UnlockAllCameraControls
    };

    virtual bool update(Message msg, bool runHealthCheck = true) = 0;
};