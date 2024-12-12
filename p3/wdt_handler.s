	.text
	.balign 2
	.global WDT
	.section __interrupt_vector_11, "ax"
	.word WDT  		; Entry for vector table

	.text
	.global redrawScreen
	.global increment_time
	.global currentMode
	.global blinkFlag
	.global blinkDigit

WDT:
	;;  Prologue: Save registers
	PUSH    R15
	PUSH    R14
	PUSH    R13
	PUSH    R12
	PUSH    R11
	PUSH    R10
	PUSH    R9
	PUSH    R8
	PUSH    R7
	PUSH    R6
	PUSH    R5
	PUSH    R4

	;;  Static variable secCount stored in memory
	;;  Access secCount using symbol reference
	MOV     &secCount, R12 ; Load secCount into R12
	ADD     #1, R12    ; Increment secCount
	MOV     R12, &secCount ; Store back updated secCount

	;;  Check if secCount == 250
	CMP     #250, R12
	JNE     EDIT_MODE	; Jump to Edit Mode logic if not equal

DISPLAY_MODE:
	MOV     &currentMode, R13 ; Load currentMode into R13
	CMP     #0, R13	  ; Check if currentMode == 0 (Display Mode)
	JNE     RESET_SEC	; Skip time increment if not in Display Mode
	CALL    #increment_time ; Call increment_time function

	MOV     #1, &redrawScreen ; Set redrawScreen = 1

RESET_SEC:
	CLR     &secCount	; Reset secCount to 0
	JMP     END_WDT	; Skip Edit Mode logic

EDIT_MODE:
	CMP     #0, R12	; Check secCount % 50
	RRA     R12	; Divide secCount by 50 (right shift 1)
	RRA     R12	; Divide secCount by 50 (right shift again)
	RRA     R12	; Divide secCount by 50 (8 total shifts)
	JNZ     END_WDT	; Skip blinking if not divisible

	MOV     &currentMode, R13 ; Load currentMode into R13
	CMP     #1, R13	  ; Check if currentMode == 1 (Edit Mode)
	JNE     END_WDT	; Skip if not in Edit Mode

	MOV     &blinkFlag, R14 ; Toggle blinkFlag
	XOR     #1, R14
	MOV     R14, &blinkFlag

	CALL    #blinkDigit	; Call blinkDigit function

END_WDT:
	;;  Epilogue: Restore registers
	POP     R4
	POP     R5
	POP     R6
	POP     R7
	POP     R8
	POP     R9
	POP     R10
	POP     R11
	POP     R12
	POP     R13
	POP     R14
	POP     R15

	;;  Ensure CPU wakes up after interrupt
	BIC     #CPUOFF, 0(SP) ; Clear CPUOFF bit in saved SR
	RETI	       ; Return from interrupt

	.data
	.balign 2
secCount:	 .word 0	; Static variable secCount initialized to 0
