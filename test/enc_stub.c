#include <stdint.h>
#include "enc_stub.h"

uint8_t stub_enc_rx_buffer[STUB_ENC_RX_BUFFER_LEN];
uint8_t stub_enc_tx_buffer[STUB_ENC_TX_BUFFER_LEN];

uint8_t stub_enc_tx_waiting;

void stub_enc_read(uint8_t *data, uint16_t len) {
    for (uint8_t i = 0; i < len; i++)
        stub_enc_rx_buffer[i] = data[i];
}

void stub_enc_write_frame(uint16_t len) {
    stub_enc_tx_waiting = 0;
    return;
}