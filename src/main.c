
#define MAJ_VERSION  0
#define MIN_VERSION  1

#include <stdio.h>
#include "../SMSlib/SMSlib.h"

#pragma save
#pragma disable_warning 85
unsigned char ram_test (unsigned char value) __z88dk_fastcall __naked {
  __asm
    di
    pop de             ; get ret address from stack
    ld a,l             ; get value to write and read
    ld hl,#0xC000      ; start of RAM
    ld bc,#0x0020      ; 32 times 256 bytes = 8 KB

_write_loop:
    ld (hl),a
    inc hl
    djnz _write_loop

    dec c
    jr nz,_write_loop

// put a long delay after the write, so that if any bit changes we can get it

    ld bc,#0x0000      ; longest delay I can imagine ;)
_delay_loop:
    .db #0xFD
    ld l,a             ; ld iyl,a since this is a pretty slow operation
    .db #0xFD
    ld l,a             ; ld iyl,a since this is a pretty slow operation
    .db #0xFD
    ld l,a             ; ld iyl,a since this is a pretty slow operation

    djnz _delay_loop
    dec c
    jr nz,_delay_loop

    .db #0xFD
    ld l,a             ; save written value in iyl
    ld hl,#0xC000      ; start of RAM
    ld bc,#0x0020      ; 32 times 256 bytes = 8 KB

_read_loop:
    ld a,(hl)
    .db #0xFD
    cp l               ; compare with value in iyl
    jr nz,_check_failed
    inc hl
    djnz _read_loop

    dec c
    jr nz,_read_loop

    ld l,#0
    push de            ; restore ret address into stack
    ei
    ret

_check_failed:
    ld l,#1
    push de            ; restore ret address into stack
    ei
    ret
  __endasm;
}
#pragma restore

unsigned char ram_test_2 (void) __z88dk_fastcall __naked {
  __asm
    di
    pop de             ; get ret address from stack
    ld a,l             ; get value to write and read
    ld hl,#0xC000      ; start of RAM
    ld bc,#0x0020      ; 32 times 256 bytes = 8 KB

_write_loop_2:
    ld (hl),l
    inc hl
    djnz _write_loop_2

    dec c
    jr nz,_write_loop_2

// put a long delay after the write, so that if any bit changes we can get it

    ld bc,#0x0000      ; longest delay I can imagine ;)
_delay_loop_2:
    .db #0xFD
    ld l,a             ; ld iyl,a since this is a pretty slow operation
    .db #0xFD
    ld l,a             ; ld iyl,a since this is a pretty slow operation
    .db #0xFD
    ld l,a             ; ld iyl,a since this is a pretty slow operation

    djnz _delay_loop_2
    dec c
    jr nz,_delay_loop_2

    ld hl,#0xC000      ; start of RAM
    ld bc,#0x0020      ; 32 times 256 bytes = 8 KB

_read_loop_2:
    ld a,(hl)
    cp l               ; compare with value in l
    jr nz,_check_failed_2
    inc hl
    djnz _read_loop_2

    dec c
    jr nz,_read_loop_2

    ld l,#0
    push de            ; restore ret address into stack
    ei
    ret

_check_failed_2:
    ld l,#1
    push de            ; restore ret address into stack
    ei
    ret
  __endasm;
}

void print_test_result (unsigned char failed) {
  if (failed) {
    SMS_configureTextRenderer(-32+96*2);    // set text to RED
    printf ("FAILED!");
  } else {
    SMS_configureTextRenderer(-32+96);      // set text to GREEN
    printf ("OK!");
  }
  SMS_configureTextRenderer(-32);           // restore text to white
}

extern const unsigned char devkitSMS_font__tiles__1bpp[768];

void main (void) {
  SMS_VRAMmemset (XYtoADDR(0,0),0,32*24);                               // clear PNT
  SMS_load1bppTiles(devkitSMS_font__tiles__1bpp,0,sizeof(devkitSMS_font__tiles__1bpp),0,1);
  SMS_load1bppTiles(devkitSMS_font__tiles__1bpp,96,sizeof(devkitSMS_font__tiles__1bpp),0,2);
  SMS_load1bppTiles(devkitSMS_font__tiles__1bpp,96*2,sizeof(devkitSMS_font__tiles__1bpp),0,3);
  SMS_setBGPaletteColor (0, RGB(0,0,0));        // black
  SMS_setBGPaletteColor (1, RGB(3,3,3));        // white
  SMS_setBGPaletteColor (2, RGB(0,3,0));        // green
  SMS_setBGPaletteColor (3, RGB(3,0,0));        // red
  SMS_configureTextRenderer(-32);
  SMS_displayOn();

  SMS_setNextTileatXY (1,1);
  printf ("sverx's SMS memtest v%d.%02d",MAJ_VERSION,MIN_VERSION);
  SMS_setNextTileatXY (1,3);
  printf ("Testing system RAM:");
  SMS_setNextTileatXY (1,4);
  printf ("-all bits to zero...");
  print_test_result (ram_test (0x00));

  SMS_setNextTileatXY (1,5);
  printf ("-all bits to one ...");
  print_test_result (ram_test (0xFF));

  SMS_setNextTileatXY (1,6);
  printf ("-all bytes to $55...");
  print_test_result (ram_test (0x55));

  SMS_setNextTileatXY (1,7);
  printf ("-all bytes to $AA...");
  print_test_result (ram_test (0xAA));

  SMS_setNextTileatXY (1,8);
  printf ("-different values...");
  print_test_result (ram_test_2 ());

  SMS_setNextTileatXY (1,10);
  printf ("RAM tests completed");

  for (;;);

}

const __at (0x0066) unsigned char overwrite_NMI[2] = {0xED, 0x45};      // this overwrites devkitSMS NMI handler with 'RETN' (this module object should be last in linker order)

SMS_EMBED_SEGA_ROM_HEADER (9999,0);
SMS_EMBED_SDSC_HEADER_AUTO_DATE(MAJ_VERSION,MIN_VERSION,"sverx","SMS memtest","memtest for Sega Master System");

// coding started on Aug 13 2020 - dedicated to my dad (happy birthday!)
