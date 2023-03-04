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
PORTC	EQU	007		; 7-Segment display
PORTD	EQU	008		; 3x4 keypad

TRISC	EQU	087		; Data direction
TRISD	EQU	088		; registers

Key		EQU	020		; Count of keys

; Initialise ports.........................................

	BANKSEL	TRISC		; Display
	CLRW			; all outputs
	MOVWF	TRISC		; 
	MOVLW	B'00000111'	; Keypad
	MOVWF	TRISD		; bidirectional

	BANKSEL	PORTC		; Display off
	CLRF	PORTC		; initially
	GOTO	main		; jump to main

; Check a row of keys .....................................

row	INCF	Key		; Count first key
	BTFSS	PORTD,0		; Check key
	GOTO	found		; and quit if on

	INCF	Key		; and repeat
	BTFSS	PORTD,1		; for second 
	GOTO	found		; key

	INCF	Key		; and repeat
	BTFSS	PORTD,2		; for third
	GOTO	found		; key
	GOTO	next		; go for next row

; Scan the keypad..........................................

scan	CLRF	Key		; Zero key count 
		BSF	3,0		; Set Carry Flag 
		BCF	PORTD,4		; Select first row
newrow	GOTO	row		; check row

next	BSF	PORTD,3		; Set fill bit
		RLF	PORTD		; Select next row
		BTFSC	3,0		; 0 into carry flag?
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
		MOVWF	PORTD		; to keypad high
		CALL	scan		; Get key number
		CALL	show		; and dsiplay it
		GOTO	main		; and repeat
		END

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;		
