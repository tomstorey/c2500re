# Serial Bootloader

This directory contains a very basic serial bootloader utility to help make development of your own applications easier, removing the need to constantly remove, program and re-install ROMs.

Instead, your code can be transferred over the serial port and executed from the routers memory.

## Get started

### Toolchain
I've set this up to work with my m68k bare metal "toolchain of sorts", details of which can be found [here](https://github.com/tomstorey/m68k_bare_metal). Follow the instructions in that repo to configure an environment which can be used to compile code for the m68k architecture.

### Build the source
From here you should be able to run the following commands to build a ROM image:

```
make crt
make && make rom
```

This results in a file called `bmbinary.rom` which must be split as odd (FW1) and even (FW2) bytes between two physical ROMs. This can often be done in the software for your favourite programmer, or you can use the included `roms.py` script to do this for you (requires Python 3).

By default, `roms.py` is called via the `make rom` command, but you can remove this from the Makefile and call it manually if desired:

`python3 roms.py`

and it will split bmbinary.rom into two files called `fw1` and `fw2`, which are then programmed and installed into their respective ROMs and sockets.

### Run it
Make sure that the jumpers beside the boot ROM sockets have been adjusted accordingly and turn the power on. The OK LED should be illuminated indicating that the bootloader is ready.

### Use it
`loader.py` is used to transfer files from your PC to the router. You can obtain some help text using `python3 loader.py --help` to see the available options.

A basic example is as follows:

`python3 loader.py -b0x100000 -e0x100400 bmbinary.rom`

This causes the contents of bmbinary.rom to be loaded starting at address 0x100000 and will then execute code from 0x100400.

## Support
I'll do my best to help you out if you run into difficulties, please raise an issue against this repo.
