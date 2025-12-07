#include "CameraStereo.h"

CameraStereo::CameraStereo()
{



}

void CameraStereo::init()
{
    int ret = this->cameraManager->start();
    if (ret < 0) {
        std::cerr << "Camera Manager failed to start" << std::endl;
        return ;
    }

    std::vector<std::shared_ptr<libcamera::Camera>> cams = cameraManager->cameras();
    if (cams.empty()) {
        std::cerr << "No cameras detected" << std::endl;
        return ;
    }

    if (cams.size() != 2) {
        std::cerr << "Camera count: " << cams.size() << std::endl;
        std::cerr << "StereoCamera requires 2" << std::endl;
        return ;
    }

    camLeft->init(cams[0]);

    camLeftId = cams[0]->id();
    camRightId = cams[0]->id();

    camLeft->init(cameraManager->get(camLeftId));
    camRight->init(cameraManager->get(camRightId));


    camLeft->acquire();
    camRight->acquire();

    this->config();
    this->allocateBuffers();
}


void CameraStereo::config()
{
    camLeft->config();
    camRight->config();
}


void CameraStereo::allocateBuffers()
{
    camLeft->allocateBuffers();
    camRight->allocateBuffers();
}


void CameraStereo::start()
{
    camLeft->start();
    camRight->start();
}


void CameraStereo::stop()
{
    camLeft->stop();
    camRight->stop();
    this->cameraManager->stop();
}


std::shared_ptr<SingleCamera> CameraStereo::getCamLeft()
{
    return camLeft;
}


std::shared_ptr<SingleCamera> CameraStereo::getCamRight()
{
    return camRight;
}