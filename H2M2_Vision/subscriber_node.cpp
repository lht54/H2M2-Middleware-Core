#include "h2m2_data.hpp"
#include "iceoryx_posh/popo/subscriber.hpp"
#include "iceoryx_posh/runtime/posh_runtime.hpp"
#include "iceoryx_hoofs/posix_wrapper/signal_watcher.hpp"
#include <opencv2/opencv.hpp>
constexpr char APP_NAME[] = "Camera-cpp-subscriber";
int main()
{
    // 1. 初始化 Iceoryx 运行时
    iox::runtime::PoshRuntime::initRuntime(APP_NAME);
    // 2. 创建订阅者
    iox::popo::Subscriber<h2m2_data> subscriber({"H2M2", "Perception", "FrontCamera"});
    int64 t0 = cv::getTickCount();
    bool is_paused = false;
    bool is_running = true;
    while(!iox::posix::hasTerminationRequested() && is_running)
    {
        subscriber
            .take()
            .and_then([&](auto& sample){
            if (sample->Frame_Number % 30 == 0)
            {
                const int N = 30;
                uint64_t t1 = cv::getTickCount();
                std::cout << "          Frame Number:" << cv::format("%5lld", (long long)sample->Frame_Number)
                          << "           Average FPS:" << cv::format("%9.1f",(double)cv::getTickFrequency() * N / (t1 - t0))
                          << "Average time per frame:" << cv::format("%9.2f",(double)(t1 - t0) * 1000.0f / cv::getTickFrequency() / N) << " ms"
                          << std::endl;
                t0 = t1;
            }
            cv::Mat frame(2160, 3840, CV_8UC3, (void*)sample->data);
            if(!is_paused)
            {
                cv::imshow("Frame", frame);
            }
            int key = cv::waitKey(1);
            if(key == 27)//ESC键退出
            {
                is_running = false;
                std::cout << "Exiting..." << std::endl;
                return;
            }
            if(key == 32)//空格键暂停/继续
            {
                is_paused = !is_paused;
                std::cout << "Enable frame processing ('space' key): " << is_paused << std::endl;
            }
    })
            .or_else([](auto& result){
                if (result != iox::popo::ChunkReceiveResult::NO_CHUNK_AVAILABLE)
                {
                    std::cout << "Error receiving chunk." << std::endl;
                }
            });
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return (EXIT_SUCCESS);
}