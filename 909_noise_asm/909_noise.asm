; to compile
; gpasm -c -p p12f675 blink_led.asm 
; gplink -m -c *.o -o blink_led.hex 
; pk2cmd-x86_64.AppImage -P -F blink_led.hex -M -T -R 
    processor 12F675
    org 0

    #include <p12f675.inc>
    __config _INTRC_OSC_NOCLKOUT & _WDT_OFF & _PWRTE_ON & _MCLRE_OFF
    ; Internal oscillator
    ; Watchdog timer off
    ; Master Clear as regular PIN

    errorlevel -302

    bcf STATUS, RP0 ; Select Memory bank 0
    clrf GPIO ; Every digital pin is low
    movlw 0x07 ; Set GP0,1,2 to digital IO
    movwf CMCON

    bsf STATUS, RP0

    bcf ANSEL, ANS0 ; Disable analog features
    bcf ANSEL, ANS1
    bcf ANSEL, ANS2
    bcf ANSEL, ANS3

    clrf TRISIO ; Tristate: everything as output


    ; Back to Bank 0 so that we have access to GPIO
    bcf STATUS, RP0

    ; Create Shift Register and execution variables
    cblock 0x20
    SHIFT_REG0
    SHIFT_REG1
    SHIFT_REG2
    SHIFT_REG3
    TEMP
    endc

    ; Initialize to all filled with 1s (xor block d in 909 circuitry)
    movlw 0xff
    movwf SHIFT_REG0
    movwf SHIFT_REG1
    movwf SHIFT_REG2
    movwf SHIFT_REG3
loop
    ; This block transfer the first tap to MSB of TEMP
    bcf TEMP, 7 ; Clear MSB of temp
    ;btfsc SHIFT_REG1, 4 ; Tap 13
    btfsc SHIFT_REG3, 3 ; Tap 28
    ;btfsc SHIFT_REG0, 2 ; Tap 3
    bsf TEMP, 7 ; Set if the bit above is set

    ; This block transfer the second tap to MSB of W
	rlf SHIFT_REG3, w ; Tap 31

    ; TEMP = TEMP ^ W
	xorwf TEMP, f

    ; MSB(TEMP) [TEMP, 7] -> C [STATE, 0]
    rlf TEMP, f

    ; Shift Register rotation, C bit enter as LSB of SHIFT_REG0
    rrf SHIFT_REG0, f 
    rrf SHIFT_REG1, f
    rrf SHIFT_REG2, f
    rrf SHIFT_REG3, f
    ; Output the shift register from the end
    movf SHIFT_REG3, w
    movwf GPIO

    goto loop

    end
