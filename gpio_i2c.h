/* 
 * File:   5143_FinalProject_GPIOI2C.h
 * Author: gahym
 *
 * Created on November 14, 2024, 11:28 AM
 */

#ifndef GPIO_I2C_H
#define	GPIO_I2C_H



typedef struct {
    
    uint32_t keys;
    uint16_t airflow;
    
} inputs_t;

int sendDisplayBytesManual(uint8_t* data, uint8_t len);
void setupDisplay();
void setDisplay(int timeslot1, int timeslot2, int colon);
void displayCurrentNote(char note, int octave, int accidental);
uint32_t readPins();
void configureExpander();
void displayHex(uint32_t hexData);

#endif	/* GPIO_I2C_H */

