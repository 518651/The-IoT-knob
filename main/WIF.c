#include "ESP Project.h"

static EventGroupHandle_t wifi_event_group;//定义一个事件的句柄
const int SCAN_DONE_BIT = BIT0;//定义事件，占用事件变量的第0位，最多可以定义32个事件。
static wifi_scan_config_t scanConf  = {
    .ssid = NULL,
    .bssid = NULL,
    .channel = 0,
    .show_hidden = 1
};//定义scanConf结构体，供函数esp_wifi_scan_start调用

static const char *TAG = "example";

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
	wifi_ap_record_t my_wifi_info;
	
    switch(event->event_id) 
	{
        case SYSTEM_EVENT_WIFI_READY:
            printf("[WIFI 配置事件] WIFI连接准备就绪！\r\n");
            break;
        case SYSTEM_EVENT_SCAN_DONE:
            printf("[WIFI 配置事件] 周围WIFI信号扫描完成！\r\n");
            printf("WIFI LIST:\n");
            
            //xEventGroupSetBits(wifi_event_group, SCAN_DONE_BIT);
		case SYSTEM_EVENT_STA_START:                         // wifi STA 开始
			printf("[WIFI 配置事件] STA启动事件!\r\n");
			ESP_ERROR_CHECK(esp_wifi_connect());                 // 连接wifi
			break;
		case SYSTEM_EVENT_STA_GOT_IP:                        // STA模式下，分配到了IP
			printf("[WIFI 配置事件] 获取IP事件!\r\n");
			printf("[WIFI 配置事件] 设备 IP: %s !\r\n", ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
			break;
		case SYSTEM_EVENT_STA_CONNECTED:                     // STA模式下，连接上了wifi	
			printf("[WIFI 配置事件] Wifi STA 连接事件!\r\n");
			esp_wifi_sta_get_ap_info(&my_wifi_info);             // STA模式下，获取模块连接上的wifi热点的信息
			printf("[WIFI 配置事件] 连接自 : %s!\r\n", my_wifi_info.ssid);
			break;
		case SYSTEM_EVENT_STA_DISCONNECTED:                  // STA模式下，断开了与wifi的连接
			printf("[WIFI 配置事件] Wifi STA断开事件，重新连接！\r\n");
			ESP_ERROR_CHECK( esp_wifi_connect() );                // 重新连接wifi              
			break;
		default:printf("[WIFI 配置事件]");
			break;
    }
	
    return ESP_OK;	
}


static void scan_task(void *pvParameters)
{
    while(1) {
        xEventGroupWaitBits(wifi_event_group, SCAN_DONE_BIT, 0, 1, portMAX_DELAY);    //等待事件被置位，即等待扫描完成
        ESP_LOGI(TAG, "WIFI scan doen");
        xEventGroupClearBits(wifi_event_group, SCAN_DONE_BIT);//清除事件标志位
 
        uint16_t apCount = 0;
        esp_wifi_scan_get_ap_num(&apCount);//Get number of APs found in last scan
        printf("Number of access points found: %d\n", apCount);
        if (apCount == 0) {
            ESP_LOGI(TAG, "Nothing AP found");
            return;
        }//如果apCount没有受到数据，则说明没有路由器
        wifi_ap_record_t *list = (wifi_ap_record_t *)malloc(sizeof(wifi_ap_record_t) * apCount);//定义一个wifi_ap_record_t的结构体的链表空间
        ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&apCount, list));//获取上次扫描中找到的AP列表。
        int i;
        printf("======================================================================\n");
        printf("             SSID             |    RSSI    |           AUTH           \n");
        printf("======================================================================\n");
        for (i=0; i<apCount; i++) {
            char *authmode;
            switch(list[i].authmode) {
            case WIFI_AUTH_OPEN:
               authmode = "WIFI_AUTH_OPEN";
               break;
            case WIFI_AUTH_WEP:
               authmode = "WIFI_AUTH_WEP";
               break;           
            case WIFI_AUTH_WPA_PSK:
               authmode = "WIFI_AUTH_WPA_PSK";
               break;           
            case WIFI_AUTH_WPA2_PSK:
               authmode = "WIFI_AUTH_WPA2_PSK";
               break;           
            case WIFI_AUTH_WPA_WPA2_PSK:
               authmode = "WIFI_AUTH_WPA_WPA2_PSK";
               break;
            default:
               authmode = "Unknown";
               break;
            }
            printf("%26.26s    |    % 4d    |    %22.22s\n",list[i].ssid, list[i].rssi, authmode);
        }//将链表的数据信息打印出来
        free(list);//释放链表
        printf("\n\n");//换行
 
        // scan again
        vTaskDelay(2000 / portTICK_PERIOD_MS);//调用延时函数，再次扫描
        //The true parameter cause the function to block until the scan is done.
        ESP_ERROR_CHECK(esp_wifi_scan_start(&scanConf, 1));//扫描所有可用的AP。
    }
 
 
}





// // 初始化WIFI 配置扫描模式 开始扫描 打印扫描到的AP信息
// static void wifi_scan(void)
// {
// 	ESP_ERROR_CHECK(esp_netif_init());					// 初始化底层TCP/IP堆栈。在应用程序启动时，应该调用此函数一次
// 	ESP_ERROR_CHECK(esp_event_loop_create_default());	// 创建默认事件循环
// 	esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();// 创建一个默认的WIFI-STA网络接口，
// 	assert(sta_netif);									// 如果初始化错误，此API将中止
// 	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
// 	ESP_ERROR_CHECK(esp_wifi_init(&cfg));				// 使用默认wifi初始化配置
 
// 	uint16_t number = DEFAULT_SCAN_LIST_SIZE;			// 默认扫描列表大小
// 	wifi_ap_record_t ap_info[DEFAULT_SCAN_LIST_SIZE];	// AP信息结构体大小
// 	uint16_t ap_count = 0;								// 初始化AP数量0
// 	memset(ap_info, 0, sizeof(ap_info));				// 初始化AP信息结构体
 
// 	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));	// 设置WiFi的工作模式为 STA
// 	ESP_ERROR_CHECK(esp_wifi_start());					// 启动WiFi连接
// 	ESP_ERROR_CHECK(esp_wifi_scan_start(NULL, true));	// 开始扫描WIFI(默认配置，阻塞方式)
// 	ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&number, ap_info));// 获取搜索的具体AP信息
// 	ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));		//接入点的数量
// 	ESP_LOGI(TAG, "Total APs scanned = %u", ap_count);
// 	for (int i = 0; (i < DEFAULT_SCAN_LIST_SIZE) && (i < ap_count); i++) {
// 		ESP_LOGI(TAG, "SSID \t\t%s", ap_info[i].ssid);		// 打印WIFI名称
// 		ESP_LOGI(TAG, "RSSI \t\t%d", ap_info[i].rssi);		// 打印信号强度	
// 		print_auth_mode(ap_info[i].authmode);				// 打印认证方式
// 		if (ap_info[i].authmode != WIFI_AUTH_WEP) {
// 			print_cipher_type(ap_info[i].pairwise_cipher, ap_info[i].group_cipher);
// 		}
// 		ESP_LOGI(TAG, "Channel \t\t%d\n", ap_info[i].primary);
// 	}
// }







//扫描WIFI源API
// static void scan_task(void *pvParameters)
//   {
//       while(1) {
//         xEventGroupWaitBits(wifi_event_group, SCAN_DONE_BIT, 0, 1, portMAX_DELAY);    //等待事件被置位，即等待扫描完成
//         //ESP_LOGI(TAG, "WIFI 扫描完成");
//         xEventGroupClearBits(wifi_event_group, SCAN_DONE_BIT);//清除事件标志位
  
//         uint16_t apCount = 0;
//         esp_wifi_scan_get_ap_num(&apCount);//Get number of APs found in last scan
//         //printf("找到以下WIFI信号: %d\n", apCount);
//         if (apCount == 0) {
//               ESP_LOGI(TAG, "设备没有扫描到任何WIFI");
//               return;
//           }//如果apCount没有受到数据，则说明没有路由器
//           wifi_ap_record_t *list = (wifi_ap_record_t *)malloc(sizeof(wifi_ap_record_t) * apCount);//定义一个wifi_ap_record_t的结构体的链表空间
//           ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&apCount, list));//获取上次扫描中找到的AP列表。
//           int i;
//         or (i=0; i<apCount; i++) {
 
//         printf("(%d,\"%s\",%d,\""MACSTR" %d\")\r\n",list[i].authmode, list[i].ssid, list[i].rssi,
//                  MAC2STR(list[i].bssid),list[i].primary);
                  
//          }//将链表的数据信息打印出来
//           free(list);//释放链表
//           printf("\n\n");//换行
//         //wifi_config_t wifi_config = { 0 };
//         // memset(&wifi_config,0,sizeof(wifi_config));
//         // memcpy(wifi_config.sta.ssid, "tplink", strlen("tplink"));
//         // memcpy(wifi_config.sta.password,"7788990011",strlen("7788990011"));
//         // ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
//         // ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
//         // ESP_ERROR_CHECK( esp_wifi_connect() );
 
//          // scan again
//          //  vTaskDelay(5000 / portTICK_PERIOD_MS);//调用延时函数，再次扫描
//          //The true parameter cause the function to block until the scan is done.
//          // ESP_ERROR_CHECK(esp_wifi_scan_start(&scanConf, 1));//扫描所有可用的AP。
//       }
 
 
//  }



//手动配网API
void user_wifi_config_sta(void)
{
    
	printf("[WIFI STA] STA配置启动，连接到Wifi : %s \r\n", WIFI_SSID);
	
    tcpip_adapter_init();
	
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
	
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
	ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
	xTaskCreate(&scan_task, "scan_task", 2048, NULL, 15, NULL);//创建扫描任务
    ESP_ERROR_CHECK(esp_wifi_scan_start(&scanConf, 1));
    wifi_config_t wifi_sta_config = 
	{
		.sta = 
		{
			.ssid = WIFI_SSID,
			.password = WIFI_PASS
		},
    };
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_sta_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );	

}

