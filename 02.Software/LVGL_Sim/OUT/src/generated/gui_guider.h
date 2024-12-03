/*
* Copyright 2024 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct
{
  
	lv_obj_t *Start_screen;
	bool Start_screen_del;
	lv_obj_t *Start_screen_cont_1;
	lv_obj_t *Start_screen_Windows_img;
	lv_obj_t *Start_screen_bar_1;
	lv_obj_t *Start_screen_textprogress_1;
	lv_obj_t *Outputs_screen;
	bool Outputs_screen_del;
	lv_obj_t *Outputs_screen_cont_1;
	lv_obj_t *Outputs_screen_cont_2;
	lv_obj_t *Outputs_screen_Consle_lable;
	lv_obj_t *Outputs_screen_Output_lable;
	lv_obj_t *Outputs_screen_Runtime_lable;
	lv_obj_t *Outputs_screen_img_1;
	lv_obj_t *Task_screen;
	bool Task_screen_del;
	lv_obj_t *Task_screen_cont_2;
	lv_obj_t *Task_screen_cont_3;
	lv_obj_t *Task_screen_Tasks;
	lv_obj_t *Task_screen_btn_1;
	lv_obj_t *Task_screen_btn_1_label;
	lv_obj_t *Task_screen_label_1;
	lv_obj_t *Task_screen_img_1;
	lv_obj_t *Settings;
	bool Settings_del;
	lv_obj_t *Settings_menu_1;
	lv_obj_t *Settings_menu_1_sidebar_page;
	lv_obj_t *Settings_menu_1_subpage_1;
	lv_obj_t *Settings_menu_1_cont_1;
	lv_obj_t *Settings_menu_1_label_1;
	lv_obj_t *Settings_menu_1_subpage_2;
	lv_obj_t *Settings_menu_1_cont_2;
	lv_obj_t *Settings_menu_1_label_2;
	lv_obj_t *Settings_menu_1_subpage_3;
	lv_obj_t *Settings_menu_1_cont_3;
	lv_obj_t *Settings_menu_1_label_3;
	lv_obj_t *Settings_menu_1_subpage_4;
	lv_obj_t *Settings_menu_1_cont_4;
	lv_obj_t *Settings_menu_1_label_4;
	lv_obj_t *Settings_menu_1_subpage_5;
	lv_obj_t *Settings_menu_1_cont_5;
	lv_obj_t *Settings_menu_1_label_5;
}lv_ui;

typedef void (*ui_setup_scr_t)(lv_ui * ui);

void ui_init_style(lv_style_t * style);

void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
                           lv_scr_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del);

void ui_animation(void * var, int32_t duration, int32_t delay, int32_t start_value, int32_t end_value, lv_anim_path_cb_t path_cb,
                       uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                       lv_anim_exec_xcb_t exec_cb, lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);


void init_scr_del_flag(lv_ui *ui);

void setup_ui(lv_ui *ui);


extern lv_ui guider_ui;


void setup_scr_Start_screen(lv_ui *ui);
void setup_scr_Outputs_screen(lv_ui *ui);
void setup_scr_Task_screen(lv_ui *ui);
void setup_scr_Settings(lv_ui *ui);
LV_IMG_DECLARE(_windowsxp_alpha_198x89);
LV_IMG_DECLARE(_indent_alpha_20x20);
LV_IMG_DECLARE(_indent_alpha_20x20);

LV_FONT_DECLARE(lv_font_Youshebiaotihei_16)
LV_FONT_DECLARE(lv_font_montserratMedium_16)
LV_FONT_DECLARE(lv_font_Youshebiaotihei_10)
LV_FONT_DECLARE(lv_font_montserratMedium_12)
LV_FONT_DECLARE(lv_font_Youshebiaotihei_12)


#ifdef __cplusplus
}
#endif
#endif
