.include "base/constants.inc" ; Constants for all the registers
.include "base/header.inc" ; Information for emulators

; 00 - ff 
.segment "ZEROPAGE"
    background: .res 2
    frames_passed: .res 1

; Game code
.segment "STARTUP"
    ; Runs on game start/reset
    Reset:
        .include "base/reset.inc" ; Clears memory and inits ppu

    ; Main game loop
    Update:
        ; Wait for nmi to increment frame
        LDA frames_passed
        :
            cmp frames_passed
            beq :-

        ; Tell ppu where to start rendering bg tiles
        bit PPU_STATUS ; resets ppu_addr
        lda #$21
        sta PPU_ADDR
        lda #$07
        sta PPU_ADDR
        
        ; Render our hello world string
            ldx #$00
        HelloWorld:
            lda HelloWorldData, x
            sta PPU_DATA
            tay
            inx
            cpy #$03
            bne HelloWorld
        
        ; Reset scroll position
        JSR ResetScroll
     
        ; Infinite game loop
        JMP Update

    ; Non-masking interupt runs once per frame
    NMI:    
        ; Copy sprite data for display
        LDA #$02 
        STA OAMDMA

        ; Increment frames passed
        inc frames_passed

        ; Return from interupt
        RTI

    ResetScroll:
        ; Set the PPU scroll
        LDA #$00
        STA PPU_SCROLL                ; Reset Horizontal Scroll
        LDA #$00
        STA PPU_SCROLL                ; Reset Vertical Scroll
        RTS

    HelloWorldData:
        .byte $07, $04, $0b, $0b, $0e, $00, $16, $0e, $11, $0b, $03

    PalettesData:
        ; Backgrounds
        .byte $31, $00, $0c, $30
        .byte $31, $01, $21, $31
        .byte $31, $06, $16, $26
        .byte $31, $09, $19, $29
        ; Sprites
        .byte $31, $11, $21, $31
        .byte $31, $37, $17, $0f
        .byte $31, $13, $23, $33
        .byte $31, $14, $24, $34


    SpriteData:
        .byte $08, $00, $00, $08
        .byte $08, $01, $00, $10
        .byte $10, $02, $00, $08
        .byte $10, $03, $00, $10
        .byte $18, $04, $00, $08
        .byte $18, $05, $00, $10
        .byte $20, $06, $00, $08
        .byte $20, $07, $00, $10

.include "base/vectors.inc"
.include "base/chars.inc"