
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>

#if !DT_NODE_EXISTS(DT_PATH(zephyr_user)) || \
	!DT_NODE_HAS_PROP(DT_PATH(zephyr_user), io_channels)
#error "No suitable devicetree overlay specified"
#endif

#define DT_SPEC_AND_COMMA(node_id, prop, idx) \
	ADC_DT_SPEC_GET_BY_IDX(node_id, idx),

/* Data of ADC io-channels specified in devicetree. */
static const struct adc_dt_spec adc_channels[] = {
	DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels,
			     DT_SPEC_AND_COMMA)
};

int main(void)
{
	int err;
	uint32_t count = 0;
	uint16_t buf;
	struct adc_sequence sequence = {
		.buffer = &buf,
		/* buffer size in bytes, not number of samples */
		.buffer_size = sizeof(buf),
	};

	/* Configure only channel 1 (index 0) */
	size_t channel_index = 1; // Change this to 1 for channel 1
	if (!device_is_ready(adc_channels[channel_index].dev)) {
		printk("ADC controller device %s not ready\n", adc_channels[channel_index].dev->name);
		return 0;
	}

	err = adc_channel_setup_dt(&adc_channels[channel_index]);
	if (err < 0) {
		printk("Could not setup channel #%zu (%d)\n", channel_index, err);
		return 0;
	}

	while (1) {
		int32_t val_mv;

		// Initialize the ADC sequence
		(void)adc_sequence_init_dt(&adc_channels[channel_index], &sequence);

		// Read from the ADC
		err = adc_read(adc_channels[channel_index].dev, &sequence);
		if (err < 0) {
			printk("Could not read (%d)\n", err);
			continue;
		}

		// Handle differential mode if necessary
		if (adc_channels[channel_index].channel_cfg.differential) {
			val_mv = (int32_t)((int16_t)buf);
		} else {
			val_mv = (int32_t)buf;
		}

		// Convert raw value to millivolts
		err = adc_raw_to_millivolts_dt(&adc_channels[channel_index], &val_mv);
		if (err < 0) {
			printk(" (value in mV not available)\n");
		} else {
			// Format output as required
			printk("- adc@channel %d = %" PRId32 " mV\n",
			       adc_channels[channel_index].channel_id, val_mv);
		}

		k_sleep(K_MSEC(1000));
	}
	return 0;
}
