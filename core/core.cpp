#include "CameraStereo.h"

void take_and_send(CameraStereo*);

int main(int argv, char** argc)
{
    CameraStereo *stereoCam = new CameraStereo();
    stereoCam->init();
    stereoCam->start();


    
    return 0;
}

void take_and_send(CameraStereo *cam)
{
    void *left = cam->getCamLeft()->frameMemory;
    void *right = cam->getCamRight()->frameMemory;



}