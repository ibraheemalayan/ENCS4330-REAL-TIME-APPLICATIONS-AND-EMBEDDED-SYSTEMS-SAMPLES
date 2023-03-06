; flip a LED every 2.5s using timer0 interrupts

	PROCESSOR 16F877A
	__CONFIG 0x3731 ; Clock = XT 4MHz, standard fuse settings
	INCLUDE "P16F877A.INC"



; see https://www.youtube.com/watch?v=02-Iu4mXsjA for in-depth explanation of the code

; ---------------------------------
; ----------- Data Area -----------
; ---------------------------------

TEMP	EQU 0x75		; Temp store
TEMP2	EQU 0x76		; Temp2 store
Timer1	EQU 0x77		; Timer1 store


; ---------------------------------
; ----------- Code Area -----------
; ---------------------------------

	ORG	0x0000 		; Start of program memory
	NOP			; For ICD mode
	GOTO START_EXECUTION

	

SETUP_PORTS

	; setup port A as analog input, port C as digital output

	; port c is used for LED
	BANKSEL TRISC
	MOVLW	0x00		; In order to set PORTC Direction to output
	MOVWF	TRISC


	BANKSEL TRISA
	MOVLW	0x01		; In order to set PORTA - Pin 0 Direction to input

	BANKSEL ADCON0
	MOVLW  0x41		; Select Channel 0, Turn on A/D Conversion, Fosc/8, ADC currently stopped
	MOVWF  ADCON0

	BANKSEL ADCON1 
	MOVLW	0x0E		; set PORTA - AN0 as analog input, rest as digital
						; and set result as left justified (ADRESH, ADRESL)
	MOVWF	ADCON1
 
	BANKSEL CMCON 
	MOVLW	0x07		; Disable Comparator
	MOVWF	CMCON

	RETURN

READ_ADC

	CALL onems ; 1ms delay to allow ADC to settle ( mute ADC warnings in Proteus )

	BANKSEL ADCON0
	BSF ADCON0, GO	   ; Start A/D Conversion
loop
	BTFSC ADCON0, GO	 ; Wait for A/D Conversion to complete
	GOTO loop

	BANKSEL ADRESH
	MOVF ADRESH, W	   ; Read the converted data most significant byte
	MOVWF TEMP		   ; Store the converted data in TEMP

	; ignore the least significant 2 bits of the result (10 bit ADC)
	; compare the result with appropriate digital value for (0.4V ~ 40 C) 
	; digital value = (0.4V / 5V) * 1024 = 82
	; but since we are ignoring the least significant 2 bits, we compare with 82/4 = 20 (0x14)

	BANKSEL PORTC

	; Test TEMP ≤ 40 C

	MOVF TEMP,W
	SUBLW 0x14
	BTFSS STATUS, C
	GOTO TEMP_GT_K
TEMP_LE_K
	BCF PORTC, 2
	CALL READ_ADC
TEMP_GT_K
	BSF PORTC, 2
	CALL READ_ADC

START_EXECUTION

	BANKSEL PORTC
	MOVLW	0x00
	MOVWF	PORTC

	CALL SETUP_PORTS

	CALL READ_ADC ; read adc


;--------------------------------------------------------------------------------------------	
;	1ms delay with 1us cycle time (1000 cycles)
;--------------------------------------------------------------------------------------------	
onems	MOVLW	D'249'		; Count for 1ms delay 
	MOVWF	Timer1		; Load count
loop1	NOP			; Pad for 4 cycle loop
	DECFSZ	Timer1, 1		; Count
	GOTO	loop1		; until Z
	RETURN			; and finish

DONE
	END