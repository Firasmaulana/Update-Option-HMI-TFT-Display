// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.11
// Project name: SquareLine_Project

#include "ui.h"
lv_chart_series_t * ui_Chart2_series_1 = NULL;
lv_chart_series_t * ui_Chart2_series_2 = NULL;
lv_chart_series_t * ui_Chart2_series_3 = NULL;

void ui_Screen7_screen_init(void)
{
    ui_Screen7 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen7, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Screen7, lv_color_hex(0x181818), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Screen7, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label16 = lv_label_create(ui_Screen7);
    lv_obj_set_width(ui_Label16, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label16, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label16, 0);
    lv_obj_set_y(ui_Label16, -110);
    lv_obj_set_align(ui_Label16, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label16, "AQMS");

    ui_Button14 = lv_btn_create(ui_Screen7);
    lv_obj_set_width(ui_Button14, 64);
    lv_obj_set_height(ui_Button14, 30);
    lv_obj_set_x(ui_Button14, -116);
    lv_obj_set_y(ui_Button14, 96);
    lv_obj_set_align(ui_Button14, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button14, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_Button14, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Button14, lv_color_hex(0x29AAE6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button14, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui_Button14, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_opa(ui_Button14, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_width(ui_Button14, 20, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_spread(ui_Button14, 2, LV_PART_MAIN | LV_STATE_FOCUSED);

    ui_LabelPM5 = lv_label_create(ui_Button14);
    lv_obj_set_width(ui_LabelPM5, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelPM5, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelPM5, 0);
    lv_obj_set_y(ui_LabelPM5, -1);
    lv_obj_set_align(ui_LabelPM5, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelPM5, "Back");

    ui_Chart2 = lv_chart_create(ui_Screen7);
    lv_obj_set_width(ui_Chart2, 229);
    lv_obj_set_height(ui_Chart2, 128);
    lv_obj_set_x(ui_Chart2, 2);
    lv_obj_set_y(ui_Chart2, -18);
    lv_obj_set_align(ui_Chart2, LV_ALIGN_CENTER);
    lv_chart_set_type(ui_Chart2, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(ui_Chart2, 24);
    lv_chart_set_range(ui_Chart2, LV_CHART_AXIS_PRIMARY_Y, 0, 50);
    lv_chart_set_range(ui_Chart2, LV_CHART_AXIS_SECONDARY_Y, 0, 50);
    lv_chart_set_div_line_count(ui_Chart2, 24, 50);
    lv_chart_set_axis_tick(ui_Chart2, LV_CHART_AXIS_PRIMARY_X, 10, 5, 24, 1, true, 50);
    lv_chart_set_axis_tick(ui_Chart2, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 12, 1, true, 50);
    lv_chart_set_axis_tick(ui_Chart2, LV_CHART_AXIS_SECONDARY_Y, 10, 5, 12, 1, true, 25);
    ui_Chart2_series_1 = lv_chart_add_series(ui_Chart2, lv_color_hex(0xFF0000),
                                                                 LV_CHART_AXIS_PRIMARY_Y);
    static lv_coord_t ui_Chart2_series_1_array[24] = { 0 };
    lv_chart_set_ext_y_array(ui_Chart2, ui_Chart2_series_1, ui_Chart2_series_1_array);
    ui_Chart2_series_2 = lv_chart_add_series(ui_Chart2, lv_color_hex(0x1100FF),
                                                                 LV_CHART_AXIS_SECONDARY_Y);
    static lv_coord_t ui_Chart2_series_2_array[24] = { 0 };
    lv_chart_set_ext_y_array(ui_Chart2, ui_Chart2_series_2, ui_Chart2_series_2_array);
    ui_Chart2_series_3 = lv_chart_add_series(ui_Chart2, lv_color_hex(0x27FF00),
                                                                 LV_CHART_AXIS_SECONDARY_Y);
    static lv_coord_t ui_Chart2_series_3_array[24] = { 0 };
    lv_chart_set_ext_y_array(ui_Chart2, ui_Chart2_series_3, ui_Chart2_series_3_array);



    lv_obj_add_event_cb(ui_Button14, ui_event_Button14, LV_EVENT_ALL, NULL);

}
