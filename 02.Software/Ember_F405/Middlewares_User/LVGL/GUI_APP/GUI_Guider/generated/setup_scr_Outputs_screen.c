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



void setup_scr_Outputs_screen(lv_ui *ui)
{
    //Write codes Outputs_screen
    ui->Outputs_screen = lv_obj_create(NULL);
    lv_obj_set_size(ui->Outputs_screen, 240, 135);
    lv_obj_set_scrollbar_mode(ui->Outputs_screen, LV_SCROLLBAR_MODE_OFF);

    //Write style for Outputs_screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Outputs_screen, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Outputs_screen_cont_1
    ui->Outputs_screen_cont_1 = lv_obj_create(ui->Outputs_screen);
    lv_obj_set_pos(ui->Outputs_screen_cont_1, 0, 0);
    lv_obj_set_size(ui->Outputs_screen_cont_1, 240, 135);
    lv_obj_set_scrollbar_mode(ui->Outputs_screen_cont_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for Outputs_screen_cont_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Outputs_screen_cont_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->Outputs_screen_cont_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->Outputs_screen_cont_1, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->Outputs_screen_cont_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Outputs_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Outputs_screen_cont_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Outputs_screen_cont_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Outputs_screen_cont_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Outputs_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Outputs_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Outputs_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Outputs_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Outputs_screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Outputs_screen_cont_2
    ui->Outputs_screen_cont_2 = lv_obj_create(ui->Outputs_screen_cont_1);
    lv_obj_set_pos(ui->Outputs_screen_cont_2, 6, 25);
    lv_obj_set_size(ui->Outputs_screen_cont_2, 225, 101);
    lv_obj_set_scrollbar_mode(ui->Outputs_screen_cont_2, LV_SCROLLBAR_MODE_OFF);

    //Write style for Outputs_screen_cont_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Outputs_screen_cont_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->Outputs_screen_cont_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->Outputs_screen_cont_2, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->Outputs_screen_cont_2, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Outputs_screen_cont_2, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Outputs_screen_cont_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Outputs_screen_cont_2, lv_color_hex(0xa2a2a2), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Outputs_screen_cont_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Outputs_screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Outputs_screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Outputs_screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Outputs_screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Outputs_screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Outputs_screen_Consle_lable
    ui->Outputs_screen_Consle_lable = lv_label_create(ui->Outputs_screen_cont_2);
    lv_label_set_text(ui->Outputs_screen_Consle_lable, "datas");
    lv_label_set_long_mode(ui->Outputs_screen_Consle_lable, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->Outputs_screen_Consle_lable, 2, 2);
    lv_obj_set_size(ui->Outputs_screen_Consle_lable, 217, 94);

    //Write style for Outputs_screen_Consle_lable, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Outputs_screen_Consle_lable, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Outputs_screen_Consle_lable, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Outputs_screen_Consle_lable, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Outputs_screen_Consle_lable, &lv_font_Youshebiaotihei_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Outputs_screen_Consle_lable, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Outputs_screen_Consle_lable, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Outputs_screen_Consle_lable, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Outputs_screen_Consle_lable, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Outputs_screen_Consle_lable, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Outputs_screen_Consle_lable, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Outputs_screen_Consle_lable, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Outputs_screen_Consle_lable, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Outputs_screen_Consle_lable, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Outputs_screen_Consle_lable, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Outputs_screen_Output_lable
    ui->Outputs_screen_Output_lable = lv_label_create(ui->Outputs_screen_cont_1);
    lv_label_set_text(ui->Outputs_screen_Output_lable, "Outputs");
    lv_label_set_long_mode(ui->Outputs_screen_Output_lable, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->Outputs_screen_Output_lable, 31, 7);
    lv_obj_set_size(ui->Outputs_screen_Output_lable, 90, 18);

    //Write style for Outputs_screen_Output_lable, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Outputs_screen_Output_lable, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Outputs_screen_Output_lable, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Outputs_screen_Output_lable, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Outputs_screen_Output_lable, &lv_font_Youshebiaotihei_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Outputs_screen_Output_lable, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Outputs_screen_Output_lable, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Outputs_screen_Output_lable, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Outputs_screen_Output_lable, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Outputs_screen_Output_lable, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Outputs_screen_Output_lable, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Outputs_screen_Output_lable, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Outputs_screen_Output_lable, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Outputs_screen_Output_lable, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Outputs_screen_Output_lable, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Outputs_screen_Runtime_lable
    ui->Outputs_screen_Runtime_lable = lv_label_create(ui->Outputs_screen_cont_1);
    lv_label_set_text(ui->Outputs_screen_Runtime_lable, "00H00M00S");
    lv_label_set_long_mode(ui->Outputs_screen_Runtime_lable, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->Outputs_screen_Runtime_lable, 130, 13);
    lv_obj_set_size(ui->Outputs_screen_Runtime_lable, 105, 9);

    //Write style for Outputs_screen_Runtime_lable, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Outputs_screen_Runtime_lable, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Outputs_screen_Runtime_lable, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Outputs_screen_Runtime_lable, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Outputs_screen_Runtime_lable, &lv_font_Youshebiaotihei_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Outputs_screen_Runtime_lable, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Outputs_screen_Runtime_lable, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Outputs_screen_Runtime_lable, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Outputs_screen_Runtime_lable, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Outputs_screen_Runtime_lable, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Outputs_screen_Runtime_lable, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Outputs_screen_Runtime_lable, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Outputs_screen_Runtime_lable, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Outputs_screen_Runtime_lable, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Outputs_screen_Runtime_lable, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Outputs_screen_img_1
    ui->Outputs_screen_img_1 = lv_img_create(ui->Outputs_screen_cont_1);
    lv_obj_add_flag(ui->Outputs_screen_img_1, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->Outputs_screen_img_1, &_indent_alpha_20x20);
    lv_img_set_pivot(ui->Outputs_screen_img_1, 50,50);
    lv_img_set_angle(ui->Outputs_screen_img_1, 0);
    lv_obj_set_pos(ui->Outputs_screen_img_1, 10, 5);
    lv_obj_set_size(ui->Outputs_screen_img_1, 20, 20);

    //Write style for Outputs_screen_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->Outputs_screen_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->Outputs_screen_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Outputs_screen_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->Outputs_screen_img_1, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Outputs_screen_label_1
    ui->Outputs_screen_label_1 = lv_label_create(ui->Outputs_screen_cont_1);
    lv_label_set_text(ui->Outputs_screen_label_1, "Runtime");
    lv_label_set_long_mode(ui->Outputs_screen_label_1, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->Outputs_screen_label_1, 130, 3);
    lv_obj_set_size(ui->Outputs_screen_label_1, 105, 9);

    //Write style for Outputs_screen_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Outputs_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Outputs_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Outputs_screen_label_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Outputs_screen_label_1, &lv_font_Youshebiaotihei_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Outputs_screen_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Outputs_screen_label_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Outputs_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Outputs_screen_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Outputs_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Outputs_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Outputs_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Outputs_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Outputs_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Outputs_screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of Outputs_screen.


    //Update current screen layout.
    lv_obj_update_layout(ui->Outputs_screen);

}
