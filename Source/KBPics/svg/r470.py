# r470.py <out.svg> [scale]: the Ringo R470 keyboard picture, drawn again at a scale (2 if not given)
#
# Grey keys on light grey, the FUNC and ENTER keys blue and L-shaped, the
# GRAPHICS key cyan; white letters, yellow words on the keys, black words
# above them and blue ones under them, small blue block graphics.
import os
import sys
import numpy as np
from kbdraw import Picture, hexcol, font

HERE = os.path.dirname(os.path.abspath(__file__))
BACK, KEY, BLUE = (207, 207, 207), (127, 127, 127), (57, 114, 197)
WHITE, BLACK, YELLOW, CYAN = (240, 240, 240), (53, 53, 53), (255, 255, 0), (139, 246, 253)

# The words, 6 pixels high (as in the original): above the keys (black),
# under them (blue) and on them (yellow)
WORDS = {
    23: 'COPY', 25: 'IF', 27: 'PAUSE', 29: 'RUN', 31: 'REM', 33: 'PLOT', 35: 'UNPLOT', 37: 'INPUT',
    39: 'RAND', 40: 'PRINT', 73: 'LOAD', 75: 'SAVE', 77: 'DIM', 79: 'FOR', 81: 'GOTO', 82: 'GOSUB',
    83: 'RETURN', 84: 'LET', 85: 'LIST', 116: 'CLEAR', 117: 'CLS', 118: 'CONT', 119: 'SCROLL',
    120: 'POKE', 121: 'NEXT', 122: 'NEW', 149: 'BREAK',
    63: 'SIN', 64: 'ARCSIN', 65: 'COS', 66: 'ARCCOS', 67: 'TAN', 68: 'ARCTAN', 69: 'USR',
    70: 'INKEY$', 71: 'RND', 108: 'STR$', 109: 'CHR$', 110: 'SQR', 111: 'ABS', 112: 'INT',
    113: 'SGN', 114: 'EXP', 115: 'LN', 142: 'LEN', 143: 'CODE', 144: 'VAL', 145: 'PEEK', 146: 'NOT',
    147: 'AT', 148: 'TAB',
    8: 'EDIT', 9: 'LPRINT', 10: 'LLIST', 41: 'OR', 42: 'THEN', 43: 'SLOW', 44: 'FAST', 45: 'TO',
    86: 'AND', 87: 'STOP', 88: 'STEP',
    96: 'INVERSE', 107: 'VIDEO',
}
# The signs (in the boxes of the original)
SIGNS = {46: '<=', 47: '<>', 48: '=>', 49: '$', 50: '"', 123: '?', 124: '(', 125: ')', 126: '<',
         127: '>', 128: ':', 129: ';', 130: ','}
# The letters and digits (Heros Bold, in the boxes of the original)
BIG = {11: '1', 12: '2', 13: '3', 14: '4', 15: '5', 16: '6', 17: '7', 18: '8', 19: '9',
       51: 'Q', 52: 'W', 53: 'E', 54: 'R', 55: 'T', 56: 'Y', 57: 'U', 58: 'I', 59: 'O', 60: 'P',
       94: 'A', 99: 'S', 100: 'D', 101: 'F', 102: 'G', 103: 'H', 104: 'J', 105: 'K', 106: 'L',
       133: 'Z', 134: 'X', 135: 'C', 136: 'V', 137: 'B', 138: 'N', 139: 'M'}
# The words of the large keys
KEYWORDS = {21: 'DELETE', 61: 'ENTER', 98: 'FUNC', 132: 'SHIFT', 140: 'SHIFT'}
# Block graphics and straight signs (- + = and the full stop): of their pixels
PIXELS = (0, 1, 2, 3, 4, 5, 6, 7, 24, 26, 28, 30, 32, 34, 36, 38, 74, 76, 78, 80, 92, 93, 95, 141)
# The arrow keys: their arrows
ARROWS = {22: 'left', 62: 'right', 97: 'up', 131: 'down'}

# The keys: FUNC and ENTER (L-shaped, by their corners, in pixels of the
# original), GRAPHICS; the arrow keys (a column of their own) keep their
# places
FUNC, ENTER, GRAPHICS = 12, 23, 45
L_KEYS = {FUNC: [(6, 61), (33, 61), (33, 105), (47, 105), (47, 132), (6, 132)],
          ENTER: [(419, 61), (461, 61), (461, 132), (432, 132), (432, 88), (419, 88)]}
WIDE = (11, 24, 35, 47, FUNC, ENTER)


def rounded(points, r):
    """A path round a polygon, its corners rounded (radius r)"""
    n = len(points)
    d = []
    for i in range(n):
        p0, p1, p2 = np.array(points[i - 1], float), np.array(points[i], float), np.array(points[(i + 1) % n], float)
        a = p1 + (p0 - p1) / np.linalg.norm(p0 - p1) * r
        b = p1 + (p2 - p1) / np.linalg.norm(p2 - p1) * r
        d.append('%s%.2f %.2f Q%.2f %.2f %.2f %.2f' % ('M' if i == 0 else 'L', a[0], a[1], p1[0], p1[1], b[0], b[1]))
    return ' '.join(d) + 'z'


def main():
    p = Picture(os.path.join(HERE, 'r470.json'), os.path.join(HERE, 'orig/r470.png'),
                float(sys.argv[2]) if len(sys.argv) > 2 else 2)
    S = p.S
    # the colours of the picture: those almost one of them are that one
    pure = np.array([BACK, KEY, BLUE, WHITE, BLACK, YELLOW, CYAN])
    p.im = pure[((p.im[:, :, None, :] - pure[None, None]) ** 2).sum(-1).argmin(-1)]

    # Keys: one pitch and width a row, the rows at their places (the second
    # one as its keys, not as FUNC and ENTER, a pixel higher)
    p.regular_rows(wide=WIDE)
    for i, k in enumerate(p.newkeys):
        if i in (FUNC, ENTER):
            k[:] = p.keys[i]
        elif 55 < k[1] < 65:
            k[1], k[3] = 61, 27
    for i, k in enumerate(p.newkeys):
        if i in L_KEYS:
            pts = [(round(x * S), round(y * S)) for x, y in L_KEYS[i]]
            p.items.append('<path fill="%s" d="%s"/>' % (hexcol(BLUE), rounded(pts, 2.5 * S)))
            continue
        x, y = round(k[0] * S), round(k[1] * S)
        w, h = round((k[0] + k[2]) * S) - x, round((k[1] + k[3]) * S) - y
        p.items.append('<rect x="%g" y="%g" width="%g" height="%g" rx="%g" fill="%s"/>'
                       % (x, y, w, h, 2.5 * S, hexcol(CYAN if i == GRAPHICS else KEY)))

    # The words: all 6 pixels high, in the font's own spacing; as close to
    # the block graphics after them as in the original (2 pixels)
    for ids, t in WORDS.items():
        p.word(ids, t, 'cnb', 6, margin=1 if ids in (96, 107) else 2, gap=2)
    for ids, t in SIGNS.items():
        p.text(ids, t, fonts=('herosb', 'cnb'))
    for ids, t in BIG.items():
        p.text(ids, t, fonts=('herosb',))
    for ids, t in KEYWORDS.items():
        p.text(ids, t, fonts=('herosb', 'cnb'))

    # GRAPHICS: blue on its key, in the middle, as the other words
    k = p.newkeys[GRAPHICS]
    fn = font('cnb')
    d, (x0, y0, x1, y1) = fn.outline('GRAPHICS')
    sc = 6 * S / fn.outline('H')[1][3]
    sx = min(1.0, (k[2] - 4) * S / ((x1 - x0) * sc))
    cx, cy = (k[0] + k[2] / 2) * S, (k[1] + k[3] / 2) * S
    p.items.append('<path fill="%s" transform="matrix(%.5f 0 0 %.5f %.3f %.3f)" d="%s"/>'
                   % (hexcol(BLUE), sc * sx, -sc, cx - sc * sx * (x0 + x1) / 2, cy + 3 * S, d))

    # 0 with its slash
    b = p.box(20)
    X, Y, W, H = p.moved(b)
    p.fit('0', X, Y, W, H, WHITE, fonts=('herosb',))
    p.items.append('<line x1="%g" y1="%g" x2="%g" y2="%g" stroke="%s" stroke-width="%g"/>'
                   % (X + W - 0.6 * S, Y + 0.6 * S, X + 0.6 * S, Y + H - 0.6 * S, hexcol(WHITE), 1.3 * S))

    # pi
    b = p.box(72)
    X, Y, W, H = p.moved(b)
    p.fit('π', X, Y, W, H, BLUE, fonts=('cnb',))

    def line(x1, y1, x2, y2, colour=YELLOW, w=1.0):
        p.items.append('<line x1="%.2f" y1="%.2f" x2="%.2f" y2="%.2f" stroke="%s" stroke-width="%g"/>'
                       % (x1, y1, x2, y2, hexcol(colour), w * S))

    # / on H: a line; ** on F and * on G: stars of three thin lines, as in
    # the original
    b = p.box(91)
    X, Y, W, H = p.moved(b)
    line(X + 0.4 * S, Y + H - 0.2 * S, X + W - 0.4 * S, Y + 0.2 * S, w=1.1)

    def star(X, Y, W, H):
        cx = X + W / 2
        line(cx, Y, cx, Y + H)
        line(X + 0.3 * S, Y + 0.25 * H, X + W - 0.3 * S, Y + 0.75 * H)
        line(X + 0.3 * S, Y + 0.75 * H, X + W - 0.3 * S, Y + 0.25 * H)

    b = p.box(89)
    X, Y, W, H = p.moved(b)
    star(X, Y, 5 * S, H)
    star(X + 7 * S, Y, 5 * S, H)
    b = p.box(90)
    X, Y, W, H = p.moved(b)
    star(X, Y, W, H)

    # Block graphics and the straight signs
    for i in PIXELS:
        p.pixels(i)

    # The arrows: a head and a shaft
    for i, direction in ARROWS.items():
        b = p.box(i)
        X, Y, W, H = p.moved(b)
        # as pointing left, in the box (u along, v across)
        pts = [(0, 0.5), (0.5, 0), (0.5, 1 / 3), (1, 1 / 3), (1, 2 / 3), (0.5, 2 / 3), (0.5, 1)]
        if direction == 'right':
            pts = [(1 - u, v) for u, v in pts]
        if direction in ('up', 'down'):
            pts = [(v, u if direction == 'up' else 1 - u) for u, v in pts]
        p.items.append('<polygon fill="%s" points="%s"/>'
                       % (hexcol(WHITE), ' '.join('%.2f,%.2f' % (X + u * W, Y + v * H) for u, v in pts)))

    print('r470, not drawn: %s' % p.unused(), file=sys.stderr)
    open(sys.argv[1], 'w').write(p.svg(BACK))


if __name__ == '__main__':
    main()
