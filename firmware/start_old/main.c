#include <atmel_start.h>
#include <main.h>

/*
i2c command table
###########################################
Instruction | Data    | Description
-------------------------------------------
0x10        | 1 byte  | servo position
0x11     	| 0 bytes | go to safe position
===========================================
Request     | Data    | Description
-------------------------------------------
0x20		| 1 byte  | servo position
0x21		| 1 byte  | on battery
0xFF		| 0 bytes | keep alive
===========================================
Confirmation          | Decription
-------------------------------------------
0x30                  | success
0x31                  | failure
0xFF				  | unknown command

i2c format
###########################################
[command, data]
(1 byte)  (1 byte)

Example:
Set servo position to 90
[0x10, 0x5A]
*/

// i2c commands
const uint8_t CMD_SET_POS    = 0x10;
const uint8_t CMD_SAFE_POS   = 0x11;
const uint8_t CMD_GET_POS    = 0x20;
const uint8_t CMD_GET_PS     = 0x21;
const uint8_t CMD_KEEP_ALIVE = 0xFF;

// i2c confirmation values
const uint8_t CONF_SUCCESS   = 0x30;
const uint8_t CONF_FAILURE   = 0x31;
const uint8_t CONF_UNKNOWN   = 0xFF;
  
// hardware configuration  
const uint8_t adc_channel    = 0;
const uint16_t pwm_period    = 10000;
  
// confugrable parameters   
const uint8_t  safe_pos      = 0;


struct io_descriptor *io;
uint8_t on_batt;
uint8_t response_buf[10];
uint8_t counter = 0;

static void tx_callback(const struct i2c_s_async_descriptor *const descr) {
	io_write(io, response_buf, counter);
	counter = 0;
}

static void rx_callback(const struct i2c_s_async_descriptor *const descr)
{
	// receive i2c command
	uint8_t command;
	io_read(io, &command, 1);

	switch (command) {
		case CMD_SET_POS: {
			// receive servo position
			uint8_t position;
			uint8_t bytes_read = io_read(io, &position, 1);

			uint8_t condition = !on_batt && bytes_read == 1;

			// set servo position
			if (condition) {
				pwm_set_parameters(&PWM_0, pwm_period, position);
			}

			// send confirmation
			// io_write(io, condition ? CONF_SUCCESS : CONF_FAILURE, 1);
			response_buf[counter++] = condition ? CONF_SUCCESS : CONF_FAILURE;
			break;
		}

		case CMD_SAFE_POS: {
			// set servo position to safe state
			pwm_set_parameters(&PWM_0, pwm_period, safe_pos);

			// send confirmation
			io_write(io, CONF_SUCCESS, 1);
			break;
		}

		case CMD_KEEP_ALIVE: {
			// send confirmation
			io_write(io, CONF_SUCCESS, 1);
			break;
		}

		default: {
			// send confirmation
			io_write(io, CONF_UNKNOWN, 1);
			break;
		}
	}
}

int main(void) {
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	// position servo at safe state
	pwm_set_parameters(&PWM_0, pwm_period, safe_pos);
	pwm_enable(&PWM_0);

	// setup and enable i2c
	i2c_s_async_get_io_descriptor(&I2C_0, &io);
	i2c_s_async_register_callback(&I2C_0, I2C_S_RX_COMPLETE, rx_callback);
	i2c_s_async_register_callback(&I2C_0, I2C_S_TX_PENDING, tx_callback);
	i2c_s_async_enable(&I2C_0);
	i2c_s_async_set_addr(&I2C_0, 0x10);

	// enable adc
	adc_sync_enable_channel(&ADC_0, adc_channel);

	/* Replace with your application code */
	while (1) {
		// determine if on battery
		on_batt = read_ps();
		delay_ms(100);
	}
}

// determine if on battery
uint8_t read_ps(void) {
	static uint8_t buffer[2];

	adc_sync_read_channel(&ADC_0, adc_channel, buffer, 2);

	uint16_t ps_result = (buffer[0] << 8) | buffer[1];
	uint16_t batt_result = (buffer[0] << 8) | buffer[1];

	return batt_result > ps_result;
}