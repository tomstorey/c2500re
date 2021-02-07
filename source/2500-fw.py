def main():
    fw1 = []
    fw2 = []

    with open('2500-fw1.bin', 'r+b') as file:
        fw1 = file.read(0x100000)

    with open('2500-fw2.bin', 'r+b') as file:
        fw2 = file.read(0x100000)

    with open('2500-fw.bin', 'r+b') as file:
        for i in range(0x100000):
            b1 = reverse_byte(fw1[i])
            b2 = reverse_byte(fw2[i])

            file.write(bytes([b2, b1]))


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
