#pragma once

class StatusSetter {
public:
    enum JikkenState {
        Off, ReadyToRecord, Recording, ReadyToDownload, Downloading
    };
    virtual void setStatus(JikkenState state, bool alsoHasDownload = false) = 0;
    virtual JikkenState getStatus(bool& alsoHasDownload) = 0;
};