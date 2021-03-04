import argparse
import os
import struct
import time
from serial import Serial


# Adjust this to point to your serial device
DEV = '/dev/tty.usbserial-AI02GOMO'


def main():
    parser = argparse.ArgumentParser(
        description='Load application software via UART'
    )
    parser.add_argument(
        '-b', '--base',
        dest='base', type=str, required=True,
        help='Base address where the software will be loaded, usually in RAM. '
             'Supply in hexadecimal format. Min 0x000000, max 0xFFFFFE.'
    )
    parser.add_argument(
        '-e', '--exec',
        dest='exec', type=str, default=None,
        help='Address to JSR to to execute loaded code.'
    )
    parser.add_argument(
        '-j', '--jump',
        dest='jump', type=str, default=None,
        help='Address to JMP to to execute loaded code.'
    )
    parser.add_argument(
        'filename',
        type=str,
        help='Filename that should be loaded. Must be a binary file, not hex.'
    )

    args = parser.parse_args()

    base = int(args.base, 16)

    if args.exec is not None:
        exec = int(args.exec, 16)
    else:
        exec = None

    if args.jump is not None:
        jump = int(args.jump, 16)
    else:
        jump = None

    filename = args.filename

    # if exec is True and jump is not None:
    if exec is not None and jump is not None:
        raise ValueError(
            'Choose one of jump or execute.'
        )

    if not (0 <= base <= 0xFFFFFE):
        raise ValueError(
            'Base has invalid value. Minimum 0x000000, maximum 0xFFFFFE.'
        )    

    if (base & 0x1) == 1:
        raise ValueError(
            'Base must be word aligned.'
        )    

    filesize = os.stat(filename).st_size

    if not (2 <= filesize <= 16777216):
        raise ValueError(
            'Size of file is invalid. Minimum 2 bytes, maximum 16777216.'
        )

    if base + filesize > 0xFFFFFF:
        raise ValueError(
            'Base address plus file size would overflow/wrap.'
        )
    
    base_be = struct.pack('>L', base)
    filesize_be = struct.pack('>L', filesize)

    with open(filename, 'r+b') as file:
        rom = file.read(filesize)
    
    data = bytes(b'\03' + filesize_be + base_be + rom)

    ser = Serial(
        DEV,
        baudrate=57600,
        timeout=1
    )

    if ser.in_waiting > 0:
        ser.read(size=ser.in_waiting)
    
    # Wait for serial loader to be available
    print('Waiting for serial loader availability:', end='', flush=True)

    failed = 0

    while True:
        ser.write(bytes([1]))
        ser.flush()

        try:
            if ord(ser.read(size=1)) == 2:
                print(' OK')
                break
        except TypeError:
            if failed == 0:
                print(' ', end='')

            failed += 1

            print('.', end='', flush=True)

            if failed == 10:
                print(' Failed after too many attempts')

                return

    # Send the code over
    start = time.time()

    print(f'Transferring {filesize} bytes to 0x{base:08X}:', end='', flush=True)

    ser.write(data)
    ser.flush()

    failed = 0

    while True:
        try:
             if ord(ser.read(size=1)) == 4:
                break
        except TypeError:
            if failed == 0:
                print(' ', end='')

            failed += 1

            print('.', end='', flush=True)

            if failed == 15:
                print(' Failed: transfer not acknowledged')

                return

    duration = time.time() - start

    print(' Done in %.3fs' % duration)

    # If user chose to execute their code, send that command
    failed = 0

    if exec is not None:
        pc = struct.pack('>L', exec)
        data = bytes(b'\x05' + pc)

        print(f'Executing from 0x{exec:08X}:', end='', flush=True)
    elif jump is not None:
        pc = struct.pack('>L', jump)
        data = bytes(b'\x05' + pc)

        print(f'Jumping to 0x{jump:08X}:', end='', flush=True)
    
    if exec is not None or jump is not None:
        ser.write(data)
        ser.flush()

        while True:
            try:
                if ord(ser.read(size=1)) == 7:
                    break
            except TypeError:
                if failed == 0:
                    print(' ', end='')
                
                failed += 1

                print('.', end='', flush=True)

                if failed == 2:
                    print(' Failed: execution not acknowledged')

                    return

        print(' OK')

    ser.close()


if __name__ == '__main__':
    main()
