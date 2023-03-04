;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; 
;	KEYPAD.ASM	MPB 	Ver 1.0		28-8-05 
; 
; 	Reads keypad and shows digit on display
; 	Design file KEYPAD.DSN
; 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
 
	PROCESSOR 16F877

PCL		EQU	002		; Program Counter
PORTB   EQU 06	; Port B Data Register        
PORTC	EQU	007		; 7-Segment display
PORTD	EQU	008		; 3x4 keypad
STATUS	EQU 003

TRISB	EQU	086		; Data direction
TRISC	EQU	087		; Data direction
TRISD	EQU	088		; registers

INTCON	EQU	0B	; Interrupt Control Register

Key	EQU	020		; Count of keys

	ORG	000		; Start of program memory
	NOP			; For ICD mode
	GOTO start	; Jump to main program

; Interrupt Service Routine ................................

	ORG	004
	BTFSS INTCON, 0	; Check if the source of interrupt is change on RB4...7.
	goto other_int

	BTFSC PORTB, 4	; Check if PortB.4 (column 4)
	goto check_col_2
	call scan
	call show		; and display it
	BCF INTCON, 0	; Reset the interrupt flag
	goto other_int

check_col_2
	BTFSC PORTB, 5	; Check if PortB.5 (column 5)
	goto check_col_3
	call scan
	call show		; and display it
	BCF INTCON, 0	; Reset the interrupt flag
	goto other_int

check_col_3
	BTFSC PORTB, 6	; Check if PortB.6 (column 6)
	goto other_int
	call scan
	call show		; and display it
	BCF INTCON, 0	; Reset the interrupt flag
	goto other_int

other_int
	movf PORTB,1 	; Read PortB (to itself) to end mismatch condition
	RETFIE			; Return from interrupt


; Initialise ports.........................................

start
	BANKSEL	TRISC		; Display. 7-segment display
	CLRW				; all outputs
	MOVWF	TRISC		; 
	MOVLW	B'01110000'	; Keypad
	MOVWF	TRISB		; PortB pins 6, 5, 4 is input (columns)

	MOVLW	B'10001000'	; enable GIE (bit 7), RBIE (bit 3)
	MOVWF	TRISB		; PortB pins

	BANKSEL	PORTC		; Display off
	CLRF	PORTC		; initially
	GOTO	main		; jump to main

; Check a row of keys .....................................

row	INCF	Key			; Count first key
	BTFSS	PORTD,0		; Check key
	GOTO	found		; and quit if on

	INCF	Key			; and repeat
	BTFSS	PORTD,1		; for second 
	GOTO	found		; key

	INCF	Key		; and repeat
	BTFSS	PORTD,2		; for third
	GOTO	found		; key
	GOTO	next		; go for next row

; Scan the keypad..........................................

scan	CLRF Key		; Zero key count 
		BSF	STATUS,0	; Set Carry Flag 
		BCF	PORTD,4		; Select first row
newrow	GOTO	row		; check row

next	BSF	PORTD,3		; Set fill bit
		RLF	PORTD		; Select next row
		BTFSC	STATUS,0		; 0 into carry flag?
		GOTO	newrow		; if not, next row
		GOTO	scan		; if so, start again

found	RETURN			; quit with key count

; Display code table.......................................

table	MOVF	Key,W		; Get key count
	ADDWF	PCL		; and calculate jump
	NOP			; into table 
	RETLW	B'00001100'	; Code for '1'
	RETLW	B'10110110'	; Code for '2'
	RETLW	B'10011110'	; Code for '3'
	RETLW	B'11001100'	; Code for '4'
	RETLW	B'11011010'	; Code for '5'
	RETLW	B'11111000'	; Code for '6'
	RETLW	B'00001110'	; Code for '7'	
	RETLW	B'11111110'	; Code for '8'
	RETLW	B'11001110'	; Code for '9'
	RETLW	B'10010010'	; Code for '*'
	RETLW	B'01111110'	; Code for '0'
	RETLW	B'11101100'	; Code for '#'

; Output display code......................................

show	CALL	table		; Get display code
		MOVWF	PORTC		; and show it
		RETURN

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Read keypad & display....

main	MOVLW	0FF		; Set all outputs 
		MOVWF	PORTD	; to keypad high
		sleep			; Put the chip in low power mode
		GOTO	main	; and repeat
		END

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;		
