# OperatingSystem
A Tiny Operating System that is intended to fit on a 
![equation](http://www.sciweavers.org/tex2img.php?eq=3%5Cfrac%7B1%7D%7B2%7D&bc=White&fc=Black&im=jpg&fs=12&ff=arev&edit=0) floppy disk
and to be bootable on any x86 PC. OS and Kernel are written using C and Assembly.

# Tools
* Bochs x86 Processor Simulator: `sudo apt-get install bochs`
and `sudo apt-get install bochs-x`.
* bcc (Bruce Evan’s C Compiler): a 16-bit C Compiler. `sudo apt-get install bcc`.
* as86, ld86: A 16 bit assembler and linker that typically comes with bcc. `sudo apt-get install bin86`.
* gcc: The standard 32-bit GNU C compiler. This generally comes with Unix.
* nasm: The netwide assembler. `sudo apt-get install nasm`.

# Features
The OS supports reading, writing and deleting from sectors on the disk. Processes can be created and scheduled to run on the processor to give an illusion of parallelism.

# Run
Type `./compileOS.sh` to run and try the OS.
