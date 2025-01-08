/********************************************************************************
    * 文件名称 ：adc_filter.h
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2024-12-19
    * 功     能：ADC滤波实现
*********************************************************************************/
#ifndef __ADC_FILTER_H
#define __ADC_FILTER_H

#include <stdint.h>

#define USE_AVERAGE_FILTER 1 //使用均值滤波
#define USE_MEDIAN_FILTER 0 //使用中值滤波
#define USE_KALMAN_FILTER 0 //使用卡尔曼滤波
#define USE_LPF_FILTER 0 //使用低通滤波
#define USE_HPF_FILTER 0 //使用高通滤波

#ifdef USE_AVERAGE_FILTER
#define AVERAGE_FILTER_SIZE 10 //均值滤波数组大小
float AverageFilter_Update(float *buf, uint16_t buf_size, float new_data);
#endif

#ifdef USE_MEDIAN_FILTER
float MedianFilter_Update(float *buf, uint16_t buf_size, float new_data);
#endif

#ifdef USE_LPF_FILTER
float LPF_Update(float alpha, float new_data, float old_data);
#endif

#ifdef USE_HPF_FILTER
float HPF_Update(float alpha, float new_data, float old_data);
#endif

#ifdef USE_KALMAN_FILTER
typedef struct {
    float Q; // 过程噪声协方差
    float R; // 测量噪声协方差
    float x; // 估计值
    float P; // 估计误差协方差
    float K; // 卡尔曼增益
} KalmanFilter;

void KalmanFilter_Init(KalmanFilter *kf, float Q, float R, float initial_value);
float KalmanFilter_Update(KalmanFilter *kf, float measurement);
#endif

#endif // __ADC_FILTER_H
