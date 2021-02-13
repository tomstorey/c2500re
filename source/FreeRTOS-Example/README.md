
# FreeRTOS on Cisco 2500 Example

This directory contains a standalone FreeRTOS application which implements a single task that will blink the OK LED on a Cisco 2500 series router.

## Get started
### Toolchain
I've set this up to work with my m68k bare metal "toolchain of sorts", details of which can be found [here](https://github.com/tomstorey/m68k_bare_metal). Follow the instructions in that repo to configure an environment which can be used to compile code for the m68k architecture.

### FreeRTOS with m68k port
To use this example, you will need to clone my FreeRTOS kernel repository which contains a Motorola 68000 port into this directory by running:

`git clone https://github.com/tomstorey/FreeRTOS-Kernel.git`

Then, move the included `FreeRTOSConfig.h` into `FreeRTOS-Kernel/include`.

### Build the source
From here you should be able to run the following commands to build a ROM image:

`make crt`
`make && make rom`

This results in a file called `bmbinary.rom` which must be split as odd (FW1) and even (FW2) bytes between two physical ROMs. This can often be done in the software for your favourite programmer, or you can use the included `roms.py` script to do this for you (requires Python 3). Simply call the script using the following command:

`python3 roms.py`

and it will split bmbinary.rom into two files called `fw1` and `fw2`, which are then programmed and installed into their respective ROMs and sockets.

### Run it
Make sure that the jumpers beside the boot ROM sockets have been adjusted accordingly and turn the power on. The OK LED should be blinking at a rate of 1hz!

## Support
I'll do my best to help you out if you run into difficulties, please raise an issue against this repo.
