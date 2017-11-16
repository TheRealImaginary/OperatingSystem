#!/bin/bash
#A Shell Script for running OS Project

#compile "kernel.c" code into file "kernel.o"
bcc -ansi -c -o kernel.o kernel.c

#compile "kernel.asm" code from x86 assembly code
as86 kernel.asm -o kernel_asm.o

#load both the compiled "C" code and the assembly code into file "kernel"
ld86 -o kernel -d kernel.o kernel_asm.o

#compile "shell.c" code into file "shell.o"
bcc -ansi -c -o shell.o shell.c

#compile "lib.asm" code from x86 assembly code
as86 lib.asm -o lib_asm.o

#load both the compiled "C" code and the assembly code into file "shell"
ld86 -o shell -d shell.o lib_asm.o

#copy kernel code into floppy image in sector 3
dd if=kernel of=floppya.img bs=512 conv=notrunc seek=3

#copy message.txt into floppy image in sector 30
dd if=message.txt of=floppya.img bs=512 count=1 seek=30 conv=notrunc

#loads map and directory (File System)
dd if=map.img of=floppya.img bs=512 count=1 seek=1 conv=notrunc
dd if=dir.img of=floppya.img bs=512 count=1 seek=2 conv=notrunc

#load message.txt in floppy (For Testing Read Sector)
./loadFile message.txt 
#load tstprg (For Testing Execute Program)
./loadFile tstprg
# load tetsprg2 (For Testing Terminate)
./loadFile tstpr2
#load shell (For Testing and using the shell)
./loadFile shell
#load hello1 (For Testing two processes with hello2)
./loadFile hello1
#load hello2 (For Testing two processes with hello1)
./loadFile hello2
#load phello (For Testing Mutitasking with shell)
./loadFile phello

#Run
echo c | bochs -f config.bxrc