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
#define BLE_READPACKET_TIMEOUT         500 
//----------------------------------------------------
uint8_t readPacket(Adafruit_BLE *ble, uint16_t timeout);
float parsefloat(uint8_t *buffer);
void printHex(const uint8_t * data, const uint32_t numBytes);
//----------------------------------------------------
// the packet buffer
extern uint8_t packetbuffer[];
//----------------------------------------------------

Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

boolean isConnected = false;

void setupBLE(){
  
  if ( !ble.begin(VERBOSE_MODE) ){
    DEBUG_PRINT_LN(F("Failed to initialize BLE"));
  }

  ble.verbose(false);
  ble.setMode(BLUEFRUIT_MODE_DATA);
  DEBUG_PRINT_LN(F("setup ble"));
  
}

void onConnect(){
  DEBUG_PRINT_LN(F("Connected"));
  DEBUG_PRINT_LN( F("Switching to DATA mode!") );
  ble.setMode(BLUEFRUIT_MODE_DATA);
}

void onDisconnect(){
  DEBUG_PRINT_LN(F("Disconnected"));
}

void checkForConnection(){
  if(ble.isConnected() && isConnected == false){
    isConnected = true;
    onConnect();
  }else if(!ble.isConnected() && isConnected == true){
    isConnected = false;
    onDisconnect();
  }
}

void checkForData(){
  if(isConnected){
    uint8_t len = readPacket(&ble, BLE_READPACKET_TIMEOUT);
    if (len == 0) return;

//     printHex(packetbuffer, len);
//     DEBUG_PRINT_LN(packetbuffer[1]);
     if(packetbuffer[1] == 'G'){
        float x, y, z;
        x = parsefloat(packetbuffer+2);
        y = parsefloat(packetbuffer+6);
        z = parsefloat(packetbuffer+10);
        
        Serial.print("Gyro\t");
        Serial.print(x); Serial.print('\t');
        Serial.print(y); Serial.print('\t');
        Serial.print(z); Serial.println();
     }
     if (packetbuffer[1] == 'C') {
      DEBUG_PRINT_LN( F("color sent") );
      uint8_t red = packetbuffer[2];
      uint8_t green = packetbuffer[3];
      uint8_t blue = packetbuffer[4];
      DEBUG_PRINT_LN(red);
      DEBUG_PRINT_LN(green);
      DEBUG_PRINT_LN(blue);

      showColor(0,red,green,blue);
     }
  }
}

void BLELoop(){
  checkForConnection();
  checkForData();
}

