#ifndef CAMERA_STEREO_H
#define CAMERA_STEREO_H

#include "Camera.h"
#include "CaptureMode.h"

class CameraStereo
{
    private:
    std::shared_ptr<Camera> camLeft = std::make_shared<Camera>();
    std::shared_ptr<Camera> camRight = std::make_unique<Camera>();
    std::string camLeftId, camRightId;
    std::unique_ptr<libcamera::CameraManager> cameraManager = std::make_unique<libcamera::CameraManager>();
    CaptureMode capMode;

    void config();
    void allocateBuffers();
    void start();
    void stop();

    
    public:
    CameraStereo();
    void init();
    void setCaptureMode(CaptureMode mode);
    void snap();




};


#endif