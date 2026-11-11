# zx81.py <out.svg> [scale]: the ZX81 keyboard picture, drawn again at a scale (2 if not given)
#
# The pictures of the ZX81's relatives (ZXpand, TS1000, ZX97) are the same
# picture with a few other texts: their scripts call draw() with their own.
# The texts of the ZX81 are found in them by their pixels (the same text,
# at most a few pixels away); what is not found is not drawn.
import math
import os
import sys
import numpy as np
from kbdraw import Picture, hexcol, arrow_points, font, comma

HERE = os.path.dirname(os.path.abspath(__file__))
KEY, RED, BLACK = (252, 252, 252), (252, 0, 0), (0, 0, 0)

# The texts of the ZX81 picture (the numbers of zx81.json)
TEXTS = {
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
BIG = set([10, 12, 14, 16, 18, 20, 22, 24, 26, 51, 52, 53, 54, 56, 57, 58, 59, 60, 61,
           96, 97, 98, 99, 100, 101, 102, 104, 105, 142, 144, 146, 147, 148, 149, 150])


def draw(name, own=None, small=None, ignore=(), colours=None, palette=None, more=None, extra=None):
    """The picture <name> (<name>.json, orig/<name>.png): the ZX81's texts
    where they are found, and its own texts ({number: text}, as words; small:
    words in small letters, as in the original)"""
    p = Picture(os.path.join(HERE, name + '.json'), os.path.join(HERE, 'orig/%s.png' % name),
                float(sys.argv[2]) if len(sys.argv) > 2 else 2)
    S = p.S
    cap = float(os.environ.get('ZX81CAP', 7))
    # colours: the picture coloured otherwise (the TK85): its key, label
    # (the white words), red and black colours; palette: its colours (any
    # other is taken as the nearest of them); more: texts of the ZX81 found
    # here otherwise than by their pixels ({ZX81 number: number here})
    col = dict(key=KEY, label=KEY, red=RED, black=BLACK)
    col.update(colours or {})
    if palette:
        pure = np.array(palette)
        p.im = pure[((p.im[:, :, None, :] - pure[None, None]) ** 2).sum(-1).argmin(-1)]
    if name == 'zx81':
        here = dict((i, i) for i in p.words)
    else:
        here = p.same_texts(os.path.join(HERE, 'zx81.json'), os.path.join(HERE, 'orig/zx81.png'),
                            colours=not palette)
    here.update(more or {})

    def V(*ids):
        """The numbers here of ZX81 texts (None if one is not found)"""
        v = tuple(here.get(i) for i in ids)
        if None in v:
            return None
        return v[0] if len(v) == 1 else v

    p.used.update(ignore)

    # Keys: one pitch and width a row; SHIFT keeps its place, and the last
    # row its line (the last two keys were 1 pixel higher)
    shift = p.key_of(p.words[V(141)])
    p.regular_rows(wide=(shift,))
    for k in p.newkeys:
        if k[1] > 170:
            k[1] = 176
    p.draw_keys(col['key'], 3)

    texts = {}
    for ids, t in TEXTS.items():
        v = V(*ids) if isinstance(ids, tuple) else V(ids)
        if v is not None:
            texts[v] = t
    texts.update(own or {})
    big = set(V(i) for i in BIG) - set([None])

    # The words (white, red and black): all of one size, as high as EDIT
    # (7 pixels; the white ones were 6), in the font's own spacing. A white
    # word above a key row keeps its base line (it grows upwards), one under
    # a key row its top line (it grows downwards), so each stays 2 pixels
    # from the keys
    for i in (73, 74, 157):
        # STR$, CHR$ and KEY$: the box of the letters (the $ went 1 pixel
        # over them)
        if V(i) is not None:
            p.words[V(i)]['y'] += 1
            p.words[V(i)]['y2'] -= 1
    for i, b in p.words.items():
        if b['c'] == list(col['label']) and b['y2'] - b['y'] == 6 and i != V(159):
            k = p.keys[p.key_of(b)]
            if b['y'] >= k[1] + k[3]:
                b['y2'] += 1
    # The signs of the font (Viktor's choice): all of one size, Heros Bold,
    # the capitals SIGN pixels high, not stretched; their tops (that of the
    # capitals) on one line a row, SIGNTOP pixels under the tops of the keys
    # (as most were); each in the middle of its box. < and > stay wide, as
    # in the original and on the ZX81 (Viktor's choice); " and ; and ,
    # below
    SIGN, SIGNTOP = 10, 4
    FONTSIGNS = '$()?£'
    HB = font('herosb')
    HCAP = HB.outline('H')[1][3]

    def font_sign(t, b, X, W):
        nk = p.newkeys[p.key_of(b)]
        d, (x0, y0, x1, y1) = HB.outline(t)
        sc = SIGN * S / HCAP
        left = X + W / 2 - sc * (x1 - x0) / 2
        p.items.append('<path fill="%s" transform="matrix(%.5f 0 0 %.5f %.3f %.3f)" d="%s"/>'
                       % (hexcol(b['c']), sc, -sc, left - sc * x0, (nk[1] + SIGNTOP + SIGN) * S, d))

    for ids, t in texts.items():
        if ids in big:
            p.text(ids, t, fonts=('herosb',))
        elif len(t) > 1 and any(c.isalpha() for c in t):
            p.word(ids, t, 'cnb', cap)
        elif t in FONTSIGNS and len(t) == 1:
            b = p.box(ids)
            X, Y, W, H = p.moved(b)
            font_sign(t, b, X, W)
        elif t in '";,':
            continue                    # below
        else:
            p.text(ids, t)
    # small words (under bigger ones): 5 pixels high (they were 4), and as
    # wide as the original's proportions allow, up to the big letter at
    # their left (1 pixel from it) and the edge of the key (1 pixel); at
    # the right, as in the original
    for ids, t in (small or {}).items():
        b = p.box(ids)
        ki = p.key_of(b)
        k, nk = p.keys[ki], p.newkeys[ki]
        h = 5
        lefts = [o['x2'] + 1 for o in p.words.values() if o is not b and p.key_of(o) == ki
                 and o['x2'] <= b['x'] and o['y'] < b['y'] + h and b['y'] < o['y2']]
        left = max(lefts + [k[0] + 2])
        right = min(b['x2'], k[0] + k[2] - 1)
        w = min((b['x2'] - b['x']) * h / (b['y2'] - b['y']), right - left)
        dx = nk[0] - k[0]
        p.fit(t, (right - w + dx) * S, b['y'] * S, w * S, h * S, b['c'], fonts=('cnb',), maxtrack=0.04)

    # SHIFT: big, as in the original
    p.text(V(141), 'SHIFT', fonts=('herosb', 'cnb'))

    # 0 with its slash, from the bottom left to the top right
    b = p.box(V(27))
    X, Y, W, H = p.moved(b)
    p.fit('0', X, Y, W, H, col['black'], fonts=('herosb',))
    p.items.append('<line x1="%g" y1="%g" x2="%g" y2="%g" stroke="%s" stroke-width="%g"/>'
                   % (X + 0.6 * S, Y + H - 0.6 * S, X + W - 0.6 * S, Y + 0.6 * S, hexcol(col['black']), 1.3 * S))

    # π: as high as the words around it, in their font
    b = p.box(V(159))
    X, Y, W, H = p.moved(b)
    p.fit('π', X - 0.25 * S, Y, W + 0.5 * S, H + 1 * S, col['label'], fonts=('cnb',))

    # / on V: a line, as thin as the strokes of the words, at 60 degrees
    # (Viktor's choice; it was steeper), as high as it was, in the middle of
    # its place
    b = p.box(V(135))
    X, Y, W, H = p.moved(b)
    dy = H - 0.4 * S
    dx = dy / math.tan(math.radians(60))
    cx, cy = X + W / 2, Y + H / 2
    p.items.append('<line x1="%.2f" y1="%.2f" x2="%.2f" y2="%.2f" stroke="%s" stroke-width="%g"/>'
                   % (cx - dx / 2, cy + dy / 2, cx + dx / 2, cy - dy / 2, hexcol(col['red']), 1.25 * S))

    # The block graphics (Viktor's choice): all of one size (about 11.5
    # pixels; they were 10 to 12 wide, 12 high), their frames thin (F, on
    # whole pixels; they were 1 or 2 pixels as their rows fell), their four
    # quarters of one size (some were a pixel narrower), each filled, empty
    # or dotted (the grey, a chequer of the original's dots) as in the
    # original; 3 and 4 pixels from the right and bottom edges of their keys
    F = max(1, int(S))
    NB = 2 * round((11.5 * S - 2 * F) / 2) + 2 * F
    for i in (11, 13, 15, 17, 19, 21, 23, 25, 62, 63, 64, 65, 66, 67, 107, 108, 109, 110, 111, 112):
        p.block(V(i), NB, F, max(1, round(S)), right=round(3 * S), bottom=round(4 * S))

    # The straight signs (Viktor's choice): the lines of - + = and of <= and
    # >= as thick as each other (T, on whole pixels; they were 1 or 2
    # pixels as their rows fell), + of two bars crossing in their middles
    T = max(1, round(S))

    def rect(l, t, w, h, colour=col['red']):
        p.items.append('<rect x="%d" y="%d" width="%d" height="%d" fill="%s"/>' % (l, t, w, h, hexcol(colour)))

    def length(v):
        """A line's length, whole pixels, as many at both sides of a line
        across it"""
        n = round(v)
        return n - (n - T) % 2

    def equals(cx, w, y1, colour):
        """= : its lines w long round cx, the first in the middle of the
        row at y1, the other 4 pixels under it"""
        L = length(w)
        l, t = round(cx - L / 2), round(y1 + S / 2 - T / 2)
        rect(l, t, L, T, colour)
        rect(l, t + round(4 * S), L, T, colour)

    b = p.box(V(103))                   # - on J
    X, Y, W, H = p.moved(b)
    L = length(W)
    rect(round(X + W / 2 - L / 2), round(Y + H / 2 - T / 2), L, T, b['c'])
    b = p.box(V(93))                    # + on K
    X, Y, W, H = p.moved(b)
    Lw, Lh = length(W), length(H)
    l, t = round(X + W / 2 - Lw / 2), round(Y + H / 2 - Lh / 2)
    rect(l, t + (Lh - T) // 2, Lw, T, b['c'])
    rect(l + (Lw - T) // 2, t, T, Lh, b['c'])
    b = p.box(V(94, 106))               # = on L
    X, Y, W, H = p.moved(b)
    equals(X + W / 2, W, Y, b['c'])
    b = p.box(V(43, 55))                # = of <= on R
    X, Y, W, H = p.moved(b)
    equals(X + W / 2, W, Y, b['c'])
    # >= on Y: > and the two lines of = were found as one
    b = p.box(V(46))
    X, Y, W, H = p.moved(b)
    p.fit('>', X, Y, 9 * S, 7 * S, b['c'], fonts=('herosb', 'cnb'))
    equals(X + 15 * S, 8 * S, Y + S, b['c'])

    # " on Q (twice) and P (Viktor's choice, after a photo of the ZX81): of
    # Heros Bold (thick, as there), QUOTE pixels high and QWIDTH wide; their
    # tops on the line of the signs; the two on Q QGAP pixels apart (more
    # than the strokes of one, as there), at the right of their place, the
    # one on P in the middle of its box
    QUOTE, QWIDTH, QGAP = 3.5, 5, 3
    qd, (q0, q1, q2, q3) = HB.outline('"')
    qsc = QUOTE * S / (q3 - q1)
    qsx = QWIDTH * S / (q2 - q0)

    def quote(left, b):
        nk = p.newkeys[p.key_of(b)]
        p.items.append('<path fill="%s" transform="matrix(%.5f 0 0 %.5f %.3f %.3f)" d="%s"/>'
                       % (hexcol(b['c']), qsx, -qsc, left - qsx * q0, (nk[1] + SIGNTOP) * S + qsc * q3, qd))

    qw = qsx * (q2 - q0)
    if V(38, 39) is not None:
        b = p.box(V(38, 39))
        X, Y, W, H = p.moved(b)
        quote(X + W - qw, b)
        quote(X + W - 2 * qw - QGAP * S, b)
    if V(50) is not None:
        b = p.box(V(50))
        X, Y, W, H = p.moved(b)
        quote(X + W / 2 - qw / 2, b)

    # : on Z, ; on X and , on the full stop (Viktor's choice, as on the
    # ZX80): the dots squares of one size (D, as large as the dots were, 3 x
    # 2 pixels), the commas of Heros, their heads as large, their tails as
    # long as the heads are high; the lower dot of : and the comma of ; as
    # far from the upper dot as in the original (7 pixels)
    b = p.box(V(132))
    X, Y, W, H = p.moved(b)
    D = round(math.sqrt(W * H))
    cd, (c0, c1, c2, c3) = comma('heros', 1.0)
    csc = D / (c2 - c0)

    def dot(l, t, colour):
        rect(l, t, D, D, colour)

    def comma_at(l, t, colour):
        p.items.append('<path fill="%s" transform="matrix(%.5f 0 0 %.5f %.3f %.3f)" d="%s"/>'
                       % (hexcol(colour), csc, -csc, l - csc * c0, t + csc * c3, cd))

    p.box(V(143))
    l, t = round(X + W / 2 - D / 2), round(Y + H / 2 - D / 2)
    dot(l, t, b['c'])
    dot(l, t + round(7 * S), b['c'])
    if V(133, 145) is not None:
        b = p.box(V(133, 145))
        X, Y, W, H = p.moved(b)
        l, t = round(X + 1.5 * S - D / 2), round(Y + S - D / 2)
        dot(l, t, b['c'])
        comma_at(l, t + round(7 * S), b['c'])
    if V(139) is not None:
        b = p.box(V(139))
        X, Y, W, H = p.moved(b)
        comma_at(round(X + 1.5 * S - D / 2), round(Y + S - D / 2), b['c'])

    # The stars (** on H, * on B): a stroke up, a bar across, two legs down
    # and a thicker middle
    def star(X, Y, W, H):
        cx = X + W / 2
        for x1, y1, x2, y2 in ((cx, Y, cx, Y + 3 * S), (X, Y + 2.5 * S, X + W, Y + 2.5 * S),
                               (cx - 0.5 * S, Y + 4 * S, X + 1.2 * S, Y + H - 0.2 * S),
                               (cx + 0.5 * S, Y + 4 * S, X + W - 1.2 * S, Y + H - 0.2 * S)):
            p.items.append('<line x1="%g" y1="%g" x2="%g" y2="%g" stroke="%s" stroke-width="%g"/>'
                           % (x1, y1, x2, y2, hexcol(col['red']), 1.1 * S))
        p.items.append('<rect x="%g" y="%g" width="%g" height="%g" fill="%s"/>'
                       % (cx - 1.5 * S, Y + 3 * S, 3 * S, 2 * S, hexcol(col['red'])))

    b = p.box(V(92))
    X, Y, W, H = p.moved(b)
    star(X, Y, 7 * S, H)
    star(X + 8 * S, Y, 7 * S, H)
    b = p.box(V(136))
    X, Y, W, H = p.moved(b)
    star(X, Y, W, H)

    # The cursor arrows above 5 - 8: outlines, the heads long; that on 5 the
    # one on 8 turned (Viktor's choice; it was a pixel longer), at the right
    # of its place
    W8 = p.moved(p.box(V(7)))[2]
    for i, direction, head, s0, s1 in ((4, 'left', 0.66, 0.3, 0.7), (7, 'right', 0.66, 0.3, 0.7),
                                       (5, 'down', 0.8, 0.25, 0.75), (6, 'up', 0.8, 0.25, 0.75)):
        b = p.box(V(i))
        X, Y, W, H = p.moved(b)
        if i in (4, 7):
            # on whole pixels, so that they are mirror images pixel for pixel
            X, W = round(X + (W - W8 if i == 4 else 0)), round(W8)
        pts = arrow_points(X, Y, W, H, direction, S, head=head, s0=s0, s1=s1)
        p.items.append('<polygon fill="none" stroke="%s" stroke-width="%g" points="%s"/>'
                       % (hexcol(col['red']), S, ' '.join('%g,%g' % q for q in pts)))

    # extra: what else the picture draws itself (extra(p))
    if extra:
        extra(p)
    print('%s, not drawn: %s' % (name, p.unused()), file=sys.stderr)
    open(sys.argv[1], 'w').write(p.svg((0, 0, 0)))


if __name__ == '__main__':
    draw('zx81')
