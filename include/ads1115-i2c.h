#ifndef __MGOS_ADS1115_I2C_H
#define __MGOS_ADS1115_I2C_H

#include "mgos.h"
#include "mgos_app.h"
#include "mgos_i2c.h"

#define MGOS_I2CDEV_DEFAULT_READ_TIMEOUT 1000

#define MGOS_ADS1115_ADDR_PIN_GND    0x48 // address pin low (GND)
#define MGOS_ADS1115_ADDR_PIN_VDD    0x49 // address pin high (VCC)
#define MGOS_ADS1115_ADDR_PIN_SDA    0x4A // address pin tied to SDA pin
#define MGOS_ADS1115_ADDR_PIN_SCL    0x4B // address pin tied to SCL pin
#define MGOS_ADS1115_ADD_DEFAULT     0x48

#define MGOS_ADS1115_REG_CONV       0x00
#define MGOS_ADS1115_REG_CFG        0x01
#define MGOS_ADS1115_REG_LO_THR     0x02
#define MGOS_ADS1115_REG_HI_THR     0x03

#define MGOS_ADS1115_CFG_OS_BIT          15
#define MGOS_ADS1115_CFG_MUX_BIT         14
#define MGOS_ADS1115_CFG_MUX_LENGTH      3
#define MGOS_ADS1115_CFG_PGA_BIT         11
#define MGOS_ADS1115_CFG_PGA_LENGTH      3
#define MGOS_ADS1115_CFG_MODE_BIT        8
#define MGOS_ADS1115_CFG_DR_BIT          7
#define MGOS_ADS1115_CFG_DR_LENGTH       3
#define MGOS_ADS1115_CFG_COMP_MODE_BIT   4
#define MGOS_ADS1115_CFG_COMP_POL_BIT    3
#define MGOS_ADS1115_CFG_COMP_LAT_BIT    2
#define MGOS_ADS1115_CFG_COMP_QUE_BIT    1
#define MGOS_ADS1115_CFG_COMP_QUE_LENGTH 2

#define MGOS_ADS1115_MUX_P0_N1           0x00 // default
#define MGOS_ADS1115_MUX_P0_N3           0x01
#define MGOS_ADS1115_MUX_P1_N3           0x02
#define MGOS_ADS1115_MUX_P2_N3           0x03
#define MGOS_ADS1115_MUX_P0_NG           0x04
#define MGOS_ADS1115_MUX_P1_NG           0x05
#define MGOS_ADS1115_MUX_P2_NG           0x06
#define MGOS_ADS1115_MUX_P3_NG           0x07

#define MGOS_ADS1115_PGA_6P144           0x00
#define MGOS_ADS1115_PGA_4P096           0x01
#define MGOS_ADS1115_PGA_2P048           0x02 // default
#define MGOS_ADS1115_PGA_1P024           0x03
#define MGOS_ADS1115_PGA_0P512           0x04
#define MGOS_ADS1115_PGA_0P256           0x05
#define MGOS_ADS1115_PGA_0P256B          0x06
#define MGOS_ADS1115_PGA_0P256C          0x07

#define MGOS_ADS1115_MV_6P144            0.187500
#define MGOS_ADS1115_MV_4P096            0.125000
#define MGOS_ADS1115_MV_2P048            0.062500 // default
#define MGOS_ADS1115_MV_1P024            0.031250
#define MGOS_ADS1115_MV_0P512            0.015625
#define MGOS_ADS1115_MV_0P256            0.007813
#define MGOS_ADS1115_MV_0P256B           0.007813 
#define MGOS_ADS1115_MV_0P256C           0.007813

#define MGOS_ADS1115_MODE_CONTINUOUS     0x00
#define MGOS_ADS1115_MODE_SINGLESHOT     0x01 // default

#define MGOS_ADS1115_REGTE_8              0x00
#define MGOS_ADS1115_REGTE_16             0x01
#define MGOS_ADS1115_REGTE_32             0x02
#define MGOS_ADS1115_REGTE_64             0x03
#define MGOS_ADS1115_REGTE_128            0x04 // default
#define MGOS_ADS1115_REGTE_250            0x05
#define MGOS_ADS1115_REGTE_475            0x06
#define MGOS_ADS1115_REGTE_860            0x07

#define MGOS_ADS1115_COMP_MODE_HYSTERESIS    0x00 // default
#define MGOS_ADS1115_COMP_MODE_WINDOW        0x01

#define MGOS_ADS1115_COMP_POL_ACTIVE_LOW     0x00 // default
#define MGOS_ADS1115_COMP_POL_ACTIVE_HIGH    0x01

#define MGOS_ADS1115_COMP_LAT_NON_LATCHING   0x00 // default
#define MGOS_ADS1115_COMP_LAT_LATCHING       0x01

#define MGOS_ADS1115_COMP_QUE_ASSERT1    0x00
#define MGOS_ADS1115_COMP_QUE_ASSERT2    0x01
#define MGOS_ADS1115_COMP_QUE_ASSERT4    0x02
#define MGOS_ADS1115_COMP_QUE_DISABLE    0x03 // default

uint8_t devAddr;
uint16_t buffer[2];
bool    devMode;
uint8_t muxMode;
uint8_t pgaMode;
struct mgos_i2c *i2c;

void init(uint8_t addr);
bool testConnection();

// SINGLE SHOT utilities
bool pollConversion(uint16_t max_retries);
void triggerConversion();

// Read the current CONVERSION register
int16_t getConversion(bool triggerAndPoll);

// Differential
int16_t getConversionP0N1();
int16_t getConversionP0N3();
int16_t getConversionP1N3();
int16_t getConversionP2N3();

// Single-ended
int16_t getConversionP0GND();
int16_t getConversionP1GND();
int16_t getConversionP2GND();
int16_t getConversionP3GND();

// Utility
float getMilliVolts(bool triggerAndPoll);
float getMvPerCount();

// CONFIG register
bool isConversionReady();
uint8_t getMultiplexer();
void setMultiplexer(uint16_t mux);
uint8_t getGain();
void setGain(uint8_t gain);
bool getMode();
void setMode(bool mode);
uint8_t getRate();
void setRate(uint16_t rate);
bool getComparatorMode();
void setComparatorMode(bool mode);
bool getComparatorPolarity();
void setComparatorPolarity(bool polarity);
bool getComparatorLatchEnabled();
void setComparatorLatchEnabled(bool enabled);
uint16_t getComparatorQueueMode();
void setComparatorQueueMode(uint8_t mode);
void setConversionReadyPinMode();

// *_THR registers
int16_t getThreshold(bool isLow);
void setThreshold(bool isLow, uint16_t threshold);

int16_t getLowThreshold();
void setLowThreshold(int16_t THRold);
int16_t getHighThreshold();
void setHighThreshold(int16_t THRold);

// DEBUG
void showConfigRegister();

// to lib
int mgos_i2c_read_reg_bit_b(struct mgos_i2c *conn, uint16_t addr, uint8_t reg, uint8_t bitNum);

int mgos_i2c_read_reg_bit_w(struct mgos_i2c *conn, uint16_t addr, uint8_t reg, uint8_t bitNum);

int mgos_i2c_read_reg_bits_b(struct mgos_i2c *conn, uint16_t addr, uint8_t reg, uint8_t bitStart, uint8_t length);

int mgos_i2c_read_reg_bits_w(struct mgos_i2c *conn, uint16_t addr, uint8_t reg, uint8_t bitStart, uint8_t length);

bool mgos_i2c_write_reg_bit_b(struct mgos_i2c *conn, uint16_t addr, uint8_t reg, uint8_t bitNum, uint8_t value);

bool mgos_i2c_write_reg_bit_w(struct mgos_i2c *conn, uint16_t addr, uint8_t reg, uint8_t bitNum, uint8_t value);

bool mgos_i2c_write_reg_bits_b(struct mgos_i2c *conn, uint16_t addr, uint8_t reg, uint8_t bitStart, uint8_t length, uint8_t value);

bool mgos_i2c_write_reg_bits_w(struct mgos_i2c *conn, uint16_t addr, uint8_t reg, uint8_t bitNum, uint8_t length, uint16_t value);


#endif // __MGOS_ADS1115_I2C