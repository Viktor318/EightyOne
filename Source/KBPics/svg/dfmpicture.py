# dfmpicture.py <form.dfm> <image name> <picture.bmp>: puts a bitmap into a
# TImage of a text form file (its Picture.Data), as the C++Builder IDE would
import struct
import sys


def picture_data(bmp):
    # the class name of the picture, then the bitmap with its size before it
    name = b'TBitmap'
    data = bytes([len(name)]) + name + struct.pack('<I', len(bmp)) + bmp
    h = data.hex().upper()
    lines = ['      ' + h[i:i + 64] for i in range(0, len(h), 64)]
    lines[-1] += '}'
    return lines


def main():
    dfm, name, bmpfile = sys.argv[1:4]
    raw = open(dfm, 'rb').read()
    crlf = b'\r\n' in raw
    lines = raw.decode('latin-1').splitlines()
    start = None
    for i, l in enumerate(lines):
        if l.strip() == 'object %s: TImage' % name:
            start = i
            break
    if start is None:
        raise SystemExit('%s: no TImage %s' % (dfm, name))
    for i in range(start, len(lines)):
        if lines[i].strip() == 'Picture.Data = {':
            first = i + 1
            break
    for i in range(first, len(lines)):
        if lines[i].rstrip().endswith('}'):
            last = i
            break
    bmp = open(bmpfile, 'rb').read()
    if bmp[:2] != b'BM':
        raise SystemExit('%s: not a .bmp' % bmpfile)
    lines[first:last + 1] = picture_data(bmp)
    nl = '\r\n' if crlf else '\n'
    open(dfm, 'wb').write((nl.join(lines) + nl).encode('latin-1'))


if __name__ == '__main__':
    main()
