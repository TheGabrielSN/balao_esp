// Controle do sistema
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

// Controle básico do C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "time.h"
#include <ctype.h>

// Controle de comunicação
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_timer.h"
#include "esp_system.h"
#include "esp_netif.h"
#include <esp_ipc.h>
#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "mqtt_client.h"

// Efeitos
#include "effects.h"

/*****************************************/
//                                       //
//               Definições              //
//                                       //
/*****************************************/

#define delay(value) vTaskDelay(value / portTICK_PERIOD_MS)

#define WIFI_SSID      "PROTOTIPAGEM"
#define WIFI_PASSWORD  "12345678"

#define MQTT_URI       "mqtt://192.168.1.120"
#define MQTT_PORT      1883

static const char* TAG = "LOG: ";

int efeito=0, nl=2, task_menu_created=0;
char topic[100], data[10], MAC[20];

TaskHandle_t TaskMenu = NULL;
TaskHandle_t TaskMain = NULL;

esp_mqtt_client_handle_t client;


/*****************************************/
//                                       //
//            Cabeçalho das              //
//               Funções                 //
//                                       //
/*****************************************/

void get_mac();
static void wifi_event_handler(void*, esp_event_base_t, int32_t, void*);
void connect_wifi();
static void mqtt_event_handler(void*, esp_event_base_t, int32_t, void*);
void mqtt_callback(const char*, char*);
void mqtt_connect();
void callMenu();
void sendMensage(const char*, int);

// Execução de fluxo do sistema
void run_main(){
    /* Controle de execução das funções do sistema
    */
    configPin();
    
    nvs_flash_init();
    connect_wifi();

    delay(100);

    mqtt_connect();

    while(1){
    }
}

/*****************************************/
//                                       //
//                 MAIN                  //
//                                       //
/*****************************************/

void app_main(void){
    /* Função main
    criação da task para execução do sistema
    */
    xTaskCreate(&run_main, "CallMain", 8192, NULL, 1, &TaskMain);
}

// Configuração para conexão ao Wifi
void get_mac(){
    /* Função para obtenção do MAC do dispositivo
    */
    uint8_t mac_adr[6];
    esp_wifi_get_mac(ESP_IF_WIFI_STA, mac_adr);
    snprintf(MAC, 20*sizeof(char), "%02x:%02x:%02x:%02x:%02x:%02x", mac_adr[0], mac_adr[1], mac_adr[2], mac_adr[3], mac_adr[4], mac_adr[5]);
    //snprintf(MAC, 20*sizeof(char), "%s", toupper(MAC));
    int j = 0;
    char ch;
    while (MAC[j]) {
        ch = MAC[j];
        MAC[j] = toupper(ch);
        j++;
    }
    printf("MAC: %s\n", MAC);

}

/*****************************************/
//                                       //
//             Definição das             //
//                Funções                //
//                                       //
/*****************************************/

static void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data){
    /* Função para controle de execução de acordo com os acontecimento do wifi
    */
    switch (event_id){
        case WIFI_EVENT_STA_START:
            printf("WiFi connecting ... \n");
            break;
        case WIFI_EVENT_STA_CONNECTED:
            printf("WiFi connected ... \n");
            get_mac();
            break;
        case WIFI_EVENT_STA_DISCONNECTED:
            printf("WiFi lost connection ... \n");
            break;
        case IP_EVENT_STA_GOT_IP:
            printf("WiFi got IP ... \n\n");
            break;
        default:
            break;
    }
}

void connect_wifi() {
    /* Fluxo de conexão para o wifi
    */
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_initiation);
    
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);
    wifi_config_t wifi_configuration = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASSWORD}};
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_configuration);

    esp_wifi_start();
    
    esp_wifi_connect();
}

void mqtt_callback(const char *topic, char *payload) {
    /* Fluxo de execução para recebimento de pacores
    */
    ESP_LOGE(TAG, "MQTT - Message arrived on topic: %s", topic);
    // Caso seja recebido um efeito
    if((strcmp(topic,MAC)==0) || (strcmp(topic,"efeito")==0)){
        efeito = atoi(payload);

        if (task_menu_created){
            vTaskDelete(TaskMenu);
        }

        // Criação de task para execução dos efeitos
        ESP_LOGE(TAG, "Create MENU Task");
        xTaskCreate(&callMenu, "CallMENU", 4096, NULL, 5, &TaskMenu);

        task_menu_created = 1;
    }

    else{
        // Caso seja envio de informações do sistema para o servidor
        sendMensage(MAC, efeito);
    }
}

static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event){
    /* Função de controle para eventos do MQTT
    */
    esp_mqtt_client_handle_t client = event->client;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGE(TAG, "Connected to MQTT broker");
            esp_mqtt_client_subscribe(client, MAC, 0);
            esp_mqtt_client_subscribe(client, "efeito", 0);
            esp_mqtt_client_subscribe(client, "PrototipagemIFPB", 0);
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            printf("\nTOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            
            snprintf(topic, (event->topic_len+1)*sizeof(char), "%s", event->topic);
            snprintf(data, (event->data_len+1)*sizeof(char), "%s", event->data);

            mqtt_callback(topic, data);
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
        }

    return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data){
    /* Gerenciamento do controle de eventos do MQTT
    */
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event_handler_cb(event_data);
}

void mqtt_connect() {
    /* Fluxo de conexão do MQTT
    */
    esp_mqtt_client_config_t mqtt_cfg = {
      .uri = MQTT_URI,
    };

    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}

void callMenu(){
    /* Chama a função de Menu para aplicação do efeito
    */
    while (1){
        MENU(efeito);
    }
   
}

void sendMensage(const char* mac, int efeito){
    /* Envio de informações para o servidor
    */
    
    wifi_ap_record_t ap_info;
    esp_wifi_sta_get_ap_info(&ap_info);

    // Obter o valor de RSSI
    int signalStrength = ap_info.rssi;

    char message[100];
    snprintf(message, sizeof(message), "'{'esp':'%s','status':'up','efeito':'%d','sinal':'%d','bateria':'0'}'", MAC, efeito, signalStrength);

    if (esp_mqtt_client_publish(client, "PrototipagemIFPB1", message, 0, 0, false) == true) {
        ESP_LOGE(TAG, "Success sending message");
    } 
    else {
        ESP_LOGE(TAG, "Error sending message");
    }
}