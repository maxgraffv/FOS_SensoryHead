#ifndef CAMERA_STEREO_H
#define CAMERA_STEREO_H

#include "SingleCamera.h"
#include "CaptureMode.h"

class CameraStereo
{
    private:
    std::shared_ptr<SingleCamera> camLeft = std::make_shared<SingleCamera>();
    std::shared_ptr<SingleCamera> camRight = std::make_shared<SingleCamera>();
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