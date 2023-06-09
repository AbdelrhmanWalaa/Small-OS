/*
 * exi_program.c
 *
 *     Created on: Apr 11, 2023
 *         Author: Abdelrhman Walaa - https://github.com/AbdelrhmanWalaa
 *    Description: This file contains all External Interrupt (EXI) functions' implementation, and ISR functions' prototypes and implementation.
 *  MCU Datasheet: AVR ATmega32 - https://ww1.microchip.com/downloads/en/DeviceDoc/Atmega32A-DataSheet-Complete-DS40002072A.pdf
 *	    Copyright: MIT License
 *
 *	               Copyright (c) Abdelrhman Walaa
 *
 *	               Permission is hereby granted, free of charge, to any person obtaining a copy
 *	               of this software and associated documentation files (the "Software"), to deal
 *	               in the Software without restriction, including without limitation the rights
 *	               to use, copy, modify, merge, publish, distribute, sub license, and/or sell
 *	               copies of the Software, and to permit persons to whom the Software is
 *	               furnished to do so, subject to the following conditions:
 *
 *	               The above copyright notice and this permission notice shall be included in all
 *	               copies or substantial portions of the Software.
 *
 *	               THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	               IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	               FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	               AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	               LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	               OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *	               SOFTWARE.
 */
 
/* MCAL */
#include "exi_private.h"
#include "exi_interface.h"

/*******************************************************************************************************************************************************************/
/* Declaration and Initialization */

/* Global Array of 3 Pointers to Functions ( because we have 3 External Interrupts ), these functions ( in APP Layer ) which those 3 Pointers will hold their addresses; are having void input arguments and void return type. */
static void ( *void_gs_apfInterrupstAction[3] ) ( void ) = { NULL, NULL, NULL };

/*******************************************************************************************************************************************************************/
/*
 Name: EXI_enablePIE
 Input: u8 InterruptId and u8 SenseControl
 Output: u8 Error or No Error
 Description: Function to enable and configure Peripheral Interrupt Enable (PIE), by setting relevant bit for each interrupt in GICR register,
 	 	 	  then configuring Sense Control in MCUCR (case interrupt 0 or 1) or MCUCSR (case interrupt 2) registers.
*/
u8 EXI_enablePIE	 ( u8 u8_a_interruptId, u8 u8_a_senseControl )
{
	/* Define local variable to set the error state = OK */
	u8 u8_l_errorState = STD_TYPES_OK;
	
	/* Check 1: InterruptId and the Sense Control are in the valid range */
	if ( ( u8_a_interruptId <= EXI_U8_INT2 ) && ( u8_a_senseControl <= EXI_U8_SENSE_RISING_EDGE ) )
	{
		/* Check 1.1: Required InterruptId */
		switch ( u8_a_interruptId )
		{
			case EXI_U8_INT0:
				/* Enable Interrupt 0 */
				SET_BIT( EXI_U8_GICR_REG, EXI_U8_INT0_BIT );

				/* Check 1.1.1: Required SenseControl */
				switch ( u8_a_senseControl )
				{
					case EXI_U8_SENSE_LOW_LEVEL		: CLR_BIT( EXI_U8_MCUCR_REG, EXI_U8_ISC00_BIT ); CLR_BIT( EXI_U8_MCUCR_REG, EXI_U8_ISC01_BIT ); break;
					case EXI_U8_SENSE_LOGICAL_CHANGE: SET_BIT( EXI_U8_MCUCR_REG, EXI_U8_ISC00_BIT ); CLR_BIT( EXI_U8_MCUCR_REG, EXI_U8_ISC01_BIT ); break;
					case EXI_U8_SENSE_FALLING_EDGE	: CLR_BIT( EXI_U8_MCUCR_REG, EXI_U8_ISC00_BIT ); SET_BIT( EXI_U8_MCUCR_REG, EXI_U8_ISC01_BIT ); break;
					case EXI_U8_SENSE_RISING_EDGE	: SET_BIT( EXI_U8_MCUCR_REG, EXI_U8_ISC00_BIT ); SET_BIT( EXI_U8_MCUCR_REG, EXI_U8_ISC01_BIT ); break;
				}
			break;

			case EXI_U8_INT1:
				/* Enable Interrupt 1 */
				SET_BIT( EXI_U8_GICR_REG, EXI_U8_INT1_BIT );

				/* Check 1.1.2: Required SenseControl */
				switch( u8_a_senseControl)
				{
					case EXI_U8_SENSE_LOW_LEVEL		: CLR_BIT( EXI_U8_MCUCR_REG, EXI_U8_ISC10_BIT ); CLR_BIT( EXI_U8_MCUCR_REG, EXI_U8_ISC11_BIT ); break;
					case EXI_U8_SENSE_LOGICAL_CHANGE: SET_BIT( EXI_U8_MCUCR_REG, EXI_U8_ISC10_BIT ); CLR_BIT( EXI_U8_MCUCR_REG, EXI_U8_ISC11_BIT ); break;
					case EXI_U8_SENSE_FALLING_EDGE	: CLR_BIT( EXI_U8_MCUCR_REG, EXI_U8_ISC10_BIT ); SET_BIT( EXI_U8_MCUCR_REG, EXI_U8_ISC11_BIT ); break;
					case EXI_U8_SENSE_RISING_EDGE	: SET_BIT( EXI_U8_MCUCR_REG, EXI_U8_ISC10_BIT ); SET_BIT( EXI_U8_MCUCR_REG, EXI_U8_ISC11_BIT ); break;
				}
			break;

			case EXI_U8_INT2:
				/* Enable Interrupt 2 */
				SET_BIT( EXI_U8_GICR_REG, EXI_U8_INT2_BIT );

				/* Check 1.1.3: Required SenseControl */
				switch( u8_a_senseControl )
				{
					case EXI_U8_SENSE_FALLING_EDGE: CLR_BIT( EXI_U8_MCUCSR_REG, EXI_U8_ISC2_BIT ); break;
					case EXI_U8_SENSE_RISING_EDGE :	SET_BIT( EXI_U8_MCUCSR_REG, EXI_U8_ISC2_BIT ); break;
				}
			break;
		}
	}
	/* Check 2: InterruptId or the Sense Control is not in the valid range */
	else
	{
		/* Update error state = NOK, wrong InterruptId or Sense Control! */
		u8_l_errorState = STD_TYPES_NOK;
	}
	
	return u8_l_errorState;
}

/*******************************************************************************************************************************************************************/
/*
 Name: EXI_disablePIE
 Input: u8 InterruptId
 Output: u8 Error or No Error
 Description: Function to disable Peripheral Interrupt Enable (PIE), by clearing relevant bit for each interrupt in GICR register.
*/
u8 EXI_disablePIE    ( u8 u8_a_interruptId )
{
	/* Define local variable to set the error state = OK */
	u8 u8_l_errorState = STD_TYPES_OK;
	
	/* Check 1: InterruptId is in the valid range */
	if ( u8_a_interruptId <= EXI_U8_INT2 )
	{
		/* Check 1.1: Required InterruptId */
		switch ( u8_a_interruptId )
		{
			case EXI_U8_INT0: CLR_BIT( EXI_U8_GICR_REG, EXI_U8_INT0_BIT ); break;
			case EXI_U8_INT1: CLR_BIT( EXI_U8_GICR_REG, EXI_U8_INT1_BIT ); break;
			case EXI_U8_INT2: CLR_BIT( EXI_U8_GICR_REG, EXI_U8_INT2_BIT ); break;
		}
	}
	/* Check 2: InterruptId is not in the valid range */
	else
	{
		/* Update error state = NOK, wrong InterruptId! */
		u8_l_errorState = STD_TYPES_OK;
	}
	
	return u8_l_errorState;
}

/*******************************************************************************************************************************************************************/
/*
 Name: EXI_intSetCallBack
 Input: u8 InterruptId and Pointer to Function that takes void and returns void
 Output: u8 Error or No Error
 Description: Function to receive an address of a function ( in APP Layer ) to be called back in ISR function of the passed Interrupt ( InterruptId ),
 	 	 	  the address is passed through a pointer to function ( INTInterruptAction ), and then pass this address to ISR function.
*/
u8 EXI_intSetCallBack( u8 u8_a_interruptId, void ( *pf_a_interruptAction ) ( void ) )
{
	/* Define local variable to set the error state = OK */
	u8 u8_l_errorState = STD_TYPES_OK;

	/* Check 1: InterruptId is in the valid range, and Pointer to Function is not equal to NULL */
	if( ( u8_a_interruptId <= EXI_U8_INT2 ) && ( pf_a_interruptAction != NULL ) )
	{
		/* Store the passed address of function ( in APP Layer ) through pointer to function ( INTInterruptAction ) into Global Array of Pointers to Functions ( INTInterruptsAction ) in the passed index ( InterruptId ). */
		void_gs_apfInterrupstAction[u8_a_interruptId] = pf_a_interruptAction;
	}
	/* Check 2: InterruptId is not in the valid range, or Pointer to Function is equal to NULL */
	else
	{
		/* Update error state = NOK, wrong InterruptId, or Pointer to Function is NULL! */
		u8_l_errorState = STD_TYPES_NOK;
	}
	
	return u8_l_errorState;
}

/*******************************************************************************************************************************************************************/

/* ISR functions' prototypes of External Interrupt Request 0 ( INT0 ), External Interrupt Request 1 ( INT1 ), and External Interrupt Request 2 ( INT2 ) respectively */
void __vector_1( void )		__attribute__((signal));
void __vector_2( void )		__attribute__((signal));
void __vector_3( void )		__attribute__((signal));

/*******************************************************************************************************************************************************************/

/* ISR function implementation of INT0 */
void __vector_1( void )
{
	/* Check: INT0 index of the Global Array is not equal to NULL */
	if ( void_gs_apfInterrupstAction[EXI_U8_INT0] != NULL )
	{
		/* Call Back the function ( in APP Layer ), which its address is stored in the Global Array of Pointers to Functions ( INTInterruptsAction ) */
		void_gs_apfInterrupstAction[EXI_U8_INT0]();
	}	
}

/*******************************************************************************************************************************************************************/

/* ISR function implementation of INT1 */
void __vector_2( void )
{
	/* Check: INT1 index of the Global Array is not equal to NULL */
	if( void_gs_apfInterrupstAction[EXI_U8_INT1] != NULL )
	{
		/* Call Back the function ( in APP Layer ), which its address is stored in the Global Array of Pointers to Functions ( INTInterruptsAction ) */
		void_gs_apfInterrupstAction[EXI_U8_INT1]();
	}	
}

/*******************************************************************************************************************************************************************/

/* ISR function implementation of INT2 */
void __vector_3( void )
{
	/* Check: INT2 index of the Global Array is not equal to NULL */
	if( void_gs_apfInterrupstAction[EXI_U8_INT2] != NULL )
	{
		/* Call Back the function ( in APP Layer ), which its address is stored in the Global Array of Pointers to Functions ( INTInterruptsAction ) */
		void_gs_apfInterrupstAction[EXI_U8_INT2]();
	}	
}

/*******************************************************************************************************************************************************************/