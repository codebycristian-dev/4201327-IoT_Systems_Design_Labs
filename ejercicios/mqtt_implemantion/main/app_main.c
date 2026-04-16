/*
 * IoT Systems Design - Lab 1 (MQTT)
 * Firmware para ESP32-C6
 *
 * Sensing   : publica temperatura simulada cada 2 s en iot/sensor
 * Actuating : suscribe a iot/control y enciende/apaga el LED (GPIO 8)
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h" /* example_connect() */
#include "esp_log.h"
#include "mqtt_client.h"

#include "cJSON.h"
#include "driver/gpio.h"
#include "esp_random.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* ------------------------------------------------------------------ */
/* Configuracion                                                      */
/* ------------------------------------------------------------------ */
#define BLINK_GPIO 2 /* LED onboard del ESP32-C6 DevKit */
#define TOPIC_SENSOR "iot/sensor"
#define TOPIC_CONTROL "iot/control"

static const char *TAG = "mqtt_iot";
static esp_mqtt_client_handle_t mqtt_client = NULL;

/* ------------------------------------------------------------------ */
/* Actuating: procesa mensaje recibido en iot/control                 */
/* ------------------------------------------------------------------ */
static void handle_control_message(const char *data, int data_len)
{
    /* Copiamos a un buffer terminado en '\0' para parsear con cJSON  */
    char buf[64];
    int len = data_len < (int)sizeof(buf) - 1 ? data_len : (int)sizeof(buf) - 1;
    memcpy(buf, data, len);
    buf[len] = '\0';

    cJSON *root = cJSON_Parse(buf);
    if (root != NULL)
    {
        cJSON *state_item = cJSON_GetObjectItem(root, "state");
        if (state_item != NULL && cJSON_IsNumber(state_item))
        {
            int state = state_item->valueint;
            gpio_set_level(BLINK_GPIO, state);
            ESP_LOGI(TAG, "Comando recibido. LED = %d", state);
        }
        cJSON_Delete(root);
    }
    else
    {
        ESP_LOGW(TAG, "JSON invalido en %s: %s", TOPIC_CONTROL, buf);
    }
}

/* ------------------------------------------------------------------ */
/* Callback unico de eventos MQTT                                     */
/* ------------------------------------------------------------------ */
static void mqtt_event_handler(void *handler_args, esp_event_base_t base,
                               int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;

    switch ((esp_mqtt_event_id_t)event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "Conectado al broker MQTT");
        esp_mqtt_client_subscribe(client, TOPIC_CONTROL, 1);
        ESP_LOGI(TAG, "Suscrito a: %s", TOPIC_CONTROL);
        break;

    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "Mensaje en %.*s", event->topic_len, event->topic);
        if (strncmp(event->topic, TOPIC_CONTROL, event->topic_len) == 0)
        {
            handle_control_message(event->data, event->data_len);
        }
        break;

    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGW(TAG, "Desconectado del broker MQTT");
        break;

    case MQTT_EVENT_ERROR:
        ESP_LOGE(TAG, "Error MQTT");
        break;

    default:
        break;
    }
}

/* ------------------------------------------------------------------ */
/* Sensing: tarea periodica que publica telemetria                    */
/* ------------------------------------------------------------------ */
static void sensor_publish_task(void *pvParameters)
{
    while (1)
    {
        if (mqtt_client != NULL)
        {
            /* Temperatura simulada entre 20.0 y 29.9 C */
            float temp = 20.0f + (esp_random() % 100) / 10.0f;

            char buffer[64];
            snprintf(buffer, sizeof(buffer),
                     "{\"temperature\": %.1f}", temp);

            esp_mqtt_client_publish(mqtt_client, TOPIC_SENSOR,
                                    buffer, 0, 0, 0);
            ESP_LOGI(TAG, "Publicado %s -> %s", TOPIC_SENSOR, buffer);
        }
        vTaskDelay(pdMS_TO_TICKS(2000)); /* cada 2 s */
    }
}

/* ------------------------------------------------------------------ */
/* app_main                                                           */
/* ------------------------------------------------------------------ */
void app_main(void)
{
    ESP_LOGI(TAG, "[APP] Arranque");
    ESP_LOGI(TAG, "[APP] Memoria libre: %" PRIu32 " bytes",
             esp_get_free_heap_size());

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* LED como salida, apagado al inicio */
    gpio_reset_pin(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(BLINK_GPIO, 0);

    /* Conexion Wi-Fi / Ethernet via helper del example */
    ESP_ERROR_CHECK(example_connect());

    /* Cliente MQTT: broker URL se configura con menuconfig */
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = CONFIG_BROKER_URL,
    };
    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID,
                                   mqtt_event_handler, NULL);
    esp_mqtt_client_start(mqtt_client);

    /* Tarea de publicacion periodica */
    xTaskCreate(sensor_publish_task, "sensor_pub", 4096, NULL, 5, NULL);
}