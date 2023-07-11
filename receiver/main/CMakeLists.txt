idf_component_register(SRCS  "main.c"
                        "./user_drivers/mytimer.c"
                        "./user_drivers/myadc.c"
                        "./user_drivers/Lookup3Hash.c"
                        "./user_drivers/RNG.c"
                        "./user_drivers/decoder.c"
                        "./user_drivers/test.c"
                        "./PHY/vlc_esp32_hardware.c"
                    INCLUDE_DIRS ".")
