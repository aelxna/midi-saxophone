#include <avr/io.h>
#include "gpio_i2c.h"


#define DISPLAYSDA PIN2_bm
#define DISPLAYSCL PIN3_bm
#define DISPLAY_ADDRESS 0x71
#define EXPANDER_ADDRESS 0x27
#define GPIOA_START_REGISTER 0x12
#define GPIOB_START_REGISTER 0x13
#define GPPUA 0x0C
#define GPPUB 0x0D

#define TWI_IS_CLOCKHELD() TWI0.MSTATUS & TWI_CLKHOLD_bm
#define TWI_IS_BUSERR() TWI0.MSTATUS & TWI_BUSERR_bm
#define TWI_IS_ARBLOST() TWI0.MSTATUS & TWI_ARBLOST_bm

#define CLIENT_NACK() TWI0.MSTATUS & TWI_RXACK_bm
#define CLIENT_ACK() !(TWI0.MSTATUS & TWI_RXACK_bm)

#define TWI_IS_BUSBUSY() ((TWI0.MSTATUS & TWI_BUSSTATE_BUSY_gc) == TWI_BUSSTATE_BUSY_gc)

#define TWI_WAIT() while (!((TWI_IS_CLOCKHELD()) || (TWI_IS_BUSERR()) || (TWI_IS_ARBLOST()) || (TWI_IS_BUSBUSY())))

void setupDisplay() {
    
    // set pin directions and enable pull up resistors
    PORTA.DIRSET = DISPLAYSDA;
    PORTA.DIRSET = DISPLAYSCL;
    PORTA.PIN2CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN3CTRL = PORT_PULLUPEN_bm;
    
    TWI0.CTRLA = TWI_SDAHOLD_50NS_gc;
    
    //Clear MSTATUS (write 1 to flags). BUSSTATE set to idle
    TWI0.MSTATUS = TWI_RIF_bm | TWI_WIF_bm | TWI_CLKHOLD_bm | TWI_RXACK_bm |
            TWI_ARBLOST_bm | TWI_BUSERR_bm | TWI_BUSSTATE_IDLE_gc;
    
    //MBAUD based on project instructions
    TWI0.MBAUD = 10;
    
    //[No ISRs] and Host Mode
    TWI0.MCTRLA = TWI_ENABLE_bm;
}

void displayHex(uint32_t hexData) {
    uint8_t data [5];
    data[0] = 0x76;
        
    data[4] = hexData & 0x0F;
    data[3] = (hexData >> 4) & 0x0F;
    data[2] = (hexData >> 8) & 0x0F;
    data[1] = (hexData >> 12) & 0x0F;
    
    sendDisplayBytesManual(data, 5);
}

void setupExpander () {
    
    // set pin directions and enable pull up resistors
    PORTA.DIRSET = DISPLAYSDA;
    PORTA.DIRSET = DISPLAYSCL;
    PORTA.PIN2CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN3CTRL = PORT_PULLUPEN_bm;
} 

int sendDisplayBytesManual(uint8_t* data, uint8_t len) {
    TWI0.MADDR = (DISPLAY_ADDRESS << 1) | 0;
    
    TWI_WAIT();
    
    uint8_t count = 0;
    
    for (count = 0; count < len; count++)
    {
        //Write a byte
        TWI0.MDATA = data[count];
        
        //Wait...
        TWI_WAIT();
        
        //If the client NACKed, then abort the write
        if (CLIENT_NACK())
        {
            return 0;
        }
        
    }

    TWI0.MCTRLB = TWI_MCMD_STOP_gc;   
    
    return 1;
    
}

void setDisplay(int timeslot1, int timeslot2, int colon) {

    uint8_t data [7];
    data[0] = 0x76;
    data[5] = 0x77;
    
    data[1] = timeslot1 / 10;
    data[2] = timeslot1 % 10;
    data[3] = timeslot2 / 10;
    data[4] = timeslot2 % 10;
             
    if (colon == 1) {
        data[6] = 0x10;
    } else {
        data[6] = 0x00;
    }
    
    sendDisplayBytesManual(data, 7);
    
}

void displayCurrentNote(char note, int octave, int accidental) {
    
    
    uint8_t data[4];
    data[0] = 0x76;
    data[1] = note;
    data[3] = octave;
    
    // natural note
    if (accidental == 0) {
        
    }
    // flat
    if (accidental == 1) {
        data[2] = 'b';
    }
    // sharp
    if (accidental == 2) {
        data[2] = 's';
    }
    
    sendDisplayBytesManual(data, 4);
    
}

int sendExpanderBytesManual(uint8_t* data, uint8_t writeAddr, uint8_t len) {
    TWI0.MADDR = (EXPANDER_ADDRESS << 1) | 0;
    
    TWI_WAIT();
    
    TWI0.MDATA = writeAddr;
    
    TWI_WAIT();
    
    uint8_t count = 0;
    
    for (count = 0; count < len; count++)
    {
        //Write a byte
        TWI0.MDATA = data[count];
        
        //Wait...
        TWI_WAIT();
        
        //If the client NACKed, then abort the write
        if (CLIENT_NACK())
        {
            return 0;
        }
        
    }

    TWI0.MCTRLB = TWI_MCMD_STOP_gc;   
    
    return 1;
    
}

void configureExpander() {
    
    uint8_t data;
    data = 255;
    
    sendExpanderBytesManual(data, GPPUA, 1);
    sendExpanderBytesManual(data, GPPUB, 1);
    
}

uint32_t readPins() {
    
    uint32_t output = 0;
    
    TWI0.MADDR = (EXPANDER_ADDRESS << 1) | 0;
    
    TWI_WAIT();
    
    TWI0.MDATA = GPIOA_START_REGISTER;
    
    TWI_WAIT();

    // change from write to read mode
    TWI0.MADDR |= 1;
    TWI0.MCTRLB = TWI_MCMD_REPSTART_gc;
    
    TWI_WAIT();
    
    output |= TWI0.MDATA;

    // stop reading new bits
    TWI0.MCTRLB = TWI_ACKACT_NACK_gc | TWI_MCMD_STOP_gc;
  
    TWI0.MADDR = (EXPANDER_ADDRESS << 1) | 0;
    
    TWI_WAIT();
    
    TWI0.MDATA = GPIOB_START_REGISTER;
    
    TWI_WAIT();

    // change from write to read mode
    TWI0.MADDR |= 1;
    TWI0.MCTRLB = TWI_MCMD_REPSTART_gc;
    
    TWI_WAIT();
    
    output |= TWI0.MDATA << 8;
    
    // stop reading new bits
    TWI0.MCTRLB = TWI_ACKACT_NACK_gc | TWI_MCMD_STOP_gc;
    
    return output;
    
}