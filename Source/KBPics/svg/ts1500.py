# ts1500.py <out.svg> [scale]: the Timex Sinclair 1500 keyboard picture, drawn again at a scale (2 if not given)
#
# The picture with the ZXpand (ts1500zxpand.py) is the same with a few other
# texts: its script calls draw() with its own. The texts of the TS1500 are
# found in it by their pixels; what is not found is not drawn.
import os
import sys
import numpy as np
from kbdraw import Picture, hexcol

HERE = os.path.dirname(os.path.abspath(__file__))
BACK, KEY, FACE, WHITE, BLACK = (123, 123, 123), (211, 209, 209), (145, 143, 143), (255, 255, 255), (0, 0, 0)

# The texts of the TS1500 picture (the numbers of ts1500.json)
TEXTS = {
    # 1 - 0
    10: '1', 11: '2', 12: '3', 13: '4', 14: '5', 15: '6', 16: '7', 17: '8', 18: '9',
    22: 'EDIT', 24: 'AND', 26: 'THEN', 28: 'TO', 36: 'GRAPHICS', 37: 'DELETE',
    # Q - P
    38: 'PLOT', 39: 'UNPLOT', 40: 'REM', 41: 'RUN', 42: 'RAND', 43: 'RETURN', 44: 'IF', 45: 'INPUT',
    46: 'POKE', 47: 'PRINT',
    58: 'Q', 59: 'W', 60: 'E', 61: 'R', 62: 'T', 63: 'Y', 64: 'U', 65: 'I', 66: 'O', 67: 'P',
    68: '"', 69: '"', 71: 'OR', 73: 'STEP', 75: '<', 78: '<', 79: '>', 83: '$', 84: '(', 85: ')', 86: '"',
    88: 'SIN', 89: 'COS', 90: 'TAN', 91: 'INT', 92: 'RND', 93: 'STR$', 94: 'CHR$', 95: 'CODE', 96: 'PEEK',
    97: 'TAB',
    # A - ENTER
    99: 'NEW', 100: 'SAVE', 101: 'DIM', 102: 'FOR', 103: 'GOTO', 104: 'GOSUB', 105: 'LOAD', 106: 'LIST',
    107: 'LET',
    109: 'A', 111: 'S', 113: 'D', 115: 'F', 117: 'G', 119: 'H', 121: 'J', 123: 'K', 125: 'L',
    128: 'STOP', 130: 'LPRINT', 132: 'SLOW', 134: 'FAST', 136: 'LLIST', 127: 'ENTER', 141: 'FUNCTION',
    144: 'ARCSIN', 145: 'ARCCOS', 146: 'ARCTAN', 147: 'SGN', 148: 'ABS', 149: 'SQR', 150: 'VAL',
    151: 'LEN', 152: 'USR',
    # SHIFT - SPACE
    153: 'COPY', 154: 'CLEAR', 155: 'CONT', 156: 'CLS', 157: 'SCROLL', 158: 'NEXT', 159: 'PAUSE',
    160: 'BREAK',
    163: 'Z', 165: 'X', 167: 'C', 169: 'V', 171: 'B', 173: 'N', 175: 'M', 178: 'SPACE',
    (183, 188): ';', 184: '?', 186: '£', 190: '<', 191: '>', 181: ',', 180: '.',
    192: 'LN', 193: 'EXP', 194: 'AT', 195: 'INKEY$', 196: 'NOT',
}
BIG = set([10, 11, 12, 13, 14, 15, 16, 17, 18, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67,
           109, 111, 113, 115, 117, 119, 121, 123, 125, 163, 165, 167, 169, 171, 173, 175])
GRAPHICS = (23, 25, 27, 29, 30, 32, 34, 35, 70, 72, 74, 77, 80, 82, 129, 131, 133, 135, 137, 139)
# the arrows on 5 - 8: a shaft and a head, in their boxes (as pointing down)
ARROWS = {20: 'left', 21: 'right', 31: 'down', 33: 'up'}
ARROW = [(4 / 18, 0), (14 / 18, 0), (14 / 18, 2 / 7), (1, 2 / 7), (0.5, 1), (0, 2 / 7), (4 / 18, 2 / 7)]


def keys(p, key, face, wide):
    """The keys of the TS1500 and the Lambda: a light square and on it a
    darker rounded top (the analysis found the tops as texts of their
    colour). One pitch and width a row (the wide keys keep theirs); the tops
    on them as on the original, moved with them. Returns the tops"""
    S = p.S
    faces = {}
    for i, (x, y, w, h) in enumerate(p.keys):
        fs = [o for o in p.words.values() if o['c'] == list(face) and x <= o['x'] and o['x2'] <= x + w
              and y <= o['y'] and o['y2'] <= y + h]
        f = max(fs, key=lambda o: (o['x2'] - o['x']) * (o['y2'] - o['y']))
        faces[i] = f
        p.used.add(f['id'])
    p.regular_rows(wide=wide)
    p.draw_keys(key, 0)
    for i, f in faces.items():
        k, nk = p.keys[i], p.newkeys[i]
        x = f['x'] + nk[0] - k[0]
        w = f['x2'] - f['x'] + nk[2] - k[2]
        l, r = round(x * S), round((x + w) * S)
        t, b = round(f['y'] * S), round(f['y2'] * S)
        p.items.append('<rect x="%d" y="%d" width="%d" height="%d" rx="%g" fill="%s"/>'
                       % (l, t, r - l, b - t, 2.5 * S, hexcol(face)))
    return faces


def star(p, X, Y, W, H, colour=(0, 0, 0)):
    """A star as on the ZX81: a stroke up, a bar across, two legs down and a
    thicker middle"""
    S = p.S
    cx = X + W / 2
    for x1, y1, x2, y2 in ((cx, Y, cx, Y + 3 * S), (X, Y + 2.5 * S, X + W, Y + 2.5 * S),
                           (cx - 0.5 * S, Y + 4 * S, X + 1.2 * S, Y + H - 0.2 * S),
                           (cx + 0.5 * S, Y + 4 * S, X + W - 1.2 * S, Y + H - 0.2 * S)):
        p.items.append('<line x1="%g" y1="%g" x2="%g" y2="%g" stroke="%s" stroke-width="%g"/>'
                       % (x1, y1, x2, y2, hexcol(colour), 1.1 * S))
    p.items.append('<rect x="%g" y="%g" width="%g" height="%g" fill="%s"/>'
                   % (cx - 1.5 * S, Y + 3 * S, 3 * S, 2 * S, hexcol(colour)))


def draw(name, own=None, small=None):
    """The picture <name> (<name>.json, orig/<name>.png): the TS1500's
    texts where they are found, and its own texts ({number: text}, as words;
    small: words in small letters under others, as in the original)"""
    p = Picture(os.path.join(HERE, name + '.json'), os.path.join(HERE, 'orig/%s.png' % name),
                float(sys.argv[2]) if len(sys.argv) > 2 else 2)
    S = p.S
    # the colours of the picture: the greys that are almost one are one
    pure = np.array([BACK, KEY, FACE, WHITE, BLACK])
    near = ((p.im[:, :, None, :] - pure[None, None]) ** 2).sum(-1).argmin(-1)
    p.im = pure[near]
    if name == 'ts1500':
        here = dict((i, i) for i in p.words)
    else:
        z = Picture(os.path.join(HERE, 'ts1500.json'), os.path.join(HERE, 'orig/ts1500.png'))
        z.im = pure[((z.im[:, :, None, :] - pure[None, None]) ** 2).sum(-1).argmin(-1)]
        here = p.same_texts(os.path.join(HERE, 'ts1500.json'), z.im)

    def V(*ids):
        v = tuple(here.get(i) for i in ids)
        if None in v:
            return None
        return v[0] if len(v) == 1 else v

    faces = keys(p, KEY, FACE, (30, 39))

    texts = {}
    for ids, t in TEXTS.items():
        v = V(*ids) if isinstance(ids, tuple) else V(ids)
        if v is not None:
            texts[v] = t
    texts.update(own or {})
    big = set(V(i) for i in BIG) - set([None])

    def on_face(ids):
        b = p.box(ids)
        p.used.difference_update(ids if isinstance(ids, tuple) else (ids,))
        return any(f['x'] <= b['x'] and b['x2'] <= f['x2'] and f['y'] <= b['y'] and b['y2'] <= f['y2']
                   for f in faces.values())

    # The words: all of one size, as most of them (6 pixels high), in the
    # font's own spacing; those on the faces kept off their edges
    for ids, t in texts.items():
        if ids in big:
            p.text(ids, t, fonts=('herosb',))
        elif len(t) > 1 and any(c.isalpha() for c in t):
            p.word(ids, t, 'cnb', 6, margin=5 if on_face(ids) else 2)
        else:
            p.text(ids, t, fonts=('herosb', 'cnb'))
    for ids, t in (small or {}).items():
        p.word(ids, t, 'cnb', 5, margin=5)

    # SHIFT: big, as in the original
    p.text(V(179), 'SHIFT', fonts=('herosb', 'cnb'))

    # 0 with its slash
    b = p.box(V(19))
    X, Y, W, H = p.moved(b)
    p.fit('0', X, Y, W, H, WHITE, fonts=('herosb',))
    p.items.append('<line x1="%g" y1="%g" x2="%g" y2="%g" stroke="#ffffff" stroke-width="%g"/>'
                   % (X + W - 0.6 * S, Y + 0.6 * S, X + 0.6 * S, Y + H - 0.6 * S, 1.3 * S))

    # π: as high as the words around it, in their font
    b = p.box(V(197))
    X, Y, W, H = p.moved(b)
    p.fit('π', X, Y, W, H, BLACK, fonts=('cnb',))

    # / on V: a line
    b = p.box(V(185))
    X, Y, W, H = p.moved(b)
    p.items.append('<line x1="%g" y1="%g" x2="%g" y2="%g" stroke="#000000" stroke-width="%g"/>'
                   % (X + 0.5 * S, Y + H - 0.2 * S, X + W - 0.5 * S, Y + 0.2 * S, 1.25 * S))

    # Block graphics (the grey ones dotted) and the straight signs: squares
    for i in GRAPHICS:
        p.dotted(V(i))
    for i in (76, 87, 140, 142, 143, 182, 187):
        p.pixels(V(i))

    # >= on Y: > and the two bars of = were found as one
    b = p.box(V(81))
    X, Y, W, H = p.moved(b)
    p.fit('>', X, Y, 9 * S, 7 * S, BLACK, fonts=('herosb', 'cnb'))
    for dy in (1, 5):
        p.items.append('<rect x="%d" y="%d" width="%d" height="%d" fill="#000000"/>'
                       % (round(X + 11 * S), round(Y + dy * S), round(X + 19 * S) - round(X + 11 * S),
                          round(Y + (dy + 1) * S) - round(Y + dy * S)))

    # The stars (** on H, * on B), as on the ZX81
    b = p.box(V(138))
    X, Y, W, H = p.moved(b)
    star(p, X, Y, 7 * S, H)
    star(p, X + 8 * S, Y, 7 * S, H)
    b = p.box(V(189))
    X, Y, W, H = p.moved(b)
    star(p, X, Y, W, H)

    # The arrows on 5 - 8: a shaft and a head
    for i, direction in ARROWS.items():
        b = p.box(V(i))
        X, Y, W, H = p.moved(b)
        pts = {'down': ARROW, 'up': [(u, 1 - v) for u, v in ARROW],
               'right': [(v, u) for u, v in ARROW], 'left': [(1 - v, u) for u, v in ARROW]}[direction]
        p.items.append('<polygon fill="#000000" points="%s"/>'
                       % ' '.join('%.2f,%.2f' % (X + u * W, Y + v * H) for u, v in pts))

    print('%s, not drawn: %s' % (name, [i for i in p.unused() if p.words[i]['c'] != list(BACK)]),
          file=sys.stderr)
    open(sys.argv[1], 'w').write(p.svg(BACK))


if __name__ == '__main__':
    draw('ts1500')
