#include "nrf24l01.h"

SPI_HandleTypeDef *nrf24l01_spi;

uint8_t REUSE_TX_PAYLOAD			= DISABLED;				//reuse last transmitted payload: disabled
uint8_t DISABLE_INTERRUPTS_ON_IRQ	= INTERRUPT_ALL;		//disable all interrupts on the IRQ-Pin
uint8_t CRC_CONFIG					= CRC_2_BYTE;			//use 2 bytes CRC
uint8_t POWER_STATE					= POWER_UP;				//device is powered up
uint8_t MODE						= PTX;					//device used as primary transmitter
uint8_t AUTO_ACKNOWLEDGE			= ENABLED;				//auto acknowledge pipe 0
uint8_t ADDRESS_WIDTH				= AW_3_BYTE;			//address width is 3 bytes
uint8_t ARD							= ARD_500us;			//auto retransmit delay: 500us
uint8_t ARC							= 7;					//auto retransmit count: 7 retransmits (max 15)
uint8_t RF_CHANNEL					= 120;					//RF channel frequency = 2400 + 120 MHz (max 2525 MHz)
uint8_t CONTINUOUS_WAVE				= DISABLED;				//continuous carrier transmit disabled
uint8_t DATA_RATE					= DR_1_MBPS;			//data rate: 1 Mbps
uint8_t RF_POWER					= POWER_MINUS_0_DBM;	//maximum transmit power
uint8_t ADDRESS[] 					= { 0xD1, 0xD1, 0xD1 };	//tx address and rx address of pipe 0
uint8_t PAYLOAD_WIDTH				= 0;					//payload size: 0 bytes
uint8_t DYN_PAYLOAD_WIDTH			= ENABLED;				//dynamic payload size: enabled
uint8_t ACK_PAYLOAD					= ENABLED;				//send payload with acknowledge: enabled
uint8_t DYN_ACK						= ENABLED;				//dynamic acknowledge: allow sending packet without requesting ACK
uint32_t TX_TIMEOUT					= 7000;					//wait 7000us for TX timeout

uint16_t send_counter = 0;									//on send packet: +(1+ARC)
uint16_t lost_counter = 0;									//on failed retransmit: +1 ; on failed packet: +(1+ARC)
float signal_quality = 0;									//signal quality: 0-100%

//initialize nrf24l01
uint8_t nrf24l01_Init(enum Mode mode, SPI_HandleTypeDef *spi_handle)
{
	//set variables and pins
	nrf24l01_spi = spi_handle;
	MODE = mode;
	nrf24l01_SetPin(PIN_CSN, HIGH);
	nrf24l01_SetPin(PIN_CE, LOW);
	HAL_Delay(10);

	//check connection to nrf24l01
	if (!nrf24l01_Working()) return 0;

	//set configuration registers
	nrf24l01_Setup();

	//Clear RX FIFO, TX FIFO and all interrupts
	nrf24l01_Interface(CMD_FLUSH_RX, &unused, 0);
	nrf24l01_Interface(CMD_FLUSH_TX, &unused, 0);
	nrf24l01_ClearInterrupts(INTERRUPT_ALL);

	//if configured, start reusing TX payload
	if (REUSE_TX_PAYLOAD == ENABLED) nrf24l01_Interface(CMD_REUSE_TX_PL, &unused, 0);

	//if configured, go to RX mode
	if (MODE == PRX) nrf24l01_SetPin(PIN_CE, HIGH);
	return 1;
}

//set configuration registers
void nrf24l01_Setup()
{
	//calculate register values
	uint8_t CONFIG 		= DISABLE_INTERRUPTS_ON_IRQ | CRC_CONFIG | POWER_STATE | MODE;
	uint8_t EN_RXADDR	= PIPE_0;
	uint8_t SETUP_RETR	= ARD | ARC;
	uint8_t RF_SETUP	= (CONTINUOUS_WAVE << 7) | DATA_RATE | RF_POWER;
	uint8_t FEATURE		= (DYN_PAYLOAD_WIDTH << 2) | (ACK_PAYLOAD << 1) | DYN_ACK;

	//set registers
	nrf24l01_Interface(WRITE | REG_CONFIG,		&CONFIG,			1);
	nrf24l01_Interface(WRITE | REG_EN_AA,		&AUTO_ACKNOWLEDGE,	1);
	nrf24l01_Interface(WRITE | REG_EN_RXADDR,	&EN_RXADDR,			1);
	nrf24l01_Interface(WRITE | REG_SETUP_AW,	&ADDRESS_WIDTH,		1);
	nrf24l01_Interface(WRITE | REG_SETUP_RETR,	&SETUP_RETR,		1);
	nrf24l01_Interface(WRITE | REG_RF_CH,		&RF_CHANNEL,		1);
	nrf24l01_Interface(WRITE | REG_RF_SETUP,	&RF_SETUP,			1);
	nrf24l01_Interface(WRITE | REG_TX_ADDR,		ADDRESS,			ADDRESS_WIDTH + 2);
	nrf24l01_Interface(WRITE | REG_RX_ADDR_P0,	ADDRESS,			ADDRESS_WIDTH + 2);
	nrf24l01_Interface(WRITE | REG_RX_PW_P0,	&PAYLOAD_WIDTH,		1);
	nrf24l01_Interface(WRITE | REG_DYNPD,		&DYN_PAYLOAD_WIDTH,	1);
	nrf24l01_Interface(WRITE | REG_FEATURE,		&FEATURE,			1);
}

//check connection to nrf24l01, return 0 for error and 1 for ok
uint8_t nrf24l01_Working()
{
	//read original value of feature register
	uint8_t original;
	nrf24l01_Interface(READ | REG_FEATURE, &original, 1);

	//flip last bit (EN_DYN_ACK) and rewrite new value to feature register
	uint8_t write;
	write = original ^ 0b00000001; //XOR flips last bit (EN_DYN_ACK)
	nrf24l01_Interface(WRITE | REG_FEATURE, &write, 1);

	//read feature register
	uint8_t read;
	nrf24l01_Interface(READ | REG_FEATURE, &read, 1);

	//reset feature register to original value
	nrf24l01_Interface(WRITE | REG_FEATURE, &original, 1);

	//return true, if the written value was read correctly
	return read == write;
}

//Transmits a packet with "payload" of size "size"
//if "ack" is true, wait for an ACK-packet
//the received ACK-payload is written to "ack_payload" and "ack_size"
//signal quality is updated based on result
//result is returned as enum Interrupts (INTERRUPT_NONE = timeout)
uint8_t nrf24l01_Transmit(uint8_t *payload, uint8_t size, uint8_t ack, uint8_t *ack_payload, uint8_t *ack_size)
{
	//write payload to TX FIFO
	if (ack) nrf24l01_Interface(CMD_W_TX_PAYLOAD, payload, size);
	else nrf24l01_Interface(CMD_W_TX_PAYLOAD_NO_ACK, payload, size);

	//clear interrupts and RX FIFO to be sure to receive the acknowledge (if RX-FIFO is full, acknowledge won't be received)
	nrf24l01_Interface(CMD_FLUSH_RX, &unused, 0);
	nrf24l01_ClearInterrupts(INTERRUPT_ALL);
	enum Interrupts interrupts = INTERRUPT_NONE;

	//transmit packet
	nrf24l01_SetPin(PIN_CE, HIGH);
	uint32_t time = micros();
	while (micros() < time + 25);
	nrf24l01_SetPin(PIN_CE, LOW);

	//if ACK expected
	if (ack)
	{
		//poll for feedback (data sent, max retransmits, or timeout)
		time = micros();
		while (micros() < time + TX_TIMEOUT)
		{
			nrf24l01_GetSTATUS(&interrupts, &unused, &unused);
			if (interrupts != INTERRUPT_NONE) break;
		}
		uint32_t time = micros();
		while (micros() < time + 50);
		nrf24l01_GetSTATUS(&interrupts, &unused, &unused);

		//get acknowledge
		*ack_size = nrf24l01_Receive(ack_payload);

		//increment signal quality counters
		send_counter += ARC + 1;

		if ((interrupts == INTERRUPT_NONE) || (interrupts & INTERRUPT_MAX_RT))
		{
			lost_counter += ARC + 1;
			nrf24l01_Interface(CMD_FLUSH_TX, &unused, 0); //Clear TX FIFO
		}
		else
		{
			uint8_t arc = 0;
			nrf24l01_GetOBSERVE_TX(&unused, &arc);
			lost_counter += arc;
		}

		//calculate signal quality every 100 packets
		if (send_counter >= 100 * (ARC + 1))
		{
			signal_quality = 100 * (1 - lost_counter/send_counter);
			send_counter = 0;
			lost_counter = 0;
		}
	}

	//if configured, start reusing TX payload again
	if (REUSE_TX_PAYLOAD == ENABLED) nrf24l01_Interface(CMD_REUSE_TX_PL, &unused, 0);

	//return interrupt result
	return interrupts;
}

//try to receive a packet
//returns payload size, payload available if size > 0
//received payload is written to "payload"
uint8_t nrf24l01_Receive(uint8_t *payload)
{
	//set variables
	uint8_t size = 0;
	enum PipeNumber pipe = PIPE_NO_NOT_USED;

	//check status for receive pipe
	nrf24l01_GetSTATUS(&unused, &pipe, &unused);

	if (pipe == PIPE_NO_0)
	{
		//get payload width and payload
		size = PAYLOAD_WIDTH;
		if (DYN_PAYLOAD_WIDTH == ENABLED) nrf24l01_Interface(CMD_R_RX_PL_WID, &size, 1);
		if (size) nrf24l01_Interface(CMD_R_RX_PAYLOAD, payload, size);
	}

	return size;
}

//writes "payload" with "size" as acknowledge to TX FIFO
void nrf24l01_WriteACKPayload(uint8_t *payload, uint8_t size)
{
	nrf24l01_Interface(CMD_FLUSH_TX, &unused, 0);
	nrf24l01_Interface(CMD_W_ACK_PAYLOAD | PIPE_NO_0, payload, size);
}

void nrf24l01_GetSTATUS(enum Interrupts *interrupts, enum PipeNumber *pipe, uint8_t *tx_full)
{
	uint8_t status = nrf24l01_Interface(CMD_NOP, 0, 0);
	*interrupts = status & MASK_INTERRUPTS;
	*pipe = (status & MASK_RX_P_NO) >> 1;
	*tx_full = status & MASK_TX_FULL;
}

void nrf24l01_GetOBSERVE_TX(uint8_t *plos, uint8_t *arc)
{
	uint8_t observe_tx;
	nrf24l01_Interface(READ | REG_OBSERVE_TX, &observe_tx, 1);
	*plos = (observe_tx & MASK_PLOS_CNT) >> 4;
	*arc = observe_tx & MASK_ARC_CNT;
}

uint8_t nrf24l01_GetRPD()
{
	uint8_t rpd;
	nrf24l01_Interface(READ | REG_RPD, &rpd, 1);
	return rpd & MASK_RPD;
}

void nrf24l01_GetFIFO_STATUS(uint8_t *tx_reuse, enum FifoStatus *tx_fifo_status, enum FifoStatus *rx_fifo_status)
{
	uint8_t fifo_status;
	nrf24l01_Interface(READ | REG_FIFO_STATUS, &fifo_status, 1);
	*tx_reuse = (fifo_status & MASK_TX_REUSE) >> 6;
	*tx_fifo_status = (fifo_status & MASK_TX_FIFO_STATUS) >> 4;
	*rx_fifo_status = fifo_status & MASK_RX_FIFO_STATUS;
}

void nrf24l01_ClearInterrupts(enum Interrupts interrupts)
{
	nrf24l01_Interface(WRITE | REG_STATUS, &interrupts, 1);
}

// change RF channel and reset packet lost counter
void nrf24l01_SetFrequency(int16_t frequency)
{
	RF_CHANNEL = frequency - 2400;
	nrf24l01_Interface(WRITE | REG_RF_CH, &RF_CHANNEL, 1);
}

uint16_t nrf24l01_GetFrequency()
{
	return 2400 + RF_CHANNEL;
}

uint8_t nrf24l01_Interface(enum Command command, uint8_t *data_bytes, uint8_t size)
{
	nrf24l01_SetPin(PIN_CSN, LOW);

	uint8_t status = nrf24l01_WriteSPI(&command, 1);

	if (size)
	{
		if ((command & 0b01100000) == WRITE) nrf24l01_WriteSPI(data_bytes, size);
		else nrf24l01_ReadSPI(data_bytes, size);
	}

	nrf24l01_SetPin(PIN_CSN, HIGH);

	return status;
}

uint8_t nrf24l01_WriteSPI(uint8_t *tx_buff, uint8_t size)
{
	uint8_t rx_buff[size];
	HAL_SPI_TransmitReceive(nrf24l01_spi, tx_buff, rx_buff, size, HAL_MAX_DELAY);
	return rx_buff[0];
}

void nrf24l01_ReadSPI(uint8_t *rx_buff, uint8_t size)
{
	uint8_t tx_buff[size];
	for (uint8_t i = 0; i < size; ++i)
	{
		tx_buff[i] = CMD_NOP;
	}
	HAL_SPI_TransmitReceive(nrf24l01_spi, tx_buff, rx_buff, size, HAL_MAX_DELAY);
}

void nrf24l01_SetPin(enum Pin pin, enum PinState state)
{
	HAL_GPIO_WritePin(GPIOB, pin, state);
}
