/********************************************************************************
    * 文件名称 ：adc_filter.c
    * 作     者：ZM
    * 版     本：V1.0
    * 编写日期 ：2024-12-19
    * 功     能：ADC滤波实现
*********************************************************************************/
#include "adc_filter.h"

/* 均值滤波 */
#ifdef USE_AVERAGE_FILTER
float AverageFilter_Update(float *buf, uint16_t buf_size, float new_data) {
    static uint16_t index = 0;
    static float sum = 0;
    static float average = 0;

    sum -= buf[index];
    sum += new_data;
    buf[index] = new_data;
    index = (index + 1) % buf_size;
    average = sum / buf_size;

    return average;
}
#endif // USE_AVERAGE_FILTE

#ifdef USE_MEDIAN_FILTER
/* 中值滤波 */
float MedianFilter_Update(float *buf, uint16_t buf_size, float new_data) {
    static uint16_t index = 0;
    static float median = 0;

    buf[index] = new_data;
    index = (index + 1) % buf_size;

    // 冒泡排序
    for (uint16_t i = 0; i < buf_size - 1; i++) {
        for (uint16_t j = 0; j < buf_size - 1 - i; j++) {
            if (buf[j] > buf[j + 1]) {
                float temp = buf[j];
                buf[j] = buf[j + 1];
                buf[j + 1] = temp;
            }
        }
    }

    if (buf_size % 2 == 0) {
        median = (buf[buf_size / 2 - 1] + buf[buf_size / 2]) / 2;
    } else {
        median = buf[buf_size / 2];
    }

    return median;
}

#endif // USE_MEDIAN_FILTER

/* 低通滤波 */
#ifdef USE_LPF_FILTER
float LPF_Update(float alpha, float new_data, float old_data) {
    return alpha * new_data + (1 - alpha) * old_data;
}
#endif // USE_LPF_FILTER

/* 高通滤波 */
#ifdef USE_HPF_FILTER
float HPF_Update(float alpha, float new_data, float old_data) {
    return alpha * (old_data + new_data);
}
#endif // USE_HPF_FILTER

/* 卡尔曼滤波 */
#ifdef USE_KALMAN_FILTER
void KalmanFilter_Init(KalmanFilter *kf, float Q, float R, float initial_value){
    kf->Q = Q;
    kf->R = R;
    kf->x = initial_value;
    kf->P = 1.0;
    kf->K = 0.0;
}

float KalmanFilter_Update(KalmanFilter *kf, float measurement) {
    // 预测更新
    kf->P += kf->Q;

    // 计算卡尔曼增益
    kf->K = kf->P / (kf->P + kf->R);

    // 更新估计值
    kf->x += kf->K * (measurement - kf->x);

    // 更新估计误差协方差
    kf->P *= (1 - kf->K);

    return kf->x;
}
#endif // USE_KALMAN_FILTER

