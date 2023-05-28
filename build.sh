avr-gcc -Wcpp -O2 -Wimplicit-function-declaration -S \
        *.c error/*.c hardware/*/*.c

mv *.s built/asm/