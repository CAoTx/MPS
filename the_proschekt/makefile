# Quelldatei
FILE = main
# Toolchain
TOOLCHAIN = arm-eb63-elf-
# Compiler
COMPILER = gcc
# Linker/Binder
LINKER = ld
# Debugger
DEBUGGER = insight
# Optimierungsstufe
OPTI = 0

# Bauen
all:
# uebersetzen der Quelldatei
	$(TOOLCHAIN)$(COMPILER) -c -g -O$(OPTI) $(FILE).c -I ../h

# Erzeugen der Assemblerdatei aus der Quelldatei	
	$(TOOLCHAIN)$(COMPILER) -S -o$(OPTI) $(FILE).c -I ../h

# Erzeugen der benoetitgen Objektdateien
	$(TOOLCHAIN)$(COMPILER) -c -g -O$(OPTI) ../boot/swi.S -o swi.o -I ../h
	$(TOOLCHAIN)$(COMPILER) -c -g -O$(OPTI) ../boot/boot_ice.S -o boot_ice.o -I ../h
	
# Binden fuer die RAM-Version 
#	$(TOOLCHAIN)$(LINKER) -Ttext 0x02000000 -O$(OPTI) boot_ice.o swi.o $(FILE).o -o $(FILE).elf  /opt/arm-eb63-elf/lib/gcc/arm-eb63-elf/4.4.6/libgcc.a
#	$(TOOLCHAIN)$(LINKER) -Ttext 0x02000000 -O$(OPTI) boot_ice.o swi.o $(FILE).o -o $(FILE).elf
	$(TOOLCHAIN)$(LINKER) -Ttext 0x02000000 -O$(OPTI) boot_ice.o swi.o $(FILE).o -o $(FILE).elf  /home/fs/Vorlagen/mpstools/lib/gcc/arm-elf-eabi/4.4.6/libgcc.a
# Debugger starten
debug:
	$(TC)$(DEBUGGER) $(FILE).elf

# Aufraeumen
clean:
	rm *.o
	rm *.s
	rm *.elf
	rm *.rom

