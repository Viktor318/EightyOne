# zx80.py <out.svg> [scale]: the ZX80 keyboard picture, drawn again at a scale (2 if not given)
#
# The picture with the ZXpand (zx80zxpand.py) is the same picture with a
# few other texts: its script calls draw() with its own. The texts of the
# ZX80 are found in it by their pixels; what is not found is not drawn.
import math
import os
import sys
from kbdraw import Picture, hexcol, arrow_points, font, comma

HERE = os.path.dirname(os.path.abspath(__file__))
KEY, YELLOW, WHITE = (72, 42, 254), (255, 201, 33), (255, 255, 255)

# The texts of the ZX80 picture (the numbers of zx80.json)
TEXTS = {
    # 1 - 0
    4: 'NOT', 5: 'AND', 6: 'THEN', 7: 'TO', 8: 'HOME', 9: 'RUBOUT',
    10: '1', 11: '2', 12: '3', 13: '4', 14: '5', 15: '6', 16: '7', 17: '8', 18: '9', 19: '0',
    # Q - P
    20: 'NEW', 21: 'LOAD', 22: 'SAVE', 23: 'RUN', 24: 'CONT', 25: 'REM', 26: 'IF', 27: 'INPUT',
    28: 'PRINT',
    34: '"', 35: '$', 36: '(', 37: ')',
    39: 'Q', 40: 'W', 41: 'E', 42: 'R', 43: 'T', 44: 'Y', 45: 'U', 46: 'I', 47: 'O', 48: 'P',
    # A - NEW LINE
    49: 'LIST', 50: 'STOP', 51: 'DIM', 52: 'FOR', 53: 'GOTO', 54: 'POKE', 55: 'RAND', 56: 'LET',
    57: 'EDIT', 67: 'NEW', 77: 'LINE',
    68: 'A', 69: 'S', 70: 'D', 71: 'F', 72: 'G', 73: 'H', 74: 'J', 75: 'K', 76: 'L',
    # SHIFT - SPACE
    78: 'CLEAR', 79: 'CLS', 80: 'GOSUB', 81: 'RET', 82: 'NEXT', 83: 'BREAK', 93: 'SHIFT',
    (85, 95): ';', 86: '?', 88: 'OR', 89: '<', 90: '>', 91: ',', 92: '£',
    96: 'Z', 97: 'X', 98: 'C', 99: 'V', 100: 'B', 101: 'N', 102: 'M', 103: 'SPACE', 104: '.',
}
BIG = set([10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
           68, 69, 70, 71, 72, 73, 74, 75, 76, 96, 97, 98, 99, 100, 101, 102])


def draw(name, own=None):
    """The picture <name> (<name>.json, orig/<name>.png): the ZX80's texts
    where they are found, and its own texts ({number: text}, as words)"""
    p = Picture(os.path.join(HERE, name + '.json'), os.path.join(HERE, 'orig/%s.png' % name),
                float(sys.argv[2]) if len(sys.argv) > 2 else 2)
    S = p.S
    if name == 'zx80':
        here = dict((i, i) for i in p.words)
    else:
        here = p.same_texts(os.path.join(HERE, 'zx80.json'), os.path.join(HERE, 'orig/zx80.png'))

    def V(*ids):
        """The numbers here of ZX80 texts (None if one is not found)"""
        v = tuple(here.get(i) for i in ids)
        if None in v:
            return None
        return v[0] if len(v) == 1 else v

    # Keys: one pitch and width a row; SHIFT keeps its place, and the last
    # row its line (the last two keys were 1 pixel higher)
    shift = p.key_of(p.words[V(93)])
    p.regular_rows(wide=(shift,))
    for k in p.newkeys:
        if k[1] > 170:
            k[1] = 183
    p.draw_keys(KEY, 3)

    texts = {}
    for ids, t in TEXTS.items():
        v = V(*ids) if isinstance(ids, tuple) else V(ids)
        if v is not None:
            texts[v] = t
    texts.update(own or {})
    big = set(V(i) for i in BIG) - set([None])
    digits = set(V(i) for i in range(10, 20)) - set([None])

    # The words (white, yellow and blue): all of one size, as high as in
    # the original and on the ZX81 (7 pixels), in the font's own spacing;
    # those above the keys in the middle of their keys. The letters and the
    # signs thin, as in the original
    for ids, t in texts.items():
        if ids in digits:
            # the digits in the middle of their keys (Viktor's choice), of
            # their sizes
            b = p.box(ids)
            X, Y, W, H = p.moved(b)
            k = p.newkeys[p.key_of(b)]
            p.fit(t, (k[0] + k[2] / 2) * S - W / 2, (k[1] + k[3] / 2) * S - H / 2, W, H, b['c'],
                  fonts=('heros',))
        elif ids in big:
            p.text(ids, t, fonts=('heros',))
        elif t in ';,':
            continue                    # with : (below)
        elif t == '"':
            # as on the Spectrum (Viktor's choice): Heros Cn Bold, 5 pixels
            # high, not stretched; in the middle of its box, at its top
            b = p.box(ids)
            X, Y, W, H = p.moved(b)
            d, (x0, y0, x1, y1) = font('cnb').outline('"')
            sc = 5 * S / (y1 - y0)
            p.items.append('<path fill="%s" transform="matrix(%.5f 0 0 %.5f %.3f %.3f)" d="%s"/>'
                           % (hexcol(b['c']), sc, -sc, X + W / 2 - sc * (x0 + x1) / 2, Y + sc * y1, d))
        elif len(t) > 1 and any(c.isalpha() for c in t):
            p.word(ids, t, 'cnb', 7, over=True)
        else:
            p.text(ids, t, fonts=('heros', 'cn'))

    # Block graphics (Viktor's choice): their frames 2 pixels thick (at
    # 100%; they were 1 or 2 pixels as their rows fell), their four quarters
    # of one size, the grey ones a chequer of whole pixel squares; as large
    # as they were (10 pixels), 3 and 4 pixels from the right and top edges
    # of their keys
    F = max(1, round(S))
    NB = 2 * round((10 * S - 2 * F) / 2) + 2 * F
    for i in (29, 30, 31, 32, 33, 58, 59, 60, 61, 62):
        p.block(V(i), NB, F, max(1, round(S)), right=round(3 * S), top=round(4 * S))

    # The straight signs (Viktor's choice): the lines of - + = as thick as
    # each other (T, on whole pixels; they were 1 or 2 pixels as their
    # rows fell), + of two bars as long, crossing in their middles; the two
    # dots of : of one size
    T = max(1, round(S))

    def rect(l, t, w, h):
        p.items.append('<rect x="%d" y="%d" width="%d" height="%d" fill="%s"/>' % (l, t, w, h, hexcol(YELLOW)))

    def bars(i):
        """The box of a sign, the length of its lines (whole pixels, as
        many at both sides of a line across it) and their left edge"""
        b = p.box(V(i))
        X, Y, W, H = p.moved(b)
        L = round(W)
        L -= (L - T) % 2
        return X, Y, W, H, L, round(X + W / 2 - L / 2)

    X, Y, W, H, L, l = bars(65)                 # - on J
    rect(l, round(Y + H / 2 - T / 2), L, T)
    X, Y, W, H, L, l = bars(64)                 # + on K
    t = round(Y + H / 2 - L / 2)
    rect(l, t + (L - T) // 2, L, T)
    rect(l + (L - T) // 2, t, T, L)
    X, Y, W, H, L, l = bars(66)                 # = on L: its lines where they were
    t = round(Y + S / 2 - T / 2)
    rect(l, t, L, T)
    rect(l, t + round(3 * S), L, T)
    # : on Z: two squares, as wide as the dots were (2 pixels), as far
    # apart (6 pixels)
    b = p.box(V(84))
    p.box(V(94))
    X, Y, W, H = p.moved(b)
    D = round(W)
    l, t = round(X + W / 2 - D / 2), round(Y + H / 2 - D / 2)
    rect(l, t, D, D)
    rect(l, t + round(6 * S), D, D)
    # ; on X (Viktor's choice): a dot as those of :, and under it, as far
    # as the dots of : are, a comma of Heros, its head as large, its tail as
    # long as the head is high (as in the original; that of the font is
    # longer)
    b = p.box(V(85, 95))
    X, Y, W, H = p.moved(b)
    l, t = round(X + S - D / 2), round(Y + S - D / 2)
    rect(l, t, D, D)
    d, (x0, y0, x1, y1) = comma('heros', 1.0)
    sc = D / (x1 - x0)
    p.items.append('<path fill="%s" transform="matrix(%.5f 0 0 %.5f %.3f %.3f)" d="%s"/>'
                   % (hexcol(YELLOW), sc, -sc, l - sc * x0, t + round(6 * S) + sc * y1, d))
    # , on the full stop (Viktor's choice): as that of ;, its head where the
    # head was
    b = p.box(V(91))
    X, Y, W, H = p.moved(b)
    l, t = round(X + S - D / 2), round(Y + S - D / 2)
    p.items.append('<path fill="%s" transform="matrix(%.5f 0 0 %.5f %.3f %.3f)" d="%s"/>'
                   % (hexcol(YELLOW), sc, -sc, l - sc * x0, t + sc * y1, d))

    # / on V: a thin line, at 60 degrees (Viktor's choice; it was steeper),
    # as high as it was, in the middle of its place
    b = p.box(V(87))
    X, Y, W, H = p.moved(b)
    dy = H - 0.4 * S
    dx = dy / math.tan(math.radians(60))
    cx, cy = X + W / 2, Y + H / 2
    p.items.append('<line x1="%.2f" y1="%.2f" x2="%.2f" y2="%.2f" stroke="%s" stroke-width="%g"/>'
                   % (cx - dx / 2, cy + dy / 2, cx + dx / 2, cy - dy / 2, hexcol(YELLOW), 1.0 * S))

    # The stars (* on P, ** on H): a bar across and two steep strokes
    def star(X, Y):
        for x1, y1, x2, y2 in ((X, Y + 2.5 * S, X + 5 * S, Y + 2.5 * S),
                               (X + 1.1 * S, Y + 0.1 * S, X + 3.9 * S, Y + 4.9 * S),
                               (X + 3.9 * S, Y + 0.1 * S, X + 1.1 * S, Y + 4.9 * S)):
            p.items.append('<line x1="%g" y1="%g" x2="%g" y2="%g" stroke="%s" stroke-width="%g"/>'
                           % (x1, y1, x2, y2, hexcol(YELLOW), 1.0 * S))

    b = p.box(V(38))
    X, Y, W, H = p.moved(b)
    star(X, Y)
    b = p.box(V(63))
    X, Y, W, H = p.moved(b)
    star(X, Y)
    star(X + 7 * S, Y)

    # The cursor arrows above 5 - 8: outlines, the heads long (as on the
    # ZX81)
    for i, direction, head, s0, s1 in ((0, 'left', 0.625, 0.3, 0.7), (3, 'right', 0.66, 0.3, 0.7),
                                       (1, 'down', 0.8, 0.25, 0.75), (2, 'up', 0.8, 0.25, 0.75)):
        b = p.box(V(i))
        X, Y, W, H = p.moved(b, 'x')
        pts = arrow_points(X, Y, W, H, direction, S, head=head, s0=s0, s1=s1)
        p.items.append('<polygon fill="none" stroke="%s" stroke-width="%g" points="%s"/>'
                       % (hexcol(YELLOW), S, ' '.join('%g,%g' % q for q in pts)))

    print('%s, not drawn: %s' % (name, p.unused()), file=sys.stderr)
    open(sys.argv[1], 'w').write(p.svg((0, 0, 0)))


if __name__ == '__main__':
    draw('zx80')
