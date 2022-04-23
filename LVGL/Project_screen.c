#include "../Krnb_hand.h"
#include "esp_timer.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "driver/gpio.h"
#include "lvgl.h"

static const char *TAG = "Screen-To-Screen";

#define SCREEN_LCD_PIXEL_CLOCK_HZ     (10 * 1000 * 1000)
#define SCREEN_LCD_BK_LIGHT_ON_LEVEL  1
#define SCREEN_LCD_BK_LIGHT_OFF_LEVEL !SCREEN_LCD_BK_LIGHT_ON_LEVEL
#define SCREEN_PIN_NUM_DATA0          19
#define SCREEN_PIN_NUM_DATA1          21
#define SCREEN_PIN_NUM_DATA2          0
#define SCREEN_PIN_NUM_DATA3          22
#define SCREEN_PIN_NUM_DATA4          23
#define SCREEN_PIN_NUM_DATA5          33
#define SCREEN_PIN_NUM_DATA6          32
#define SCREEN_PIN_NUM_DATA7          27
#define SCREEN_PIN_NUM_PCLK           18
#define SCREEN_PIN_NUM_CS             4
#define SCREEN_PIN_NUM_DC             5
#define SCREEN_PIN_NUM_RST            -1
#define SCREEN_PIN_NUM_BK_LIGHT       2


//屏幕宽高
#define SCREEN_LCD_H_RES              240
#define SCREEN_LCD_V_RES              240

#define SCREEN_LCD_CMD_BITS           8
#define SCREEN_LCD_PARAM_BITS         8

#define SCREEN_LVGL_TICK_PERIOD_MS    2

extern void SCREEN_lvgl_ui(lv_obj_t *scr);

static bool SCREEN_notify_lvgl_flush_ready(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata, void *user_ctx)
{
    lv_disp_drv_t *disp_driver = (lv_disp_drv_t *)user_ctx;
    lv_disp_flush_ready(disp_driver);
    return false;
}

static void SCREEN_lvgl_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map)
{
    esp_lcd_panel_handle_t panel_handle = (esp_lcd_panel_handle_t) drv->user_data;
    int offsetx1 = area->x1;
    int offsetx2 = area->x2;
    int offsety1 = area->y1;
    int offsety2 = area->y2;
    // copy a buffer's content to a specific area of the display
    esp_lcd_panel_draw_bitmap(panel_handle, offsetx1, offsety1, offsetx2 + 1, offsety2 + 1, color_map);
}

static void SCREEN_increase_lvgl_tick(void *arg)
{
    /* Tell LVGL how many milliseconds has elapsed */
    lv_tick_inc(SCREEN_LVGL_TICK_PERIOD_MS);
}


void Screen_Use_main(void){
    static lv_disp_draw_buf_t disp_buf;  //声明屏幕缓存区
    static lv_disp_drv_t disp_drv; //声明回调

    gpio_config_t bk_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << SCREEN_PIN_NUM_BK_LIGHT
    };
    ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));
    gpio_set_level(SCREEN_PIN_NUM_BK_LIGHT, SCREEN_LCD_BK_LIGHT_OFF_LEVEL);
    esp_lcd_i80_bus_handle_t i80_bus = NULL;
    esp_lcd_i80_bus_config_t bus_config = {
        .dc_gpio_num = SCREEN_PIN_NUM_DC,
        .wr_gpio_num = SCREEN_PIN_NUM_PCLK,
        .data_gpio_nums = {
            SCREEN_PIN_NUM_DATA0,
            SCREEN_PIN_NUM_DATA1,
            SCREEN_PIN_NUM_DATA2,
            SCREEN_PIN_NUM_DATA3,
            SCREEN_PIN_NUM_DATA4,
            SCREEN_PIN_NUM_DATA5,
            SCREEN_PIN_NUM_DATA6,
            SCREEN_PIN_NUM_DATA7,
        },
        .bus_width = 8,
        .max_transfer_bytes = SCREEN_LCD_H_RES * 40 * sizeof(uint16_t)
    };
    ESP_ERROR_CHECK(esp_lcd_new_i80_bus(&bus_config, &i80_bus));
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_io_i80_config_t io_config = {
        .cs_gpio_num = SCREEN_PIN_NUM_CS,
        .pclk_hz = SCREEN_LCD_PIXEL_CLOCK_HZ,
        .trans_queue_depth = 10,
        .dc_levels = {
            .dc_idle_level = 0,
            .dc_cmd_level = 0,
            .dc_dummy_level = 0,
            .dc_data_level = 1,
        },
        .on_color_trans_done = SCREEN_notify_lvgl_flush_ready,
        .user_ctx = &disp_drv,
        .lcd_cmd_bits = SCREEN_LCD_CMD_BITS,
        .lcd_param_bits = SCREEN_LCD_PARAM_BITS,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i80(i80_bus, &io_config, &io_handle));

    esp_lcd_panel_handle_t panel_handle = NULL;
    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = SCREEN_PIN_NUM_RST,
        .color_space = ESP_LCD_COLOR_SPACE_RGB,
        .bits_per_pixel = 16,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(io_handle, &panel_config, &panel_handle));

    esp_lcd_panel_reset(panel_handle);
    esp_lcd_panel_init(panel_handle);
    esp_lcd_panel_invert_color(panel_handle, true);
    esp_lcd_panel_set_gap(panel_handle, 0, 20);

    //打开LCD背光
    gpio_set_level(SCREEN_PIN_NUM_BK_LIGHT, SCREEN_LCD_BK_LIGHT_ON_LEVEL);

    //初始化LVGL库
    lv_init();
    // LVGL使用的alloc draw缓冲区
    // 建议选择至少1/10屏幕大小的绘图缓冲区
    lv_color_t *buf1 = heap_caps_malloc(SCREEN_LCD_H_RES * 20 * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf1);
    lv_color_t *buf2 = heap_caps_malloc(SCREEN_LCD_H_RES * 20 * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf2);
    // 初始化LVGL绘制缓冲区
    lv_disp_draw_buf_init(&disp_buf, buf1, buf2, SCREEN_LCD_H_RES * 20);

    //将显示驱动程序注册到LVGL
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = SCREEN_LCD_H_RES;
    disp_drv.ver_res = SCREEN_LCD_V_RES;
    disp_drv.flush_cb = SCREEN_lvgl_flush_cb;
    disp_drv.draw_buf = &disp_buf;
    disp_drv.user_data = panel_handle;
    lv_disp_t *disp = lv_disp_drv_register(&disp_drv);

    // LVGL的勾选接口（使用esp_定时器生成2ms周期事件）
    const esp_timer_create_args_t lvgl_tick_timer_args = {
        .callback = &SCREEN_increase_lvgl_tick,
        .name = "lvgl_tick"
    };
    esp_timer_handle_t lvgl_tick_timer = NULL;
    ESP_ERROR_CHECK(esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(lvgl_tick_timer, SCREEN_LVGL_TICK_PERIOD_MS * 1000));

    //显示LVGL动画
    lv_obj_t *scr = lv_disp_get_scr_act(disp);
    SCREEN_lvgl_ui(scr);

    while (1) {
        // 提高LVGL的任务优先级和/或缩短处理器周期可以提高性能
        vTaskDelay(pdMS_TO_TICKS(10));
        // 运行lv_timer_handler的任务的优先级应低于运行lv_tick_inc
        lv_timer_handler();
    }
}