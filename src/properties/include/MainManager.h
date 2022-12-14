class MainManager {
public:
    enum Message {
        CameraSetupComplete, CameraOnlyMode, Quit, LockAllCameraControls, UnlockAllCameraControls
    };

    virtual bool update(Message msg, bool runHealthCheck = true) = 0;
};