/*******************************************************************************
* Copyright 2016 ROBOTIS CO., LTD.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#include <DynamixelShield.h>

#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_MEGA2560)
  #include <SoftwareSerial.h>
  SoftwareSerial soft_serial(7, 8); // DYNAMIXELShield UART RX/TX
  #define DEBUG_SERIAL soft_serial
#elif defined(ARDUINO_SAM_DUE) || defined(ARDUINO_SAM_ZERO)
  #define DEBUG_SERIAL SerialUSB    
#else
  #define DEBUG_SERIAL Serial
#endif

#define TIMEOUT 10    //default communication timeout 10ms
#define OPERATING_MODE_ADDR         11
#define OPERATING_MODE_ADDR_LEN     1
#define TORQUE_ENABLE_ADDR          64
#define TORQUE_ENABLE_ADDR_LEN      1
#define LED_ADDR                    65
#define LED_ADDR_LEN                1
#define GOAL_POSITION_ADDR          116
#define GOAL_POSITION_ADDR_LEN      4
#define PRESENT_POSITION_ADDR       132
#define PRESENT_POSITION_ADDR_LEN   4

uint8_t enable = 1;
uint8_t disable = 0;

const uint8_t DXL_ID = 1;
const float DXL_PROTOCOL_VERSION = 2.0;

uint8_t operatingMode = 3;
uint32_t goalPosition1 = 512;
uint32_t goalPosition2 = 2048;

DynamixelShield dxl;

void setup() {
  // put your setup code here, to run once:
  
  // Use UART port of DYNAMIXEL Shield to debug.
  DEBUG_SERIAL.begin(115200);   //Set debugging port baudrate to 115200bps
  while(!DEBUG_SERIAL);         //Wait until the serial port for terminal is opened
  
  // Set Port baudrate to 57600bps. This has to match with DYNAMIXEL baudrate.
  dxl.begin(57600);
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);

  // Turn off torque when configuring items in EEPROM area
  if(dxl.write(DXL_ID, TORQUE_ENABLE_ADDR, (uint8_t*)&disable , TORQUE_ENABLE_ADDR_LEN, TIMEOUT))
    DEBUG_SERIAL.println("Torque off");
  else
    DEBUG_SERIAL.println("Error : Torque off");

  // Set Operating Mode
  if(dxl.write(DXL_ID, OPERATING_MODE_ADDR, (uint8_t*)&operatingMode, OPERATING_MODE_ADDR_LEN, TIMEOUT))
    DEBUG_SERIAL.println("Set operating mode");
  else
    DEBUG_SERIAL.println("Error : Set operating mode");

  // Turn on torque
  if(dxl.write(DXL_ID, TORQUE_ENABLE_ADDR, (uint8_t*)&enable, TORQUE_ENABLE_ADDR_LEN, TIMEOUT))
    DEBUG_SERIAL.println("Torque on");
  else
    DEBUG_SERIAL.println("Error : Torque on");
}

void loop() {
  // put your main code here, to run repeatedly:

  // LED On
  DEBUG_SERIAL.println("LED ON");
  dxl.write(DXL_ID, LED_ADDR, (uint8_t*)&enable, LED_ADDR_LEN, TIMEOUT);
  delay(500);
  
  // Please refer to e-Manual(http://emanual.robotis.com/docs/en/parts/interface/dynamixel_shield/) for available range of value. 
  // Set Goal Position
  DEBUG_SERIAL.print("Goal Position : ");
  DEBUG_SERIAL.println(goalPosition1);
  dxl.write(DXL_ID, GOAL_POSITION_ADDR, (uint8_t*)&goalPosition1, GOAL_POSITION_ADDR_LEN, TIMEOUT);
  delay(1000);
  
  // LED Off
  DEBUG_SERIAL.println("LED OFF");
  dxl.write(DXL_ID, LED_ADDR, (uint8_t*)&disable, LED_ADDR_LEN, TIMEOUT);
  delay(500);

  // Set Goal Position
  DEBUG_SERIAL.print("Goal Position : ");
  DEBUG_SERIAL.println(goalPosition2);
  dxl.write(DXL_ID, GOAL_POSITION_ADDR, (uint8_t*)&goalPosition2, GOAL_POSITION_ADDR_LEN, TIMEOUT);
  delay(1000);
}
