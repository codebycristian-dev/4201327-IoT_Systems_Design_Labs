set(requires esp-tls nvs_flash esp_netif esp_http_server json driver esp_wifi esp_event esp_system)

idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES ${requires})