# Uncomment one of the following lines to produce ROM images of the correct
# size to suit your EEPROMs
# ROMSZ = 524288      # 4Mbit ROMs
# ROMSZ = 262144      # 2Mbit ROMs
ROMSZ = 131072      # 1Mbit ROMs

def main():
    rom = []
    fw1 = []
    fw2 = []

    # Read in the source binary
    with open('bmbinary.rom', 'r+b') as file:
        rom = file.read(ROMSZ * 2)

    # Split into odd and even ROMs, and reverse the bit order of each byte
    for idx, data in enumerate(rom):
        if (idx & 0x1) == 0:
            # Even ROM data
            fw2.append(reverse_byte(data))
        else:
            # Odd ROM data
            fw1.append(reverse_byte(data))

    # Pad the two ROM images to fill unused space
    fw1 += [0xFF] * (ROMSZ - len(fw1))
    fw2 += [0xFF] * (ROMSZ - len(fw2))

    # Write Odd ROM file
    with open('fw1', 'w+b') as file:
        file.write(bytes(fw1))

    # Write Even ROM file
    with open('fw2', 'w+b') as file:
        file.write(bytes(fw2))


def reverse_byte(b):
    """ Reverse the contents of a byte
    """
    b &= 0xFF

    if b == 0 or b == 0xFF:
        return b

    b = ((b & 0xF) << 4) | (b >> 4)
    b = ((b & 0x33) << 2) | ((b & 0xCC) >> 2)
    b = ((b & 0x55) << 1) | ((b & 0xAA) >> 1)

    return b


if __name__ == '__main__':
    main()
