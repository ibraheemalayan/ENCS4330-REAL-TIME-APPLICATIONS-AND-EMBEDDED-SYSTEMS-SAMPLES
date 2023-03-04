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


; ---------------------------------
; ----------- Code Area -----------
; ---------------------------------

    ORG	0x0000 		; Start of program memory
    NOP			; For ICD mode
    GOTO START_EXECUTION

	

SETUP_PORT_A_ANALOG_PORT_C_DIGITAL

    ; port c is used for debugging
	BANKSEL TRISC
	MOVLW	0x00		; In order to set PORTC Direction to output
	MOVWF	TRISC


    BANKSEL TRISA
    MOVLW	0x01		; In order to set PORTA - Pin 0 Direction to input

    BANKSEL ADCON0
    MOVLW  0x41        ; Select Channel 0, Turn on A/D Conversion, Fosc/8, ADC currently stopped
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

    BANKSEL ADCON0
    BSF ADCON0, GO       ; Start A/D Conversion
loop
    BTFSC ADCON0, GO     ; Wait for A/D Conversion to complete
    GOTO loop

    BANKSEL ADRESH
    MOVF ADRESH, W       ; Read the converted data most significant byte
    MOVWF TEMP           ; Store the converted data in TEMP

    ; ignore the least significant 2 bits of the result (10 bit ADC)
    ; compare the result with 0x40 (64 -> 256) (1.25V)

    BANKSEL PORTC

    ; Test TEMP ≤ ( 0x40 , 1.25V )

    MOVF TEMP,W
    SUBLW 0x40
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

	CALL INIT_DEBUG_PORT
    CALL SETUP_PORT_A_ANALOG_PORT_C_DIGITAL

	CALL READ_ADC ; read adc

DONE
	END