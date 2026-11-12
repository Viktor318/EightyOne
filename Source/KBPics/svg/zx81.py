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
    # The red texts (not SHIFT): of Heros (thin, as on the ZX81; Viktor's
    # choice). The white ones above and under the keys: WGAP pixels from
    # them (above to the base line, under from the tops of the capitals),
    # beginning WLEFT (pixels of the picture) from the left edges of their
    # keys (Viktor's choice; they were where they were in the original)
    WGAP, WLEFT = 2, round(3 * S)
    # the red words at most REDW of the font's own width, as the big letters
    # (Viktor's choice; narrower where there is no room)
    REDW = 0.8

    def red(ids):
        return list(p.words[ids if isinstance(ids, int) else ids[0]]['c']) == list(col['red'])

    # The red texts and signs on the keys (Viktor's choice, as on the ZX81):
    # their right edges as far from the right edges of their keys as those
    # of the graphics; FUNCTION, GRAPHICS, RUBOUT and the words of the big
    # keys (NEW LINE, SPACE, SHIFT) in the middle of their keys. Each is
    # marked as drawn (reds: its items, its key, whether in the middle,
    # whether on whole pixels), and moved at the end; those side by side on
    # a key moved together
    reds = []
    CENTRED = ('GRAPHICS', 'RUBOUT', 'DELETE', 'FUNCTION', 'NEW', 'LINE', 'ENTER', 'SPACE', 'SHIFT')

    def mark(b, start, centre=False, whole=False):
        # whole: on whole pixels, to be moved by whole pixels
        if len(p.items) > start:
            reds.append((start, len(p.items), p.key_of(b), centre, whole))

    def on_key(b):
        kx, ky, kw, kh = p.keys[p.key_of(b)]
        return kx <= b['x'] and b['x2'] <= kx + kw and ky <= b['y'] and b['y2'] <= ky + kh

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
    # The signs of the font (Viktor's choice): all of one size, of Heros
    # (thin, as on the ZX81), the capitals SIGN pixels high, not stretched;
    # their tops (that of the capitals) on one line a row, SIGNTOP pixels
    # under the tops of the keys (as most were); each in the middle of its
    # box. < and > stay wide, as in the original and on the ZX81 (Viktor's
    # choice); " and ; and , below
    SIGN, SIGNTOP = 10, 4
    FONTSIGNS = '$()?£'
    SF = font('heros')

    def font_sign(t, b, X, W):
        nk = p.newkeys[p.key_of(b)]
        d, (x0, y0, x1, y1) = SF.outline(t)
        sc = SIGN * S / SF.outline('H')[1][3]
        left = X + W / 2 - sc * (x1 - x0) / 2
        p.items.append('<path fill="%s" transform="matrix(%.5f 0 0 %.5f %.3f %.3f)" d="%s"/>'
                       % (hexcol(b['c']), sc, -sc, left - sc * x0, (nk[1] + SIGNTOP + SIGN) * S, d))

    # The big letters and digits (Viktor's choice): all BIGW times as wide
    # as in the font, narrow as on the ZX81 (they were made as wide as their
    # boxes, 0.64 to 1 times as wide; of the font's own widths W would reach
    # its graphic), as high as they were, in the middle of their boxes
    BIGW = 0.8

    def big_box(b, t):
        X, Y, W, H = p.moved(b)
        d, (x0, y0, x1, y1) = font('herosb').outline(t)
        w = BIGW * (x1 - x0) * H / (y1 - y0)
        return X + W / 2 - w / 2, Y, w, H

    for ids, t in texts.items():
        start = len(p.items)
        if ids in big:
            b = p.box(ids)
            p.fit(t, *big_box(b, t), colour=b['c'], fonts=('herosb',))
        elif len(t) > 1 and any(c.isalpha() for c in t):
            b = p.box(ids)
            if list(b['c']) == list(col['label']) and not on_key(b):
                k = p.key_of(b)
                kx, ky, kw, kh = p.keys[k]
                nk = p.newkeys[k]
                if b['y'] + b['y2'] < 2 * ky + kh:
                    base = round(nk[1] * S) - WGAP * S
                else:
                    base = round((nk[1] + nk[3]) * S) + (WGAP + cap) * S
                p.word(ids, t, 'cnb', cap, left=round(nk[0] * S) + WLEFT, base=base)
            elif red(ids):
                p.word(ids, t, 'heros', cap, width=REDW)
            else:
                p.word(ids, t, 'cnb', cap)
        elif t in FONTSIGNS and len(t) == 1:
            b = p.box(ids)
            X, Y, W, H = p.moved(b)
            font_sign(t, b, X, W)
        elif t in '";,<>':
            continue                    # below
        else:
            p.text(ids, t, fonts=('heros',) if red(ids) else ('cnb', 'herosb'))
        b = p.box(ids)
        if ids not in big and on_key(b) and (red(ids) or t in CENTRED):
            mark(b, start, t in CENTRED)
    # small words (under bigger ones): SMALLH pixels high (they were 4), the
    # red ones at most as wide as the red words, the others as the original's
    # proportions allow, but beginning SMALLGAP pixels (of the picture)
    # after the big letter at their left (Viktor's choice); their right
    # edges where those of the red texts are (see reds); as far from the
    # word above them as from the graphic under them (Viktor's choice)
    SMALLH, SMALLGAP = 5, 2
    F = max(1, int(S))
    NB = 2 * round((11.5 * S - 2 * F) / 2) + 2 * F
    for ids, t in (small or {}).items():
        b = p.box(ids)
        ki = p.key_of(b)
        nk = p.newkeys[ki]
        h = SMALLH
        right = round((nk[0] + nk[2]) * S) - round(3 * S)
        lefts = [big_box(p.words[i], texts[i]) for i in big if p.key_of(p.words[i]) == ki]
        left = max([x + w for x, y, w, hh in lefts] + [nk[0] * S + 2 * S]) + SMALLGAP
        if red(ids):
            # at most as wide as the red words (REDW of the font's own width)
            d, (x0, y0, x1, y1) = SF.outline(t)
            w = min(REDW * (x1 - x0) * h * S / (y1 - y0), right - left)
        else:
            w = min((b['x2'] - b['x']) * h / (b['y2'] - b['y']) * S, right - left)
        above = max(o['y2'] for o in p.words.values() if o is not b and p.key_of(o) == ki
                    and o['y2'] <= b['y'] and o['c'] == b['c'])
        above = (above + nk[1] - p.keys[ki][1]) * S
        graphic = round((nk[1] + nk[3]) * S) - round(4 * S) - NB
        start = len(p.items)
        p.fit(t, right - w, (above + graphic - h * S) / 2, w, h * S, b['c'],
              fonts=('heros',) if red(ids) else ('cnb',), maxtrack=0.04)
        if red(ids):
            mark(b, start)

    # SHIFT: big, as in the original
    start = len(p.items)
    p.text(V(141), 'SHIFT', fonts=('herosb', 'cnb'))
    mark(p.box(V(141)), start, True)

    # 0 with its slash, from the bottom left to the top right
    b = p.box(V(27))
    X, Y, W, H = big_box(b, '0')
    p.fit('0', X, Y, W, H, col['black'], fonts=('herosb',))
    p.items.append('<line x1="%g" y1="%g" x2="%g" y2="%g" stroke="%s" stroke-width="%g"/>'
                   % (X + 0.6 * S, Y + H - 0.6 * S, X + W - 0.6 * S, Y + 0.6 * S, hexcol(col['black']), 1.3 * S))

    # π: as high as the words around it, in their font, where they are
    b = p.box(V(159))
    X, Y, W, H = p.moved(b)
    nk = p.newkeys[p.key_of(b)]
    p.fit('π', round(nk[0] * S) + WLEFT, round((nk[1] + nk[3]) * S) + WGAP * S, W + 0.5 * S, H + 1 * S,
          col['label'], fonts=('cnb',))

    # / on V: a line, as thin as the strokes of the words, at 60 degrees
    # (Viktor's choice; it was steeper), as high as it was, in the middle of
    # its place
    b = p.box(V(135))
    X, Y, W, H = p.moved(b)
    start = len(p.items)
    dy = H - 0.4 * S
    dx = dy / math.tan(math.radians(60))
    cx, cy = X + W / 2, Y + H / 2
    p.items.append('<line x1="%.2f" y1="%.2f" x2="%.2f" y2="%.2f" stroke="%s" stroke-width="%g"/>'
                   % (cx - dx / 2, cy + dy / 2, cx + dx / 2, cy - dy / 2, hexcol(col['red']), 1.25 * S))
    mark(b, start)

    # The block graphics (Viktor's choice): all of one size (about 11.5
    # pixels; they were 10 to 12 wide, 12 high), their frames thin (F, on
    # whole pixels; they were 1 or 2 pixels as their rows fell), their four
    # quarters of one size (some were a pixel narrower), each filled, empty
    # or dotted (the grey, a chequer of the original's dots) as in the
    # original; 3 and 4 pixels from the right and bottom edges of their keys
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
    start = len(p.items)
    rect(round(X + W / 2 - L / 2), round(Y + H / 2 - T / 2), L, T, b['c'])
    mark(b, start, whole=True)
    b = p.box(V(93))                    # + on K: its two lines as long
    X, Y, W, H = p.moved(b)             # as the wider was (Viktor's choice;
    Lw = Lh = length(9 * S)             # 9 and 7 pixels)
    l, t = round(X + W / 2 - Lw / 2), round(Y + H / 2 - Lh / 2)
    start = len(p.items)
    rect(l, t + (Lh - T) // 2, Lw, T, b['c'])
    rect(l + (Lw - T) // 2, t, T, Lh, b['c'])
    mark(b, start, whole=True)
    b = p.box(V(94, 106))               # = on L
    X, Y, W, H = p.moved(b)
    start = len(p.items)
    equals(X + W / 2, W, Y, b['c'])
    mark(b, start, whole=True)

    # < and > (Viktor's choice, as on the ZX81): wide, LTW pixels wide and
    # LTH high (thin, of Heros, stretched; they were 8 or 9 wide), LTGAP
    # pixels from the = or the other one next to them (they were 2 or 3); in
    # the middle of the line of the = (as they were)
    LTW, LTH, LTGAP = 11, 7, 2.5

    def chevron(t, left, cy, colour):
        p.fit(t, left, cy - LTH * S / 2, LTW * S, LTH * S, colour, fonts=('heros',))

    def bars(left, cy, colour):
        L = length(8 * S)
        t = round(cy - 2 * S - T / 2)
        rect(round(left), t, L, T, colour)
        rect(round(left), t + round(4 * S), L, T, colour)

    for first, second in ((42, '='), (44, 45), (46, '='), (137, None), (138, None)):
        if V(first) is None:
            continue
        b = p.box(V(first))
        X, Y, W, H = p.moved(b)
        cy = Y + 3.5 * S
        t = '>' if first in (45, 46, 138) else '<'
        start = len(p.items)
        chevron(t, X, cy, b['c'])
        if second == '=':
            if first == 42:
                p.box(V(43, 55))
            bars(X + (LTW + LTGAP) * S, cy, b['c'])
        elif second is not None:
            p.box(V(second))
            chevron('>', X + (LTW + LTGAP) * S, cy, b['c'])
        mark(b, start, whole=second == '=')

    # " on Q (twice) and P (Viktor's choice, after a photo of the ZX81): of
    # Heros, QUOTE pixels high and QWIDTH wide; their
    # tops on the line of the signs; the two on Q QGAP pixels apart (more
    # than the strokes of one, as there), at the right of their place, the
    # one on P in the middle of its box
    QUOTE, QWIDTH, QGAP = 3.5, 5, 3
    qd, (q0, q1, q2, q3) = SF.outline('"')
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
        start = len(p.items)
        quote(X + W - qw, b)
        quote(X + W - 2 * qw - QGAP * S, b)
        mark(b, start)
    if V(50) is not None:
        b = p.box(V(50))
        X, Y, W, H = p.moved(b)
        start = len(p.items)
        quote(X + W / 2 - qw / 2, b)
        mark(b, start)

    # : on Z, ; on X and , on the full stop (Viktor's choice, as on the
    # ZX80): the dots squares of one size (D, as large as the dots were, 3 x
    # 2 pixels), the commas of Heros, their heads as large, their tails as
    # long as the heads are high; the lower dot of : and the comma of ; as
    # far from the upper dot as on the ZX80 (6 pixels, Viktor's choice; they
    # were 7)
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
    start = len(p.items)
    dot(l, t, b['c'])
    dot(l, t + round(6 * S), b['c'])
    mark(b, start, whole=True)
    if V(133, 145) is not None:
        b = p.box(V(133, 145))
        X, Y, W, H = p.moved(b)
        l, t = round(X + 1.5 * S - D / 2), round(Y + S - D / 2)
        start = len(p.items)
        dot(l, t, b['c'])
        mark(b, start, whole=True)
        start = len(p.items)
        comma_at(l, t + round(6 * S), b['c'])
        mark(b, start)
    if V(139) is not None:
        b = p.box(V(139))
        X, Y, W, H = p.moved(b)
        start = len(p.items)
        comma_at(round(X + 1.5 * S - D / 2), round(Y + S - D / 2), b['c'])
        mark(b, start)

    # The stars (** on H, * on B): five pointed, as on the ZX81 (Viktor's
    # choice, after a photo of it): five strokes from the middle with round
    # ends, STARW thick, and a full middle (a circle STARC across); in the
    # middle of their places
    STARW, STARC = 1.4, 0.9

    def star(X, Y, W, H):
        w = STARW * S
        r = (H - w) / (1 + math.sin(math.radians(54)))
        cx, cy = X + W / 2, Y + w / 2 + r
        for k in range(5):
            ang = math.radians(90 + 72 * k)
            p.items.append('<line x1="%.2f" y1="%.2f" x2="%.2f" y2="%.2f" stroke="%s" '
                           'stroke-width="%.2f" stroke-linecap="round"/>'
                           % (cx, cy, cx + r * math.cos(ang), cy - r * math.sin(ang), hexcol(col['red']), w))
        p.items.append('<circle cx="%.2f" cy="%.2f" r="%.2f" fill="%s"/>' % (cx, cy, STARC * S, hexcol(col['red'])))

    # (Viktor's choice: a pixel higher than they were, and so a pixel apart
    # more on H)
    SG = S
    b = p.box(V(92))
    X, Y, W, H = p.moved(b)
    H += SG
    start = len(p.items)
    star(X, Y, 7 * S + SG, H)
    star(X + 8 * S + SG, Y, 7 * S + SG, H)
    mark(b, start)
    b = p.box(V(136))
    X, Y, W, H = p.moved(b)
    start = len(p.items)
    star(X, Y, W + SG, H + SG)
    mark(b, start)

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
        start = len(p.items)
        p.items.append('<polygon fill="none" stroke="%s" stroke-width="%g" points="%s"/>'
                       % (hexcol(col['red']), S, ' '.join('%g,%g' % q for q in pts)))
        mark(b, start, whole=i in (4, 7))

    # The red texts and signs moved (see reds above): on each key those
    # side by side (one over the other they are moved each)
    GR = round(3 * S)
    for k in sorted(set(r[2] for r in reds)):
        nk = p.newkeys[k]
        area = (nk[0] * S - 8, nk[1] * S - 8, (nk[0] + nk[2]) * S + 8, (nk[1] + nk[3]) * S + 8)
        lines = []
        for start, end, kk, centre, whole in reds:
            if kk != k:
                continue
            ink = p.ink_box([(start, end)], area)
            if ink is None:
                continue
            for ln in lines:
                if ink[1] < ln['y1'] and ln['y0'] < ink[3]:
                    ln['ranges'].append((start, end))
                    ln['x0'], ln['x1'] = min(ln['x0'], ink[0]), max(ln['x1'], ink[2])
                    ln['y0'], ln['y1'] = min(ln['y0'], ink[1]), max(ln['y1'], ink[3])
                    ln['centre'] |= centre
                    ln['whole'] |= whole
                    break
            else:
                lines.append(dict(ranges=[(start, end)], x0=ink[0], y0=ink[1], x1=ink[2], y1=ink[3],
                                  centre=centre, whole=whole))
        for ln in lines:
            if ln['centre']:
                dx = (nk[0] + nk[2] / 2) * S - (ln['x0'] + ln['x1']) / 2
            else:
                dx = round((nk[0] + nk[2]) * S) - GR - ln['x1']
            p.move(ln['ranges'], round(dx) if ln['whole'] else dx)

    # extra: what else the picture draws itself (extra(p))
    if extra:
        extra(p)
    print('%s, not drawn: %s' % (name, p.unused()), file=sys.stderr)
    open(sys.argv[1], 'w').write(p.svg((0, 0, 0)))


if __name__ == '__main__':
    draw('zx81')
