target=atmega328p
fcpu=16000000UL

printf "\033[36m BUILD \033[35mSTEP 1 |\033[37;m Compiling sources\n"
#avr-gcc -Wcpp -O2 -Wimplicit-function-declaration -S *.c error/*.c hardware/*/*.c 2> buildlog.txt
avr-gcc -g -Wall -Os -lc -O2 -DF_CPU=$fcpu -mmcu=$target -o built/bios.o  *.c hardware/*.c hardware/*/*.c error/*.c 2> buildlog.txt
printf "              | INFO: Log is saved to buildlog.txt. Please report any errors.\n"

printf "\033[36m BUILD \033[35mSTEP 2 |\033[37;m Move assembly files \033[37m(diagnostical purposes)\n"
mv *.s built/asm/
rm *.tmp