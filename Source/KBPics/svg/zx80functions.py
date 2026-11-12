# zx80functions.py <out.svg> [scale]: the ZX80 functions picture (the window
# beside the ZX80 keyboard: INTEGRAL FUNCTIONS and the eight functions under
# it), drawn again at a scale (2 if not given)
#
# The original (orig/zx80functions.png, 130 x 54) has no keys; the places of
# its words are written here: their left edges and base lines, in pixels of
# the original. The words are of Heros Bold (Viktor's choice: wide, as the
# letters of the original), as high as the words of the ZX80 (7 pixels),
# in the font's own spacing.
import sys
from kbdraw import font, hexcol

WHITE, YELLOW, BLACK = (255, 255, 255), (255, 201, 33), (0, 0, 0)
SIZE = (130, 54)

# The title, and the functions in three columns (as in the original)
TITLE = ('INTEGRAL FUNCTIONS', 6, 13)
COLUMNS = (6, 56, 103)
LINES = (28, 38, 48)
FUNCTIONS = (('CHR$', 'STR$', 'TL$'), ('PEEK', 'CODE', 'RND'), ('USR', 'ABS'))


def main():
    S = float(sys.argv[2]) if len(sys.argv) > 2 else 2
    f = font('herosb')
    sc = 7 * S / f.outline('H')[1][3]
    items = []

    def word(t, left, base, colour):
        d, (x0, y0, x1, y1) = f.outline(t)
        items.append('<path fill="%s" transform="matrix(%.5f 0 0 %.5f %.3f %.3f)" d="%s"/>'
                     % (hexcol(colour), sc, -sc, left - sc * x0, base, d))

    t, left, base = TITLE
    word(t, left * S, base * S, WHITE)
    for x, words in zip(COLUMNS, FUNCTIONS):
        for y, t in zip(LINES, words):
            word(t, x * S, y * S, YELLOW)

    W, H = int((SIZE[0] * S * 4 + 2) // 4), int((SIZE[1] * S * 4 + 2) // 4)
    out = ['<svg xmlns="http://www.w3.org/2000/svg" width="%d" height="%d" viewBox="0 0 %d %d">' % (W, H, W, H),
           '<!-- The EightyOne ZX80 functions picture, drawn again by Source/KBPics/svg (see its '
           'README.md) from the original pixel picture. The letters are outlines of TeX Gyre '
           'Heros (GUST Font License). -->',
           '<rect width="100%%" height="100%%" fill="%s"/>' % hexcol(BLACK)]
    out += items
    out.append('</svg>')
    open(sys.argv[1], 'w').write('\n'.join(out))


if __name__ == '__main__':
    main()
