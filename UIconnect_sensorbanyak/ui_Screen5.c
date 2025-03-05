// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.11
// Project name: SquareLine_Project

#include "ui.h"

void ui_Screen5_screen_init(void)
{
    ui_Screen5 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen5, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Screen5, lv_color_hex(0x181818), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Screen5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label12 = lv_label_create(ui_Screen5);
    lv_obj_set_width(ui_Label12, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label12, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label12, 0);
    lv_obj_set_y(ui_Label12, -110);
    lv_obj_set_align(ui_Label12, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label12, "AQMS");

    ui_Button7 = lv_btn_create(ui_Screen5);
    lv_obj_set_width(ui_Button7, 64);
    lv_obj_set_height(ui_Button7, 30);
    lv_obj_set_x(ui_Button7, -116);
    lv_obj_set_y(ui_Button7, 93);
    lv_obj_set_align(ui_Button7, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button7, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_Button7, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Button7, lv_color_hex(0x29AAE6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button7, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui_Button7, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_opa(ui_Button7, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_width(ui_Button7, 20, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_spread(ui_Button7, 2, LV_PART_MAIN | LV_STATE_FOCUSED);

    ui_LabelPM7 = lv_label_create(ui_Screen5);
    lv_obj_set_width(ui_LabelPM7, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelPM7, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelPM7, -116);
    lv_obj_set_y(ui_LabelPM7, 93);
    lv_obj_set_align(ui_LabelPM7, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelPM7, "Back");

    ui_Button8 = lv_btn_create(ui_Screen5);
    lv_obj_set_width(ui_Button8, 100);
    lv_obj_set_height(ui_Button8, 50);
    lv_obj_set_x(ui_Button8, -81);
    lv_obj_set_y(ui_Button8, -12);
    lv_obj_set_align(ui_Button8, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button8, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_Button8, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_shadow_color(ui_Button8, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_opa(ui_Button8, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_width(ui_Button8, 20, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_spread(ui_Button8, 2, LV_PART_MAIN | LV_STATE_FOCUSED);

    ui_LabelOFF2 = lv_label_create(ui_Screen5);
    lv_obj_set_width(ui_LabelOFF2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelOFF2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelOFF2, -84);
    lv_obj_set_y(ui_LabelOFF2, -54);
    lv_obj_set_align(ui_LabelOFF2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelOFF2, "OFF");

    ui_Button9 = lv_btn_create(ui_Screen5);
    lv_obj_set_width(ui_Button9, 100);
    lv_obj_set_height(ui_Button9, 50);
    lv_obj_set_x(ui_Button9, 77);
    lv_obj_set_y(ui_Button9, -12);
    lv_obj_set_align(ui_Button9, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button9, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_Button9, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_shadow_color(ui_Button9, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_opa(ui_Button9, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_width(ui_Button9, 20, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_spread(ui_Button9, 2, LV_PART_MAIN | LV_STATE_FOCUSED);

    ui_LabelOFF1 = lv_label_create(ui_Screen5);
    lv_obj_set_width(ui_LabelOFF1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelOFF1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelOFF1, 76);
    lv_obj_set_y(ui_LabelOFF1, -54);
    lv_obj_set_align(ui_LabelOFF1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelOFF1, "OFF");

    ui_Dropdown1 = lv_dropdown_create(ui_Screen5);
    lv_obj_set_width(ui_Dropdown1, 150);
    lv_obj_set_height(ui_Dropdown1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Dropdown1, 473);
    lv_obj_set_y(ui_Dropdown1, -50);
    lv_obj_set_align(ui_Dropdown1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Dropdown1, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags



    lv_obj_add_event_cb(ui_Button7, ui_event_Button7, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Button8, ui_event_Button8, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Button9, ui_event_Button9, LV_EVENT_ALL, NULL);

}
