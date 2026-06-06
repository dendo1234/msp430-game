void spi_init();
void spi_wait_not_busy();
void spi_send_byte(uint8_t byte);
void spi_send_bytes(const uint8_t* bytes, uint16_t count);
void spi_recive_byte(uint8_t* byte);
void spi_recive_long(uint32_t* l);
