idf_component_register(SRCS "gpio_example_main.c"
                            "./user_drivers/light.c"
                            "./user_drivers/motor.c"
                            "./user_drivers/servo.c"
                            "./user_drivers/encoder.c"
                    INCLUDE_DIRS "."
                                 )
