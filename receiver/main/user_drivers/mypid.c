#include "./include/mypid.h"
#include "./include/servo.h"
 void pcnt_on_reach(pcnt_unit_handle_t unit, const pcnt_watch_event_data_t *edata, void *user_ctx)
{
    // if(edata->watch_point_value==PCNT_HIGH_LIMIT)
    // turn_left45();   
    // else
    // turn_right45();
}

void encoder_init(pcnt_unit_handle_t* out_unit,int a, int b)
{
    //install pcnt unit
    pcnt_unit_config_t unit_config ={
        .high_limit=PCNT_HIGH_LIMIT,
        .low_limit=PCNT_LOW_LIMIT,
        .flags.accum_count=1,
    };

    pcnt_unit_handle_t pcnt_unit = NULL;
    pcnt_new_unit(&unit_config, &pcnt_unit);

    //set glitch filter
    pcnt_glitch_filter_config_t filter_config ={
        .max_glitch_ns=1000,
    };
    pcnt_unit_set_glitch_filter(pcnt_unit,&filter_config);

    //install pcnt  channels
    pcnt_chan_config_t chan_a_config ={
        .edge_gpio_num = a,
        .level_gpio_num = b,
    };
    pcnt_channel_handle_t pcnt_chan_a =NULL;
    pcnt_new_channel(pcnt_unit,&chan_a_config,&pcnt_chan_a);

    pcnt_chan_config_t chan_b_config ={
        .edge_gpio_num = b,
        .level_gpio_num = a,
    };
    pcnt_channel_handle_t pcnt_chan_b =NULL;
    pcnt_new_channel(pcnt_unit,&chan_b_config,&pcnt_chan_b);

    //set edge and level actions for pcnt channels
    pcnt_channel_set_edge_action(pcnt_chan_a,PCNT_CHANNEL_EDGE_ACTION_DECREASE,PCNT_CHANNEL_EDGE_ACTION_INCREASE);
    pcnt_channel_set_level_action(pcnt_chan_a,PCNT_CHANNEL_LEVEL_ACTION_KEEP,PCNT_CHANNEL_LEVEL_ACTION_INVERSE);
    pcnt_channel_set_edge_action(pcnt_chan_b, PCNT_CHANNEL_EDGE_ACTION_INCREASE,PCNT_CHANNEL_EDGE_ACTION_DECREASE);
    pcnt_channel_set_level_action(pcnt_chan_b,PCNT_CHANNEL_LEVEL_ACTION_KEEP,PCNT_CHANNEL_LEVEL_ACTION_INVERSE);


    //enable counter events
    int watch_points[]= {PCNT_LOW_LIMIT,PCNT_HIGH_LIMIT};
    for(size_t i=0;i<sizeof(watch_points)/sizeof(int);i++)
    {
        pcnt_unit_add_watch_point(pcnt_unit,watch_points[i]);
    }
    pcnt_event_callbacks_t cbs ={
        .on_reach = pcnt_on_reach,
    };
    pcnt_unit_register_event_callbacks(pcnt_unit,&cbs,NULL);

    //enable pcnt unit
    pcnt_unit_enable(pcnt_unit);
    //clear pcnt unit
    pcnt_unit_clear_count(pcnt_unit);
    //start pcnt unit
    pcnt_unit_start(pcnt_unit);

    *out_unit=pcnt_unit;
}

int Incremental_PI (int Encoder, int Target)
{
    static float Bias, Pwm;
    Bias = Encoder - Target;
    Pwm += Velocity_KP * (Bias - last_bias) + Velocity_KI * Bias;
    last_bias =Bias;
    return Pwm;
}
