; flip a LED every 2.5s using timer0 interrupts

	PROCESSOR 16F877A
	__CONFIG 0x3731 ; Clock = XT 4MHz, standard fuse settings
	INCLUDE "P16F877A.INC"


; see the following link for more information on timers (summary)
; https://exploreembedded.com/wiki/PIC16f877a_Timer

; ---------------------------------
; ----------- Data Area -----------
; ---------------------------------

TIMER_INDEX		   EQU 0x28
TEMP	EQU 0x75		; Temp store
TEMP2	EQU 0x76		; Temp2 store
LCD_RS_PIN	EQU 4		; Register select output bit
LCD_E_PIN	EQU 5		; Enable display input
TMR0     EQU 0x01		; Timer0 register
TMR0_COUNT     EQU 0x78	; Timer0 count


; ---------------------------------
; ----------- Code Area -----------
; ---------------------------------

    ORG	0x0000 		; Start of program memory
    NOP			; For ICD mode
    GOTO START_EXECUTION

	ORG	0x0004 ; ISR
	GOTO 	ISR_FOR_TIMER


ISR_FOR_TIMER

    BANKSEL PORTC
    BSF PORTC, 1

    

    INCF TMR0_COUNT, 1

    MOVF TMR0_COUNT, W
    SUBLW 0xFA ; 10ms * 250 = 2.5s
    BTFSS STATUS, C
    GOTO TMR_COUNT_GT_100

TMR_COUNT_LT_100
    
    CALL RESET_TMR0 ; start next 10ms cycle
    RETFIE

TMR_COUNT_GT_100

    CALL RESET_TMR0

    MOVLW 0x00
    MOVWF TMR0_COUNT

    ; flip the LED
    BTFSS PORTC, 3 ; if LED is on, skip go to turn on
    GOTO TURN_ON

TURN_OFF
    BCF PORTC, 3
    RETFIE

TURN_ON
    BSF PORTC, 3
    RETFIE



SETUP_PORTS_DIGITAL

	BANKSEL ADCON1 
	MOVLW	0x06		; Disable A/D Conversion
	MOVWF	ADCON1

	BANKSEL CMCON 
	MOVLW	0x07		; Disable Comparator
	MOVWF	CMCON

	RETURN


INIT_DEBUG_PORT

	; port c is used for debugging
	BANKSEL TRISC
	MOVLW	0x00		; In order to set PORTC Direction to output
	MOVWF	TRISC

    BANKSEL PORTC
    MOVLW	0x00
	MOVWF	PORTC

    NOP
	RETURN

SETUP_INTERRUPTS

    BANKSEL INTCON

    BCF INTCON,INTF     ; Clear Interrupt Flag
    BSF INTCON,GIE      ; Enable Global Interrupt Bit
	BSF INTCON, PEIE    ; Enable Peripheral Interrupts
    BSF INTCON, TMR0IE    ; Enable Timer0 Overflow Interrupt Bit

    BCF INTCON, TMR0IF  ; Clear Timer0 Overflow Interrupt Flag

    BANKSEL OPTION_REG

    MOVLW 0x00
    MOVWF OPTION_REG
    
    ; set prescaler to 1:256
    BSF OPTION_REG, PS0
    BSF OPTION_REG, PS1
    BSF OPTION_REG, PS2

    BANKSEL TMR0

    MOVLW 0x00
    MOVWF TMR0_COUNT

    CALL RESET_TMR0

    RETURN

RESET_TMR0

    BANKSEL TMR0
    ; move 217 to TMR0 to get 10ms overflow
    MOVLW 0xD9
    MOVWF TMR0

    BANKSEL INTCON
    
    BCF INTCON, TMR0IF  ; Clear Timer0 Overflow Interrupt Flag
    BCF		INTCON,INTF		; clear the Interrupt flag 

    RETURN

START_EXECUTION

	CALL INIT_DEBUG_PORT
    CALL SETUP_PORTS_DIGITAL

    CALL SETUP_INTERRUPTS
    CALL RESET_TMR0










	CALL BLINK_CURSOR ; blink cursor




; instruction delay of 10us * W (each instruction is 1us)
DELAY_W	
	MOVWF	TEMP
loop_start

	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP

	DECFSZ	TEMP
	GOTO	loop_start
	RETURN


; instruction delay of ms * W (each loop is 10ms)
DELAY_W_10_MS	
	MOVWF	TEMP2
	MOVLW	0xFA
lp_st
    
	CALL DELAY_W
	CALL DELAY_W
	CALL DELAY_W
	CALL DELAY_W

	DECFSZ	TEMP2
	GOTO	lp_st
	RETURN

; used to announce reaching a section of the code
BLINK_CURSOR

    BANKSEL PORTC

    BSF	PORTC, 5

	; delay 10ms * 40 = 400ms
	MOVLW 0x28
	CALL DELAY_W_10_MS

    BCF	PORTC, 5

	; delay 10ms * 20 = 200ms
	MOVLW 0x14
	CALL DELAY_W_10_MS
    

	GOTO BLINK_CURSOR ; loop for ever



DONE
	END