    SEI ; disable all interupts
    CLD ; disable decimal mode

    ; Disable sound
    LDX #$40 
    STX APU_FRAME_COUNTER

    ; Init the stack register
    LDX #$FF
    TXS
    INX ; FF + 1 => #$00

    ; Zero out ppu registers
    STX PPU_CTRL
    STX PPU_MASK

    ; Disable sound channel
    STX DMC_FREQ

    ; Wait for vblack
:
    BIT PPU_STATUS
    BPL :-

    TXA ; load a with 0

ClearMemory:
    STA $0000, X ; $0000 => $00FF
    STA $0100, X ; $0100 => $01FF
    STA $0300, X ; $0300 => $03FF
    STA $0400, X ; $0400 => $04FF
    STA $0500, X ; $0500 => $05FF
    STA $0600, X ; $0600 => $06FF
    STA $0700, X ; $0700 => $07FF
    
    ; Special memory for sprites info
    LDA #$FF
    STA $0200, X ; $0200 => $02FF
    LDA #$00


    INX ; Make it 1, loops at 255
    BNE ClearMemory

    ; Wait for vblack
:
    BIT PPU_STATUS
    BPL :-

    ; Let ppu know where to store sprite info
    LDA #$02
    STA OAMDMA
    NOP

    ; Setup ppu to write pallete data
    LDA #$3F
    STA PPU_ADDR
    LDA #$00
    STA PPU_ADDR

    ; Load palletes
    LDX #$00
LoadPallets:
    LDA PalettesData, X
    STA PPU_DATA
    INX 
    CPX #$20 ; 4x4 = 16
    BNE LoadPallets

; Clear nametable
    LDX #$00
    LDY #$00
    LDA $2002
    LDA #$20
    STA $2006
    LDA #$00
    STA $2006
ClearNametable:
    STA PPU_DATA
    INX
    BNE ClearNametable
    INY
    CPY #$08
    BNE ClearNametable

; Enable interupts
    CLI

    LDA #%10000000 ; enable nmi
    STA PPU_CTRL

    BIT PPU_SCROLL

    LDA #%00011110
    STA PPU_MASK