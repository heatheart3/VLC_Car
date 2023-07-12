idf_component_register(SRCS  "main.c"
                        "./user_drivers/mytimer.c"
                        "./user_drivers/myadc.c"

                        "./user_drivers/test.c"

                        "./PHY/Lookup3Hash.c"
                        "./PHY/RNG.c"
                        "./PHY/decoder.c"
                        "./PHY/vlc_esp32_hardware.c"
                    INCLUDE_DIRS ".")
