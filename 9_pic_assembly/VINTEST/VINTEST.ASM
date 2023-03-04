;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;	Project:		Interfacing PICs 
;	Source File Name:	VINTEST.ASM		
;	Devised by:		MPB		
;	Date:			19-12-05
;	Status:			Final version
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; 	Demonstrates simple analogue input
;	using an external reference voltage of 2.56V
;	The 8-bit result is converted to BCD for display
;	as a voltage using the standard LCD routines.
;	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	PROCESSOR 16F877A
;	Clock = XT 4MHz, standard fuse settings
	__CONFIG 0x3731

;	LABEL EQUATES	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	#INCLUDE "P16F877A.INC" 	; standard labels 	

; GPR 70 - 75 allocated to included LCD display routine

count	EQU	30	; Counter for ADC setup delay
ADbin	EQU	31	; Binary input value
huns	EQU	32	; Hundreds digit in decimal value
tens	EQU	33	; Tens digit in decimal value
ones	EQU	34	; Ones digit in decimal value

; PROGRAM BEGINS ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	ORG	0		; Default start address 
	NOP			; required for ICD mode
	
; Port & display setup.....................................

	BANKSEL	TRISC		; Select bank 1
	CLRF	TRISD		; Display port is output
	MOVLW	B'00000011'	; Analogue input setup code
	MOVWF	ADCON1		; Left justify result, 
				; Port A = analogue inputs

	BANKSEL PORTC		; Select bank 0
	CLRF	PORTD		; Clear display outputs
	MOVLW	B'01000001'	; Analogue input setup code
	MOVWF	ADCON0		; f/8, RA0, done, enable  

	CALL	inid		; Initialise the display

; MAIN LOOP ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

start	CALL	getADC		; read input
		CALL	condec		; convert to decimal
		CALL	putLCD		; display input
		GOTO	start		; jump to main loop

; SUBROUTINES ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Read ADC input and store ................................

getADC	BSF	ADCON0,GO	; start ADC..
wait	BTFSC	ADCON0,GO	; ..and wait for finish
		GOTO	wait
		MOVF	ADRESH,W	; store result high byte
		RETURN		  		
	
; Convert input to decimal ................................

condec	MOVWF	ADbin		; get ADC result
		CLRF	huns		; zero hundreds digit
		CLRF	tens		; zero tens digit
		CLRF	ones		; zero ones digit

; Calclulate hundreds......................................

		BSF	STATUS,C	; set carry for subtract
		MOVLW	D'100'		; load 100
sub1	SUBWF	ADbin		; and subtract from result
		INCF	huns		; count number of loops
		BTFSC	STATUS,C	; and check if done
		GOTO	sub1		; no, carry on

		ADDWF	ADbin		; yes, add 100 back on
		DECF	huns		; and correct loop count

; Calculate tens digit.....................................

		BSF	STATUS,C	; repeat process for tens
		MOVLW	D'10'		; load 10
sub2	SUBWF	ADbin		; and subtract from result
		INCF	tens		; count number of loops
		BTFSC	STATUS,C	; and check if done
		GOTO	sub2		; no, carry on

		ADDWF	ADbin		; yes, add 100 back on
		DECF	tens		; and correct loop count
		MOVF	ADbin,W		; load remainder
		MOVWF	ones		; and store as ones digit

		RETURN				; done

; Output to display........................................

putLCD	BCF	Select,RS	; set display command mode
		MOVLW	080		; code to home cursor
		CALL	send	; output it to display
		BSF	Select,RS	; and restore data mode

; Convert digits to ASCII and display......................

	MOVLW	030			; load ASCII offset
	ADDWF	huns		; convert hundreds to ASCII
	ADDWF	tens		; convert tens to ASCII
	ADDWF	ones		; convert ones to ASCII

	MOVF	huns,W		; load hundreds code
	CALL	send		; and send to display
	MOVLW	'.'			; load point code
	CALL	send		; and output
	MOVF	tens,W		; load tens code
	CALL	send		; and output
	MOVF	ones,W		; load ones code
	CALL	send		; and output
	MOVLW	' '			; load space code
	CALL	send		; and output
	MOVLW	'V'			; load volts code
	CALL	send		; and output
	MOVLW	'o'			; load volts code
	CALL	send		; and output
	MOVLW	'l'			; load volts code
	CALL	send		; and output
	MOVLW	't'			; load volts code
	CALL	send		; and output
	MOVLW	's'			; load volts code
	CALL	send		; and output

	RETURN				; done

; INCLUDED ROUTINES ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Include LCD driver routines
;
	#INCLUDE "LCDIS.INC"
;	Contains routines:
;	inid:	Initialises display
;	onems:	1 ms delay
;	xms:	X ms delay
;		Receives X in W
;	send:	Sends a character to display
;		Receives: Control code in W (Select,RS=0)
;		  	  ASCII character code in W (RS=1)	

	END	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
