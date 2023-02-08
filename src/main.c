
#define MAJ_VERSION  0
#define MIN_VERSION  30

#include <stdio.h>
#include "../SMSlib/SMSlib.h"

extern volatile unsigned int KeysStatus,PreviousKeysStatus;
extern unsigned char VDPReg[2];

/* ********************************************************************* */
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

/* ********************************************************************* */
#pragma save
#pragma disable_warning 85
unsigned char vram_test (unsigned char value) __z88dk_fastcall __naked {
  __asm
    ld a,l             ; get value to write and read
    ld hl,#0x4000      ; start of VRAM (write)
    rst #0x08          ; write VDP control port
    ld bc,#0x00BE      ; B = 0 and C = #0xBE (VDP data port)
    ld l,#64           ; 64 by 256 = 16 KB

_write_loop_vram:
    out (c),a
    djnz _write_loop_vram
    dec l
    jr nz,_write_loop_vram

// put a long delay after the write, so that if any bit changes we can get it

    ld bc,#0x0000      ; longest delay I can imagine ;)
_delay_loop_vram:
    .db #0xFD
    ld l,a             ; ld iyl,a since this is a pretty slow operation
    .db #0xFD
    ld l,a             ; ld iyl,a since this is a pretty slow operation
    .db #0xFD
    ld l,a             ; ld iyl,a since this is a pretty slow operation

    djnz _delay_loop_vram
    dec c
    jr nz,_delay_loop_vram

    ld hl,#0x0000      ; start of VRAM (read)
    rst #0x08          ; write VDP control port
    ld bc,#0x00BE      ; B = 0 and C = #0xBE (VDP data port)
    ld l,#64           ; 64 by 256 = 16 KB

_read_loop_vram:
    in h,(c)
    cp h               ; compare with written value
    jr nz,_check_failed_vram
    djnz _read_loop_vram
    dec l
    jr nz,_read_loop_vram

    ld l,#0
    ret

_check_failed_vram:
    ld l,#1
    ret
  __endasm;
}
#pragma restore

unsigned char vram_test_2 (void) __z88dk_fastcall __naked {
  __asm
    ld hl,#0x4000      ; start of VRAM (write)
    rst #0x08          ; write VDP control port
    ld bc,#0x00BE      ; B = 0 and C = #0xBE (VDP data port)
    ld hl,#64          ; H = 0, L = 64 (64 by 256 = 16 KB)

_write_loop_vram_2:
    out (c),h
    inc h
    djnz _write_loop_vram_2
    dec l
    jr nz,_write_loop_vram_2

// put a long delay after the write, so that if any bit changes we can get it

    ld bc,#0x0000      ; longest delay I can imagine ;)
_delay_loop_vram_2:
    .db #0xFD
    ld l,a             ; ld iyl,a since this is a pretty slow operation
    .db #0xFD
    ld l,a             ; ld iyl,a since this is a pretty slow operation
    .db #0xFD
    ld l,a             ; ld iyl,a since this is a pretty slow operation

    djnz _delay_loop_vram_2
    dec c
    jr nz,_delay_loop_vram_2

    ld hl,#0x0000      ; start of VRAM (read)
    rst #0x08          ; write VDP control port
    ld bc,#0x00BE      ; B = 0 and C = #0xBE (VDP data port)
    ld hl,#64          ; H = 0, L = 64 (64 by 256 = 16 KB)

_read_loop_vram_2:
    in a,(c)
    cp h               ; compare with written value
    jr nz,_check_failed_vram_2
    inc h
    djnz _read_loop_vram_2
    dec l
    jr nz,_read_loop_vram_2
    ld l,#0
    ret

_check_failed_vram_2:
    ld l,#1
    ret
  __endasm;
}

/* ********************************************************************* */

#pragma save
#pragma disable_warning 85
unsigned char sram_test (unsigned char value) __z88dk_fastcall __naked {
  __asm
    di
    pop de             ; get ret address from stack
    ld a,l             ; get value to write and read
    ld hl,#0x8000      ; start of SRAM
    ld bc,#0x0040      ; 64 times 256 bytes = 16 KB

_write_loop_3:
    ld (hl),a
    inc hl
    djnz _write_loop_3

    dec c
    jr nz,_write_loop_3

// put a long delay after the write, so that if any bit changes we can get it

    ld bc,#0x0000      ; longest delay I can imagine ;)
_delay_loop_3:
    .db #0xFD
    ld l,a             ; ld iyl,a since this is a pretty slow operation
    .db #0xFD
    ld l,a             ; ld iyl,a since this is a pretty slow operation
    .db #0xFD
    ld l,a             ; ld iyl,a since this is a pretty slow operation

    djnz _delay_loop_3
    dec c
    jr nz,_delay_loop_3

    .db #0xFD
    ld l,a             ; save written value in iyl
    ld hl,#0x8000      ; start of SRAM
    ld bc,#0x0040      ; 64 times 256 bytes = 16 KB

_read_loop_3:
    ld a,(hl)
    .db #0xFD
    cp l               ; compare with value in iyl
    jr nz,_check_failed_3
    inc hl
    djnz _read_loop_3

    dec c
    jr nz,_read_loop_3

    ld l,#0
    push de            ; restore ret address into stack
    ei
    ret

_check_failed_3:
    ld l,#1
    push de            ; restore ret address into stack
    ei
    ret
  __endasm;
}
#pragma restore

unsigned char sram_test_2 (void) __z88dk_fastcall __naked {
  __asm
    di
    pop de             ; get ret address from stack
    ld a,l             ; get value to write and read
    ld hl,#0x8000      ; start of SRAM
    ld bc,#0x0040      ; 64 times 256 bytes = 16 KB

_write_loop_4:
    ld (hl),l
    inc hl
    djnz _write_loop_4

    dec c
    jr nz,_write_loop_4

// put a long delay after the write, so that if any bit changes we can get it

    ld bc,#0x0000      ; longest delay I can imagine ;)
_delay_loop_4:
    .db #0xFD
    ld l,a             ; ld iyl,a since this is a pretty slow operation
    .db #0xFD
    ld l,a             ; ld iyl,a since this is a pretty slow operation
    .db #0xFD
    ld l,a             ; ld iyl,a since this is a pretty slow operation

    djnz _delay_loop_4
    dec c
    jr nz,_delay_loop_4

    ld hl,#0x8000      ; start of SRAM
    ld bc,#0x0040      ; 64 times 256 bytes = 16 KB

_read_loop_4:
    ld a,(hl)
    cp l               ; compare with value in l
    jr nz,_check_failed_4
    inc hl
    djnz _read_loop_4

    dec c
    jr nz,_read_loop_4

    ld l,#0
    push de            ; restore ret address into stack
    ei
    ret

_check_failed_4:
    ld l,#1
    push de            ; restore ret address into stack
    ei
    ret
  __endasm;
}

/* ********************************************************************* */

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

void init_display (void) {
  extern const unsigned char devkitSMS_font__tiles__1bpp[768];
  SMS_displayOff();
  SMS_VRAMmemset (XYtoADDR(0,0),0,32*28*2);                             // clear PNT
  SMS_load1bppTiles(devkitSMS_font__tiles__1bpp,0,sizeof(devkitSMS_font__tiles__1bpp),0,1);
  SMS_load1bppTiles(devkitSMS_font__tiles__1bpp,96,sizeof(devkitSMS_font__tiles__1bpp),0,2);
  SMS_load1bppTiles(devkitSMS_font__tiles__1bpp,96*2,sizeof(devkitSMS_font__tiles__1bpp),0,3);
  SMS_setBGPaletteColor (0, RGB(0,0,0));        // black
  SMS_setBGPaletteColor (1, RGB(3,3,3));        // white
  SMS_setBGPaletteColor (2, RGB(0,3,0));        // green
  SMS_setBGPaletteColor (3, RGB(3,0,0));        // red
  SMS_configureTextRenderer(-32);
  SMS_initSprites();
  SMS_copySpritestoSAT();
  SMS_displayOn();
  SMS_setNextTileatXY (1,1);
  printf ("sverx's SMS memtest v%d.%02d",MAJ_VERSION,MIN_VERSION);
  SMS_VDPturnOffFeature(VDPFEATURE_FRAMEIRQ);
}

void init_display_vram_tests (void) {
  init_display();
  SMS_setNextTileatXY (1,3);
  printf ("Testing system VIDEO RAM:");
  SMS_setNextTileatXY (1,4);
}

void main (void) {

  init_display();

/* *** RAM tests ******************************************************* */

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

  // more tests here!

  SMS_setNextTileatXY (1,10);
  printf ("RAM tests completed");

  SMS_setNextTileatXY (1,12);
  printf ("Press any controller button");
  SMS_setNextTileatXY (1,13);
  printf ("to start VIDEO RAM tests");
  SMS_setNextTileatXY (1,14);
  printf ("(screen will be off a few secs)");

  KeysStatus=0;
  PreviousKeysStatus=0;
  VDPReg[0]=0x04;
  VDPReg[1]=0x20;
  SMS_setFrameInterruptHandler(0);
  SMS_displayOn();
  SMS_VDPturnOnFeature(VDPFEATURE_FRAMEIRQ);

  while (!(SMS_getKeysPressed() & (PORT_A_KEY_UP|PORT_A_KEY_DOWN|PORT_A_KEY_LEFT|PORT_A_KEY_RIGHT|PORT_A_KEY_1|PORT_A_KEY_2|
                                   PORT_B_KEY_UP|PORT_B_KEY_DOWN|PORT_B_KEY_LEFT|PORT_B_KEY_RIGHT|PORT_B_KEY_1|PORT_B_KEY_2)));    // wait for any key press

/* *** VIDEO RAM tests ************************************************* */

  SMS_VDPturnOffFeature(VDPFEATURE_FRAMEIRQ);
  SMS_displayOff();

  if (vram_test(0x00)) {                             // notify if this test failed
    init_display_vram_tests();
    printf ("-all bits to zero...");
    print_test_result(1);
  } else if (vram_test(0xFF)) {                      // notify if this test failed
    init_display_vram_tests();
    printf ("-all bits to one ...");
    print_test_result(1);
  } else if (vram_test(0x55)) {                      // notify if this test failed
    init_display_vram_tests();
    printf ("-all bytes to $55...");
    print_test_result(1);
  } else if (vram_test(0xAA)) {                      // notify if this test failed
    init_display_vram_tests();
    printf ("-all bytes to $AA...");
    print_test_result(1);
  } else if (vram_test_2()) {                        // notify if this test failed
    init_display_vram_tests();
    printf ("-different values...");
    print_test_result(1);

  // more tests here!

  } else {                                           // notify all tests were OK!
    init_display_vram_tests();
    printf ("-all tests       ...");
    print_test_result(0);

    SMS_setNextTileatXY (1,10);
    printf ("VIDEO RAM tests completed");

    SMS_setNextTileatXY (1,12);
    printf ("Press any controller button");
    SMS_setNextTileatXY (1,13);
    printf ("to start cart SRAM tests");
  }

  KeysStatus=0;
  PreviousKeysStatus=0;
  SMS_setFrameInterruptHandler(0);
  SMS_VDPturnOnFeature(VDPFEATURE_FRAMEIRQ);

  while (!(SMS_getKeysPressed() & (PORT_A_KEY_UP|PORT_A_KEY_DOWN|PORT_A_KEY_LEFT|PORT_A_KEY_RIGHT|PORT_A_KEY_1|PORT_A_KEY_2|
                                   PORT_B_KEY_UP|PORT_B_KEY_DOWN|PORT_B_KEY_LEFT|PORT_B_KEY_RIGHT|PORT_B_KEY_1|PORT_B_KEY_2)));    // wait for any key press


/* *** SRAM tests ******************************************************* */

  init_display();

  SMS_VDPturnOffFeature(VDPFEATURE_FRAMEIRQ);

  SMS_enableSRAMBank(0);  // activate SRAM bank 0

  SMS_setNextTileatXY (1,3);
  printf ("Testing cart SRAM (bank 0):");
  SMS_setNextTileatXY (1,4);
  printf ("-all bits to zero...");
  print_test_result (sram_test (0x00));

  SMS_setNextTileatXY (1,5);
  printf ("-all bits to one ...");
  print_test_result (sram_test (0xFF));

  SMS_setNextTileatXY (1,6);
  printf ("-all bytes to $55...");
  print_test_result (sram_test (0x55));

  SMS_setNextTileatXY (1,7);
  printf ("-all bytes to $AA...");
  print_test_result (sram_test (0xAA));

  SMS_setNextTileatXY (1,8);
  printf ("-different values...");
  print_test_result (sram_test_2 ());

  // more tests here!

  SMS_enableSRAMBank(1);  // activate SRAM bank 1

  SMS_setNextTileatXY (1,10);
  printf ("Testing cart SRAM (bank 1):");
  SMS_setNextTileatXY (1,11);
  printf ("-all bits to zero...");
  print_test_result (sram_test (0x00));

  SMS_setNextTileatXY (1,12);
  printf ("-all bits to one ...");
  print_test_result (sram_test (0xFF));

  SMS_setNextTileatXY (1,13);
  printf ("-all bytes to $55...");
  print_test_result (sram_test (0x55));

  SMS_setNextTileatXY (1,14);
  printf ("-all bytes to $AA...");
  print_test_result (sram_test (0xAA));

  SMS_setNextTileatXY (1,15);
  printf ("-different values...");
  print_test_result (sram_test_2 ());

  // more tests here!

  SMS_disableSRAM(); // deactivate SRAM

  SMS_setNextTileatXY (1,17);
  printf ("Cart SRAM tests completed");

  for (;;);

}

const __at (0x0066) unsigned char overwrite_NMI[2] = {0xED, 0x45};      // this overwrites devkitSMS NMI handler with 'RETN' (this module object should be last in linker order)

SMS_EMBED_SEGA_ROM_HEADER (9999,0);
SMS_EMBED_SDSC_HEADER_AUTO_DATE(MAJ_VERSION,MIN_VERSION,"sverx","SMS memtest","memtest for Sega Master System");

// coding started on Aug 13 2020 - dedicated to my dad (happy birthday!)
