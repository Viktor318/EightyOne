# make.py [scale ...]: draws the pictures again
#
#   <name>.svg              the drawing at 2.5x, the size the program shows
#                           at most (kept in the repository)
#   out/<name>_<scale>x.png the pictures at the scales asked (2.5 if none)
#   out/<name>.bmp          the 2.5x picture in 256 colours, for the forms
#                           (dfmpicture.py puts it into a .dfm)
#
# Needs Python 3 with numpy, scipy, scikit-image, Pillow and fontTools,
# the TeX Gyre Heros fonts, and rsvg-convert (librsvg).
import os
import subprocess
import sys
from PIL import Image

HERE = os.path.dirname(os.path.abspath(__file__))
OUT = os.path.join(HERE, 'out')
NAMES = ['spectrum48', 'spectrum128', 'keypad128', 'zx81', 'zx81zxpand', 'ts1000', 'ts1000zxpand',
         'zx97']


def draw(name, scale, svg):
    subprocess.check_call([sys.executable, os.path.join(HERE, name + '.py'), svg, str(scale)])


def render(svg, png):
    subprocess.check_call(['rsvg-convert', svg, '-o', png])


def main():
    scales = [float(a) for a in sys.argv[1:]] or [2.5]
    os.makedirs(OUT, exist_ok=True)
    for name in NAMES:
        draw(name, 2.5, os.path.join(HERE, name + '.svg'))
        for scale in scales:
            tag = ('%g' % scale)
            svg = os.path.join(OUT, '%s_%sx.svg' % (name, tag))
            png = os.path.join(OUT, '%s_%sx.png' % (name, tag))
            draw(name, scale, svg)
            render(svg, png)
        png = os.path.join(OUT, name + '_2.5x_bmp.png')
        render(os.path.join(HERE, name + '.svg'), png)
        colours256(Image.open(png).convert('RGB')).save(os.path.join(OUT, name + '.bmp'))
        os.remove(png)


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
