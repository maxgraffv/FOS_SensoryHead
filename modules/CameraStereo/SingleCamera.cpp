#include "SingleCamera.h"

SingleCamera::SingleCamera()
{

}

void SingleCamera::requestComplete(libcamera::Request *request)
{
    if (request->status() == libcamera::Request::RequestCancelled)
        return ;

    const auto &buffers = request->buffers();

    for (auto &[stream, buffer] : buffers)
    {
        const libcamera::FrameMetadata &metadata = buffer->metadata();

        std::cout << metadata.sequence << std::endl;
        const auto &metaPlanes = metadata.planes();

        const libcamera::FrameBuffer::Plane &plane = buffer->planes()[0];
        const libcamera::FrameMetadata::Plane &planeMeta = metaPlanes[0];

        this->frameSize = plane.length;
        this->frameMemory = mmap(NULL, plane.length, PROT_READ, MAP_SHARED, plane.fd.get(), 0);
        if (frameMemory == MAP_FAILED)
        {
            std::cerr << "mmap() failed" << std::endl;
            continue;
        }


		/*
		struct FrameView
		{
			uint8_t *data;
			size_t   sizeBytes;

			uint32_t width;
			uint32_t height;
			uint32_t stride;      // liczba bajtów na linię
			libcamera::PixelFormat pixelFormat;
		};

		FrameView frameView{
            .data        = static_cast<uint8_t *>(memory),
            .sizeBytes   = planeMeta.bytesused,
            .width       = stream->configuration().size.width,
            .height      = stream->configuration().size.height,
            .stride      = stream->configuration().stride,
            .pixelFormat = stream->configuration().pixelFormat,
        };
		*/

        // modify(frame)
        // send(frame)
        munmap(frameMemory,plane.length);
    }

    int remaining = --framesToGrab;
    if (remaining <= 0)
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.notify_one();
        return;
    }

    request->reuse(libcamera::Request::ReuseBuffers);
    int ret = this->libcam->queueRequest(request);
    if (ret < 0)
        std::cerr << "Failed to queue request: " << ret << std::endl;
}

void SingleCamera::init(std::shared_ptr<libcamera::Camera> libcam)
{
    this->libcam = libcam;
}

void SingleCamera::acquire()
{
    int ret = this->libcam->acquire();
    if (ret < 0) {
        std::cerr << "Camera " <<this->id << "couldn't be acquired" << std::endl;
        return ;
    }
}


void SingleCamera::config()
{
    this->configuration = this->libcam->generateConfiguration( {libcamera::StreamRole::VideoRecording} );
    this->streamConfig = &configuration->at(0);

    streamConfig->size.width = 1920;
    streamConfig->size.height = 1080;

    streamConfig->pixelFormat = libcamera::formats::YUV420;
    /*
		[  Y plane (W*H bajtów)  ]
		[ U plane (W*H/4 bajtów) ]
		[ V plane (W*H/4 bajtów) ]
	*/

    std::cout << "Default viewfinder configuration is: " << streamConfig->toString() << std::endl;
    std::cout << "Pixel format: " << streamConfig->pixelFormat.toString() << std::endl;

    this->configuration->validate();
    std::cout << "Validated viewfinder configuration is: " << streamConfig->toString() << std::endl;

    this->libcam->configure(configuration.get());
}


void SingleCamera::allocateBuffers()
{
    this->allocator = new libcamera::FrameBufferAllocator(this->libcam);

    for (libcamera::StreamConfiguration &cfg : *configuration) {
        int ret = allocator->allocate(cfg.stream());
        if (ret < 0) {
            std::cerr << "Can't allocate buffers" << std::endl;
            return ;
        }

        size_t allocated = this->allocator->buffers(cfg.stream()).size();
        std::cout << "Allocated " << allocated << " buffers for stream" << std::endl;
    }
}


void SingleCamera::start()
{
    this->stream = this->streamConfig->stream();

    this->buffers = &this->allocator->buffers(this->stream);
    
    for (unsigned int i = 0; i < buffers->size(); i++) {
        std::unique_ptr<libcamera::Request> req = this->libcam->createRequest();
        if (!req) {
            std::cerr << "Can't create a request" << std::endl;
            return ;
        }

        const std::unique_ptr<libcamera::FrameBuffer> &buffer = buffers->at(i);
        int ret = req->addBuffer(stream, buffer.get());
        if (!req) {
            std::cerr << "Can't set buffer for request" << std::endl;
            return ;
        }
        requests.push_back(std::move(req));
    }

    this->libcam->requestCompleted.connect(this, &SingleCamera::requestComplete);

    this->libcam->start();
    for (std::unique_ptr<libcamera::Request> &req : requests) {
        this->libcam->queueRequest(req.get());
    }

    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]{return framesToGrab <= 0;});
        // lock.unlock();
    }

}


void SingleCamera::stop()
{
    this->libcam->stop();
    this->allocator->free(this->stream);
    delete this->allocator;
    this->libcam->release();
    this->libcam.reset();
}


int SingleCamera::getFrameSize()
{
    return this->frameSize;
}