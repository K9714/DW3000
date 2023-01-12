#include <Arduino.h>
#ifndef AU_DW3000_H
#define AU_DW3000_H

// Arduino Uno - DWM3000 Shield Pin Map
#define SPI_CLK   13
#define SPI_MISO  12
#define SPI_MOSI  11
#define SPI_CSn   10
#define IRQ       8
#define RSTn      7

#define LED_RX    4
#define LED_TX    3

#define SPI_POL   1     // Arduino <- TX
#define SPI_PHA   0     // Arduino -> RX


// DW3000 
#ifndef FALSE
#define FALSE     0
#endif

#ifndef TRUE
#define TRUE      1
#endif

#define JUNK      0x00

// FAST
#define FAST_CMD_TXRXOFF  0x0
#define FAST_CMD_TX       0x1
#define FAST_CMD_RX       0x2


// Register
#define GEN_CFG_AES_1 0x00
#define GEN_CFG_AES_2 0x01
#define RX_BUFFER_0   0x12
#define RX_BUFFER_1   0x13
#define TX_BUFFER     0x14


// Sub Address
#define DEV_ID      0x00
#define DEV_ID_LEN  4

#define EUI         0x04
#define EUI_LEN     5

#define OUI         0x04
#define OUI_LEN     8

#define SYS_CFG     0x10
#define SYS_CFG_LEN 4
  // SYS_CFG
  #define FEEN        0x01
  #define DIS_FCS_TX  0x02
  #define DIS_FCE     0x04
  #define DIS_DRXB    0x08
  #define PHR_MODE    0x10
  #define PHR_6M8     0x20
  #define SPI_CRCEN   0x40
  #define CIA_IPATOV  0x80

  #define CIA_STS     0x01
  #define RXWTOE      0x02
  #define RXAUTR      0x04
  #define AUTO_ACK    0x08
  #define CP_SPC_SP1  0x10
  #define CP_SPC_SP2  0x20
  #define CP_SPC_SP3  0x30
  #define CP_SDC      0x80

  #define PDOA_MODE_1 0x01
  #define PDOA_MODE_2 0x02
  #define PDOA_MODE_3 0x03
  #define FAST_AAT    0x04

#define SPI_RD_CRC      0x18
#define SPI_RD_CRC_LEN  1

#define TX_FCTRL      0x24
#define TX_FCTRL_LEN  6
  #define TX_FCTRL_TXPSR_32   0x4
  #define TX_FCTRL_TXPSR_64   0x1
  #define TX_FCTRL_TXPSR_128  0x5
  #define TX_FCTRL_TXPSR_256  0x9
  #define TX_FCTRL_TXPSR_512  0xD
  #define TX_FCTRL_TXPSR_1024 0x2
  #define TX_FCTRL_TXPSR_1536 0x6
  #define TX_FCTRL_TXPSR_2048 0xA
  #define TX_FCTRL_TXPSR_4096 0x3

#define SYS_STATUS      0x44
#define SYS_STATUS_LEN  6
  #define SYS_STATUS_TXFRS_MASK 0x80U
  #define SYS_STATUS_RXFCG_MASK 0x4000U


#define RX_FINFO      0x4C
#define RX_FINFO_LEN  4
  #define RX_FINFO_RXFLEN_MASK  0x3FF







// SPI Transaction Mask
#define READ      0x00
#define WRITE     0x01
#define FAST_CMD  0x80

#define ADDR_8    0x00
#define ADDR_16   0x02

#define ADDR_MASK1 0x80
#define ADDR_MASK2 0x3F

#define M1        0x4000
#define M0        0x8000


typedef unsigned char uint8_t;
//typedef unsigned short uint16_t;
//typedef unsigned int uint32_t;
//typedef uint8_t bool;


// DW3000 Config
typedef enum {
  FAST,
  SHORT,
  FULL,
  MASK_01,
  MASK_10,
  MASK_11,
} SPI_WRITE_TYPE;

typedef struct {
  uint8_t data[SYS_CFG_LEN];
} DW3000_SYS_CFG;

typedef struct {
  uint16_t tx_flen;     // 10bit, payload length
  uint8_t tx_br;        // 1bit, bit rate (0: 850Kb/s, 1: 6.81Mb/s)
  uint8_t tr;           // 1bit, 0
  uint8_t tx_psr;       // 4bit, PDF pp. 86
  uint16_t txb_offset;  // 10bit, 0(zero ranging)
  uint8_t fine_plen;    // 8bit, 0
} DW3000_TX_FCTRL;


void DW3000_readData(uint8_t reg, uint8_t offset, uint8_t len, uint8_t *data);
void DW3000_writeData(uint8_t reg, uint8_t offset, uint8_t len, uint8_t *data, SPI_WRITE_TYPE type);

void DW3000_setConfig(DW3000_SYS_CFG* sys_cfg);
void DW3000_getConfig();
void DW3000_init(DW3000_SYS_CFG* sys_cfg);

void DW3000_getDevID();
void DW3000_getEUI();
void DW3000_getOUI();
void DW3000_clearIDLE();
void DW3000_startTX();
void DW3000_startRX();
void DW3000_writeTXFCTRL(DW3000_TX_FCTRL *txfctrl);
void DW3000_writeTXData(uint16_t len, uint8_t* data, uint16_t bufferOffset);
uint32_t DW3000_getSysStatus();
void DW3000_setSysStatus(uint32_t data);
uint16_t DW3000_getRXFrameLength();
void DW3000_getRXData(uint16_t len, uint8_t* data, uint16_t bufferOffset);

#endif