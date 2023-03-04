;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;	CALC.ASM	MPB	Ver 1.0		28-8-05
;
;	Simple calculator 
;	Single digit input, two digit results
;	Integer handling only  
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	PROCESSOR 16F877
;	Clock = XT 4MHz, standard fuse settings
	__CONFIG 0x3731

;	LABEL EQUATES	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	INCLUDE "P16F877A.INC"	


Char	EQU	30	; Display character code
Num1	EQU	31	; First number input
Num2	EQU	32	; Second number input
Result	EQU	33	; Calculated result
Oper	EQU	34	; Operation code store 
Temp	EQU	35	; Temporary register for subtract
Kcount	EQU	36	; Count of keys hit
Kcode	EQU	37	; ASCII code for key
Msd		EQU	38	; Most significant digit of result
Lsd		EQU	39	; Least significant digit of result
Kval	EQU	40	; Key numerical value

RS	EQU	1	; Register select output bit
E	EQU	2	; Display data strobe

; Program begins ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	ORG	0		; Default start address 
	NOP			; required for ICD mode

	BANKSEL	TRISC		; Select bank 1
	MOVLW	B'11110000'	; Keypad direction code
	MOVWF	TRISC		;  
	CLRF	TRISD		; Display port is output

	BANKSEL PORTC		; Select bank 0
	MOVLW	0FF		; 
	MOVWF	PORTC		; Set keypad outputs high
	CLRF	PORTD		; Clear display outputs
	GOTO	start		; Jump to main program


; MAIN LOOP ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

start	CALL	inid		; Initialise the display
		MOVLW	0x80		; position to home cursor
		BCF	Select,RS	; Select command mode
		CALL	send		; and send code

		CLRW	Char		; ASCII = 0
		CLRW	Kval		; Key value = 0
		CLRW	DFlag		; Digit flags = 0

scan	CALL	keyin		; Scan keypad
		MOVF	Char,1		; test character code
		BTFSS	STATUS,Z	; key pressed?
		GOTO	keyon		; yes - wait for release
		GOTO	scan		; no - scan again

keyon	MOVF	Char,W		; Copy.. 
		MOVWF	Kcode		; ..ASCIIcode
		MOVLW	D'50'		; delay for..
		CALL	xms		; ..50ms debounce

wait	CALL	keyin		; scan keypad again
		MOVF	Char,1		; test character code
		BTFSS	STATUS,Z	; key pressed?
		GOTO	wait		; no - rescan
		CALL	disout		; yes - show symbol

		INCF	Kcount		; inc count..
		MOVF	Kcount,W	; ..of keys pressed
		ADDWF	PCL		; jump into table	
		NOP
		GOTO	first		; process first key
		GOTO	scan		; get operation key
		GOTO	second		; process second symbol
		GOTO	calc		; calculate result
		GOTO	clear		; clear display

first	MOVF	Kval,W		; store..
		MOVWF	Num1		; first num
		GOTO	scan		; and get op key

second	MOVF	Kval,W		; store..
		MOVWF	Num2		; second number 
		GOTO	scan		; and get equals key


; SUBROUTINES ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Include LCD driver routine
	
	INCLUDE	"LCDIS.INC"


; Scan keypad .............................................

keyin	MOVLW	00F		; deselect..
		MOVWF	PORTC		; ..all rows
		BCF		PORTC,0		; select row A
		CALL	onems		; wait output stable

		BTFSC	PORTC,4		; button 7?
		GOTO	b8		; no
		MOVLW	'7'		; yes
		MOVWF	Char		; load key code
		MOVLW	07		; and
		MOVWF	Kval		; key value
		RETURN

b8		BTFSC	PORTC,5		; button 8?
		GOTO	b9		; no
		MOVLW	'8'		; yes	
		MOVWF	Char
		MOVLW	08
		MOVWF	Kval
		RETURN
	
b9		BTFSC	PORTC,6		; button 9?
		GOTO	bd		; no
		MOVLW	'9'		; yes
		MOVWF	Char
		MOVLW	09
		MOVWF	Kval
		RETURN
	
bd		BTFSC	PORTC,7		; button /?
		GOTO	rowb		; no
		MOVLW	'/'		; yes
		MOVWF	Char		; store key code
		MOVWF	Oper		; store operator symbol
		RETURN

rowb	BSF		PORTC,0		; select row B
		BCF		PORTC,1
		CALL	onems

		BTFSC	PORTC,4		; button 4?
		GOTO	b5		; no
		MOVLW	'4'		; yes
		MOVWF	Char
		MOVLW	04
		MOVWF	Kval
		RETURN
	
b5		BTFSC	PORTC,5		; button 5?
		GOTO	b6		; no
		MOVLW	'5'		; yes
		MOVWF	Char
		MOVLW	05
		MOVWF	Kval
		RETURN
	
b6		BTFSC	PORTC,6		; button 6?
		GOTO	bm		; no
		MOVLW	'6'		; yes
		MOVWF	Char
		MOVLW	06
		MOVWF	Kval
		RETURN
	
bm	BTFSC	PORTC,7		; button x?
	GOTO	rowc		; no
	MOVLW	'x'		; yes
	MOVWF	Char
	MOVWF	Oper
	RETURN

rowc	BSF	PORTC,1		; select row C
	BCF	PORTC,2
	CALL	onems

	BTFSC	PORTC,4		; button 1?
	GOTO	b2		; no
	MOVLW	'1'		; yes
	MOVWF	Char
	MOVLW	01
	MOVWF	Kval
	RETURN
		
b2	BTFSC	PORTC,5		; button 2?
	GOTO	b3		; no
	MOVLW	'2'		; yes
	MOVWF	Char
	MOVLW	02
	MOVWF	Kval
	RETURN
	
b3	BTFSC	PORTC,6		; button 3?
	GOTO	bs		; no
	MOVLW	'3'		; yes
	MOVWF	Char
	MOVLW	03
	MOVWF	Kval
	RETURN

bs	BTFSC	PORTC,7		; button -?
	GOTO	rowd		; no
	MOVLW	'-'		; yes
	MOVWF	Char
	MOVWF	Oper
	RETURN

rowd	BSF	PORTC,2		; select row D
	BCF	PORTC,3
	CALL	onems

	BTFSC	PORTC,4		; button C?
	GOTO	b0		; no	
	MOVLW	'c'		; yes
	MOVWF	Char
	MOVWF	Oper
	RETURN
		
b0	BTFSC	PORTC,5		; button 0?
	GOTO	be		; no
	MOVLW	'0'		; yes
	MOVWF	Char
	MOVLW	00
	MOVWF	Kval
	RETURN
	
be	BTFSC	PORTC,6		; button =?
	GOTO	bp		; no
	MOVLW	'='		; yes
	MOVWF	Char
	RETURN

bp	BTFSC	PORTC,7		; button +?
	GOTO	done		; no
	MOVLW	'+'		; yes
	MOVWF	Char
	MOVWF	Oper
	RETURN

done	BSF	PORTC,3		; clear last row
	CLRF	Char		; character code = 0
	RETURN


; Write display ...........................................

disout	MOVF	Kcode,W		; Load the code
	BSF	Select,RS	; Select data mode
	CALL	send		; and send code
	RETURN

; Process operations ......................................

calc	MOVF	Oper,W		; check for add
	MOVWF	Temp		; load input op code
	MOVLW	'+'		; load plus code
	SUBWF	Temp		; compare
	BTFSC	STATUS,Z	; and check if same
	GOTO	add		; yes, jump to op

	MOVF	Oper,W		; check for subtract 
	MOVWF	Temp
	MOVLW	'-'
	SUBWF	Temp
	BTFSC	STATUS,Z
	GOTO	sub

	MOVF	Oper,W		; check for multiply 
	MOVWF	Temp
	MOVLW	'x'
	SUBWF	Temp
	BTFSC	STATUS,Z
	GOTO	mul
	
	MOVF	Oper,W		; check for divide
	MOVWF	Temp
	MOVLW	'/'
	SUBWF	Temp
	BTFSC	STATUS,Z
	GOTO	div
	GOTO	scan		; rescan if key invalid


; Calclate results from 2 input numbers ...................
 
add	MOVF	Num1,W		; get first number
	ADDWF	Num2,W		; add second
	MOVWF	Result		; and store result
	GOTO	outres		; display result


sub	BSF	STATUS,C	; Negative detect flag
	MOVF	Num2,W		; get first number
	SUBWF	Num1,W		; subtract second
	MOVWF	Result		; and store result
	
	BTFSS	STATUS,C	; answer negative?
	GOTO	minus		; yes, minus result
	GOTO	outres		; display result

minus	MOVLW	'-'		; load minus sign	
	BSF	Select,RS	; Select data mode
	CALL	send		; and send symbol

	COMF	Result		; invert all bits
	INCF	Result		; add 1
	GOTO	outres		; display result


mul	MOVF	Num1,W		; get first number
	CLRF	Result		; total to Z
add1	ADDWF	Result		; add to total
	DECFSZ	Num2		; num2 times and
	GOTO	add1		; repeat if not done
	GOTO	outres		; done, display result


div	CLRF	Result		; total to Z
	MOVF	Num2,W		; get divisor
	BCF	STATUS,C	; set C flag
sub1	INCF	Result		; count loop start
	SUBWF	Num1		; subtract 
	BTFSS	STATUS,Z	; exact answer?
	GOTO	neg		; no
	GOTO	outres		; yes, display answer
neg	BTFSC	STATUS,C	; gone negative?
	GOTO	sub1		; no - repeat
	DECF	Result		; correct the result
	MOVF	Num2,W		; get divisor
	ADDWF	Num1		; calc remainder

	MOVF	Result,W	; load result
	ADDLW	030		; convert to ASCII
	BSF	Select,RS	; Select data mode
	CALL	send		; and send result

	MOVLW	'r'		; indicate remainder
	CALL	send
	MOVF	Num1,W
	ADDLW	030		; convert to ASCII	
	CALL	send
	GOTO	scan


; Convert binary to BCD ...................................

outres	MOVF	Result,W	; load result
	MOVWF	Lsd		; into low digit store
	CLRF	Msd		; high digit = 0
	BSF	STATUS,C	; set C flag
	MOVLW	D'10'		; load 10

again	SUBWF	Lsd		; sub 10 from result
	INCF	Msd		; inc high digit
	BTFSC	STATUS,C	; check if negative
	GOTO	again		; no, keep going
	ADDWF	Lsd		; yes, add 10 back 
	DECF	Msd		; inc high digit


; display 2 digit BCD result ..............................

	MOVF	Msd,W		; load high digit result
	BTFSC	STATUS,Z	; check if Z
	GOTO	lowd		; yes, dont display Msd

	ADDLW	030		; convert to ASCII
	BSF	Select,RS	; Select data mode
	CALL	send		; and send Msd

lowd	MOVF	Lsd,W		; load low digit result
	ADDLW	030		; convert to ASCII
	BSF	Select,RS	; Select data mode
	CALL	send		; and send Msd

	GOTO	scan		; scan for clear key
	

; Restart ................................................

clear	MOVLW	01		; code to clear display
	BCF	Select,RS	; Select data mode
	CALL	send		; and send code	
	CLRF	Kcount		; reset count of keys
	GOTO	scan		; and rescan keypad


	END	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
