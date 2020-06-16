#ifndef NRF24L01_PTX_H_
#define NRF24L01_PTX_H_

#include "main.h"
#include "stm32f1xx_hal.h"

enum Command
{
	READ						= (uint8_t) 0b00000000,	// Register read
	WRITE						= (uint8_t) 0b00100000,	// Register write
	CMD_R_RX_PAYLOAD			= (uint8_t) 0b01100001,	// Read RX payload
	CMD_W_TX_PAYLOAD			= (uint8_t) 0b10100000,	// Write TX payload
	CMD_FLUSH_TX				= (uint8_t) 0b11100001,	// Flush TX FIFO
	CMD_FLUSH_RX				= (uint8_t) 0b11100010,	// Flush RX FIFO
	CMD_REUSE_TX_PL				= (uint8_t) 0b11100011,	// Reuse TX payload
	CMD_R_RX_PL_WID				= (uint8_t) 0b01100000,	// Read RX payload width
	CMD_W_ACK_PAYLOAD			= (uint8_t) 0b10101000,	// Write ACK payload (pipe number must be added)
	CMD_W_TX_PAYLOAD_NO_ACK		= (uint8_t) 0b10110000,	// Disables AUTOACK on specific packet (TX-Mode)
	CMD_NOP						= (uint8_t) 0b11111111	// No operation (used for reading status register)
};

enum Register
{
	REG_CONFIG					= (uint8_t) 0b00000000,	// Configuration register
	REG_EN_AA					= (uint8_t) 0b00000001,	// Enable "Auto acknowledgment"
	REG_EN_RXADDR				= (uint8_t) 0b00000010,	// Enable RX addresses
	REG_SETUP_AW				= (uint8_t) 0b00000011,	// Setup of address widths
	REG_SETUP_RETR				= (uint8_t) 0b00000100,	// Setup of automatic retransmit
	REG_RF_CH					= (uint8_t) 0b00000101,	// RF channel
	REG_RF_SETUP				= (uint8_t) 0b00000110,	// RF setup register
	REG_STATUS					= (uint8_t) 0b00000111,	// Status register
	REG_OBSERVE_TX				= (uint8_t) 0b00001000,	// Transmit observe register
	REG_RPD						= (uint8_t) 0b00001001,	// Received power detector
	REG_RX_ADDR_P0				= (uint8_t) 0b00001010,	// Receive address data pipe 0
	REG_RX_ADDR_P1				= (uint8_t) 0b00001011,	// Receive address data pipe 1
	REG_RX_ADDR_P2				= (uint8_t) 0b00001100,	// Receive address data pipe 2
	REG_RX_ADDR_P3				= (uint8_t) 0b00001101,	// Receive address data pipe 3
	REG_RX_ADDR_P4				= (uint8_t) 0b00001110,	// Receive address data pipe 4
	REG_RX_ADDR_P5				= (uint8_t) 0b00001111,	// Receive address data pipe 5
	REG_TX_ADDR					= (uint8_t) 0b00010000,	// Transmit address
	REG_RX_PW_P0				= (uint8_t) 0b00010001,	// Number of bytes in RX payload in data pipe 0
	REG_RX_PW_P1				= (uint8_t) 0b00010010,	// Number of bytes in RX payload in data pipe 1
	REG_RX_PW_P2				= (uint8_t) 0b00010011,	// Number of bytes in RX payload in data pipe 2
	REG_RX_PW_P3				= (uint8_t) 0b00010100,	// Number of bytes in RX payload in data pipe 3
	REG_RX_PW_P4				= (uint8_t) 0b00010101,	// Number of bytes in RX payload in data pipe 4
	REG_RX_PW_P5				= (uint8_t) 0b00010110,	// Number of bytes in RX payload in data pipe 5
	REG_FIFO_STATUS				= (uint8_t) 0b00010111,	// FIFO status register
	REG_DYNPD					= (uint8_t) 0b00011100,	// Enable dynamic payload length
	REG_FEATURE					= (uint8_t) 0b00011101	// Feature register
};

enum Mask
{
	MASK_INTERRUPTS				= (uint8_t) 0b01110000,	// interrupts
	MASK_RX_P_NO				= (uint8_t) 0b00001110,	// Pipe number for payload available
	MASK_TX_FULL				= (uint8_t) 0b00000001,	// TX FIFO full
	MASK_PLOS_CNT				= (uint8_t) 0b11110000,	// lost package counter
	MASK_ARC_CNT				= (uint8_t) 0b00001111,	// retransmit counter
	MASK_RPD					= (uint8_t) 0b00000001,	// received power detector
	MASK_TX_REUSE				= (uint8_t) 0b01000000,	// TX reuse last transmitted package
	MASK_TX_FIFO_STATUS			= (uint8_t) 0b00110000,	// TX FIFO Status
	MASK_RX_FIFO_STATUS			= (uint8_t) 0b00000011	// RX FIFO Status
};

//----------------------------------ENUMS----------------------------------

enum Pin
{
	PIN_CE						= NRF24_CE_Pin,
	PIN_CSN						= NRF24_CSN_Pin
};

enum PinState
{
	LOW							= (uint8_t) 0,
	HIGH						= (uint8_t) 1
};

enum State
{
	DISABLED					= (uint8_t) 0,
	ENABLED						= (uint8_t) 1
};

enum PipeNumber
{
	PIPE_NO_0 					= (uint8_t) 0b000,
	PIPE_NO_1 					= (uint8_t) 0b001,
	PIPE_NO_2 					= (uint8_t) 0b010,
	PIPE_NO_3 					= (uint8_t) 0b011,
	PIPE_NO_4 					= (uint8_t) 0b100,
	PIPE_NO_5 					= (uint8_t) 0b101,
	PIPE_NO_NOT_USED 			= (uint8_t) 0b110,
	PIPE_NO_EMPTY 				= (uint8_t) 0b111
};

enum Pipe
{
	PIPE_NONE			 		= (uint8_t) 0b00000000,
	PIPE_0		 				= (uint8_t) 0b00000001,
	PIPE_1		 				= (uint8_t) 0b00000010,
	PIPE_2		 				= (uint8_t) 0b00000100,
	PIPE_3		 				= (uint8_t) 0b00001000,
	PIPE_4		 				= (uint8_t) 0b00010000,
	PIPE_5		 				= (uint8_t) 0b00100000,
	PIPE_ALL 					= (uint8_t) 0b00111111
};

enum Interrupts
{
	INTERRUPT_ALL 				= (uint8_t) 0b01110000,
	INTERRUPT_MAX_RT 			= (uint8_t) 0b00010000,
	INTERRUPT_TX_DS 			= (uint8_t) 0b00100000,
	INTERRUPT_RX_DR 			= (uint8_t) 0b01000000,
	INTERRUPT_MAX_RT_AND_TX_DS	= (uint8_t) 0b00110000,
	INTERRUPT_MAX_RT_AND_RX_DR	= (uint8_t) 0b01010000,
	INTERRUPT_TX_DS_AND_RX_DR	= (uint8_t) 0b01100000,
	INTERRUPT_NONE				= (uint8_t) 0b00000000
};

enum CRCType
{
	CRC_2_BYTE	 				= (uint8_t) 0b00001100,
	CRC_1_BYTE 					= (uint8_t) 0b00001000,
	CRC_DISABLED				= (uint8_t) 0b00000000
};

enum PowerState
{
	POWER_DOWN 					= (uint8_t) 0b00000000,
	POWER_UP	 				= (uint8_t) 0b00000010
};

enum Mode
{
	PTX		 					= (uint8_t) 0b00000000,
	PRX			 				= (uint8_t) 0b00000001,
};

enum AddressWidth
{
	AW_ILLEGAL			 		= (uint8_t) 0b00000000,
	AW_3_BYTE		 			= (uint8_t) 0b00000001,
	AW_4_BYTE		 			= (uint8_t) 0b00000010,
	AW_5_BYTE		 			= (uint8_t) 0b00000011
};

enum ARD
{
	ARD_250us			 		= (uint8_t) 0b00000000,
	ARD_500us		 			= (uint8_t) 0b00010000,
	ARD_750us		 			= (uint8_t) 0b00100000,
	ARD_1000us		 			= (uint8_t) 0b00110000,
	ARD_1250us			 		= (uint8_t) 0b01000000,
	ARD_1500us			 		= (uint8_t) 0b01010000,
	ARD_1750us			 		= (uint8_t) 0b01100000,
	ARD_2000us			 		= (uint8_t) 0b01110000,
	ARD_2250us			 		= (uint8_t) 0b10000000,
	ARD_2500us			 		= (uint8_t) 0b10010000,
	ARD_2750us			 		= (uint8_t) 0b10100000,
	ARD_3000us			 		= (uint8_t) 0b10110000,
	ARD_3250us			 		= (uint8_t) 0b11000000,
	ARD_3500us			 		= (uint8_t) 0b11010000,
	ARD_3750us			 		= (uint8_t) 0b11100000,
	ARD_4000us			 		= (uint8_t) 0b11110000
};

enum DataRate
{
	DR_250_KBPS					= (uint8_t) 0b00100000,
	DR_1_MBPS					= (uint8_t) 0b00000000,
	DR_2_MBPS					= (uint8_t) 0b00001000
};

enum RFPowerOption
{
	POWER_MINUS_18_DBM			= (uint8_t) 0b00000000,
	POWER_MINUS_12_DBM			= (uint8_t) 0b00000010,
	POWER_MINUS_6_DBM			= (uint8_t) 0b00000100,
	POWER_MINUS_0_DBM			= (uint8_t) 0b00000110
};

enum FifoStatus
{
	FIFO_NORMAL			 		= (uint8_t) 0b00,
	FIFO_EMPTY		 			= (uint8_t) 0b01,
	FIFO_FULL		 			= (uint8_t) 0b10
};

//----------------------------------VARIABLEN----------------------------------

float nrf24l01_signal_quality;
uint8_t unused;

//----------------------------------METHODS----------------------------------

uint8_t nrf24l01_Init(enum Mode mode, SPI_HandleTypeDef *spi_handle);
void nrf24l01_Setup();
uint8_t nrf24l01_Working();
uint8_t nrf24l01_Transmit(uint8_t *payload, uint8_t size, uint8_t ack, uint8_t *ack_payload, uint8_t *ack_size);
uint8_t nrf24l01_Receive(uint8_t *payload);
void nrf24l01_WriteACKPayload(uint8_t *payload, uint8_t size);

void nrf24l01_GetSTATUS(enum Interrupts *interrupts, enum PipeNumber *pipe, uint8_t *tx_full);
void nrf24l01_GetOBSERVE_TX(uint8_t *plos, uint8_t *arc);
uint8_t nrf24l01_GetRPD();
void nrf24l01_GetFIFO_STATUS(uint8_t *tx_reuse, enum FifoStatus *tx_fifo_status, enum FifoStatus *rx_fifo_status);

void nrf24l01_ClearInterrupts(enum Interrupts interrupts);
void nrf24l01_SetFrequency(int16_t frequency);
uint16_t nrf24l01_GetFrequency();

uint8_t nrf24l01_Interface(enum Command command, uint8_t *data_bytes, uint8_t size);
uint8_t nrf24l01_WriteSPI(uint8_t *tx_buff, uint8_t size);
void nrf24l01_ReadSPI(uint8_t *rx_buff, uint8_t size);
void nrf24l01_SetPin(enum Pin pin, enum PinState state);

#endif /* NRF24L01_PTX_H_ */
