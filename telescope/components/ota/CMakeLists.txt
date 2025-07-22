idf_component_register(SRCS
                        OTAUpdater.cpp
                       INCLUDE_DIRS 
                        .
                       REQUIRES
# local components
                        wifi
# system components
                        app_update
                        cxx
                        esp_http_client
                        esp_https_ota
                        nvs_flash
                       EMBED_TXTFILES ${project_dir}/server_certs/ca_cert.pem)