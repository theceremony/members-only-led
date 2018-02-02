//----------------------------------------------------
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
//----------------------------------------------------
#define BUFSIZE                        128
#define VERBOSE_MODE                   true
//----------------------------------------------------
#define BLUEFRUIT_SPI_CS               8
#define BLUEFRUIT_SPI_IRQ              7
#define BLUEFRUIT_SPI_RST              4
//----------------------------------------------------



Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);


void setupBLE(){
  DEBUG_PRINT_LN("setup ble");
}

