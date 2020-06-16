#ifndef NRF24_NRF24_CONFIGURATION_H_
#define NRF24_NRF24_CONFIGURATION_H_

/**
 * Initial configuration for nrf24 antenna
 */

uint8_t REUSE_TX_PAYLOAD			= DISABLED;				///< Reuse last transmitted payload: disabled
uint8_t DISABLE_INTERRUPTS_ON_IRQ	= INTERRUPT_ALL;		///< Disable all interrupts on the IRQ-Pin
uint8_t CRC_CONFIG					= CRC_2_BYTE;			///< Use 2 bytes CRC
uint8_t POWER_STATE					= POWER_UP;				///< Device is powered up
uint8_t MODE						= PTX;					///< Device used as primary transmitter
uint8_t AUTO_ACKNOWLEDGE			= ENABLED;				///< Auto acknowledge pipe 0
uint8_t ADDRESS_WIDTH				= AW_3_BYTE;			///< Address width is 3 bytes
uint8_t ARD							= ARD_500us;			///< Auto retransmit delay: 500us
uint8_t ARC							= 7;					///< Auto retransmit count: 7 retransmits (max 15)
uint8_t RF_CHANNEL					= 120;					///< RF channel frequency = 2400 + 120 MHz (max 2525 MHz)
uint8_t CONTINUOUS_WAVE				= DISABLED;				///< Continuous carrier transmit disabled
uint8_t DATA_RATE					= DR_1_MBPS;			///< Data rate: 1 Mbps
uint8_t RF_POWER					= POWER_MINUS_0_DBM;	///< Maximum transmit power
uint8_t ADDRESS[] 					= { 0xD1, 0xD1, 0xD1 };	///< Tx address and rx address of pipe 0
uint8_t PAYLOAD_WIDTH				= 0;					///< Payload size: 0 bytes
uint8_t DYN_PAYLOAD_WIDTH			= ENABLED;				///< Dynamic payload size: enabled
uint8_t ACK_PAYLOAD					= ENABLED;				///< Send payload with acknowledge: enabled
uint8_t DYN_ACK						= ENABLED;				///< Dynamic acknowledge: allow sending packet without requesting ACK
uint32_t TX_TIMEOUT					= 7000;					///< Wait 7000us for TX timeout

#endif /* NRF24_NRF24_CONFIGURATION_H_ */
