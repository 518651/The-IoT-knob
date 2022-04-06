#include "ESP Project.h"



// /* FreeRTOS event group to signal when we are connected & ready to make a request */
// static EventGroupHandle_t s_wifi_event_group;

// /* The event group allows multiple bits for each event,
//    but we only care about one event - are we connected
//    to the AP with an IP? */
// static const int CONNECTED_BIT = BIT0;
// static const int ESPTOUCH_DONE_BIT = BIT1;
// static const char *TAG = "smartconfig_example";

// static void smartconfig_example_task(void * parm);

// static void event_handler(void* arg, esp_event_base_t event_base,
//                                 int32_t event_id, void* event_data)
// {
//     if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
//         xTaskCreate(smartconfig_example_task, "smartconfig_example_task", 4096, NULL, 3, NULL);
//     } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
//         esp_wifi_connect();
//         xEventGroupClearBits(s_wifi_event_group, CONNECTED_BIT);
//     } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
//         xEventGroupSetBits(s_wifi_event_group, CONNECTED_BIT);
//     } else if (event_base == SC_EVENT && event_id == SC_EVENT_SCAN_DONE) {
//         ESP_LOGI(TAG, "扫描完成");
//     } else if (event_base == SC_EVENT && event_id == SC_EVENT_FOUND_CHANNEL) {
//         ESP_LOGI(TAG, "找到信道");
//     } else if (event_base == SC_EVENT && event_id == SC_EVENT_GOT_SSID_PSWD) {
//         ESP_LOGI(TAG, "检测到有SSID和密码");

//         smartconfig_event_got_ssid_pswd_t *evt = (smartconfig_event_got_ssid_pswd_t *)event_data;
//         wifi_config_t wifi_config;
//         uint8_t ssid[33] = { 0 };
//         uint8_t password[65] = { 0 };
//         uint8_t rvd_data[33] = { 0 };

//         bzero(&wifi_config, sizeof(wifi_config_t));
//         memcpy(wifi_config.sta.ssid, evt->ssid, sizeof(wifi_config.sta.ssid));
//         memcpy(wifi_config.sta.password, evt->password, sizeof(wifi_config.sta.password));
//         wifi_config.sta.bssid_set = evt->bssid_set;
//         if (wifi_config.sta.bssid_set == true) {
//             memcpy(wifi_config.sta.bssid, evt->bssid, sizeof(wifi_config.sta.bssid));
//         }

//         memcpy(ssid, evt->ssid, sizeof(evt->ssid));
//         memcpy(password, evt->password, sizeof(evt->password));
//         ESP_LOGI(TAG, "SSID:%s", ssid);
//         ESP_LOGI(TAG, "密码为:%s", password);
//         if (evt->type == SC_TYPE_ESPTOUCH_V2) {
//             ESP_ERROR_CHECK( esp_smartconfig_get_rvd_data(rvd_data, sizeof(rvd_data)) );
//             ESP_LOGI(TAG, "RVD_DATA:");
//             for (int i=0; i<33; i++) {
//                 printf("%02x ", rvd_data[i]);
//             }
//             printf("\n");
//         }

//         ESP_ERROR_CHECK( esp_wifi_disconnect() );
//         ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
//         esp_wifi_connect();
//     } else if (event_base == SC_EVENT && event_id == SC_EVENT_SEND_ACK_DONE) {
//         xEventGroupSetBits(s_wifi_event_group, ESPTOUCH_DONE_BIT);
//     }
// }

// static void demo_initialise_wifi(void)
// {
//     ESP_ERROR_CHECK(esp_netif_init());
//     s_wifi_event_group = xEventGroupCreate();
//     ESP_ERROR_CHECK(esp_event_loop_create_default());
//     esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
//     assert(sta_netif);

//     wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
//     ESP_ERROR_CHECK( esp_wifi_init(&cfg) );

//     ESP_ERROR_CHECK( esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL) );
//     ESP_ERROR_CHECK( esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL) );
//     ESP_ERROR_CHECK( esp_event_handler_register(SC_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL) );

//     ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
//     ESP_ERROR_CHECK( esp_wifi_start() );
// }

// static void smartconfig_example_task(void * parm)
// {
//     EventBits_t uxBits;
//     ESP_ERROR_CHECK( esp_smartconfig_set_type(SC_TYPE_ESPTOUCH) );
//     smartconfig_start_config_t cfg = SMARTCONFIG_START_CONFIG_DEFAULT();
//     ESP_ERROR_CHECK( esp_smartconfig_start(&cfg) );
//     while (1) {
//         uxBits = xEventGroupWaitBits(s_wifi_event_group, CONNECTED_BIT | ESPTOUCH_DONE_BIT, true, false, portMAX_DELAY);
//         if(uxBits & CONNECTED_BIT) {
//             ESP_LOGI(TAG, "连接到ap的WiFi");
//         }
//         if(uxBits & ESPTOUCH_DONE_BIT) {
//             ESP_LOGI(TAG, "智能配置完毕");
//             esp_smartconfig_stop();
//             vTaskDelete(NULL);
//         }
//     }
// }


void app_main(void)
{
     ESP_ERROR_CHECK( nvs_flash_init() );
    user_wifi_config_sta();
}