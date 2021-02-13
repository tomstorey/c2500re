def main():
    rom = []
    fw1 = []
    fw2 = []

    with open('bmbinary.rom', 'r+b') as file:
        rom = file.read(0x100000)

    # Produce Odd and Even ROMs
    for idx, data in enumerate(rom):
        if (idx & 0x1) == 0:
            # Even ROM data
            fw2.append(reverse_byte(data))
        else:
            # Odd ROM data
            fw1.append(reverse_byte(data))

    fw1 += [0xFF] * (524288 - len(fw1))
    fw2 += [0xFF] * (524288 - len(fw2))

    # Write Odd ROM file
    with open('fw1', 'w+b') as file:
        file.write(bytes(fw1))

    # Write Even ROM file
    with open('fw2', 'w+b') as file:
        file.write(bytes(fw2))


def reverse_byte(b):
    b &= 0xFF

    if b == 0 or b == 0xFF:
        return b

    b = ((b & 0xF) << 4) | (b >> 4)
    b = ((b & 0x33) << 2) | ((b & 0xCC) >> 2)
    b = ((b & 0x55) << 1) | ((b & 0xAA) >> 1)

    return b


if __name__ == '__main__':
    main()
