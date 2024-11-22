# Benchmark demo

＃＃ 概述

基准演示测试了各种情况下的性能。
例如矩形、边框、阴影、文本、图像混合、图像变换、混合模式等。
所有测试均以 50% 的不透明度重复。

测试期间对象的大小和位置使用伪随机数设置，以使基准测试可重复。

屏幕顶部显示当前测试步骤的标题以及上一步的结果。

## 运行基准测试
-在 `lv_conf.h` 或同等位置设置 `LV_USE_DEMO_BENCHMARK 1`
-在 `lv_init()` 并初始化驱动程序之后调用 `lv_demo_benchmark()`
-如果您只想出于任何目的运行特定场景（例如调试、性能优化等），您可以调用 `lv_demo_benchmark_run_scene()` 而不是 `lv_demo_benchmark()` 并传递场景编号。
-如果通过将宏“LV_USE_LOG”设置为“1”并将跟踪级别“LV_LOG_LEVEL”设置为“LV_LOG_LEVEL_USER”或更高来启用跟踪输出，则基准测试结果将以“csv”格式打印。
-如果你想知道测试何时完成，可以在调用 `lv_demo_benchmark()` 或 `lv_demo_benchmark_run_scene()` 之前通过 `lv_demo_benchmark_register_finished_handler()` 注册一个回调函数。
-如果您想了解系统的最大渲染性能，请在`lv_demo_benchmark()`之前调用`lv_demo_benchmark_set_max_speed(true)`。

## 解释结果

FPS 的测量方式如下：
-加载下一步
-在显示驱动程序的“monitor_cb”中累积渲染时间和周期数
-测量 1 秒
-计算`FPS = time_sum /render_cnt`

请注意，对于简单情况，它可能会导致非常高的 FPS 结果。
例如。如果在 5 毫秒内绘制一些简单的矩形，基准测试将显示它的帧速率为 200 FPS。
因此它忽略了“LV_DISP_REFR_PERIOD”，它告诉 LVGL 应该多久刷新一次屏幕。
换句话说，基准测试显示的是纯渲染时间的 FPS。

默认情况下，仅刷新更改的区域。这意味着如果 1 毫秒内仅更改几个像素，基准测试将显示 1000 FPS。要测量全屏刷新的性能，请取消注释“lv_demo_benchmark.c”中“monitor_cb()”中的“lv_obj_invalidate(lv_scr_act())”。

![LVGL 基准测试运行](screenshot1.png)
如果您正在进行 2D 图像处理优化的性能分析，由 disp_flush() 引入的 LCD 延迟（将数据刷新到 LCD）可能会稀释 LVGL 绘图过程的性能结果，从而使您更难看到优化结果（增益或增益）损失）。为了避免此类问题，请：

1. 在 `disp_flush()` 中使用一个标志来控制 LCD 刷新。例如：

```c
volatile bool disp_flush_enabled = true;

/* Enable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_enable_update(void)
{
    disp_flush_enabled = true;
}

/* Disable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_disable_update(void)
{
    disp_flush_enabled = false;
}

static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    if(disp_flush_enabled) {
        GLCD_DrawBitmap(area->x1,                   //!< x
                        area->y1,                   //!< y
                        area->x2 - area->x1 + 1,    //!< width
                        area->y2 - area->y1 + 1,    //!< height
                        (const uint8_t *)color_p);
    }

    /*IMPORTANT!!!
     *Inform the graphics library that you are ready with the flushing*/
    lv_disp_flush_ready(disp_drv);
}
```

2. Disable flushing before calling `lv_demo_benchmark()` or `lv_demo_benchmark_run_scene()`, for example:

```c
extern void disp_enable_update(void);
extern void disp_disable_update(void);

static void on_benchmark_finished(void)
{
    disp_enable_update();
}

int main(void)
{    
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();

    LV_LOG("Running LVGL Benchmark...");
    LV_LOG("Please stand by...");
    LV_LOG("NOTE: You will NOT see anything until the end.");

    disp_disable_update();
    
    lv_demo_benchmark_set_finished_cb(&on_benchmark_finished);
    lv_demo_benchmark_set_max_speed(true);
    lv_demo_benchmark();
    
    //lv_demo_benchmark_run_scene(43);      // run scene no 31

    ...
    while(1){
        lv_timer_handler();                 //! run lv task at the max speed
    }
}
```



3. Alternatively, you can use trace output to get the benchmark results in csv format by:
   - Setting macro `LV_USE_LOG` to `1` 
   - Setting trace level `LV_LOG_LEVEL` to `LV_LOG_LEVEL_USER` or higher.




## Result summary
In the end, a table is created to display measured FPS values.

On top of the summary screen, the "Weighted FPS" value is shown.
In this, the result of the more common cases are taken into account with a higher weight.

"Opa. speed" shows the speed of the measurements with opacity compared to full opacity.
E.g. "Opa. speed = 90%" means that rendering with opacity is 10% slower.

In the first section of the table, "Slow but common cases", those cases are displayed which are considered common but were slower than 20 FPS.

Below this in the "All cases section" all the results are shown. The < 10 FPS results are shown with red, the >= 10 but < 20 FPS values are displayed with orange.


![LVGL benchmark result summary](https://github.com/lvgl/lvgl/tree/master/demos/benchmark/screenshot2.png?raw=true)
