class MainManager {
public:
    enum Message {
        CameraSetupComplete, CameraOnlyMode, NormalQuit, LockAllCameraControls, UnlockAllCameraControls
    };

    virtual void update(Message msg) = 0;
};