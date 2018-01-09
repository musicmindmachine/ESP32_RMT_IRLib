/*
ESP32 RMT
- transmitted signal can optionally be modulated by a carrier wave
- 8 channels share a 512x32-bit RAM block (8x64=512)
- each channel has 64x 32bit items (can be extended if needed), hence can send or receive 64 bits of data
- each item contains 2x 16bit entries, 1bit level + 15bit period (length)
- each item meant to be a pair of pulse and idle periods (mark & silent) representing a bit in the waveform
- a zero period is interpreted as an end-marker: the transmitter will stop transmitting once it has read this
*/
#include "driver/rmt.h"

// RMT constants
#define RMT_RX_ACTIVE_LEVEL  	0   /*!< If we connect with a IR receiver, the data is active low */
#define RMT_TX_CARRIER_EN		1   /*!< Enable carrier for IR transmitter */
#define RMT_CARRIER_FREQ_36		36000 // 36k, RC5
#define RMT_CARRIER_FREQ_38		38000 // 38k, NEC
#define RMT_CARRIER_DUTY		33		// recommended carrier duty-cycle is 1/4 or 1/3 (22?)

#define RMT_TX_CHANNEL    		RMT_CHANNEL_1     /*!< RMT channel for transmitter */
#define RMT_TX_GPIO_NUM  		GPIO_NUM_27     /*!< GPIO number for transmitter signal */
#define RMT_RX_CHANNEL    		RMT_CHANNEL_0     /*!< RMT channel for receiver */
#define RMT_RX_GPIO_NUM  		GPIO_NUM_19     /*!< GPIO number for receiver */
#define RMT_CLK_DIV      		100    /*!< RMT counter clock divider */
#define RMT_TICK_10_US    		(80000000/RMT_CLK_DIV/100000)   /*!< RMT counter value for 10 us.(Source clock is APB clock) */
#define RMT_ITEM_DURATION(d)  	((d & 0x7fff)*10/RMT_TICK_10_US)  /*!< Parse duration time from memory register value */

#define RMT_TX_DATA_NUM  		1    /*!< tx data number 1 */

#define rmt_item32_TIMEOUT_US  	9500   /*!< RMT receiver timeout value(us) */
//

#ifdef __cplusplus
extern "C" {
#endif

void rmt_tx_init();
void rmt_rx_init();
void rmt_fill_item_level(rmt_item32_t* item, int high_us, int low_us);
void rmt_fill_end_item(rmt_item32_t* item);
void rmt_fill_item_level_me(rmt_item32_t* item, int length_us, bool start_idle);

#ifdef __cplusplus
}
#endif