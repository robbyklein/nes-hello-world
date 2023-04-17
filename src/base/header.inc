; Headers for emulators
.segment "HEADER"
    .byte "NES"
    .byte $1a
    .byte $02 ; 2 * 16kb PRG ROM
    .byte $01 ; 1 * 8kb CHR ROM
    .byte %00000001 ; mapper and mirroring
    .byte $00
    .byte $00
    .byte $00
    .byte $00
    .byte $00, $00, $00, $00, $00 ; fill up header segment