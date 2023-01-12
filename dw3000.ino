#include "AU_DW3000.h"

DW3000_SYS_CFG sys_cfg = {
  { DIS_DRXB | PHR_6M8 | CIA_IPATOV | SPI_CRCEN,
    CIA_STS | RXWTOE,
    PDOA_MODE_3,
    0
  }
};

DW3000_TX_FCTRL txfctrl = { 0, 1, 0, TX_FCTRL_TXPSR_64, 0, 0 };

#define RUN_TX_MODE
//#define RUN_RX_MODE

static uint8_t tx_msg[] = {0xC5, 0, 'D', 'E', 'C', 'A', 'W', 'A', 'V', 'E'};
static uint8_t rx_buffer[127] = {0,};

void setup() {
  Serial.begin(115200);

  DW3000_getConfig();
  DW3000_init(&sys_cfg);
  DW3000_getDevID();
  DW3000_getEUI();
  DW3000_getOUI();
  DW3000_getConfig();

  
  delay(1000);
}

void loop() {
#ifdef RUN_TX_MODE
  // Set txfctrl tx_flen
  txfctrl.tx_flen = sizeof(tx_msg);

  DW3000_writeTXData(sizeof(tx_msg), tx_msg, 0);
  DW3000_writeTXFCTRL(&txfctrl);
  DW3000_startTX();

  while(!(DW3000_getSysStatus() & SYS_STATUS_TXFRS_MASK))
  { Serial.println(DW3000_getSysStatus()); };

  

  DW3000_setSysStatus(SYS_STATUS_TXFRS_MASK);

  Serial.println("TX Frame Sent.");

  delay(500);
  
  tx_msg[1]++;
#endif
#ifdef RUN_RX_MODE
  uint32_t status_reg = 0;
  uint16_t frame_len = 0;
  DW3000_startRX();

  while(!((status_reg = DW3000_getSysStatus()) & SYS_STATUS_RXFCG_MASK)) {
    Serial.println(status_reg & SYS_STATUS_RXFCG_MASK);
  };

  if (status_reg & SYS_STATUS_RXFCG_MASK) {
    frame_len = DW3000_getRXFrameLength();
    Serial.println(frame_len);
    if (0 < frame_len <= 127) {
      DW3000_getRXData(frame_len-2, rx_buffer, 0);
    }
    DW3000_setSysStatus(SYS_STATUS_RXFCG_MASK);

    Serial.println("Frame Received!");
    char msg[127] = {0, };
    sprintf(msg, "%s", rx_buffer);
    Serial.println(msg);
  }
  
#endif
}
