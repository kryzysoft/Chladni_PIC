/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.77
        Device            :  PIC16F18446
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"
#include "math.h"
#include "string.h"
#include "stdio.h"
#include "stdint.h"

/*
                         Main application
 */

static void sendLine(char *line);
static void sendChar(char character);
static void newIncrementValue(uint32_t incrementValue);
static uint32_t incrementValue = 0x000001A9;
static void setFrequency(float frequency);
static double frequency = 0;
static bool outOn = true;
            
void main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    while (1)
    {
        uint8_t character = 0;
        character = EUSART1_Read();
        if((character >= '0')&&(character <='9'))
        {
            uint8_t value = character - '0';
            frequency = 10*frequency;
            frequency = frequency + value;
            sendChar(character);
        }
        if((character == 13)||(character == 10))
        {
            sendChar('\r');
            sendChar('\n');
            
            setFrequency(frequency);
            
        }
        if(character == 'u')
        {
            incrementValue ++;
            newIncrementValue(incrementValue);
        }
        if(character == 'd')
        {
            incrementValue --;
            newIncrementValue(incrementValue);
        }
        if(character == 'z')
        {
            setFrequency(181.0);
        }
        if(character == 'x')
        {
            setFrequency(553.0);
        }
        if(character == 'c')
        {
            setFrequency(1030.0);
        }
        if(character == 'v')
        {
            setFrequency(1620.0);
        }
        if(character == 'b')
        {
            setFrequency(3241.0);
        }
        if(character == 'n')
        {
            setFrequency(3590.0);
        }
        if(character == ' ')
        {
            if(outOn)
            {
                outOn = false;
                RC5PPS = 0x00;   
            }
            else
            {
                outOn = true;
                RC5PPS = 0x18;
            }
        }
    }
}

static void setFrequency(float frequency)
{
    incrementValue = round(frequency*1048576.0/250000.0);
    newIncrementValue(incrementValue);
}

static void newIncrementValue(uint32_t incrementValue)
{
    NCO1INCU = ((incrementValue & 0x000F0000)>>16);
    NCO1INCH = ((incrementValue & 0x0000FF00)>>8);
    NCO1INCL = ((incrementValue & 0x000000FF)>>0);
            
    double realFrequency = 250000.0 * incrementValue / 1048576.0;
    char number[30];
    sprintf(number,"%f",realFrequency);
    sendLine(number);
    frequency = 0;
}

static void sendChar(char character)
{
    if(EUSART1_is_tx_ready())
    {
        EUSART1_Write(character);
    }
}

static void sendLine(char *line)
{
    for(uint8_t i=0; i<strlen(line); i++)
    {
        sendChar(line[i]);
    }
    sendChar('\r');
    sendChar('\n');
}
/**
 End of File
*/