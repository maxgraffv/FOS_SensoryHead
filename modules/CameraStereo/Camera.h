#ifndef CAMERA_H
#define CAMERA_H

#include <libcamera/libcamera.h>
#include <iostream>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <iomanip>
#include <memory>
#include <thread>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include "CaptureMode.h"

class Camera
{
    private:
        std::shared_ptr<libcamera::Camera> libcam;
        std::unique_ptr<libcamera::CameraConfiguration> configuration = nullptr;
        libcamera::StreamConfiguration *streamConfig = nullptr;
        libcamera::FrameBufferAllocator *allocator = nullptr;
        libcamera::Stream *stream = nullptr;
        const std::vector<std::unique_ptr<libcamera::FrameBuffer>> *buffers = nullptr;
        std::vector<std::unique_ptr<libcamera::Request>> requests;

        std::mutex mtx;
        std::condition_variable cv;
        std::atomic<int> framesToGrab{100};

        std::string id;    
        CaptureMode capMode;


    public:
        Camera();
        void requestComplete(libcamera::Request *request);
        void init(std::shared_ptr<libcamera::Camera> libcam);
        void acquire();
        void config();
        void allocateBuffers();
        void start();
        void stop();
        
        void snap();


};




#endif