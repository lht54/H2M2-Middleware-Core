#ifndef H2M2_DATA_HPP
#define H2M2_DATA_HPP
#define H2M2_BUFFER_SIZE (3840 * 2160 * 3)
#include <thread>
#include <chrono>
#include <cstring>
#include <iostream>
#include <cstdint>
struct h2m2_data
{
    h2m2_data() noexcept
    {

    }
    uint32_t Frame_Number = 0;//帧序号
    double time = 0.0;//时间戳
    uint32_t data_size = 0;//真实数据大小
    uint8_t data[H2M2_BUFFER_SIZE];//4k视频
};
#endif