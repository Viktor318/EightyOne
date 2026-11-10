# make.py [percent ...]: draws the pictures again
#
#   <name>.svg              the drawing at 100% (1.75 times the original,
#                           884 x 390), kept in the repository
#   ../png/<name>.png       the picture the program holds: at 200%, twice
#                           the drawing (1768 x 780), in 256 colours (the
#                           colours of the surfaces exactly), kept in the
#                           repository
#   ../../KBPics<n>.rc      the pictures for the program, named after the
#                           TImages they are shown for: in RC_FILES files,
#                           as the linker of C++Builder 5 fails on a .res
#                           file of 1 MB or more
#   out/<name>_<p>.png      the pictures at the sizes asked (in percent), as
#                           the program shows them: scaled down smoothly
#                           from the 200% picture (none if not asked)
#
# Needs Python 3 with numpy, scipy, scikit-image, Pillow and fontTools,
# the TeX Gyre Heros fonts, and rsvg-convert (librsvg).
import os
import subprocess
import sys
from PIL import Image

HERE = os.path.dirname(os.path.abspath(__file__))
OUT = os.path.join(HERE, 'out')
PNG = os.path.join(HERE, '..', 'png')
RC = os.path.join(HERE, '..', '..', 'KBPics%d.rc')
RC_FILES = 3
RC_LIMIT = 1000000

# The drawings: the scale they are drawn at (of the original pictures,
# 505 x 223), 100% in the program; it holds them at twice that, its
# largest size
SCALE = 1.75
STORED = 2

# The pictures and the TImages they are shown for (in Keyboard_.dfm, the
# keypad in Keypad_.dfm)
PICTURES = [('spectrum48', 'spec48kb'), ('spectrum16', 'spec16kb'), ('spectrum128', 'spec128kb'),
            ('keypad128', 'KeypadUk'), ('zx81', 'zx81kb'), ('zx81zxpand', 'zx81zxpandkb'),
            ('ts1000', 'ts1000kb'), ('ts1000zxpand', 'ts1000zxpandkb'), ('zx97', 'zx97kb'),
            ('zx80', 'zx80kb'), ('zx80zxpand', 'zx80zxpandkb'), ('spectrumplus2', 'specPlus2kb'),
            ('spectrumplus3', 'specPlus3kb'), ('tc2048', 'tc2048kb'), ('ts2068', 'ts2068kb'),
            ('ts1500', 'ts1500kb'), ('ts1500zxpand', 'ts1500zxpandkb'), ('ace', 'acekb'),
            ('lambda8300', 'lambdakb'), ('tk85', 'tk85kb')]
NAMES = [name for name, image in PICTURES]


def draw(name, svg):
    subprocess.check_call([sys.executable, os.path.join(HERE, name + '.py'), svg, str(SCALE)])


def render(svg, png):
    subprocess.check_call(['rsvg-convert', '-z', str(STORED), svg, '-o', png])


def main():
    percents = [int(a) for a in sys.argv[1:]]
    os.makedirs(OUT, exist_ok=True)
    os.makedirs(PNG, exist_ok=True)
    for name in NAMES:
        svg = os.path.join(HERE, name + '.svg')
        png = os.path.join(PNG, name + '.png')
        draw(name, svg)
        render(svg, png)
        # without the alpha channel (the background covers it all)
        im = colours256(Image.open(png).convert('RGB'))
        im.save(png, optimize=True)
        im = im.convert('RGB')
        for p in percents:
            size = (scaled(im.width, p), scaled(im.height, p))
            im.resize(size, Image.BOX).save(os.path.join(OUT, '%s_%d.png' % (name, p)))
    write_rc()


def scaled(stored, percent):
    """The size of a picture at a percent, as the program makes it: half the
    200% picture, then the percent of that"""
    full = stored // STORED
    return (full * percent + 50) // 100


def write_rc():
    for k in range(RC_FILES):
        part = PICTURES[k::RC_FILES]
        size = sum(os.path.getsize(os.path.join(PNG, name + '.png')) for name, image in part)
        if size >= RC_LIMIT:
            raise SystemExit('KBPics%d.rc: %d bytes, too many: more files needed' % (k + 1, size))
        lines = ['// The keyboard pictures drawn again (Source/KBPics/svg; make.py writes',
                 '// this file): PNG at 200%, named after the TImages they are shown for.',
                 '// They are in %d files, each less than 1 MB' % RC_FILES,
                 '']
        lines += ['%s RCDATA "KBPics\\\\png\\\\%s.png"' % (image, name) for name, image in part]
        open(RC % (k + 1), 'w', newline='\r\n').write('\n'.join(lines) + '\n')


def colours256(im):
    """The picture in 256 colours, without dithering: the colours of the
    surfaces (the keys, the background, the letters) kept exactly, the
    mixes at the edges of the letters as near as they can be"""
    import numpy as np
    cols, counts = np.unique(np.array(im).reshape(-1, 3), axis=0, return_counts=True)
    if len(cols) <= 256:
        return im.quantize(256, method=Image.Quantize.MEDIANCUT, dither=Image.Dither.NONE)
    fixed = [int(v) for c, n in zip(cols, counts) if n >= 200 for v in c]
    k = len(fixed) // 3
    rest = im.quantize(256 - k, method=Image.Quantize.MAXCOVERAGE,
                       dither=Image.Dither.NONE).getpalette()[:3 * (256 - k)]
    palette = Image.new('P', (1, 1))
    palette.putpalette((fixed + rest + [0] * 768)[:768])
    return im.quantize(palette=palette, dither=Image.Dither.NONE)


if __name__ == '__main__':
    main()
