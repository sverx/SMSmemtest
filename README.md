SMSmemtest
==========

SMSmemtest is a program for the SEGA Master System and SEGA Game Gear (in SMS mode) to test the console's internal RAM and Video RAM memory and cartridge RAM memory, if present.

It performs some basic tests on all the internal 8 KiB system RAM, on the 16 KiB Video RAM and also on the (up to 32KiB) cartridge RAM reporting results after that.

Can be compiled using latest devkitSMS with:


```
# compile the source:
sdcc -c -mz80 --peep-file peep-rules.txt main.c

# link with devkitSMS' SMSlib and crt0 module
sdcc -o SMSmemtest.ihx -mz80 --no-std-crt0 --data-loc 0xC000 ../crt0/crt0_sms.rel ../SMSlib/SMSlib.lib main.rel

# create final ROM (out will create a 32 KiB ROM - if you need a 64 KiB ROM please run 'ihx2sms -pm SMSmemtest.ihx SMSmemtest.sms' instead)
ihx2sms SMSmemtest.ihx SMSmemtest.sms
```

