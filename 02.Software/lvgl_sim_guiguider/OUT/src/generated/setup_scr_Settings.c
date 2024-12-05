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



void setup_scr_Settings(lv_ui *ui)
{
    //Write codes Settings
    ui->Settings = lv_obj_create(NULL);
    lv_obj_set_size(ui->Settings, 240, 135);
    lv_obj_set_scrollbar_mode(ui->Settings, LV_SCROLLBAR_MODE_OFF);

    //Write style for Settings, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Settings, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Settings_menu_1
    ui->Settings_menu_1 = lv_menu_create(ui->Settings);

    //Create sidebar page for menu Settings_menu_1
    ui->Settings_menu_1_sidebar_page = lv_menu_page_create(ui->Settings_menu_1, "Settings");
    lv_menu_set_sidebar_page(ui->Settings_menu_1, ui->Settings_menu_1_sidebar_page);
    lv_obj_set_scrollbar_mode(ui->Settings_menu_1_sidebar_page, LV_SCROLLBAR_MODE_OFF);

    //Create subpage for Settings_menu_1
    ui->Settings_menu_1_subpage_1 = lv_menu_page_create(ui->Settings_menu_1, NULL);
    ui->Settings_menu_1_cont_1 = lv_menu_cont_create(ui->Settings_menu_1_sidebar_page);
    ui->Settings_menu_1_label_1 = lv_label_create(ui->Settings_menu_1_cont_1);
    lv_label_set_text(ui->Settings_menu_1_label_1, "RGB");
    lv_obj_set_size(ui->Settings_menu_1_label_1, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(ui->Settings_menu_1_subpage_1, LV_SCROLLBAR_MODE_OFF);
    lv_menu_set_load_page_event(ui->Settings_menu_1, ui->Settings_menu_1_cont_1, ui->Settings_menu_1_subpage_1);

    //Create subpage for Settings_menu_1
    ui->Settings_menu_1_subpage_2 = lv_menu_page_create(ui->Settings_menu_1, NULL);
    ui->Settings_menu_1_cont_2 = lv_menu_cont_create(ui->Settings_menu_1_sidebar_page);
    ui->Settings_menu_1_label_2 = lv_label_create(ui->Settings_menu_1_cont_2);
    lv_label_set_text(ui->Settings_menu_1_label_2, "Buzzer");
    lv_obj_set_size(ui->Settings_menu_1_label_2, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(ui->Settings_menu_1_subpage_2, LV_SCROLLBAR_MODE_OFF);
    lv_menu_set_load_page_event(ui->Settings_menu_1, ui->Settings_menu_1_cont_2, ui->Settings_menu_1_subpage_2);

    //Create subpage for Settings_menu_1
    ui->Settings_menu_1_subpage_3 = lv_menu_page_create(ui->Settings_menu_1, NULL);
    ui->Settings_menu_1_cont_3 = lv_menu_cont_create(ui->Settings_menu_1_sidebar_page);
    ui->Settings_menu_1_label_3 = lv_label_create(ui->Settings_menu_1_cont_3);
    lv_label_set_text(ui->Settings_menu_1_label_3, "EEPROM");
    lv_obj_set_size(ui->Settings_menu_1_label_3, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(ui->Settings_menu_1_subpage_3, LV_SCROLLBAR_MODE_OFF);
    lv_menu_set_load_page_event(ui->Settings_menu_1, ui->Settings_menu_1_cont_3, ui->Settings_menu_1_subpage_3);

    //Create subpage for Settings_menu_1
    ui->Settings_menu_1_subpage_4 = lv_menu_page_create(ui->Settings_menu_1, NULL);
    ui->Settings_menu_1_cont_4 = lv_menu_cont_create(ui->Settings_menu_1_sidebar_page);
    ui->Settings_menu_1_label_4 = lv_label_create(ui->Settings_menu_1_cont_4);
    lv_label_set_text(ui->Settings_menu_1_label_4, "FLASH");
    lv_obj_set_size(ui->Settings_menu_1_label_4, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(ui->Settings_menu_1_subpage_4, LV_SCROLLBAR_MODE_OFF);
    lv_menu_set_load_page_event(ui->Settings_menu_1, ui->Settings_menu_1_cont_4, ui->Settings_menu_1_subpage_4);

    //Create subpage for Settings_menu_1
    ui->Settings_menu_1_subpage_5 = lv_menu_page_create(ui->Settings_menu_1, NULL);
    ui->Settings_menu_1_cont_5 = lv_menu_cont_create(ui->Settings_menu_1_sidebar_page);
    ui->Settings_menu_1_label_5 = lv_label_create(ui->Settings_menu_1_cont_5);
    lv_label_set_text(ui->Settings_menu_1_label_5, "MPU");
    lv_obj_set_size(ui->Settings_menu_1_label_5, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_scrollbar_mode(ui->Settings_menu_1_subpage_5, LV_SCROLLBAR_MODE_OFF);
    lv_menu_set_load_page_event(ui->Settings_menu_1, ui->Settings_menu_1_cont_5, ui->Settings_menu_1_subpage_5);
    lv_event_send(ui->Settings_menu_1_cont_1, LV_EVENT_CLICKED, NULL);
    lv_obj_set_pos(ui->Settings_menu_1, 0, 0);
    lv_obj_set_size(ui->Settings_menu_1, 240, 135);
    lv_obj_set_scrollbar_mode(ui->Settings_menu_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for Settings_menu_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Settings_menu_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Settings_menu_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Settings_menu_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Settings_menu_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Settings_menu_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_Settings_menu_1_extra_sidebar_page_main_default
    static lv_style_t style_Settings_menu_1_extra_sidebar_page_main_default;
    ui_init_style(&style_Settings_menu_1_extra_sidebar_page_main_default);

    lv_style_set_bg_opa(&style_Settings_menu_1_extra_sidebar_page_main_default, 255);
    lv_style_set_bg_color(&style_Settings_menu_1_extra_sidebar_page_main_default, lv_color_hex(0x909090));
    lv_style_set_bg_grad_dir(&style_Settings_menu_1_extra_sidebar_page_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_radius(&style_Settings_menu_1_extra_sidebar_page_main_default, 0);
    lv_obj_add_style(ui->Settings_menu_1_sidebar_page, &style_Settings_menu_1_extra_sidebar_page_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_Settings_menu_1_extra_option_btns_main_default
    static lv_style_t style_Settings_menu_1_extra_option_btns_main_default;
    ui_init_style(&style_Settings_menu_1_extra_option_btns_main_default);

    lv_style_set_text_color(&style_Settings_menu_1_extra_option_btns_main_default, lv_color_hex(0x151212));
    lv_style_set_text_font(&style_Settings_menu_1_extra_option_btns_main_default, &lv_font_Youshebiaotihei_12);
    lv_style_set_text_opa(&style_Settings_menu_1_extra_option_btns_main_default, 255);
    lv_style_set_text_align(&style_Settings_menu_1_extra_option_btns_main_default, LV_TEXT_ALIGN_CENTER);
    lv_style_set_pad_top(&style_Settings_menu_1_extra_option_btns_main_default, 10);
    lv_style_set_pad_bottom(&style_Settings_menu_1_extra_option_btns_main_default, 10);
    lv_obj_add_style(ui->Settings_menu_1_cont_5, &style_Settings_menu_1_extra_option_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->Settings_menu_1_cont_4, &style_Settings_menu_1_extra_option_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->Settings_menu_1_cont_3, &style_Settings_menu_1_extra_option_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->Settings_menu_1_cont_2, &style_Settings_menu_1_extra_option_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->Settings_menu_1_cont_1, &style_Settings_menu_1_extra_option_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_CHECKED for &style_Settings_menu_1_extra_option_btns_main_checked
    static lv_style_t style_Settings_menu_1_extra_option_btns_main_checked;
    ui_init_style(&style_Settings_menu_1_extra_option_btns_main_checked);

    lv_style_set_text_color(&style_Settings_menu_1_extra_option_btns_main_checked, lv_color_hex(0x9ab700));
    lv_style_set_text_font(&style_Settings_menu_1_extra_option_btns_main_checked, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_Settings_menu_1_extra_option_btns_main_checked, 255);
    lv_style_set_text_align(&style_Settings_menu_1_extra_option_btns_main_checked, LV_TEXT_ALIGN_CENTER);
    lv_style_set_border_width(&style_Settings_menu_1_extra_option_btns_main_checked, 0);
    lv_style_set_radius(&style_Settings_menu_1_extra_option_btns_main_checked, 0);
    lv_style_set_bg_opa(&style_Settings_menu_1_extra_option_btns_main_checked, 60);
    lv_style_set_bg_color(&style_Settings_menu_1_extra_option_btns_main_checked, lv_color_hex(0x00e0b8));
    lv_style_set_bg_grad_dir(&style_Settings_menu_1_extra_option_btns_main_checked, LV_GRAD_DIR_NONE);
    lv_obj_add_style(ui->Settings_menu_1_cont_5, &style_Settings_menu_1_extra_option_btns_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_add_style(ui->Settings_menu_1_cont_4, &style_Settings_menu_1_extra_option_btns_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_add_style(ui->Settings_menu_1_cont_3, &style_Settings_menu_1_extra_option_btns_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_add_style(ui->Settings_menu_1_cont_2, &style_Settings_menu_1_extra_option_btns_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_add_style(ui->Settings_menu_1_cont_1, &style_Settings_menu_1_extra_option_btns_main_checked, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style state: LV_STATE_DEFAULT for &style_Settings_menu_1_extra_main_title_main_default
    static lv_style_t style_Settings_menu_1_extra_main_title_main_default;
    ui_init_style(&style_Settings_menu_1_extra_main_title_main_default);

    lv_style_set_text_color(&style_Settings_menu_1_extra_main_title_main_default, lv_color_hex(0x41485a));
    lv_style_set_text_font(&style_Settings_menu_1_extra_main_title_main_default, &lv_font_Youshebiaotihei_12);
    lv_style_set_text_opa(&style_Settings_menu_1_extra_main_title_main_default, 255);
    lv_style_set_text_align(&style_Settings_menu_1_extra_main_title_main_default, LV_TEXT_ALIGN_CENTER);
    lv_style_set_bg_opa(&style_Settings_menu_1_extra_main_title_main_default, 255);
    lv_style_set_bg_color(&style_Settings_menu_1_extra_main_title_main_default, lv_color_hex(0xebeaea));
    lv_style_set_bg_grad_dir(&style_Settings_menu_1_extra_main_title_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_pad_top(&style_Settings_menu_1_extra_main_title_main_default, 0);
    lv_style_set_pad_bottom(&style_Settings_menu_1_extra_main_title_main_default, 0);
    lv_menu_t * Settings_menu_1_menu= (lv_menu_t *)ui->Settings_menu_1;
    lv_obj_t * Settings_menu_1_title = Settings_menu_1_menu->sidebar_header_title;
    lv_obj_set_size(Settings_menu_1_title, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_add_style(lv_menu_get_sidebar_header(ui->Settings_menu_1), &style_Settings_menu_1_extra_main_title_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);











    //The custom code of Settings.


    //Update current screen layout.
    lv_obj_update_layout(ui->Settings);

}
