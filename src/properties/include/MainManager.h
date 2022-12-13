class MainManager {
public:
    enum Message {
        CameraSetupComplete, CameraOnlyMode, NormalQuit
    };

    virtual void update(Message msg) = 0;
};