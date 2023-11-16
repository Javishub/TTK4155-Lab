
#include "mcp2515.h"
#include <stdint.h>
#define DDR_SPI DDRB
#define DD_SS PB4
#define DD_MOSI PB5
#define DD_MISO PB6
#define DD_SCK PB7


void spi_set_ss(){
	PORTB |= (1 << DD_SS);
}

void spi_clear_ss(){
	PORTB &= ~(1 << DD_SS);
}

void spi_master_init(){
	DDR_SPI = (1 << DD_MOSI)|(1 << DD_SCK)|(1 << DD_SS);
	SPCR = (1 << SPE)|(1 << MSTR)|(1 << SPR0)|(1 << SPIE);
	spi_set_ss();
}

void spi_write(char cData){
	SPDR = cData;
	while(!(SPSR & (1 << SPIF))){}
}

uint8_t spi_read(){
	SPDR = 0xFF;
	while(!(SPSR & (1 << SPIF))){}
	return SPDR;
}

void mcp_reset(){
	spi_clear_ss();
	spi_write(MCP_RESET);
	spi_set_ss();
}

uint8_t mcp_read(uint8_t address) {
	spi_clear_ss();
	spi_write(MCP_READ);
	spi_write(address); //Adressen on MCP to read from
	uint8_t data = spi_read();
	spi_set_ss();

	return data;
}

void mcp_write(uint8_t address, uint8_t data) {
	spi_clear_ss();
	spi_write(MCP_WRITE);
	spi_write(address); //Adress to write to
	spi_write(data);
	spi_set_ss();
}

char mcp_read_status() {
	spi_clear_ss();
	spi_write(MCP_READ_STATUS);
	char data = spi_read();
	spi_set_ss();

	return data;
}

void mcp_bit_modify(uint8_t address, uint8_t mask, uint8_t data) {
	spi_clear_ss();
	spi_write(MCP_BITMOD);
	spi_write(address); //Adress we want to change bit(s)
	spi_write(mask);
	spi_write(data); 
	spi_set_ss();
}

void mcp_init(){
	spi_master_init();
	mcp_reset();
	
	
	_delay_ms(1);
	
	uint8_t value = mcp_read(MCP_CANSTAT);
	if ((value & MODE_MASK) != MODE_CONFIG)
	{
		printf("MCP2515 isn't in config mode after reset. CANSTAT: %x\r\n", value);
	}
}


void mcp_request_to_send(int bfr_nr){
	spi_clear_ss();
	bfr_nr = bfr_nr%3;
	char data = MCP_RTS_TX0;
	if (bfr_nr == 0)
	{
		data = MCP_RTS_TX0;
	}else if (bfr_nr == 1)
	{
		data = MCP_RTS_TX1;
	}else if (bfr_nr == 2)
	{
		data = MCP_RTS_TX2;
	}
	spi_write(data);
	spi_set_ss();
}

void mcp_set_mode(uint8_t mode){
	mcp_bit_modify(MCP_CANCTRL, 0b11100000, mode);
}

void mcp_set_bitrate(){
	mcp_set_mode(MODE_CONFIG);
	uint8_t NUMBER_OF_TQ = (1+PHASE_SEG1+PHASE_SEG2+PROP_SEG);
	uint8_t BRP = MCP_CPU / (2*NUMBER_OF_TQ*BAUDRATE);
	
	mcp_write(MCP_CNF1, (SJW - 1) <<6 | (BRP -1));
	mcp_write(MCP_CNF2, BTLMODE | SAMPLE_1X | ((PHASE_SEG1-1) << 3) | (PROP_SEG-1));
	mcp_write(MCP_CNF3, 0x00 | (PHASE_SEG2-1));
}

