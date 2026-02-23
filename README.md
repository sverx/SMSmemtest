SMSmemtest
==========

SMSmemtest is a program for the SEGA Master System and SEGA Game Gear (in SMS mode) to test the console's internal and video memory, and cartridge memory if present.

First the program checks for RAM presence. If the console RAM is missing or completely not working, the background will turn red (but probably only a few BIOSes can even get to start the program if no RAM is there). Then it performs some basic tests on the internal 8 KiB system RAM, on the 16 KiB Video RAM and also on the (up to 32KiB) cartridge RAM.

Can be compiled using latest devkitSMS and the modified crt0 module in this project using:


```
# compile the source:
sdcc -c -mz80 --peep-file peep-rules.txt main.c

# link with devkitSMS' SMSlib and with modified crt0 module
sdcc -o SMSmemtest.ihx -mz80 --no-std-crt0 --data-loc 0xC000 crt0_memtest.rel ../SMSlib/SMSlib.lib main.rel

# create final ROM (it will create a 32 KiB ROM - if you need a 64 KiB ROM please run 'ihx2sms -pm SMSmemtest.ihx SMSmemtest.sms' instead)
ihx2sms SMSmemtest.ihx SMSmemtest.sms
```

