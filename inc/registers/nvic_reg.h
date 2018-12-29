/*
 * nvic_reg.h
 *
 *  Created on: 29 dec. 2018
 *      Author: Ludo
 */

#ifndef NVIC_REG_H
#define NVIC_REG_H

/*** NVIC registers ***/

typedef struct {
	volatile unsigned int ISER;			// Interrupt set-enable register.
	unsigned int RESERVED0[31];			// Reserved 0xE000E104.
	volatile unsigned int ICER;			// Interrupt clear-enable register.
	unsigned int RESERVED1[31];			// Reserved 0xE000E184.
	volatile unsigned int ISPR;			// Interrupt set-pending register.
	unsigned int RESERVED2[31];			// Reserved 0xE000E204.
	volatile unsigned int ICPR;    		// Interrupt clear-pending register.
	unsigned int RESERVED3[95];			// Reserved 0xE000E300.
	volatile unsigned char IPR[32];		// Interrupt priority registers 0 to 7.
} NVIC_BaseAddress;

/*** NVIC base address ***/

#define NVIC	((NVIC_BaseAddress*) ((unsigned int) 0xE000E100))

#endif /* NVIC_REG_H_ */
