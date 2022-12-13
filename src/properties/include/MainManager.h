class MainManager {
public:
    enum Message {
        CameraSetupComplete
    };

    virtual void update(Message msg) = 0;
};