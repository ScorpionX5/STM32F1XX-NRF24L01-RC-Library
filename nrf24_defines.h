#ifndef NRF24_NRF24_DEFINES_H_
#define NRF24_NRF24_DEFINES_H_

#include "main.h"

/** NRF24L01P SPI command words. */
typedef enum
{
    READ                        = (uint8_t) 0b00000000,   ///< Read registers. Lowest 5 bits specify register address.
    WRITE                       = (uint8_t) 0b00100000,   ///< Write to registers. Lowest 5 bits specify register address. Executable in power down or standby mode only.
    CMD_R_RX_PAYLOAD            = (uint8_t) 0b01100001,   ///< Read RX-payload: 1 – 32 bytes. A read operation always starts at byte 0. Payload is deleted from FIFO after it is read. Used in RX mode.
    CMD_W_TX_PAYLOAD            = (uint8_t) 0b10100000,   ///< Write TX-payload: 1 – 32 bytes. A write operation always starts at byte 0 used in TX payload.
    CMD_FLUSH_TX                = (uint8_t) 0b11100001,   ///< Flush TX FIFO, used in TX mode.
    CMD_FLUSH_RX                = (uint8_t) 0b11100010,   ///< Flush RX FIFO, used in RX modeShould not be executed during transmission of acknowledge, that is, acknowledge package will not be completed.
    CMD_REUSE_TX_PL             = (uint8_t) 0b11100011,   ///< Used for a PTX device. Reuse last transmitted payload. TX payload reuse is active until W_TX_PAYLOAD or FLUSH TX is executed. TX payload reuse must not be activated or deactivated during package transmission.
    CMD_R_RX_PL_WID             = (uint8_t) 0b01100000,   ///< Read RX payload width for the top R_RX_PAYLOAD in the RX FIFO.
    CMD_W_ACK_PAYLOAD           = (uint8_t) 0b10101000,   ///< Used in RX mode. Write Payload to be transmitted together with ACK packet on PIPE specified by lowest 3 bits (000 to 101). Maximum three ACK packet payloads can be pending. Payloads with same PIPE are handled using first in - first out principle. Write payload: 1– 32 bytes. A write operation always starts at byte 0.
    CMD_W_TX_PAYLOAD_NO_ACK     = (uint8_t) 0b10110000,   ///< Used in TX mode. Disables AUTOACK on this specific packet.
    CMD_NOP                     = (uint8_t) 0b11111111    ///< No Operation. Might be used to read the STATUS register.
} NRF24_Command;

/** NRF24L01P hardware register addresses. */
typedef enum
{
    REG_CONFIG                  = (uint8_t) 0b00000000,   ///< Configuration register
    REG_EN_AA                   = (uint8_t) 0b00000001,   ///< Enable 'Auto Acknowledgment' function. Disable this functionality to be compatible with nRF2401.
    REG_EN_RXADDR               = (uint8_t) 0b00000010,   ///< Enable RX addresses
    REG_SETUP_AW                = (uint8_t) 0b00000011,   ///< Setup of address widths (common for all data pipes)
    REG_SETUP_RETR              = (uint8_t) 0b00000100,   ///< Setup of automatic retransmission
    REG_RF_CH                   = (uint8_t) 0b00000101,   ///< RF channel
    REG_RF_SETUP                = (uint8_t) 0b00000110,   ///< RF setup register
    REG_STATUS                  = (uint8_t) 0b00000111,   ///< Status register
    REG_OBSERVE_TX              = (uint8_t) 0b00001000,   ///< Transmit observe register
    REG_RPD                     = (uint8_t) 0b00001001,   ///< Received power detector
    REG_RX_ADDR_P0              = (uint8_t) 0b00001010,   ///< Receive address data pipe 0 (5 bytes length)
    REG_RX_ADDR_P1              = (uint8_t) 0b00001011,   ///< Receive address data pipe 1 (5 bytes length)
    REG_RX_ADDR_P2              = (uint8_t) 0b00001100,   ///< Receive address data pipe 2
    REG_RX_ADDR_P3              = (uint8_t) 0b00001101,   ///< Receive address data pipe 3
    REG_RX_ADDR_P4              = (uint8_t) 0b00001110,   ///< Receive address data pipe 4
    REG_RX_ADDR_P5              = (uint8_t) 0b00001111,   ///< Receive address data pipe 5
    REG_TX_ADDR                 = (uint8_t) 0b00010000,   ///< Transmit address
    REG_RX_PW_P0                = (uint8_t) 0b00010001,   ///< Number of bytes in RX payload in data pipe 0
    REG_RX_PW_P1                = (uint8_t) 0b00010010,   ///< Number of bytes in RX payload in data pipe 1
    REG_RX_PW_P2                = (uint8_t) 0b00010011,   ///< Number of bytes in RX payload in data pipe 2
    REG_RX_PW_P3                = (uint8_t) 0b00010100,   ///< Number of bytes in RX payload in data pipe 3
    REG_RX_PW_P4                = (uint8_t) 0b00010101,   ///< Number of bytes in RX payload in data pipe 4
    REG_RX_PW_P5                = (uint8_t) 0b00010110,   ///< Number of bytes in RX payload in data pipe 5
    REG_FIFO_STATUS             = (uint8_t) 0b00010111,   ///< FIFO status register
    REG_DYNPD                   = (uint8_t) 0b00011100,   ///< Enable dynamic payload length
    REG_FEATURE                 = (uint8_t) 0b00011101    ///< Feature register
} NRF24_Register;

/** NRF24L01P bitmasks for values in [REGISTERS] */
typedef enum
{
    MASK_INTERRUPTS             = (uint8_t) 0b01110000,   ///< [STATUS] Interrupts (Data ready RX FIFO interrupt | Data sent TX FIFO interrupt | Maximum number of TX retransmits interrupt )
    MASK_RX_P_NO                = (uint8_t) 0b00001110,   ///< [STATUS] Pipe number for payload available
    MASK_TX_FULL                = (uint8_t) 0b00000001,   ///< [STATUS] TX FIFO full
    MASK_PLOS_CNT               = (uint8_t) 0b11110000,   ///< [OBSERVE_TX] Count lost packets
    MASK_ARC_CNT                = (uint8_t) 0b00001111,   ///< [OBSERVE_TX] Count retransmitted packets
    MASK_RPD                    = (uint8_t) 0b00000001,   ///< [RPD] Received power detector
    MASK_TX_REUSE               = (uint8_t) 0b01000000,   ///< [FIFO_STATUS] TX reuse last transmitted package
    MASK_TX_FIFO_STATUS         = (uint8_t) 0b00110000,   ///< [FIFO_STATUS] TX FIFO Status ( full | empty )
    MASK_RX_FIFO_STATUS         = (uint8_t) 0b00000011    ///< [FIFO_STATUS] RX FIFO Status ( full | empty )
} NRF24_Mask;

/** NRF24 pin states */
typedef enum
{
	LOW							= (uint8_t) 0, ///< PIN low
	HIGH						= (uint8_t) 1  ///< PIN high
} NRF24_PinState;

/** NRF24 configuration state */
typedef enum
{
	DISABLED					= (uint8_t) 0,   ///< Configuration disabled
	ENABLED						= (uint8_t) 1    ///< Configuration enabled
} NRF24_ConfigState;

/** Possible values for RX_P_NO in STATUS register (Pipe number for which payload is available in RX FIFO) */
typedef enum
{
	PIPE_NO_0 					= (uint8_t) 0b000,   ///< Payload available on pipe 0
	PIPE_NO_1 					= (uint8_t) 0b001,   ///< Payload available on pipe 1
	PIPE_NO_2 					= (uint8_t) 0b010,   ///< Payload available on pipe 2
	PIPE_NO_3 					= (uint8_t) 0b011,   ///< Payload available on pipe 3
	PIPE_NO_4 					= (uint8_t) 0b100,   ///< Payload available on pipe 4
	PIPE_NO_5 					= (uint8_t) 0b101,   ///< Payload available on pipe 5
	PIPE_NO_NOT_USED 			= (uint8_t) 0b110,   ///< [Unused value]
	PIPE_NO_EMPTY 				= (uint8_t) 0b111    ///< RX FIFO Empty
} NRF24_PipeNumber;

/** Possible values for EN_RXADDR or EN_AA register (Enable pipes and AA on pipes) */
typedef enum
{
	PIPE_NONE			 		= (uint8_t) 0b00000000,   ///< No pipes enabled
	PIPE_0		 				= (uint8_t) 0b00000001,   ///< Pipe 0 enabled
	PIPE_1		 				= (uint8_t) 0b00000010,   ///< Pipe 1 enabled
	PIPE_2		 				= (uint8_t) 0b00000100,   ///< Pipe 2 enabled
	PIPE_3		 				= (uint8_t) 0b00001000,   ///< Pipe 3 enabled
	PIPE_4		 				= (uint8_t) 0b00010000,   ///< Pipe 4 enabled
	PIPE_5		 				= (uint8_t) 0b00100000,   ///< Pipe 5 enabled
	PIPE_ALL 					= (uint8_t) 0b00111111    ///< All pipes enabled
} NRF24_Pipe;

/** Possible values for interrups in STATUS register (shifted as positioned in register) */
typedef enum
{
	INTERRUPT_ALL 				= (uint8_t) 0b01110000,   ///< All interrupts set (Data ready RX FIFO interrupt, Data sent TX FIFO interrupt, Maximum number of TX retransmits interrupt)
	INTERRUPT_MAX_RT 			= (uint8_t) 0b00010000,   ///< Maximum number of TX retransmits interrupt
	INTERRUPT_TX_DS 			= (uint8_t) 0b00100000,   ///< Data sent TX FIFO interrupt
	INTERRUPT_RX_DR 			= (uint8_t) 0b01000000,   ///< Data ready RX FIFO interrupt
	INTERRUPT_MAX_RT_AND_TX_DS	= (uint8_t) 0b00110000,   ///< Data sent TX FIFO interrupt, Maximum number of TX retransmits interrupt
	INTERRUPT_MAX_RT_AND_RX_DR	= (uint8_t) 0b01010000,   ///< Data ready RX FIFO interrupt, Maximum number of TX retransmits interrupt
	INTERRUPT_TX_DS_AND_RX_DR	= (uint8_t) 0b01100000,   ///< Data ready RX FIFO interrupt, Data sent TX FIFO interrupt
	INTERRUPT_NONE				= (uint8_t) 0b00000000    ///< No interrupts set
} NRF24_Interrupts;

/** Possible values for CRC configuration in CONFIG register (shifted as positioned in register)*/
typedef enum
{
	CRC_2_BYTE	 				= (uint8_t) 0b00001100,   ///< Enable CRC with 2 bytes encoding scheme
	CRC_1_BYTE 					= (uint8_t) 0b00001000,   ///< Enable CRC with 1 byte encoding scheme
	CRC_DISABLED				= (uint8_t) 0b00000000    ///< Disable CRC
} NRF24_CRCType;

/** Possible values for power status in CONFIG register (shifted as positioned in register)*/
typedef enum
{
	POWER_DOWN 					= (uint8_t) 0b00000000,   ///< Power down module
	POWER_UP	 				= (uint8_t) 0b00000010    ///< Power up module
} NRF24_PowerState;

/** Possible values for PRIM_RX in CONFIG register (shifted as positioned in register) */
typedef enum
{
	PTX		 					= (uint8_t) 0b00000000,   ///< Act as primary transmitter
	PRX			 				= (uint8_t) 0b00000001    ///< Act as primary receiver
} NRF24_Mode;

/** Possible values for SETUP_AW register (setting common address widths for all registers) */
typedef enum
{
	AW_ILLEGAL			 		= (uint8_t) 0b00000000,   ///< [Illegal value]
	AW_3_BYTE		 			= (uint8_t) 0b00000001,   ///< Use 3 byte addresses
	AW_4_BYTE		 			= (uint8_t) 0b00000010,   ///< Use 4 byte addresses
	AW_5_BYTE		 			= (uint8_t) 0b00000011    ///< Use 5 byte addresses
} NRF24_AddressWidth;

/** Possible values for auto retransmit delay (ARD) in SETUP_RETR register (shifted as positioned in register) */
typedef enum
{
	ARD_250us			 		= (uint8_t) 0b00000000,   ///< Wait 250us from end of transmission to start of next transmission
	ARD_500us		 			= (uint8_t) 0b00010000,   ///< Wait 500us from end of transmission to start of next transmission
	ARD_750us		 			= (uint8_t) 0b00100000,   ///< Wait 750us from end of transmission to start of next transmission
	ARD_1000us		 			= (uint8_t) 0b00110000,   ///< Wait 1000us from end of transmission to start of next transmission
	ARD_1250us			 		= (uint8_t) 0b01000000,   ///< Wait 1250us from end of transmission to start of next transmission
	ARD_1500us			 		= (uint8_t) 0b01010000,   ///< Wait 1500us from end of transmission to start of next transmission
	ARD_1750us			 		= (uint8_t) 0b01100000,   ///< Wait 1750us from end of transmission to start of next transmission
	ARD_2000us			 		= (uint8_t) 0b01110000,   ///< Wait 2000us from end of transmission to start of next transmission
	ARD_2250us			 		= (uint8_t) 0b10000000,   ///< Wait 2250us from end of transmission to start of next transmission
	ARD_2500us			 		= (uint8_t) 0b10010000,   ///< Wait 2500us from end of transmission to start of next transmission
	ARD_2750us			 		= (uint8_t) 0b10100000,   ///< Wait 2750us from end of transmission to start of next transmission
	ARD_3000us			 		= (uint8_t) 0b10110000,   ///< Wait 3000us from end of transmission to start of next transmission
	ARD_3250us			 		= (uint8_t) 0b11000000,   ///< Wait 3250us from end of transmission to start of next transmission
	ARD_3500us			 		= (uint8_t) 0b11010000,   ///< Wait 3500us from end of transmission to start of next transmission
	ARD_3750us			 		= (uint8_t) 0b11100000,   ///< Wait 3750us from end of transmission to start of next transmission
	ARD_4000us			 		= (uint8_t) 0b11110000    ///< Wait 4000us from end of transmission to start of next transmission
} NRF24_ARD;

/** Possible values for data rate in RF_SETUP register (shifted as positioned in register) */
typedef enum
{
	DR_250_KBPS					= (uint8_t) 0b00100000,   ///< Use a datarate of 250 kbps
	DR_1_MBPS					= (uint8_t) 0b00000000,   ///< Use a datarate of 1 Mbps
	DR_2_MBPS					= (uint8_t) 0b00001000    ///< Use a datarate of 2 Mbps
} NRF24_DataRate;

/** Possible values for RF output power in RF_SETUP register (shifted as positioned in register) */
typedef enum
{
	POWER_MINUS_18_DBM			= (uint8_t) 0b00000000,   ///< Set TX power to -18 dBm
	POWER_MINUS_12_DBM			= (uint8_t) 0b00000010,   ///< Set TX power to -12 dBm
	POWER_MINUS_6_DBM			= (uint8_t) 0b00000100,   ///< Set TX power to -6 dBm
	POWER_MINUS_0_DBM			= (uint8_t) 0b00000110    ///< Set TX power to 0 dBm
} NRF24_RFOutputPower;

/** Possible values for 2bit FIFO status in FIFO_STATUS register */
typedef enum
{
	FIFO_NORMAL			 		= (uint8_t) 0b00,   ///< Data in FIFO but FIFO not full
	FIFO_EMPTY		 			= (uint8_t) 0b01,   ///< No data in FIFO
	FIFO_FULL		 			= (uint8_t) 0b10    ///< FIFO full
} NRF24_FifoStatus;

/** NRF24 pins */
typedef enum
{
	PIN_CE						= NRF24_CE_Pin,   ///< Chip enable active high, used to activate the chip in RX or TX mode
	PIN_CSN						= NRF24_CSN_Pin   ///< SPI chip select (active low)
} NRF24_Pin;


#endif /* NRF24_NRF24_DEFINES_H_ */
