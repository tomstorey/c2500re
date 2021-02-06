# Cisco 2500 Series Reverse Engineering
Documented here is my effort to reverse engineer enough detail about the Cisco 2500 series router platform in order to be able to run my own code.

A particular goal was to get FreeRTOS running, which involved creating a new port.

### Rationale
These days, a Cisco 2500 doesnt make much of a router, at least not for modern broadband speeds. In terms of processing power it is somewhat limited also.

But, with a Motorola 68EC030 on board, and a selection of other peripherals including an ethernet controller, it makes for an interesting platform that one might use to "hack about", and presents a ready-built platform on which to do so. They are readily available on ebay for very little money, and have a sturdy steel 19" rack mountable case.

At the time of writing, Coronavirus is also (still) a thing, and this was a good project to keep me occupied and distracted from the general goings on of the world until it passes.

### Platform Brief
The Cisco 2501 which I used during this project has the following notable features:

  * A Motorola 68EC030 CPU clocked at 20MHz
  * A Philips SCN2681 dual UART (for the console and aux ports)
  * A Hitachi HD46570 serial communications controller (two serial WAN ports)
  * A AMD AM79C90 ("LANCE") ethernet controller with AUI physical attachment
  * A 72 pin SIMM slot accepting up to 16Mbyte of DRAM
  * 2x 80 pin SIMM slots accepting up to 8Mbyte of FLASH each
  * 2x 32 pin PLCC sockets for boot ROM
  * A 32Kbyte EEPROM (NVRAM) storing the routers configuration 

These features will be addressed in more detail in their respective sections below.

Two Cisco branded chips on board seem to be quite central to the operation of the router. Almost all memories and peripherals are routed to/through these two chips in some way or another. The chips also appear to be largely responsible for generating all of the control signals to the peripherals and CPU, such as interrupts, bus error signals, etc.

Fortunately, they do not seem to "get in the way" very much, although there are some registers contained within that need to be appropriately configured to ensure proper operation of the router. Largely it seems that they provide muxing and arbitration between all of the peripherals and the CPU, including generating DRAM refresh signals.

## Memories

### Bootroms
Two 32 pin PLCC sockets hold the bootrom code. This includes a basic monitor which can be used to probe around the system, and also a minimal IOS image, perhaps enough to run the router if the IOS image in flash is not present or damaged.

The ROMs on my board are 8mbit in size each:

  * FW1 odd bytes
  * FW2 even bytes

The ROMs present a 16 bit wide data path from which the CPU can read instructions and data.

Bit order in these two ROMs was found to be reversed: the least significant bit of a ROM corresponds with its most significant bit as seen by the CPU. A small script was used to reverse the order of the bits and combine odd/even bytes into a single image which could then be loaded into Ghidra for disassembly and analysis.

Bootroms are located at 0x01000000 in the memory map.

**Special note:** A Motorola 68000 CPU expects to read two long words forming the initial stack pointer and PC beginning from address 0, therefore, the bootroms are mapped to address 0 for the first two long word reads, after which they are remapped to 0x01000000 for code execution.

8mbit ROMs are particularly interesting in a PLCC32 package. What would normally be the Write Enable pin is replaced with an additional address line. To use smaller sized ROMs in these sockets, two jumpers must be modified to map this signal correctly. Beside the FW2 socket are two 3 pin headers which can be used to configure the function of pin 31:

  * bridging the AB positions configures the sockets for 8mbit ROMs
  * bridging the A position with the unlabelled position configures the sockets for smaller sized ROMs

The arrangement of address lines on pins 1 and 31 varies slightly between 8mbit ROMs and the 4mbit ROMs I have been using for testing (SST39SF040):

<table>
    <thead>
        <tr>
            <th>Size</th><th>Pin 1</th><th>Pin 31</th>
        </tr>
    </thead>
    <tbody>
        <tr>
        	<td>8mbit</td>
        	<td>A19</td>
        	<td>A18</td>
        </tr>
        <tr>
        	<td>4mbit</td>
        	<td>A18</td>
        	<td>WE/</td>
        </tr>
    </tbody>
</table>

I haven't yet tested enough code to get to the 18th bit region of code to determine if this is actually an issue, but from first glance it looks like a compatible ROM will need to be found, the ROM image rearranged appropriately, or code size limited to 1mbit per ROM (2mbit total).

*See Memory Configuration Register section for details about configuring the valid address window for the bootrom sockets.*

### NVRAM
NVRAM is a 32Kbyte EEPROM which holds the routers configuration register (ala 0x2102) and configuration (startup config).

The NVRAM is soldered to the board as opposed to being socketed. For your own purposes, you might completely ignore this chip, or you might write code to erase and re-program it yourself.

The NVRAM chip is located at 0x02000000 in the memory map.

The NVRAM presents an 8 bit data path to the CPU, thus providing only byte reads/writes.

### DRAM
One 72 pin SIMM socket provides for a maximum of 16Mbyte of DRAM. Refreshing is handled automatically by one of the Cisco branded chips.

DRAM is located from address 0 in the memory map.

Although the bootroms are seemingly mapped to address 0 at least initially, addresses 0-7 are readable and writeable thereafter as part of the RAM itself.

*See Memory Configuration Register section for details about configuring the valid address window for the DRAM.*

### Flash
Two 80 pin SIMM sockets provide for 8Mbyte of Flash each, totalling 16Mbyte of persistent storage.

Flash is located from address 0x03000000 in the memory map.

Presence detect pins of the Flash modules are partially used by one of the Cisco branded chips to enable each of the sockets:

  * if PD6 of CODE0 is pulled low, address window 0x03000000-037FFFFF is enabled
  * if CODE0 is enabled, and PD6 of CODE1 is pulled low, address window 0x03800000-03FFFFFF is enabled

The CODE1 socket cannot be enabled if the CODE0 socket is not enabled.

Attempting to read or write to an address in a window that is not enabled generates a bus error exception.

A word size register was identified at address 0x0211000A which seems to indicate the presence of flash modules in each socket.

**Flash Presence 0x0211000A**
<table>
    <thead>
        <tr>
            <th>Bit 15</th><th></th><th></th><th></th><th></th><th></th><th></th><th>8</th>
            <th>7</th><th></th><th></th><th></th><th></th><th></th><th></th><th>Bit 0</th>
        </tr>
    </thead>
    <tbody>
        <tr>
        	<td></td>
        	<td></td>
        	<td></td>
            <td align="center">R-?</td>
            <td align="center">R-?</td>
        	<td></td>
        	<td></td>
            <td align="center">R-?</td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        </tr>
        <tr>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td>C0P6</td>
        	<td>C1P6</td>
        	<td></td>
        	<td></td>
        	<td>C0P4</td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        </tr>
    </tbody>
</table>

Bit 12: C0P6: CODE0 PD6<br>
&nbsp;&nbsp;&nbsp;&nbsp;0: PD6 is grounded by flash module<br>
&nbsp;&nbsp;&nbsp;&nbsp;1: PD6 is high - not connected or flash module not present<br>
Bit 11: C1P6: CODE1 PD6<br>
&nbsp;&nbsp;&nbsp;&nbsp;0: PD6 is grounded by flash module<br>
&nbsp;&nbsp;&nbsp;&nbsp;1: PD6 is high - not connected or flash module not present<br>
Bit 8: C0P4: CODE0 PD4<br>
&nbsp;&nbsp;&nbsp;&nbsp;0: PD4 is grounded by flash module<br>
&nbsp;&nbsp;&nbsp;&nbsp;1: PD4 is high - not connected or flash module not present

## Peripherals

### Dual UART
A Philips SCN2681 provides two serial ports. Each serial port has an RS232 level converter and is presented on an 8P8C socket. Channel A corresponds to the Console port while channel B is the Aux port.

A 3.6864MHz external oscillator is used to derive all of the common baud rates.

The UART is located at address 0x02120100 in the memory map. It generates interrupts at IRQ 5.

16 registers are either readable or writeable from this address as per the datasheet.

Below is a simple assembly routine to configure the Console port for 9600,8,N,1:

```
    clr.b   0x02120105
    move.b  #0x10, 0x02120102
    move.b  #0x13, 0x02120100
    move.b  #0x7, 0x02120100
    move.b  #0xE0, 0x02120104
    move.b  #0xBB, 0x02120101
    move.b  #0x5, 0x02120102
    clr.b   0x0212010D
    move.b  #0x1, 0x0212010E
```

Data can then be written to the transmit holding register or read from the receive holding register, both located at address 0x02120103.

Refer to the SCN2681 datasheet for more details.

### Watchdog
One of the Cisco branded chips implements a general purpose timer, and also a watchdog.

The timer has a prescaler register of word size located at address 0x02120070. It generates interrupts at IRQ 7.

The watchdog also has a prescaler register of word size located at address 0x02120050, and a byte size control register located at 0x02120040.

Analysis of the disassembled bootrom code showed that Cisco writes the timer prescaler with a value of 0xFA0 (4000) which generates interrupts at a rate of 4ms. This reveals that the base frequency for the timer is 1MHz. Following this they write a value of 0x3E80 (16000) to the watchdog prescaler register and then clear the watchdog control register which would result in the watchdog being disabled. At this point then then delay for a bit and observe a memory located to see if it has changed, indicating that the timer is running.

When testing my own code, following a similar method did not enable the timer unless the watchdog was also enabled, therefore I may have missed something that needs to be initialised first.

Reading the timer prescaler will give you the current count, not the period constant.

**Timer Prescaler 0x02120070**
<table>
    <thead>
        <tr>
            <th>Bit 15</th><th></th><th></th><th></th><th></th><th></th><th></th><th>8</th>
            <th>7</th><th></th><th></th><th></th><th></th><th></th><th></th><th>Bit 0</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td colspan="16" align="center">R/w-0</td>
        </tr>
        <tr>
            <td colspan="16" align="center">TIMERPS</td>
        </tr>
    </tbody>
</table>

Bit 15-0: TIMERPS: Timer prescaler<br>
&nbsp;&nbsp;&nbsp;&nbsp;0: Input frequency divided by 65536<br>
&nbsp;&nbsp;&nbsp;&nbsp;1: Input frequency divided by 1<br>
&nbsp;&nbsp;&nbsp;&nbsp;2: Input frequency divided by 2<br>
&nbsp;&nbsp;&nbsp;&nbsp;...<br>
&nbsp;&nbsp;&nbsp;&nbsp;65535: Input frequency divided by 65535

The watchdog prescaler register does not appear to be linear like the timer prescaler, so it either operates on a different frequency or has additional delay built in on top of the prescaler value. Some values I tested are provided below with approximate timeout periods.

It also seems that it may be a write-once register. In my own tests I was not able to modify the value of the watchdog prescaler once the value had been set.

**Watchdog Prescaler 0x02120050**
<table>
    <thead>
        <tr>
            <th>Bit 15</th><th></th><th></th><th></th><th></th><th></th><th></th><th>8</th>
            <th>7</th><th></th><th></th><th></th><th></th><th></th><th></th><th>Bit 0</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td colspan="16" align="center">R/w-0</td>
        </tr>
        <tr>
            <td colspan="16" align="center">WDTO</td>
        </tr>
    </tbody>
</table>

Bit 15-0: WDTO: Watchdog timeout<br>
&nbsp;&nbsp;&nbsp;&nbsp;0x0000: ~81.8ms<br>
&nbsp;&nbsp;&nbsp;&nbsp;...<br>
&nbsp;&nbsp;&nbsp;&nbsp;0x4000: ~32.57ms<br>
&nbsp;&nbsp;&nbsp;&nbsp;...<br>
&nbsp;&nbsp;&nbsp;&nbsp;0x8000: ~49.00ms<br>
&nbsp;&nbsp;&nbsp;&nbsp;...<br>
&nbsp;&nbsp;&nbsp;&nbsp;0xFFFF: ~81.75ms

**Watchdog Control Register 0x02120040**
<table>
    <thead>
        <tr>
            <th>Bit 7</th><th></th><th></th><th></th><th></th><th></th><th></th><th>Bit 0</th>
        </tr>
    </thead>
    <tbody>
        <tr>
        	<td></td>
            <td align="center">R/W-0</td>
            <td></td>
            <td></td>
            <td></td>
            <td></td>
            <td></td>
            <td></td>
        </tr>
        <tr>
            <td></td>
            <td align="center">WDTEN</td>
            <td></td>
            <td></td>
            <td></td>
            <td></td>
            <td></td>
            <td></td>
        </tr>
    </tbody>
</table>

Bit 6: WDTEN: Watchdog timer enable<br>
&nbsp;&nbsp;&nbsp;&nbsp;0: watchdog disabled<br>
&nbsp;&nbsp;&nbsp;&nbsp;1: watchdog enabled

The following code example shows how to configure the timer and enable the watchdog timer. The watchdog may be enabled and disabled at will by setting or clearing the WDTEN bit.

```
    /* Configure timer prescaler value */
    move.w  #0xFA0, 0x02120070

    /* Configure the WDT and enable it */
    move.w  #0x3E80, 0x02120050
    ori.b   #0x40, 0x02120040
```

Once the watchdog is enabled, it will reset the system once it times out. The following code example shows how to reset the watchdog timer.

```
    /* Reset the watchdog */
    move.w  #0x3E80, 0x02120050
    ori.b   #0x40, 0x02120040
```

### Hitachi HD64570 Serial Communications Adapter
The operation of the HD64570 is quite complicated, and has an extensive datasheet, so I am covering here some general notes about some of its other useful features.

The peripheral is located at address 0x02132000 in the memory map. It generates interrupts at IRQ 4.

The HD64570 includes 4 timer channels which can be used to generate interrupts to the CPU. A unique advantage of these timer channels being at IRQ 4 is that they can be masked, which means they may be used to generate, for example, a tick interrupt for FreeRTOS.

The input clock frequency to the HD64570 is 10MHz, and this is divided by 8 internally to produce the Base Clock for the timer channels, giving them an input frequency of 1.25MHz. Therefore, a prescaler of 5000 would produce a 4ms interval.

The datasheet contains extensive notes and details about the configuration and operation of these timers.

*See Peripheral Access Register section for steps required to be able to access the HD64570.*

### NOVRAM
There is a X24C44 256bit, non-volatile SRAM located on the board, arranged as 16x 16 bit words.

A byte sized register located at address 0x02110060 provides means to implement a software bit-bangable interface to read and write the NOVRAM.

Within the NOVRAM, addresses 2-7 were identified to contain the MAC address of the ethernet interface.

The DI and DO pins are wired together, resulting in a 3 wire interface.

**NOVRAM Bit Bang Register 0x02110060**
<table>
    <thead>
        <tr>
            <th>Bit 7</th><th></th><th></th><th></th><th></th><th></th><th></th><th>Bit 0</th>
        </tr>
    </thead>
    <tbody>
        <tr>
        	<td></td>
            <td></td>
            <td></td>
            <td></td>
            <td align="center">R/W-0</td>
            <td align="center">R/W-0</td>
            <td align="center">R/W-0</td>
            <td align="center">R-?</td>
        </tr>
        <tr>
            <td></td>
            <td></td>
            <td></td>
            <td></td>
            <td align="center">CE</td>
            <td align="center">SK</td>
            <td align="center">DO</td>
            <td align="center">DI</td>
        </tr>
    </tbody>
</table>

Bit 3: CE: Chip Enable/Select<br>
&nbsp;&nbsp;&nbsp;&nbsp;0: NOVRAM is idle<br>
&nbsp;&nbsp;&nbsp;&nbsp;1: NOVRAM is addressed<br>
Bit 2: SK: Serial Clock<br>
&nbsp;&nbsp;&nbsp;&nbsp;0: Clock signal is idle<br>
&nbsp;&nbsp;&nbsp;&nbsp;1: Clock signal is active<br>
Bit 1: DO: Data Out<br>
&nbsp;&nbsp;&nbsp;&nbsp;0: Writing a zero<br>
&nbsp;&nbsp;&nbsp;&nbsp;1: Writing a one<br>
Bit 0: DI: Data In<br>
&nbsp;&nbsp;&nbsp;&nbsp;0: Reading a zero<br>
&nbsp;&nbsp;&nbsp;&nbsp;1: Reading a one


### LANCE Ethernet Controller
TODO

## Configuration and Control Registers
There are a number of registers that I identified by analysing the disassembled bootrom, and have been able to determine their function or make reasonably educated guesses about.

### System Control Register
So called because I saw references to the "SCR" in parts of the disassembled bootrom, and this seems to be a fitting name.

This word size register is located at address 0x02110000.

A notable feature is that it enables control of the "OK LED" which is located next to the console/aux ports. Blinkenlights!

**System Control Register 0x02110000**
<table>
    <thead>
        <tr>
            <th>Bit 15</th><th></th><th></th><th></th><th></th><th></th><th></th><th>8</th>
            <th>7</th><th></th><th></th><th></th><th></th><th></th><th></th><th>Bit 0</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td align="center" colspan="8">R-?</td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td align="center">R/W-0</td>
        	<td align="center">R/W-0</td>
        	<td align="center">R/W-0</td>
        	<td></td>
        	<td align="center">R/W-1</td>
        </tr>
        <tr>
        	<td align="center" colspan="8">Parity test result</td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td align="center">LED</td>
        	<td align="center">PTRA</td>
        	<td align="center">PTEN</td>
        	<td></td>
        	<td align="center">BEV</td>
        </tr>
    </tbody>
</table>

Bits 15-8: Parity test result<br>
Bit 4: LED: Control of the "OK" LED<br>
&nbsp;&nbsp;&nbsp;&nbsp;0: LED is off<br>
&nbsp;&nbsp;&nbsp;&nbsp;1: LED is on<br>
Bit 3: PTRA: Parity test result acquire<br>
&nbsp;&nbsp;&nbsp;&nbsp;0: ?<br>
&nbsp;&nbsp;&nbsp;&nbsp;1: Result acquired on next read<br>
Bit 2: PTEN: Parity test enable<br>
&nbsp;&nbsp;&nbsp;&nbsp;0: Test is disabled<br>
&nbsp;&nbsp;&nbsp;&nbsp;1: Test is enabled<br>
Bit 0: BEV: Bus Error Vector<br>
&nbsp;&nbsp;&nbsp;&nbsp;This bit is set on reset, and must be cleared by the user. Exact function unknown.

### Memory Configuration Register
This is a name I have come up with for this register, as its primary purpose seems to be to configure the valid address windows for DRAM and the bootroms.

This word size register is located at address 0x02110002.

In the bootroms, the sizes of the memory and ROMs are determined and this register is configured appropriately. Afterwards, attempting to read or write to addresses outside of the valid windows results in a bus error.

Once set, subsequent attempts to modify the size of RAM resulted in a crash.

The ROMSZ field in one setting seemd capable of supporting 1Mbit or 16Mbit ROMs according to bootsom disassembly, although the exact mechanism for making this work is unknown as there doesnt physically seem to be enough pins to support such an address space on a PLCC32 package.

**Memory Configuration Register 0x02110002**
<table>
    <thead>
        <tr>
            <th>Bit 15</th><th></th><th></th><th></th><th></th><th></th><th></th><th>8</th>
            <th>7</th><th></th><th></th><th></th><th></th><th></th><th></th><th>Bit 0</th>
        </tr>
    </thead>
    <tbody>
        <tr>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td align="center" colspan="2">R/W-0</td>
        	<td align="center" colspan="2">R/W-0</td>
        	<td></td>
        	<td></td>
        </tr>
        <tr>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td align="center" colspan="2">RAMSZ</td>
        	<td align="center" colspan="2">ROMSZ</td>
        	<td></td>
        	<td></td>
        </tr>
    </tbody>
</table>

Bits 15-8: Unknown purpose<br>
Bits 5-4: RAMSZ: RAM size<br>
&nbsp;&nbsp;&nbsp;&nbsp;00: 16MB - valid address range 0x00000000-00FFFFFF<br>
&nbsp;&nbsp;&nbsp;&nbsp;01: 4MB - valid address range 0x00000000-003FFFFF<br>
&nbsp;&nbsp;&nbsp;&nbsp;10: 2MB - valid address range 0x00000000-001FFFFF<br>
&nbsp;&nbsp;&nbsp;&nbsp;11: 8MB - valid address range 0x00000000-007FFFFF<br>
Bits 3-2: ROMSZ: ROM size<br>
&nbsp;&nbsp;&nbsp;&nbsp;00: 8Mbit (2MB) - valid address range 0x01000000-011FFFFF<br>
&nbsp;&nbsp;&nbsp;&nbsp;01: 4Mbit (1MB) - valid address range 0x01000000-010FFFFF<br>
&nbsp;&nbsp;&nbsp;&nbsp;10: 2Mbit (512KB) - valid address range 0x01000000-0107FFFF<br>
&nbsp;&nbsp;&nbsp;&nbsp;11: 1Mbit (256KB) or 16Mbit? - valid address range 0x01000000-013FFFFF<br>
Bit 0: Unknown purpose<br>
&nbsp;&nbsp;&nbsp;&nbsp;This bit is set in the bootrom code, but always reads as zero and doesn't seem to matter if it is set or not.

### Peripheral Access Register
The exact purpose of this register is still somewhat unknown. At times in the disassembly I see that it is written to with a value of 0x3F, followed by a small delay, and then cleared.

This is a word size register located at 0x02110004.

In my own testing, writing to this register was required before it was possible to interact with the HD64570. At time of writing I havent yet tried to interact with the LANCE ethernet controller, but presumably it will also enable access to it as well. The value written to this register did not seem to matter, I tried individual bits, but any combination, even writing zero seemed to enable access. Presumably it is safer to follow Cisco's own example.

**Peripheral Access Register 0x02110004**
<table>
    <thead>
        <tr>
            <th>Bit 15</th><th></th><th></th><th></th><th></th><th></th><th></th><th>8</th>
            <th>7</th><th></th><th></th><th></th><th></th><th></th><th></th><th>Bit 0</th>
        </tr>
    </thead>
    <tbody>
        <tr>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td align="center" colspan="6">R/W-0</td>
        </tr>
        <tr>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td></td>
        	<td align="center" colspan="6">?</td>
        </tr>
    </tbody>
</table>

Bits 5-0: Exact purpose unknown

In the bootrom disassembly, the following code is used to write to this register and initialise peripheral access:

```
    /* Enable access to peripherals */
    ori.w   #0x3F, 0x02110004
    nop
    nop
    nop
    nop
    andi.w  #0xFFC0, 0x02110004
```

### Other Registers
There are potentially some other registers which were readable and others writable, but which I have not yet found any references to in the bootrom disassembly so their purpose is unknown:

* 0x02110006: 0000 (read only)
* 0x02110008: 2F02 in bootloader, FF28 when IOS is running
* 0x0211000a: 0000 (read only)
* 0x02110010: 0000 (writeable)
* 0x02110012: 0000 (writeable)
* 0x02110014: 0000 (writeable)
* 0x02110016: 0000 (writeable)
* 0x02110018: 0000 (writeable, mask FF01)

Writing word 0xFFFF to both 0x02110016 and 0x02110018 will cause a reset (CPU reset pin is asserted, so not a crash).

### Minimal Startup Code
The following represents the minimal code to get the system configured and able to function.

```
_start:
    /* Startup delay */
    move.l  #1000, %d0

0:  subq.l  #1, %d0
    bgt     0b

    /* Configure VBR */
    lea     0x01000000, %a0
    movec   %a0, %vbr

    /* Clear BEV bit in SCR */
    andi.w  #0xFFFE, 0x02110000

    /* Configure ROM and RAM sizes in MCR */
    move.w  #0x0034, 0x02110002     /* 8MByte RAM, 4Mbit ROMs */
    
    /* Enable access to peripherals */
    ori.w   #0x3F, 0x02110004
    nop
    nop
    nop
    nop
    andi.w  #0xFFC0, 0x02110004

    /* Other initialisation code - memory tests, crt0, etc */

    jmp     main
```
