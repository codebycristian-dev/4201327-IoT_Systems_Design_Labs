/* ==========================================================
 * Lab 0: Minimal IoT Implementation (HTTP)
 * Based on ESP-IDF simple_server example
 * Modified for IoT Systems Design - ESP32-C6
 * ========================================================== */
#include "esp_wifi.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <esp_log.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include "esp_netif.h"
#include "protocol_examples_common.h"
#include "protocol_examples_utils.h"
#include "esp_tls_crypto.h"
#include <esp_http_server.h>
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_tls.h"
#include "esp_check.h"

/* ---- Lab 0 Additions ---- */
#include "cJSON.h"
#include "driver/gpio.h"
#include "esp_random.h"

// Define the onboard LED pin for the ESP32-C6
#define BLINK_GPIO 2

static const char *TAG = "iot_lab";

/* ---------------------------------------------------
 * SENSING CAPABILITY (GET /api/sensor)
 * --------------------------------------------------- */
static esp_err_t sensor_get_handler(httpd_req_t *req)
{
    // Generate a dummy temperature between 20.0 and 29.9
    float temp = 20.0 + (esp_random() % 100) / 10.0;

    // Format as JSON
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "{\"temperature\": %.1f}", temp);

    // Send Response
    httpd_resp_set_type(req, "application/json");

    // Allow cross-origin requests from the dashboard
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");

    httpd_resp_send(req, buffer, HTTPD_RESP_USE_STRLEN);

    ESP_LOGI(TAG, "Telemetry requested. Sent: %s", buffer);
    return ESP_OK;
}

static const httpd_uri_t api_sensor = {
    .uri = "/api/sensor",
    .method = HTTP_GET,
    .handler = sensor_get_handler,
    .user_ctx = NULL};

/* ---------------------------------------------------
 * ACTUATING CAPABILITY (POST /api/control)
 * --------------------------------------------------- */
static esp_err_t control_post_handler(httpd_req_t *req)
{
    char buf[100];
    int ret, remaining = req->content_len;

    // Read the incoming payload
    if (remaining >= sizeof(buf))
    {
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }
    if ((ret = httpd_req_recv(req, buf, remaining)) <= 0)
    {
        return ESP_FAIL;
    }
    buf[ret] = '\0'; // Null-terminate

    // Parse the JSON payload: {"state": 1} or {"state": 0}
    cJSON *root = cJSON_Parse(buf);
    int state = 0;
    if (root != NULL)
    {
        cJSON *state_item = cJSON_GetObjectItem(root, "state");
        if (state_item != NULL && cJSON_IsNumber(state_item))
        {
            state = state_item->valueint;

            // Actuate the hardware
            gpio_set_level(BLINK_GPIO, state);
            ESP_LOGI(TAG, "Actuating Command Received. LED State: %d", state);
        }
        cJSON_Delete(root);
    }

    // Send Acknowledgment
    httpd_resp_set_type(req, "application/json");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_send(req, "{\"status\": \"ok\"}", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static const httpd_uri_t api_control = {
    .uri = "/api/control",
    .method = HTTP_POST,
    .handler = control_post_handler,
    .user_ctx = NULL};

/* ---------------------------------------------------
 * CORS preflight handler (OPTIONS)
 * --------------------------------------------------- */
static esp_err_t options_handler(httpd_req_t *req)
{
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Headers", "Content-Type");
    httpd_resp_send(req, NULL, 0);
    return ESP_OK;
}

static const httpd_uri_t options_sensor = {
    .uri = "/api/sensor",
    .method = HTTP_OPTIONS,
    .handler = options_handler,
    .user_ctx = NULL};

static const httpd_uri_t options_control = {
    .uri = "/api/control",
    .method = HTTP_OPTIONS,
    .handler = options_handler,
    .user_ctx = NULL};

/* ---------------------------------------------------
 * HTTP Server Start / Stop
 * --------------------------------------------------- */
static httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK)
    {
        ESP_LOGI(TAG, "Registering URI handlers");

        // Register custom IoT endpoints
        httpd_register_uri_handler(server, &api_sensor);
        httpd_register_uri_handler(server, &api_control);

        // Register CORS preflight handlers
        httpd_register_uri_handler(server, &options_sensor);
        httpd_register_uri_handler(server, &options_control);

        return server;
    }

    ESP_LOGI(TAG, "Error starting server!");
    return NULL;
}

static esp_err_t stop_webserver(httpd_handle_t server)
{
    return httpd_stop(server);
}

static void disconnect_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data)
{
    httpd_handle_t *server = (httpd_handle_t *)arg;
    if (*server)
    {
        ESP_LOGI(TAG, "Stopping webserver");
        if (stop_webserver(*server) == ESP_OK)
        {
            *server = NULL;
        }
        else
        {
            ESP_LOGE(TAG, "Failed to stop http server");
        }
    }
}

static void connect_handler(void *arg, esp_event_base_t event_base,
                            int32_t event_id, void *event_data)
{
    httpd_handle_t *server = (httpd_handle_t *)arg;
    if (*server == NULL)
    {
        ESP_LOGI(TAG, "Starting webserver");
        *server = start_webserver();
    }
}

/* ---------------------------------------------------
 * MAIN
 * --------------------------------------------------- */
void app_main(void)
{
    static httpd_handle_t server = NULL;

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Initialize Actuator Hardware (LED)
    gpio_reset_pin(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(BLINK_GPIO, 0); // Start OFF

    /* This helper function configures Wi-Fi or Ethernet, as selected in
     * menuconfig. Read "Establishing Wi-Fi or Ethernet Connection" section
     * in examples/protocols/README.md for more information. */
    ESP_ERROR_CHECK(example_connect());

    /* Register event handlers to stop the server when Wi-Fi disconnects,
     * and re-start it upon connection. */
#ifdef CONFIG_EXAMPLE_CONNECT_WIFI
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &connect_handler, &server));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &disconnect_handler, &server));
#endif

    /* Start the server for the first time */
    server = start_webserver();
}