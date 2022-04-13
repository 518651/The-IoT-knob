#include <string.h>
#include <sys/param.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_wpa2.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_smartconfig.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>


#include "LVGL/components/components/lvgl/lvgl.h"
#include "LVGL/components/components/lvgl/demos/lv_demos.h"

#include "TCP/tcp_server.h"
#include "WIFI/User_WIFI.h"

#define AP_WIFI_SSID                        "ESP32_AP"
#define AP_WIFI_PASS                        ""
#define AP_MAX_CONN_STA                     1

#define TCP_SERVER_TASK_STK_SIZE            2048
#define TCP_SERVER_TASK_PRIO                7
#define PROCESS_CLIENT_TASK_STK_SIZE        2048
#define PROCESS_CLIENT_TASK_PRIO            7

#define TCP_SERVER_PORT                     6666
#define TCP_SERVER_LISTEN_CLIENT_NUM        3


/* Private macro -------------------------------------------------------------*/
#define TAG_XLI                   __func__

/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static EventGroupHandle_t wifi_event_group;
static TaskHandle_t app_tcp_server_single_conn_task_handler;
static TaskHandle_t app_tcp_server_multi_conn_task_handler;

/* Private function ----------------------------------------------------------*/
static void app_tcp_server_single_conn_task(void *arg);
static void app_tcp_server_multi_conn_task(void *arg);


void user_wifi_config_sta(void); //配置WIFI API 

static esp_err_t event_handler(void *ctx, system_event_t *event);

static void _wifi_init_ap(void);

void LVGL_Screen(void);