# dfmclear.py <form.dfm> <image name> ...: takes the pictures out of TImages
# of a text form file (their Picture.Data), as the C++Builder IDE would when
# the picture is cleared. The program loads the pictures drawn again from
# its resources (KBPics.rc) instead
import sys


def main():
    dfm, names = sys.argv[1], sys.argv[2:]
    raw = open(dfm, 'rb').read()
    crlf = b'\r\n' in raw
    lines = raw.decode('latin-1').splitlines()
    for name in names:
        start = None
        for i, l in enumerate(lines):
            if l.strip() == 'object %s: TImage' % name:
                start = i
                break
        if start is None:
            raise SystemExit('%s: no TImage %s' % (dfm, name))
        first = None
        for i in range(start + 1, len(lines)):
            if lines[i].strip().startswith('object ') or lines[i].strip() == 'end':
                break
            if lines[i].strip() == 'Picture.Data = {':
                first = i
                break
        if first is None:
            print('%s: %s has no picture' % (dfm, name))
            continue
        for i in range(first + 1, len(lines)):
            if lines[i].rstrip().endswith('}'):
                last = i
                break
        del lines[first:last + 1]
    nl = '\r\n' if crlf else '\n'
    open(dfm, 'wb').write((nl.join(lines) + nl).encode('latin-1'))


if __name__ == '__main__':
    main()
