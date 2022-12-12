#pragma once

class StatusSetter {
public:
    enum JikkenState {
        Off, RecReady, Recording, ReadyToDownload, Downloading
    };
    virtual void setStatus(JikkenState state, bool alsoHasDownload = false) = 0;
};