#include "VideoSubscriber.h"

#include <map>
#include <opencv2/videoio.hpp>

const int buffSize = 32;

class CameraVideoCleaner : public VideoSubscriber {
    cv::Rect crop;
    cv::Size size;
    cv::VideoWriter* rec;
    cv::Mat gcFrame1;
    int expectedFrames;
    bool started = false;
    std::mutex* waitHandle;

public:
    CameraVideoCleaner(cv::VideoWriter* rec, cv::Rect crop, cv::Size size, int expectedFrames, std::mutex* waitHandle) 
            : buff(GreycropBufferCache(this, buffSize)) {
        this->rec = rec;
        this->crop = crop;
        this->size = size;
        this->expectedFrames = expectedFrames;
        this->waitHandle = waitHandle;
    }
    void onReceiveMat(const cv::Mat& mat) override;
    bool isDone() override { return done; };
    void onRemove() override { waitHandle->unlock(); };

    // GreycropBufferCache uses this
    void greycrop(const cv::Mat& mat, cv::Mat& greycropped);

    bool failure = false;

private:
    /**
     * Keeps track of uncropped frame buffer, and caches the greycropped frames.
     * So, if a greycropped frame is required, greycropping only needs to happen once.
     */
    class GreycropBufferCache {
        CameraVideoCleaner* parent;
    public:
        std::map<size_t, cv::Mat> gcs;
        std::vector<cv::Mat> raws;
        GreycropBufferCache(CameraVideoCleaner* parent, size_t size) {
            this->parent = parent;
            raws.reserve(size);
        }

        // Caching
        cv::Mat gcAt(size_t index) {
            if (gcs.count(index) == 1) return gcs.at(index);

            cv::Mat greycropped;
            parent->greycrop(raws[index], greycropped);
            gcs.insert({index, greycropped});
            return greycropped;
        }
    };

    bool matDiff(const cv::Mat& mat1, const cv::Mat& mat2);
    size_t gcFrame2BuffPos();

    // Help cleaner keep state
    bool done = false;
    GreycropBufferCache buff;
    int absFrameNum = 0, frameNum = 1;

    enum CleanerState {
        Frame1, FindingFrame2, Main
    };

    CleanerState state = Frame1;
};