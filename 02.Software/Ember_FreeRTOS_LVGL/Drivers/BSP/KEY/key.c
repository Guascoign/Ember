#include "KEY\key.h"
#include "BEEP/beep.h"
#include "circle_buffer.h"







//KEY1初始化
struct KEY_Device KEY1 = {
    .KEY_GPIO_Port = KEY1_GPIO_Port,
    .GPIO_Pin = KEY1_Pin,
    .name = "KEY1",
    .Status = KEY_EVENT_RELEASE,
    .priv_data = NULL
};
//KEY2初始化
struct KEY_Device KEY2 = {
    .KEY_GPIO_Port = KEY2_GPIO_Port,
    .GPIO_Pin = KEY2_Pin,
    .name = "KEY2",
    .Status = KEY_EVENT_RELEASE,
    .priv_data = NULL
};
