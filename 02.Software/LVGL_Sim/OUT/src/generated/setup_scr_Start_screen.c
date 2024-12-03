/*
* Copyright 2024 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"



void setup_scr_Start_screen(lv_ui *ui)
{
    //Write codes Start_screen
    ui->Start_screen = lv_obj_create(NULL);
    lv_obj_set_size(ui->Start_screen, 240, 135);
    lv_obj_set_scrollbar_mode(ui->Start_screen, LV_SCROLLBAR_MODE_OFF);

    //Write style for Start_screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Start_screen, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Start_screen_cont_1
    ui->Start_screen_cont_1 = lv_obj_create(ui->Start_screen);
    lv_obj_set_pos(ui->Start_screen_cont_1, 0, 0);
    lv_obj_set_size(ui->Start_screen_cont_1, 240, 135);
    lv_obj_set_scrollbar_mode(ui->Start_screen_cont_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for Start_screen_cont_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Start_screen_cont_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->Start_screen_cont_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->Start_screen_cont_1, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->Start_screen_cont_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Start_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Start_screen_cont_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Start_screen_cont_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Start_screen_cont_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Start_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Start_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Start_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Start_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Start_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Start_screen_Windows_img
    ui->Start_screen_Windows_img = lv_img_create(ui->Start_screen_cont_1);
    lv_obj_add_flag(ui->Start_screen_Windows_img, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->Start_screen_Windows_img, &_windowsxp_alpha_198x89);
    lv_img_set_pivot(ui->Start_screen_Windows_img, 50,50);
    lv_img_set_angle(ui->Start_screen_Windows_img, 0);
    lv_obj_set_pos(ui->Start_screen_Windows_img, 21, 6);
    lv_obj_set_size(ui->Start_screen_Windows_img, 198, 89);

    //Write style for Start_screen_Windows_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->Start_screen_Windows_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->Start_screen_Windows_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Start_screen_Windows_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->Start_screen_Windows_img, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Start_screen_bar_1
    ui->Start_screen_bar_1 = lv_bar_create(ui->Start_screen_cont_1);
    lv_obj_set_style_anim_time(ui->Start_screen_bar_1, 100, 0);
    lv_bar_set_mode(ui->Start_screen_bar_1, LV_BAR_MODE_NORMAL);
    lv_bar_set_range(ui->Start_screen_bar_1, 0, 100);
    lv_bar_set_value(ui->Start_screen_bar_1, 100, LV_ANIM_ON);
    lv_obj_set_pos(ui->Start_screen_bar_1, 11, 105);
    lv_obj_set_size(ui->Start_screen_bar_1, 218, 20);

    //Write style for Start_screen_bar_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Start_screen_bar_1, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Start_screen_bar_1, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Start_screen_bar_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Start_screen_bar_1, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Start_screen_bar_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for Start_screen_bar_1, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Start_screen_bar_1, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Start_screen_bar_1, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Start_screen_bar_1, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Start_screen_bar_1, 10, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write codes Start_screen_Percent_label
    ui->Start_screen_Percent_label = lv_label_create(ui->Start_screen);
    lv_label_set_text(ui->Start_screen_Percent_label, "0.00%");
    lv_label_set_long_mode(ui->Start_screen_Percent_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->Start_screen_Percent_label, 75, 108);
    lv_obj_set_size(ui->Start_screen_Percent_label, 100, 15);

    //Write style for Start_screen_Percent_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Start_screen_Percent_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Start_screen_Percent_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Start_screen_Percent_label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Start_screen_Percent_label, &lv_font_Youshebiaotihei_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Start_screen_Percent_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Start_screen_Percent_label, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Start_screen_Percent_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Start_screen_Percent_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Start_screen_Percent_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Start_screen_Percent_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Start_screen_Percent_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Start_screen_Percent_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Start_screen_Percent_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Start_screen_Percent_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of Start_screen.


    //Update current screen layout.
    lv_obj_update_layout(ui->Start_screen);

}
