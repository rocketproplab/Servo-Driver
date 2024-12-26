#include <atmel_start.h>

struct io_descriptor *io;
const uint8_t buf[8] = {0xB0, 0x0B, 0x55, 0xDE, 0xAD, 0xBE, 0xEF, 0x13};
uint8_t buf_read[8];

int main(void) {
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	// setup and enable i2c
	i2c_s_sync_get_io_descriptor(&I2C_0, &io);
	i2c_s_sync_enable(&I2C_0);
	i2c_s_sync_set_addr(&I2C_0, 0x10);

	while (1) {
		io_read(io, buf_read, 3);
		io_write(io, buf, 4);
	}
}