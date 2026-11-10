# zx81.py <out.svg> [scale]: the ZX81 keyboard picture, drawn again at a scale (2 if not given)
import os
import sys
from kbdraw import Picture, hexcol, arrow_points

HERE = os.path.dirname(os.path.abspath(__file__))
p = Picture(os.path.join(HERE, 'zx81.json'), os.path.join(HERE, 'orig/zx81.png'), float(sys.argv[2]) if len(sys.argv) > 2 else 2)
S = p.S
KEY, RED, BLACK = (252, 252, 252), (252, 0, 0), (0, 0, 0)
CAP = float(os.environ.get('ZX81CAP', 7))

# Keys: one pitch and width a row; SHIFT keeps its place, and the last row
# its line (the last two keys were 1 pixel higher)
p.regular_rows(wide=(30,))
for k in p.newkeys:
    if k[1] > 170:
        k[1] = 176
p.draw_keys(KEY, 3)

texts = {
    # 1 - 0
    0: 'EDIT', 1: 'AND', 2: 'THEN', 3: 'TO', 8: 'GRAPHICS', 9: 'RUBOUT',
    10: '1', 12: '2', 14: '3', 16: '4', 18: '5', 20: '6', 22: '7', 24: '8', 26: '9',
    # Q - P
    28: 'PLOT', 29: 'UNPLOT', 30: 'REM', 31: 'RUN', 32: 'RAND', 33: 'RETURN', 34: 'IF',
    35: 'INPUT', 36: 'POKE', 37: 'PRINT',
    40: 'OR', 41: 'STEP',
    51: 'Q', 52: 'W', 53: 'E', 54: 'R', 56: 'T', 57: 'Y', 58: 'U', 59: 'I', 60: 'O', 61: 'P',
    68: 'SIN', 69: 'COS', 70: 'TAN', 71: 'INT', 72: 'RND', 73: 'STR$', 74: 'CHR$', 75: 'CODE',
    76: 'PEEK', 77: 'TAB',
    # A - NEW LINE
    78: 'NEW', 79: 'SAVE', 80: 'DIM', 81: 'FOR', 82: 'GOTO', 83: 'GOSUB', 84: 'LOAD',
    85: 'LIST', 86: 'LET',
    87: 'STOP', 88: 'LPRINT', 89: 'SLOW', 90: 'FAST', 91: 'LLIST', 95: 'FUNCTION',
    96: 'A', 97: 'S', 98: 'D', 99: 'F', 100: 'G', 101: 'H', 102: 'J', 104: 'K', 105: 'L',
    113: 'NEW', 114: 'LINE',
    115: 'ARCSIN', 116: 'ARCCOS', 117: 'ARCTAN', 118: 'SGN', 119: 'ABS', 120: 'SQR', 121: 'VAL',
    122: 'LEN', 123: 'USR',
    # SHIFT - SPACE
    124: 'BREAK', 125: 'COPY', 126: 'CLEAR', 127: 'CONT', 128: 'CLS', 129: 'SCROLL',
    130: 'NEXT', 131: 'PAUSE',
    142: 'Z', 144: 'X', 146: 'C', 147: 'V', 148: 'B', 149: 'N', 150: 'M', 151: '.',
    152: 'SPACE',
    153: 'LN', 154: 'EXP', 155: 'AT', (156, 157): 'INKEY$', 158: 'NOT',
    # red signs
    38: '"', 39: '"', 50: '"', 42: '<', 44: '<', 45: '>', 47: '$', 48: '(', 49: ')',
    (133, 145): ';', 134: '?', 137: '<', 138: '>', 139: ',', 140: '£',
}
big = set([10, 12, 14, 16, 18, 20, 22, 24, 26, 51, 52, 53, 54, 56, 57, 58, 59, 60, 61,
           96, 97, 98, 99, 100, 101, 102, 104, 105, 142, 144, 146, 147, 148, 149, 150])

# The words (white, red and black): all of one size, as high as EDIT (7
# pixels; the white ones were 6), in the font's own spacing. A white word
# above a key row keeps its base line (it grows upwards), one under a key
# row its top line (it grows downwards), so each stays 2 pixels from the
# keys
for i in (73, 74):
    # STR$ and CHR$: the box of the letters (the $ went 1 pixel over them)
    p.words[i]['y'] += 1
    p.words[i]['y2'] -= 1
p.words[157]['y'] += 1
p.words[157]['y2'] -= 1
for i, b in p.words.items():
    if b['c'] == list(KEY) and b['y2'] - b['y'] == 6 and i != 159:
        k = p.keys[p.key_of(b)]
        if b['y'] >= k[1] + k[3]:
            b['y2'] += 1
for ids, t in texts.items():
    if ids in big:
        p.text(ids, t, fonts=('herosb',))
    elif len(t) > 1 and t.replace('$', '').isalpha():
        p.word(ids, t, 'cnb', CAP)
    else:
        p.text(ids, t)

# SHIFT: big, as in the original
p.text(141, 'SHIFT', fonts=('herosb', 'cnb'))

# 0 with its slash, from the bottom left to the top right
b = p.box(27)
X, Y, W, H = p.moved(b)
p.fit('0', X, Y, W, H, BLACK, fonts=('herosb',))
p.items.append('<line x1="%g" y1="%g" x2="%g" y2="%g" stroke="#000000" stroke-width="%g"/>'
               % (X + 0.6 * S, Y + H - 0.6 * S, X + W - 0.6 * S, Y + 0.6 * S, 1.3 * S))

# π: as high as the words around it, in their font
b = p.box(159)
X, Y, W, H = p.moved(b)
p.fit('π', X - 0.25 * S, Y, W + 0.5 * S, H + 1 * S, KEY, fonts=('cnb',))

# / on V: a line, as thin as the strokes of the words
b = p.box(135)
X, Y, W, H = p.moved(b)
p.items.append('<line x1="%g" y1="%g" x2="%g" y2="%g" stroke="%s" stroke-width="%g"/>'
               % (X + 0.5 * S, Y + H - 0.2 * S, X + W - 0.5 * S, Y + 0.2 * S, hexcol(RED), 1.25 * S))

# Block graphics and the straight signs (- + = :): made of squares
for i in (11, 13, 15, 17, 19, 21, 23, 25, 62, 63, 64, 65, 66, 67,
          43, 55, 93, 94, 103, 106, 132, 143):
    p.pixels(i)


# The grey block graphics (A - H): the frame and the black parts on whole
# pixels, as the other block graphics; the dots (the grey of the ZX81, a
# chequer of single pixels) spread evenly over the rows and columns they
# fill, so that they are of one size and one spacing at every scale
def dotted_graphic(i):
    b = p.box(i)
    X, Y, _, _ = p.moved(b)
    x0, x1 = b['x'], b['x2']
    edge = lambda v, o: round(o + v * S)
    solid, dots, band = [], [], []
    rows = list(range(b['y'], b['y2']))

    def rect(l, t, r, bt, out):
        out.append('M%g %gh%gv%gh%gz' % (l, t, r - l, bt - t, -(r - l)))

    for y in rows + [None]:
        inner = None if y is None else (p.im[y, x0 + 1:x1 - 1] == 0).all(-1)
        if y is not None and inner.any() and not inner.all():
            band.append(y)
            continue
        if band:
            # a band of dotted rows: its pixels split evenly
            l, r = edge(1, X), edge(x1 - x0 - 1, X)
            t, bt = edge(band[0] - b['y'], Y), edge(band[-1] + 1 - b['y'], Y)
            cw, ch = (r - l) / (x1 - x0 - 2), (bt - t) / len(band)
            for n, yy in enumerate(band):
                for m, x in enumerate(range(x0 + 1, x1 - 1)):
                    if (p.im[yy, x] == 0).all():
                        rect(l + m * cw, t + n * ch, l + (m + 1) * cw, t + (n + 1) * ch, dots)
                ty, by = edge(yy - b['y'], Y), edge(yy + 1 - b['y'], Y)
                rect(edge(0, X), ty, l, by, solid)
                rect(r, ty, edge(x1 - x0, X), by, solid)
            band = []
        if y is None:
            break
        ty, by = edge(y - b['y'], Y), edge(y + 1 - b['y'], Y)
        if inner.all():
            rect(edge(0, X), ty, edge(x1 - x0, X), by, solid)
        else:
            rect(edge(0, X), ty, edge(1, X), by, solid)
            rect(edge(x1 - x0 - 1, X), ty, edge(x1 - x0, X), by, solid)
    p.items.append('<path fill="#000000" d="%s"/>' % ''.join(solid))
    p.items.append('<path fill="#000000" d="%s"/>' % ''.join(dots))


for i in (107, 108, 109, 110, 111, 112):
    dotted_graphic(i)

# >= on Y: > and the two bars of = were found as one
b = p.box(46)
X, Y, W, H = p.moved(b)
p.fit('>', X, Y, 9 * S, 7 * S, RED, fonts=('herosb', 'cnb'))
for dy in (1, 5):
    p.items.append('<rect x="%g" y="%g" width="%g" height="%g" fill="%s"/>'
                   % (round(X + 11 * S), round(Y + dy * S), round(X + 19 * S) - round(X + 11 * S),
                      round(Y + (dy + 1) * S) - round(Y + dy * S), hexcol(RED)))


# The stars (** on H, * on B): a stroke up, a bar across, two legs down and a
# thicker middle
def star(X, Y, W, H):
    cx = X + W / 2
    for x1, y1, x2, y2 in ((cx, Y, cx, Y + 3 * S), (X, Y + 2.5 * S, X + W, Y + 2.5 * S),
                           (cx - 0.5 * S, Y + 4 * S, X + 1.2 * S, Y + H - 0.2 * S),
                           (cx + 0.5 * S, Y + 4 * S, X + W - 1.2 * S, Y + H - 0.2 * S)):
        p.items.append('<line x1="%g" y1="%g" x2="%g" y2="%g" stroke="%s" stroke-width="%g"/>'
                       % (x1, y1, x2, y2, hexcol(RED), 1.1 * S))
    p.items.append('<rect x="%g" y="%g" width="%g" height="%g" fill="%s"/>'
                   % (cx - 1.5 * S, Y + 3 * S, 3 * S, 2 * S, hexcol(RED)))


b = p.box(92)
X, Y, W, H = p.moved(b)
star(X, Y, 7 * S, H)
star(X + 8 * S, Y, 7 * S, H)
b = p.box(136)
X, Y, W, H = p.moved(b)
star(X, Y, W, H)

# The cursor arrows above 5 - 8: outlines, the heads long
for i, direction, head, s0, s1 in ((4, 'left', 0.625, 0.3, 0.7), (7, 'right', 0.66, 0.3, 0.7),
                                   (5, 'down', 0.8, 0.25, 0.75), (6, 'up', 0.8, 0.25, 0.75)):
    b = p.box(i)
    X, Y, W, H = p.moved(b)
    pts = arrow_points(X, Y, W, H, direction, S, head=head, s0=s0, s1=s1)
    p.items.append('<polygon fill="none" stroke="%s" stroke-width="%g" points="%s"/>'
                   % (hexcol(RED), S, ' '.join('%g,%g' % q for q in pts)))

print('not drawn:', p.unused(), file=sys.stderr)
open(sys.argv[1], 'w').write(p.svg((0, 0, 0)))
