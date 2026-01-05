#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <time.h>
#include <zephyr/drivers/adc.h>

#define LOW_THRESHOLD 30       
#define HIGH_THRESHOLD 1000    
#define MEASUREMENT_INTERVAL_MS 1000  


#define DT_SPEC_AND_COMMA(node_id, prop, idx) \
    ADC_DT_SPEC_GET_BY_IDX(node_id, idx),

/* Data of ADC io-channels specified in devicetree. */
static const struct adc_dt_spec adc_channels[] = {
    DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels,
                         DT_SPEC_AND_COMMA)};
uint16_t adcBuf;
struct adc_sequence sequence = {
    .buffer = &adcBuf,
    /* buffer size in bytes, not number of samples */
    .buffer_size = sizeof(adcBuf),
};
int32_t val_mv;
int time_ms;
int err;
uint32_t count = 0;
int tipsCount = 0;
float rainLevel;
int64_t myTime = 0;
int adcVal[3] = {0};

void main(void) {

    int err;

    if (!adc_is_ready_dt(&adc_channels[0]))
    {
        printk("ADC controller device %s not ready\n", adc_channels[0].dev->name);
        return 0;
    }

    err = adc_channel_setup_dt(&adc_channels[0]);
    if (err < 0)
    {
        printk("Could not setup channel #%d (%d)\n", 0, err);
        return 0;
    }

    while (1) {
        for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++)
        {
            if (!adc_is_ready_dt(&adc_channels[i]))
            {
                printk("ADC controller device %s not ready\n", adc_channels[i].dev->name);
                return 0;
            }

            err = adc_channel_setup_dt(&adc_channels[i]);
            if (err < 0)
            {
                printk("Could not setup channel #%d (%d)\n", 0, err);
                return 0;
            }
        }

        for (int zeroIdx = 0; zeroIdx < ARRAY_SIZE(adc_channels); zeroIdx++)
        {
            adcVal[zeroIdx] = 0;
        }
        /* Configure channels individually prior to sampling. */
        // printk("ADC reading[%u]:\n", count++);
        for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++)
        {
            int32_t val_mv;

            // printk("- %s, channel %d: ",
            //        adc_channels[i].dev->name,
            //        adc_channels[i].channel_id);

            (void)adc_sequence_init_dt(&adc_channels[i], &sequence);
            err = adc_read(adc_channels[i].dev, &sequence);
            if (err < 0)
            {
                // printk("Could not read (%d)\n", err);
                continue;
            }
            if (adc_channels[i].channel_cfg.differential)
            {
                val_mv = (int32_t)((int16_t)adcBuf);
            }
            else
            {
                val_mv = (int32_t)adcBuf;
            }
            // printk("%" PRId32, val_mv);
            err = adc_raw_to_millivolts_dt(&adc_channels[i],
                                           &val_mv);
            if (err < 0)
            {
                // printk(" (value in mV not available)\n");
            }
            else
            {
                printk(" = %" PRId32 " mV\n", val_mv);
                for (int adcIdx = 0; adcIdx < ARRAY_SIZE(adc_channels); adcIdx++)
                {
                    if (adcIdx == i)
                    {
                        adcVal[adcIdx] = val_mv;
                    }
                }
            }
        }
        /*for rain*/

        // printk("%ld\n", time_ms);

        if (adcVal[0] >= 2600 && adcVal[0] <= 3300)
        {
            time_ms = k_uptime_get() / 1000;
            tipsCount++;
            rainLevel = (tipsCount) * 0.25;
            printk("Rain level is %.2fmm\n", rainLevel);
        }
        else
        {
            printk("Rain not detected !!\n");
        }
        k_msleep(100);
    }

    //  k_msleep(100);

}
