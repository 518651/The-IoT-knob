#include "../Krnb_hand.h"


static void cheak_wifi_config_in_nvs_system(void);


typedef enum{
    wifi_state=0;
    wifi_config_address=0XAA;

}wifi_info_struct_t;

static void cheak_wifi_config_in_nvs_system(void){
    nvs_handle_t wifi_config_send_nvs;
    wifi_config_t wifi_config;
    esp_err_t wifi_config_error;
    uint8_t u8wifiConfigVal=0;
    uint8_t u8ssid[33]={0};
    uint8_t u8Password[55]={0};
    size_t len=0;
    uint8_t u8GetWifi_flag;

    bzero(&wifi_config,sizeof(wifi_config_t));

    wifi_config_error=nvs_open("Wifi_config",NVS_READWRITE,&wifi_config_send_nvs);
    nvs_get_u8(wifi_config_send_nvs,"WifiConfigFlag",&u8wifiConfigVal);
    printf("WIFI FLAG ADDRESS SIZEOF:%X \r\n",u8wifiConfigVal);

    if (wifi_config_error==ESP_OK)
    {
        printf("NVS存储系统准备        [Susscess]");
        wifi_config_error=nvs_set_u32(wifi_config_send_nvs,"wifi_uppdata",2048);
        if (wifi_config_error==ESP_OK)
        {
            printf("配网数据|查询/更新准备        [Susscess]");
        }
        else{
            printf("配网数据|查询/更新准备        [ERROR]");
        }
        if (u8wifiConfigVal==wifi_config)
        {
            len=sizeof(u8ssid);
            wifi_config_error=nvs_get_str(wifi_config_send_nvs,"SSID",(char*)u8ssid,&len);
            if (wifi_config_error==ESP_OK)
            {
                memcpy(wifi_config.sta.ssid,u8ssid,sizeof(wifi_config.sta.ssid));
                printf("读取到配网数据:  WIFI名称:[%s]",u8ssid);
                u8GetWifi_flag++;
            }
            len=sizeof(u8Password);
            wifi_config_error=nvs_get_str(wifi_config_send_nvs,"PASSWORD",(char*)u8Password,&len);
            if (wifi_config_error==ESP_OK)
            {
                memcpy(wifi_config.sta.password,u8Password,sizeof(wifi_config.sta.password));
                printf("读取到配网数据:  WIFI密码:[%s]",u8Password);
                u8GetWifi_flag++;
            }
            nvs_close(wifi_config_send_nvs);
            //读取数据完毕,执行WIFI初始化

            /*
                //如果WIFI标识位为2，那么说明读取到原先的WIFI,将带入信息重新连接
            */


            if (u8GetWifiFlag==2) 
            {
                ESP_ERROR_CHEAK(esp_wifi_disconnect());
                ESP_ERROR_CHEAK(esp_wifi_set_config(ESP_IF_WIFI_STA,&wifi_config));
                ESP_ERROR_CHEAK(esp_wifi_connect());

            }
            else{
                xTaskCreate(auto_connect_network,"auto_connect_network",4096,NULL,3,NULL);
            
            }

           
        }
        else
        {
          nvs_close(wifi_config_send_nvs);
          //读取数据完毕,执行WIFI初始化
          xTaskCreate(auto_connect_network,"auto_connect_network",4096,NULL,3,NULL);
          printf("获取WIFI配置数据文件失败,正在重新启动配置网络方案");
        }
            

    }
    

}


//WIFI事件
static void event_handler(void* arg,esp_event_base_t event_base,int32_t event_id,void* event_data){
    if (event_base==WIFI_EVENT&&event_id==WIFI_EVENT_STA_START)
    {
        /* code */
        //系统预留方案
        //自动配网函数
    }
    else if (event_base==WIFI_EVENT&&event_id==WIFI_EVENT_STA_CONNECTED)
    {
        printf("WIFI CONNECT [Success!]");
    }
    else if (event_base==WIFI_EVENT&&event_id==WIFI_EVENT_STA_DISCONNECTED)
    {
        /* 
            WIFI重新执行连接
         */
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        /* IP事件 */
    }
    else if (event_base == SC_EVENT && event_id == SC_EVENT_SCAN_DONE)
    {
        /* 扫描输出 */
    }
    else if (event_base == SC_EVENT && event_id == SC_EVENT_FOUND_CHANNEL)
    {
        /* 啥都没有扫到 */
    }
    else if (event_base == SC_EVENT && event_id == SC_EVENT_GOT_SSID_PSWD)
    {
        /* 获取到SSID和PASSWORD */
        smartconfig_event_got_ssid_pswd_t *evt = (smartconfig_event_got_ssid_pswd_t *)event_data;
        wifi_config_t wifi_config;
        uint8_t ssid[33] = { 0 };
        uint8_t password[65] = { 0 };

        bzero(&wifi_config, sizeof(wifi_config_t));
        memcpy(wifi_config.sta.ssid, evt->ssid, sizeof(wifi_config.sta.ssid));
        memcpy(wifi_config.sta.password, evt->password, sizeof(wifi_config.sta.password));
        wifi_config.sta.bssid_set = evt->bssid_set;
        if (wifi_config.sta.bssid_set == true) {
            memcpy(wifi_config.sta.bssid, evt->bssid, sizeof(wifi_config.sta.bssid));
        }

        memcpy(ssid, evt->ssid, sizeof(evt->ssid));
        memcpy(password, evt->password, sizeof(evt->password));
        ESP_LOGI(TAG, "SSID:%s", ssid);
        ESP_LOGI(TAG, "PASSWORD:%s", password);

        //存放当前的配网信息
        nvs_handle_t wificonfig_set_handle;
        ESP_ERROR_CHECK( nvs_open("wificonfig",NVS_READWRITE,&wificonfig_set_handle) );
        ESP_ERROR_CHECK( nvs_set_u8(wificonfig_set_handle,"WifiConfigFlag", wifi_configed) );
        ESP_ERROR_CHECK( nvs_set_str(wificonfig_set_handle,"SSID",(const char *)ssid) );
        ESP_ERROR_CHECK( nvs_set_str(wificonfig_set_handle,"PASSWORD", (const char *)password) );
        ESP_ERROR_CHECK( nvs_commit(wificonfig_set_handle) );
        nvs_close(wificonfig_set_handle);

        //使用获取的配网信息链接无线网络
        ESP_ERROR_CHECK( esp_wifi_disconnect() );
        ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
        ESP_ERROR_CHECK( esp_wifi_connect() );
    } else if (event_base == SC_EVENT && event_id == SC_EVENT_SEND_ACK_DONE) {
        xEventGroupSetBits(s_wifi_event_group, ESPTOUCH_DONE_BIT);
    }

    
    
    
    
    

}