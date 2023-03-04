; 	LCDIS.INC		MPB	19-12-05
;
;	Include file to operate 16x2 LCD display
; 	Uses GPR 70 - 75
; 	
;	Final version
;-------------------------------------------------------------------------------------------

Timer1	EQU	70		; 1ms count register
TimerX	EQU	71		; Xms count register
Var	EQU	72		; Output variable
Point	EQU	73		; Program table pointer
Select	EQU	74		; Used to set or clear RS bit
OutCod	EQU	75		; Temp store for output code

RS	EQU	1		; Register select output bit
E	EQU	2		; Enable output clocks display input


;--------------------------------------------------------------------------------------------	
;	1ms delay with 1us cycle time (1000 cycles)
;--------------------------------------------------------------------------------------------	
onems	MOVLW	D'249'		; Count for 1ms delay 
	MOVWF	Timer1		; Load count
loop1	NOP			; Pad for 4 cycle loop
	DECFSZ	Timer1		; Count
	GOTO	loop1		; until Z
	RETURN			; and finish

;---------------------------------------------------------------------------------------------	
;	Delay Xms
;	Receives count in W, uses Onems
;---------------------------------------------------------------------------------------------	
xms	MOVWF	TimerX		; Count for X ms
loopX	CALL	onems		; Delay 1ms
	DECFSZ	TimerX		; Repeat X times 
	GOTO	loopX		; until Z
	RETURN			; and finish

;---------------------------------------------------------------------------------------------	
;	Generate data/command clock siganl E
;---------------------------------------------------------------------------------------------	
pulseE	BSF	PORTD,E		; Set E high
	CALL	onems		; Delay 1ms
	BCF	PORTD,E		; Reset E low
	CALL	onems		; Delay 1ms
	RETURN			; done

;---------------------------------------------------------------------------------------------	
;	Send a command byte in two nibbles from RB4 - RB7
;	Receives command in W, uses PulseE and Onems
;---------------------------------------------------------------------------------------------	
send	MOVWF	OutCod		; Store output code
	ANDLW	0F0		; Clear low nybble
	MOVWF	PORTD		; Output high nybble
	BTFSC	Select,RS	; Test RS bit
	BSF	PORTD,RS	; and set for data
	CALL	pulseE		; and clock display register
	CALL	onems		; wait 1ms for display to complete

	SWAPF	OutCod		; Swap low and high nybbles 
	MOVF	OutCod,W	; Retrieve output code
	ANDLW	0F0		; Clear low nybble
	MOVWF	PORTD		; Output low nybble
	BTFSC	Select,RS	; Test RS bit
	BSF	PORTD,RS	; and set for data
	CALL	pulseE		; and clock display register
	CALL	onems		; wait 1ms for display to complete
	RETURN			; done

;---------------------------------------------------------------------------------------------	
;	Initialise the display
;	Uses Send
;---------------------------------------------------------------------------------------------	
inid	MOVLW	D'100'		; Load count for 100ms delay
	CALL	xms		; and wait for display start
	MOVLW	0F0		; Mask for select code
	MOVWF	Select		; High nybble not masked

	MOVLW	0x30		; Load initial nibble
	MOVWF	PORTD		; and output it to display
	CALL	pulseE		; Latch initial code
	MOVLW	D'5'		; Set delay 5ms
	CALL	xms		; and wait
	CALL	pulseE		; Latch initial code again
	CALL	onems		; Wait 1ms
	CALL	pulseE		; Latch initial code again
	BCF	PORTD,4		; Set 4-bit mode
	CALL	pulseE		; Latch it
	
	MOVLW	0x28		; Set 4-bit mode, 2 lines
	CALL	send		; and send code
	MOVLW	0x08		; Switch off display
	CALL	send		; and send code
	MOVLW	0x01		; Code to clear display
	CALL	send		; and send code
	MOVLW	0x06		; Enable cursor auto inc  
	CALL	send		; and send code
	MOVLW	0x80		; Zero display address
	CALL	send		; and send code
	MOVLW	0x0C		; Turn on display  
	CALL	send		; and send code

	RETURN			; Done
	