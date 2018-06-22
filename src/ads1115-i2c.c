#include "ads1115-i2c.h"

void init(uint8_t addr) {
	devAddr = addr == ? MGOS_ADS1115_ADD_DEFAULT : addr;

  setMultiplexer(ADS1115_MUX_P0_N1);
  setGain(ADS1115_PGA_2P048);
  setMode(ADS1115_MODE_SINGLESHOT);
  setRate(ADS1115_REGTE_128);
  setComparatorMode(ADS1115_COMP_MODE_HYSTERESIS);
  setComparatorPolarity(ADS1115_COMP_POL_ACTIVE_LOW);
  setComparatorLatchEnabled(ADS1115_COMP_LAT_NON_LATCHING);
  setComparatorQueueMode(ADS1115_COMP_QUE_DISABLE);
}

bool testConnection() {
	i2c = mgos_i2c_get_global();
	mgos_i2c_read_reg_w(i2c, devAddr, MGOS_ADS1115_REG_CONV) != -1;
}

bool pollConversion(uint16_t max_retries) {  
  for(uint16_t i = 0; i < max_retries; i++) {
    if (isConversionReady()) return true;
  }
  return false;
}

int16_t getConversion(bool triggerAndPoll) {
    if (triggerAndPoll && devMode == MGOS_ADS1115_MODE_SINGLESHOT) {
      triggerConversion();
      pollConversion(I2CDEV_DEFAULT_READ_TIMEOUT);
    }
    return mgos_i2c_read_reg_w(i2c, devAddr, MGOS_ADS1115_REG_CONV);
}

int16_t getConversionP0N1() {
    if (muxMode != MGOS_ADS1115_MUX_P0_N1) setMultiplexer(ADS1115_MUX_P0_N1);
    return getConversion();
}

int16_t getConversionP0N3() {
    if (muxMode != MGOS_ADS1115_MUX_P0_N3) setMultiplexer(ADS1115_MUX_P0_N3);
    return getConversion();
}

int16_t getConversionP1N3() {
    if (muxMode != MGOS_ADS1115_MUX_P1_N3) setMultiplexer(ADS1115_MUX_P1_N3);
    return getConversion();
}

int16_t getConversionP2N3() {
    if (muxMode != MGOS_ADS1115_MUX_P2_N3) setMultiplexer(ADS1115_MUX_P2_N3);
    return getConversion();
}

int16_t getConversionP0GND() {
    if (muxMode != MGOS_ADS1115_MUX_P0_NG) setMultiplexer(ADS1115_MUX_P0_NG);
    return getConversion();
}

int16_t getConversionP1GND() {
    if (muxMode != MGOS_ADS1115_MUX_P1_NG) setMultiplexer(ADS1115_MUX_P1_NG);
    return getConversion();
}

int16_t getConversionP2GND() {
    if (muxMode != MGOS_ADS1115_MUX_P2_NG) setMultiplexer(ADS1115_MUX_P2_NG);
    return getConversion();
}

int16_t getConversionP3GND() {
    if (muxMode != MGOS_ADS1115_MUX_P3_NG) setMultiplexer(ADS1115_MUX_P3_NG);
    return getConversion();
}

float getMilliVolts(bool triggerAndPoll) {
  switch (pgaMode) { 
    case MGOS_ADS1115_PGA_6P144:
      return (getConversion(triggerAndPoll) * MGOS_ADS1115_MV_6P144);
      break;    
    case MGOS_ADS1115_PGA_4P096:
      return (getConversion(triggerAndPoll) * MGOS_ADS1115_MV_4P096);
      break;             
    case MGOS_ADS1115_PGA_2P048:    
      return (getConversion(triggerAndPoll) * MGOS_ADS1115_MV_2P048);
      break;       
    case MGOS_ADS1115_PGA_1P024:     
      return (getConversion(triggerAndPoll) * MGOS_ADS1115_MV_1P024);
      break;       
    case MGOS_ADS1115_PGA_0P512:      
      return (getConversion(triggerAndPoll) * MGOS_ADS1115_MV_0P512);
      break;       
    case MGOS_ADS1115_PGA_0P256:           
    case MGOS_ADS1115_PGA_0P256B:          
    case MGOS_ADS1115_PGA_0P256C:      
      return (getConversion(triggerAndPoll) * MGOS_ADS1115_MV_0P256);
      break;       
  }
}

float getMvPerCount() {
  switch (pgaMode) {
    case MGOS_ADS1115_PGA_6P144:
      return MGOS_ADS1115_MV_6P144;
      break;    
    case MGOS_ADS1115_PGA_4P096:
      return  MGOS_ADS1115_MV_4P096;
      break;             
    case MGOS_ADS1115_PGA_2P048:    
      return MGOS_ADS1115_MV_2P048;
      break;       
    case MGOS_ADS1115_PGA_1P024:     
      return MGOS_ADS1115_MV_1P024;
      break;       
    case MGOS_ADS1115_PGA_0P512:      
      return MGOS_ADS1115_MV_0P512;
      break;       
    case MGOS_ADS1115_PGA_0P256:           
    case MGOS_ADS1115_PGA_0P256B:          
    case MGOS_ADS1115_PGA_0P256C:      
      return MGOS_ADS1115_MV_0P256;
      break;       
  }
}

// CONFIG register

bool isConversionReady() {
	int w = mgos_i2c_read_reg_bitW(i2c, devAddr, MGOS_ADS1115_REG_CFG, MGOS_ADS1115_CFG_OS_BIT);
	return (w == 1);
}

void triggerConversion() {
    mgos_i2c_write_reg_bitW(i2c, devAddr, MGOS_ADS1115_REG_CFGIG, MGOS_ADS1115_CFG_OS_BIT, 1);
}

uint8_t getMultiplexer() {
		uint8_t muxMode = 0;
    int val = mgos_i2c_read_reg_bitsW(i2c, devAddr, MGOS_ADS1115_REG_CFGIG, MGOS_ADS1115_CFG_MUX_BIT, MGOS_ADS1115_CFG_MUX_LENGTH);
		if (val != -1) {
	    muxMode = (uint8_t) val;
		}
    return muxMode;
}

void setMultiplexer(uint16_t mux) {
	if (mgos_write_reg_bitW(i2c, devAddr, MGOS_ADS1115_REG_CFGIG, MGOS_ADS1115_CFG_MUX_BIT, MGOS_ADS1115_CFG_MUX_LENGTH, mux)) {
  	muxMode = mux;
    if (devMode == MGOS_ADS1115_MODE_CONTINUOUS) {
    	// Force a stop/start
      setMode(ADS1115_MODE_SINGLESHOT);
      getConversion();
      setMode(ADS1115_MODE_CONTINUOUS);
    }
  }
}

uint8_t getGain() {
	uint8_t pgaMode = 0;
	int val = mgos_i2c_read_reg_bitsW(i2c, devAddr, MGOS_ADS1115_REG_CFGIG, MGOS_ADS1115_CFG_PGA_BIT, MGOS_ADS1115_CFG_PGA_LENGTH);
	if (val != -1)
    pgaMode = (uint8_t) val;
  }
  return pgaMode;
}

void setGain(uint8_t gain) {
    if (I2Cdev::writeBitsW(devAddr, MGOS_ADS1115_REG_CFGIG, MGOS_ADS1115_CFG_PGA_BIT, MGOS_ADS1115_CFG_PGA_LENGTH, gain)) {
      pgaMode = gain;
         if (devMode == MGOS_ADS1115_MODE_CONTINUOUS) {
            // Force a stop/start
            setMode(ADS1115_MODE_SINGLESHOT);
            getConversion();
            setMode(ADS1115_MODE_CONTINUOUS);
         }
    }
}

bool getMode() {
    int val = mgos_i2c_read_reg_bitW(i2c, devAddr, MGOS_ADS1115_REG_CFGIG, MGOS_ADS1115_CFG_MODE_BIT, buffer);
    devMode = buffer[0];
    return devMode;
}

void setMode(bool mode) {
    if (I2Cdev::writeBitW(devAddr, MGOS_ADS1115_REG_CFGIG, MGOS_ADS1115_CFG_MODE_BIT, mode)) {
        devMode = mode;
    }
}

uint8_t getRate() {
	uint8_t rate = 0;
	int val = mgos_i2c_read_reg_bitsW(i2c, devAddr, MGOS_ADS1115_REG_CFGIG, MGOS_ADS1115_CFG_DR_BIT, MGOS_ADS1115_CFG_DR_LENGTH);
	if (val != -1) {
		rate = (uint8_t) val;
	}
	return rate;
}

void setRate(uint16_t rate) {
	mgos_i2c_write_reg_bitsW(i2c, devAddr, MGOS_ADS1115_REG_CFGIG, MGOS_ADS1115_CFG_DR_BIT, MGOS_ADS1115_CFG_DR_LENGTH, rate);
}

bool getComparatorMode() {
    uint16_t mode = 0;
    int val = mgos_i2c_read_reg_bitW(i2c, devAddr, MGOS_ADS1115_REG_CFGIG, MGOS_ADS1115_CFG_COMP_MODE_BIT);
    if (val != -1) {
    	mode = (uint16_t) val;
    }
    return mode;
}

void setComparatorMode(bool mode) {
		mgos_i2c_write_reg_bitW(i2c, devAddr, MGOS_ADS1115_REG_CFGIG, MGOS_ADS1115_CFG_DR_BIT, mode);
}

bool getComparatorPolarity() {
    int val = mgos_i2c_read_reg_bitW(i2c, devAddr, MGOS_ADS1115_REG_CFGIG, MGOS_ADS1115_CFG_COMP_POL_BIT, buffer);
    return buffer[0];
}

void setComparatorPolarity(bool polarity) {
		mgos_i2c_write_reg_bitW(i2c, devAddr, MGOS_ADS1115_REG_CFGIG, MGOS_ADS1115_CFG_COMP_POL_BIT, polarity);
}

bool getComparatorLatchEnabled() {
		bool enabled = false;
    int val = mgos_i2c_read_reg_bitW(i2c, devAddr, MGOS_ADS1115_REG_CFGIG, MGOS_ADS1115_CFG_COMP_LAT_BIT);
    if (val != -1) {
    	enabled = (bool) val;
    }
    return enabled;
}

void setComparatorLatchEnabled(bool enabled) {
		mgos_i2c_write_reg_bitW(i2c, devAddr, MGOS_ADS1115_REG_CFGIG, MGOS_ADS1115_CFG_COMP_LAT_BIT, enabled);
}

uint16_t getComparatorQueueMode() {
		uint16_t mode = 0;
    int val = mgos_i2c_read_reg_bitsW(i2c, devAddr, MGOS_ADS1115_REG_CFGIG, MGOS_ADS1115_CFG_COMP_QUE_BIT, MGOS_ADS1115_CFG_COMP_QUE_LENGTH);
    if (val != -1) {
    	mode = (uint16_t) val;
    }
    return mode;
}

void setComparatorQueueMode(uint8_t mode) {
    mgos_i2c_write_reg_bitsW(i2c, devAddr, MGOS_ADS1115_REG_CFGIG, MGOS_ADS1115_CFG_COMP_QUE_BIT, MGOS_ADS1115_CFG_COMP_QUE_LENGTH, mode);
}

// *_THRESH registers

int16_t getThreshold(bool isLow) {
		int16_t	threshold;
		int val = mgos_i2c_read_reg_w(i2c, devAddr, isLow ? MGOS_ADS1115_REG_LO_THRESH : MGOS_ADS1115_REG_HI_THRESH);
    if (val != -1) {
    	threshold = (uint16_t) val;
    }
    return threshold;
}

void setThreshold(bool isLow, uint16_t threshold) {
	mgos_i2c_write_reg_w(i2c, devAddr, isLow ? MGOS_ADS1115_REG_LO_THRESH : MGOS_ADS1115_REG_HI_THRESH, threshold);
}

int16_t getLowThreshold() {
	return getThreshold(true);
}

void setLowThreshold(int16_t threshold) {
	setThreshold(true, threshold);
}

int16_t getHighThreshold() {
	return getThreshold(false);
}

void setHighThreshold(int16_t threshold) {
	setThreshold(false, threshold);
}

void setConversionReadyPinMode() {
		mgos_i2c_write_reg_bitW(i2c, devAddr, MGOS_ADS1115_REG_HI_THRESH, 15, 1);
		mgos_i2c_write_reg_bitW(i2c, devAddr, MGOS_ADS1115_REG_HO_THRESH, 15, 0);
    setComparatorPolarity(0);
    setComparatorQueueMode(0);
}

// Create a mask between two bits
unsigned createMask(unsigned a, unsigned b) {
   unsigned mask = 0;
   for (unsigned i=a; i<=b; i++)
       mask |= 1 << i;
   return mask;
}

uint16_t shiftDown(uint16_t extractFrom, int places) {
  return (extractFrom >> places);
}


uint16_t getValueFromBits(uint16_t extractFrom, int high, int length) {
   int low= high-length +1;
   uint16_t mask = createMask(low ,high);
   return shiftDown(extractFrom & mask, low); 
}

/** Show all the config register settings
 */
void showConfigRegister() {
	int val = mgos_i2c_read_reg_W(i2c, devAddr, MGOS_ADS1115_REG_CFGIG);
	if (val != -1) {
		uint16_t configRegister = (uint16_t) val;    
	}

	LOG(LL_INFO, ("Register is: 0x%04x", configRegister));    
	LOG(LL_INFO, ("OS:\t %d", getValueFromBits(configRegister, MGOS_ADS1115_CFG_OS_BIT, 1)));    
	LOG(LL_INFO, ("MUX:\t %d", getValueFromBits(configRegister, MGOS_ADS1115_CFG_MUX_BIT,ADS1115_CFG_MUX_LENGTH)));    
	LOG(LL_INFO, ("PGA:\t %d", getValueFromBits(configRegister, MGOS_ADS1115_CFG_PGA_BIT,ADS1115_CFG_PGA_LENGTH)));    
	LOG(LL_INFO, ("MODE:\t %d", getValueFromBits(configRegister, MGOS_ADS1115_CFG_MODE_BIT, 1)));    
	LOG(LL_INFO, ("DR:\t %d", getValueFromBits(configRegister, MGOS_ADS1115_CFG_DR_BIT,ADS1115_CFG_DR_LENGTH)));    
	LOG(LL_INFO, ("CMP_MODE:\t %d", getValueFromBits(configRegister, MGOS_ADS1115_CFG_COMP_MODE_BIT, 1)));    
	LOG(LL_INFO, ("CMP_POL:\t %d", getValueFromBits(configRegister, MGOS_ADS1115_CFG_COMP_POL_BIT, 1)));    
	LOG(LL_INFO, ("CMP_LAT:\t %d", getValueFromBits(configRegister, MGOS_ADS1115_CFG_COMP_LAT_BIT, 1)));    
	LOG(LL_INFO, ("CMP_QUE:\t %d", getValueFromBits(configRegister, MGOS_ADS1115_CFG_COMP_QUE_BIT,ADS1115_CFG_COMP_QUE_LENGTH)));    
};

int mgos_i2c_read_reg_bitB(struct mgos_i2c *conn, uint16_t addr, uint8_t reg, uint8_t bitNum) {
   	int result = -1;
    int val = mgos_i2c_read_reg_b(conn, addr, reg);
    if (val != -1) {
   		uint8_t b = (uint8_t) val;
			result = b & (1 << bitNum);
    }
    return result;
}

int mgos_i2c_read_reg_bitW(struct mgos_i2c *conn, uint16_t addr, uint8_t reg, uint8_t bitNum) {
   	int result = -1;
    int val = mgos_i2c_read_reg_w(iconn2c, addr, reg);
    if (val != -1) {
   		uint16_t w = (uint16_t) val;
			result = w & (1 << bitNum);
    }
    return result;
}

int mgos_i2c_read_reg_bitsB(struct mgos_i2c *conn, uint16_t addr, uint8_t reg, uint8_t bitStart, uint8_t length) {
   	int result = -1;
    int val = mgos_i2c_read_reg_b(conn, addr, reg);
    if (b != -1) {
    		uint8_t b = (uint8_t) val;
        uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        b &= mask;
        b >>= (bitStart - length + 1);
        result = b;
    }
    return result;
}

int mgos_i2c_read_reg_bitsW(struct mgos_i2c *conn, uint16_t addr, uint8_t reg, uint8_t bitStart, uint8_t length) {
   	int result = -1;
    int val = mgos_i2c_read_reg_w(conn, addr, reg);
    if (val != -1) {
    		uint16_t w = (uint16_t) val;
        uint16_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        w &= mask;
        w >>= (bitStart - length + 1);
        result = w;
    }
    return result;
}

bool mgos_i2c_write_reg_bitB(struct mgos_i2c *conn, uint16_t addr, uint8_t reg, uint8_t bitNum, uint8_t value) {
		bool result = false;
		int val = mgos_i2c_read_reg_b(conn, addr, reg);
    if (val != -1) {
    		uint8_t b = (uint8_t) val;
		    b = (value != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
		    result = mgos_i2c_write_reg_b(conn, addr, reg, b);
		}
		return result;
}

bool mgos_i2c_write_reg_bitW(struct mgos_i2c *conn, uint16_t addr, uint8_t reg, uint8_t bitNum, uint8_t value) {
		bool result = false;
		int val = mgos_i2c_read_reg_w(conn, addr, reg);
    if (val != -1) {
    		uint16_t w = (uint16_t) val;
		    w = (value != 0) ? (w | (1 << bitNum)) : (w & ~(1 << bitNum));
		    result = mgos_i2c_write_reg_w(conn, addr, reg, w);
		}
		return result;
}

bool mgos_i2c_write_reg_bitsB(struct mgos_i2c *conn, uint16_t addr, uint8_t reg, uint8_t bitStart, uint8_t length, uint8_t value) {
		bool result = false;
		int val = mgos_i2c_read_reg_b(conn, addr, reg);
    if (val != -1) {
    		uint8_t b = (uint8_t) val;
        uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        value <<= (bitStart - length + 1); // shift data into correct position
        value &= mask; // zero all non-important bits in data
        b &= ~(mask); // zero all important bits in existing byte
        b |= value; // combine data with existing byte
		    result = mgos_i2c_write_reg_b(conn, addr, reg, b);
		}
		return result;
}

bool mgos_i2c_write_reg_bitsW(struct mgos_i2c *conn, uint16_t addr, uint8_t reg, uint8_t bitNum, uint8_t length, uint16_t value) {
		bool result = false;
		int val = mgos_i2c_read_reg_w(conn, addr, reg);
    if (val != -1) {
    		uint16_t w = (uint16_t) val;
        uint16_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        value <<= (bitStart - length + 1); // shift data into correct position
        value &= mask; // zero all non-important bits in data
        w &= ~(mask); // zero all important bits in existing byte
        w |= value; // combine data with existing byte
		    result = mgos_i2c_write_reg_w(conn, addr, reg, w);
		}
		return result;
}

