#ifndef _STUB_ENC_H_
#define _STUB_ENC_H_

#include <stdint.h>

#define STUB_ENC_RX_BUFFER_LEN 1518
#define STUB_ENC_TX_BUFFER_LEN 1518

extern uint8_t stub_enc_rx_buffer[STUB_ENC_RX_BUFFER_LEN];
extern uint8_t stub_enc_tx_buffer[STUB_ENC_TX_BUFFER_LEN];
extern uint8_t stub_enc_tx_waiting;

void stub_enc_read(uint8_t *data, uint16_t len);
void stub_enc_write_frame(uint16_t len);


#endif /* _STUB_ENC_H */