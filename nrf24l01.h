#ifndef NRF24L01_PTX_H_
#define NRF24L01_PTX_H_

#include "stm32f1xx_hal.h"
#include "NRF24/nrf24_defines.h"

//----------------------------------VARIABLEN----------------------------------

float nrf24l01_signal_quality;
uint8_t unused;

//----------------------------------METHODS----------------------------------

uint8_t nrf24l01_Init(NRF24_Mode mode, SPI_HandleTypeDef *spi_handle);
void nrf24l01_Setup();
uint8_t nrf24l01_Working();
uint8_t nrf24l01_Transmit(uint8_t *payload, uint8_t size, uint8_t ack, uint8_t *ack_payload, uint8_t *ack_size);
uint8_t nrf24l01_Receive(uint8_t *payload);
void nrf24l01_WriteACKPayload(uint8_t *payload, uint8_t size);

void nrf24l01_GetSTATUS(NRF24_Interrupts *interrupts, NRF24_PipeNumber *pipe, uint8_t *tx_full);
void nrf24l01_GetOBSERVE_TX(uint8_t *plos, uint8_t *arc);
uint8_t nrf24l01_GetRPD();
void nrf24l01_GetFIFO_STATUS(uint8_t *tx_reuse, NRF24_FifoStatus *tx_fifo_status, NRF24_FifoStatus *rx_fifo_status);

void nrf24l01_ClearInterrupts(NRF24_Interrupts interrupts);
void nrf24l01_SetFrequency(int16_t frequency);
uint16_t nrf24l01_GetFrequency();

uint8_t nrf24l01_Interface(NRF24_Command command, uint8_t *data_bytes, uint8_t size);
uint8_t nrf24l01_WriteSPI(uint8_t *tx_buff, uint8_t size);
void nrf24l01_ReadSPI(uint8_t *rx_buff, uint8_t size);
void nrf24l01_SetPin(NRF24_Pin pin, NRF24_PinState state);

#endif /* NRF24L01_PTX_H_ */
