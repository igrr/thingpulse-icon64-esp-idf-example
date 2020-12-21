idf_component_register(SRCS "example_main.cpp"
                            "play.c"
                            "storage.c"
                       INCLUDE_DIRS ".")

spiffs_create_partition_image(storage ../data FLASH_IN_PROJECT)
