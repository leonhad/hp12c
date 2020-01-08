/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support 
 * ----------------------------------------------------------------------------
 * Copyright (c) 2008, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

//------------------------------------------------------------------------------
/// \dir
/// !Purpose
/// 
/// Definition and functions for using AT91SAM7L-related features, such
/// has PIO pins, memories, etc.
/// 
/// !Usage
/// -# The code for booting the board is provided by board_cstartup.S and
///    board_lowlevel.c.
/// -# For using board PIOs, board characteristics (clock, etc.) and external
///    components, see board.h.
/// -# For manipulating memories (remapping, SDRAM, etc.), see board_memories.h.
//------------------------------------------------------------------------------
 
//------------------------------------------------------------------------------
/// \unit
/// !Purpose
/// 
/// Definition of AT91SAM7L-EK characteristics, AT91SAM7L-dependant PIOs and
/// external components interfacing.
/// 
/// !Usage
/// -# For operating frequency information, see "SAM7L-EK - Operating frequencies".
/// -# For using portable PIO definitions, see "SAM7L-EK - PIO definitions".
//------------------------------------------------------------------------------

#ifndef BOARD_H 
#define BOARD_H

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#if defined(at91sam7l64)
    #include "at91sam7l64/at91sam7l64.h"
#elif defined(at91sam7l128)
    #include "at91sam7l128/at91sam7l128.h"
#else
    #error Board does not support the specified chip.
#endif

//------------------------------------------------------------------------------
//         Definitions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM7L-EK - Board Description"
/// This page lists several definition related to the board description
///
/// !Definitions
/// - BOARD_NAME

/// Name of the board.
#define BOARD_NAME "AT91SAM7L-EK"
/// Board definition.
#define at91sam7lek
/// Family definition.
#define at91sam7l
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM7L-EK - Operating frequencies"
/// This page lists several definition related to the board operating frequency
/// (when using the initialization done by board_lowlevel.c).
/// 
/// !Definitions
/// - BOARD_MCK

/// Master clock frequency (when using board_lowlevel.c).
#define BOARD_MCK               (32768 * 915)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM7L-EK - PIO definitions"
/// This pages lists all the pio definitions contained in board.h. The constants
/// are named using the following convention: PIN_* for a constant which defines
/// a single Pin instance (but may include several PIOs sharing the same
/// controller), and PINS_* for a list of Pin instances.
///  
/// !PCK
/// - PIN_PCK0
/// 
/// !DBGU
/// - PINS_DBGU
/// 
/// !LEDs
/// - PIN_LED_DS1
/// - PIN_LED_DS2
/// - PIN_LED_DS3
/// - PIN_LED_DS4
/// - PINS_LEDS
/// - LED_DS1
/// - LED_DS2
/// - LED_DS3
/// - LED_DS4
///
/// !Push buttons
/// - PIN_PUSHBUTTON_1
/// - PIN_PUSHBUTTON_2
/// - PIN_PUSHBUTTON_3
/// - PIN_PUSHBUTTON_4
/// - PINS_PUSHBUTTONS
/// - PUSHBUTTON_BP1
/// - PUSHBUTTON_BP2
/// - PUSHBUTTON_BP3
/// - PUSHBUTTON_BP4
///
/// !USART
/// - PIN_USART0_TXD_POW
/// 
/// !SPI
/// - PIN_SPI_MOSI
/// - PIN_SPI_MISO
/// - PIN_SPI_SPCK
/// - PIN_SPI_NPCS3
/// - PINS_SPI
/// 
/// !USB
/// - PIN_USB_VBUS
/// - PIN_USB_PULLUP
/// 
/// !Keyboard Matrix
/// - PIN_KBMATRIX_ROW_0
/// - PIN_KBMATRIX_ROW_1
/// - PIN_KBMATRIX_ROW_2
/// - PIN_KBMATRIX_ROW_3
/// - PIN_KBMATRIX_ROW_4
/// - PINS_KBMATRIX_ROWS
/// - PIN_KBMATRIX_COL_0
/// - PIN_KBMATRIX_COL_1
/// - PIN_KBMATRIX_COL_2
/// - PIN_KBMATRIX_COL_3
/// - PIN_KBMATRIX_COL_4
/// - PIN_KBMATRIX_COL_5
/// - PIN_KBMATRIX_COL_6
/// - PINS_KBMATRIX_COLS
/// - PINS_KBMATRIX_WAKEUP
///
/// !PWMC
/// - PIN_PWMC_PWM0
/// - PIN_PWMC_PWM1
/// - PIN_PWMC_PWM2
///
/// !TWI
/// - PINS_TWI

/// PCK0 pin definition.
#define PIN_PCK0  {1 << 23, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_PERIPH_A, PIO_DEFAULT}

/// DBGU pins (DTXD and DRXD) definitions.
#define PINS_DBGU   {0x00030000, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_PERIPH_A, PIO_DEFAULT}

/// LED #0 pin definition.
#define PIN_LED_0  {1 << 7, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT}
/// LED #1 pin definition.
#define PIN_LED_1  {1 << 8, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT}
/// LED #2 pin definition.
#define PIN_LED_2  {1 << 9, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_OUTPUT_1, PIO_DEFAULT}
/// List of all led definitions.
#define PINS_LEDS  PIN_LED_0, PIN_LED_1, PIN_LED_2

/// LED DS1 index.
#define LED_DS1     0
/// LED DS2 index.
#define LED_DS2     1
/// Power LED index.
#define LED_POWER   2

/// Push button #1 pin definition
#define PIN_PUSHBUTTON_1 {1 << 0, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT, PIO_DEGLITCH | PIO_PULLUP}
/// Push button #2 pin definition
#define PIN_PUSHBUTTON_2 {1 << 1, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT, PIO_DEGLITCH | PIO_PULLUP}
/// List of all pushbuttons definitions.
#define PINS_PUSHBUTTONS PIN_PUSHBUTTON_1, PIN_PUSHBUTTON_2

/// Keyboard column #0 pin definition.
#define PIN_KEYBOARD_COL0 {1 << 10, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT}

/// Pushbutton 1 index
#define PUSHBUTTON_BP1  0
/// Pushbutton 2 index
#define PUSHBUTTON_BP2  1

/// USART0 RXD pin definition.
#define PIN_USART0_RXD  {1 << 12, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_PERIPH_A, PIO_DEFAULT}
/// USART0 TXD pin definition.
#define PIN_USART0_TXD  {1 << 13, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_PERIPH_A, PIO_DEFAULT}
/// USART0 TXD pin definition to avoid IRDA transceiver consumption.
#define PIN_USART0_TXD_POW  {1 << 13, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT}
/// USART0 SCK pin definition
#define PIN_USART0_SCK  {1 << 9, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_PERIPH_B, PIO_DEFAULT}

/// SPI MISO pin definition.
#define PIN_SPI_MISO  {1 << 19, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_PERIPH_A, PIO_DEFAULT}
/// SPI MOSI pin definition.
#define PIN_SPI_MOSI  {1 << 20, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_PERIPH_A, PIO_DEFAULT}
/// SPI SPCK pin definition.
#define PIN_SPI_SPCK  {1 << 21, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_PERIPH_A, PIO_DEFAULT}
/// SPI NPCS0 pin definition.
#define PIN_SPI_NPCS0 {1 << 18, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_PERIPH_A, PIO_DEFAULT}
/// SPI NPCS3 pin definition.
#define PIN_SPI_NPCS3 {1 << 22, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_PERIPH_A, PIO_DEFAULT}
/// List of SPI pin definitions (does not include NPCS pins).
#define PINS_SPI      PIN_SPI_MISO, PIN_SPI_MOSI, PIN_SPI_SPCK

/// Keyboard rows ( scanning output )
#define PIN_KBMATRIX_ROW_0  {1 <<  0, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_OUTPUT_1, PIO_OPENDRAIN | PIO_PULLUP}
#define PIN_KBMATRIX_ROW_1  {1 <<  1, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_OUTPUT_1, PIO_OPENDRAIN | PIO_PULLUP}
#define PIN_KBMATRIX_ROW_2  {1 <<  2, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_OUTPUT_1, PIO_OPENDRAIN | PIO_PULLUP}
#define PIN_KBMATRIX_ROW_3  {1 <<  3, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_OUTPUT_1, PIO_OPENDRAIN | PIO_PULLUP}
#define PIN_KBMATRIX_ROW_4  {1 <<  4, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_OUTPUT_1, PIO_OPENDRAIN | PIO_PULLUP}
#define PINS_KBMATRIX_ROWS  PIN_KBMATRIX_ROW_0, PIN_KBMATRIX_ROW_1, PIN_KBMATRIX_ROW_2, \
                            PIN_KBMATRIX_ROW_3, PIN_KBMATRIX_ROW_4

/// Keyboard columns ( capture input )
#define PIN_KBMATRIX_COL_0   {1 << 10, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT, PIO_PULLUP}
#define PIN_KBMATRIX_COL_1   {1 << 11, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT, PIO_PULLUP}
#define PIN_KBMATRIX_COL_2   {1 << 25, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT, PIO_PULLUP}
#define PIN_KBMATRIX_COL_3   {1 << 26, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT, PIO_PULLUP}
#define PIN_KBMATRIX_COL_4   {1 << 27, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT, PIO_PULLUP}
#define PIN_KBMATRIX_COL_5   {1 << 28, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT, PIO_PULLUP}
#define PIN_KBMATRIX_COL_6   {1 << 29, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT, PIO_PULLUP}
#define PINS_KBMATRIX_COLS   PIN_KBMATRIX_COL_0, PIN_KBMATRIX_COL_1, PIN_KBMATRIX_COL_2, \
                             PIN_KBMATRIX_COL_3, PIN_KBMATRIX_COL_4, PIN_KBMATRIX_COL_5, \
                             PIN_KBMATRIX_COL_6
/// Keyboard matrix pins in wake-up mode.
#define PINS_KBMATRIX_WAKEUP \
    {0x0000001F, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT, PIO_PULLUP}, \
    {0x3E000C00, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT}

/// PWMC PWM0 pin definition.
#define PIN_PWMC_PWM0  {1 << 7, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_PERIPH_A, PIO_DEFAULT}
/// PWMC PWM1 pin definition.
#define PIN_PWMC_PWM1  {1 << 8, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_PERIPH_A, PIO_DEFAULT}
/// PWMC PWM2 pin definition.
#define PIN_PWMC_PWM2  {1 << 9, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_PERIPH_A, PIO_DEFAULT}

/// TWI pins definition.
#define PINS_TWI  {0x00000C00, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_PERIPH_A, PIO_DEFAULT}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM7L-EK - Memories"
/// This page lists definitions related to internal & external on-board memories.
/// 
/// !Embedded Flash
/// - BOARD_FLASH_EEFC
/// - BOARD_FLASH_IAP_ADDRESS

/// Indicates chip has an Enhanced EFC.
#define BOARD_FLASH_EEFC
/// Address of the IAP function in ROM.
#define BOARD_FLASH_IAP_ADDRESS         0x400008
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// \page "SAM7L-EK - External components"
/// This page lists the definitions related to external on-board components
/// located in the board.h file for the AT91SAM7L-EK.
///
/// !MS5540B (barometer)
/// - PIN_MS5540B_DOUT
///
/// !AT45 Dataflash Card
/// - BOARD_AT45_A_SPI_BASE
/// - BOARD_AT45_A_SPI_ID
/// - BOARD_AT45_A_SPI_PINS
/// - BOARD_AT45_A_SPI
/// - BOARD_AT45_A_NPCS
/// - BOARD_AT45_A_NPCS_PIN

/// Base address of SPI peripheral connected to the dataflash.
#define BOARD_AT45_A_SPI_BASE         AT91C_BASE_SPI
/// Identifier of SPI peripheral connected to the dataflash.
#define BOARD_AT45_A_SPI_ID           AT91C_ID_SPI
/// Pins of the SPI peripheral connected to the dataflash.
#define BOARD_AT45_A_SPI_PINS         PINS_SPI
/// Dataflahs SPI number.
#define BOARD_AT45_A_SPI              0
/// Chip select connected to the dataflash.
#define BOARD_AT45_A_NPCS             0
/// Chip select pin connected to the dataflash.
#define BOARD_AT45_A_NPCS_PIN         PIN_SPI_NPCS0

/// MS5540B sensor DOUT pin definition.
#define PIN_MS5540B_DOUT {1 << 19, AT91C_BASE_PIOC, AT91C_ID_PIOC, PIO_INPUT, PIO_DEFAULT}
//------------------------------------------------------------------------------

#endif //#ifndef BOARD_H

