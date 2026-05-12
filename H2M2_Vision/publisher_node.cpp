#include "h2m2_data.hpp"
#include <iceoryx_posh/popo/publisher.hpp>
#include "iceoryx_posh/runtime/posh_runtime.hpp"
#include "iceoryx_posh/mepoo/chunk_header.hpp"
#include "iceoryx_hoofs/posix_wrapper/signal_watcher.hpp"
#include <opencv2/opencv.hpp>


constexpr char APP_NAME[] = "Camera-cpp-publisher";

bool geth2m2_data(h2m2_data* const object, 
    const uint32_t temp_Frame_number,
    const double temp_time,
    const uint32_t temp_data_size,
    const uint8_t* temp_data) noexcept
{
    if(object == nullptr || temp_data == nullptr)
    {
        return false;
    }

    if(temp_data_size <= H2M2_BUFFER_SIZE)
    {
        object->Frame_Number = temp_Frame_number;
        object->time = temp_time;
        object->data_size = temp_data_size;
        std::memcpy(object->data,temp_data,temp_data_size);
        return true;
    }
    else
    {
        return false;
    }
}

int main()
{
    iox::runtime::PoshRuntime::initRuntime(APP_NAME);
    iox::popo::Publisher<h2m2_data> publisher({"H2M2", "Perception", "FrontCamera"});
    cv::VideoCapture cap("test.mp4");
    if(!cap.isOpened())
    {
        std::cerr << "Error opening video stream or file" << std::endl;
        return -1;
    }
    cv::Mat frame;
    uint32_t frame_count = 0;
    while(!iox::posix::hasTerminationRequested())
    {
        cap >> frame;
        if(frame.empty())
        {
            std::cerr << "Error reading frame" << std::endl;
            return -1;
        }
        publisher.loan().and_then([&](auto& sample){
            if(!geth2m2_data(sample.get(),
            frame_count,
            0.0,
            frame.total()*frame.elemSize(),
            frame.data))
            {
                std::cerr << "Frame data size exceeds maximum payload size!" << std::endl;
                std::exit(-1);
            }
            sample.publish();
            frame_count++;
        })
        .or_else([](auto& error){
            std::cerr << "Unable to loan sample,error: " << error << std::endl;
        });
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return 0;
}