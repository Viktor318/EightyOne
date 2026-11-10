# timex.py: the Timex TC2048 and TS2068 keyboard pictures, drawn again
# (tc2048.py and ts2068.py call draw())
#
# The two pictures are the same, but for their colours (the TC2048 white
# on black, the TS2068 black on light grey) and five words under the keys.
# The frames, lines, keys and block graphics are lines of whole pixels, so
# they are drawn as they are, of their pixels (their edges on whole pixels
# at every scale, so sharp); the texts are found on and around each key
# and drawn again in TeX Gyre Heros: the words above and under the keys
# and on the large keys 5 pixels high, those on the keys 4 (as in the
# original: there is no room for more), all in the font's own spacing.
import os
import sys
import numpy as np
from kbdraw import Picture, hexcol

HERE = os.path.dirname(os.path.abspath(__file__))
FACE, BAR = (166, 166, 164), (210, 210, 214)

# The colour names of the top strip, from the left
NAMES = ['BLUE', 'RED', 'MAGENTA', 'GREEN', 'CYAN', 'YELLOW', 'WHITE', 'BLACK']

# Each key (the faces of the analysis, in their order): the letter or digit,
# the word on the key, the sign or word in the bar, the words above and
# under the key
ROW1 = [('1', '!', 'EDIT', 'DEF FN'), ('2', '@', 'CAPS LOCK', 'FN'), ('3', '#', 'TRUE VIDEO', 'LINE'),
        ('4', '$', 'INV.VIDEO', 'OPEN #'), ('5', '%', None, 'CLOSE #'), ('6', '&', None, 'MOVE'),
        ('7', "'", None, 'ERASE'), ('8', '(', None, 'POINT'), ('9', ')', 'GRAPHICS', 'CAT'),
        ('0', '_', 'DELETE', 'FORMAT')]
ROW2 = [('Q', 'PLOT', '<=', 'SIN', 'ASN'), ('W', 'DRAW', '<>', 'COS', 'ACS'), ('E', 'REM', '>=', 'TAN', 'ATN'),
        ('R', 'RUN', '<', 'INT', 'VERIFY'), ('T', 'RAND', '>', 'RND', 'MERGE'),
        ('Y', 'RETRN', 'AND', 'STR$', '['), ('U', 'IF', 'OR', 'CHR$', ']'), ('I', 'INPUT', 'AT', 'CODE', 'IN'),
        ('O', 'POKE', ';', 'PEEK', 'OUT'), ('P', 'PRINT', '"', 'TAB', '©')]
ROW3 = [('A', 'NEW', 'STOP', 'READ', '~'), ('S', 'SAVE', 'NOT', 'RESTORE', '|'), ('D', 'DIM', 'STEP', 'DATA', '\\'),
        ('F', 'FOR', 'TO', 'SGN', '{'), ('G', 'GOTO', 'THEN', 'ABS', '}'), ('H', 'GOSUB', '↑', 'SQR', 'CIRCLE'),
        ('J', 'LOAD', '-', 'VAL', 'VAL$'), ('K', 'LIST', '+', 'LEN', 'SCREEN$'), ('L', 'LET', '=', 'USR', 'ATTR')]
ROW4 = [('Z', 'COPY', ':', 'LN', 'BEEP'), ('X', 'CLEAR', '£', 'EXP', 'INK'), ('C', 'CONT', '?', 'LPRINT', 'PAPER'),
        ('V', 'CLS', '/', 'LLIST', 'FLASH'), ('B', 'BORDR', '*', 'BIN', 'BRIGHT'), ('N', 'NEXT', ',', 'INKEY$', 'OVER'),
        ('M', 'PAUSE', '.', 'PI', 'INVERSE')]
# the large keys: their words, line by line
LARGE = {29: ['ENTER'], 30: ['CAPS', 'SHIFT'], 38: ['SYMBL', 'SHIFT'], 39: ['BREAK'], 40: ['CAPS', 'SHIFT']}
# The arrows above 5 - 8 (in pixels; their edges)
ARROWS = [
    [(196, 17.5), (203, 14), (203, 16), (205, 16), (205, 19), (203, 19), (203, 21)],
    [(239, 14), (245, 14), (245, 15), (247, 15), (242, 20), (237, 15), (239, 15)],
    [(284, 14), (289, 19), (287, 19), (287, 20), (281, 20), (281, 19), (279, 19)],
    [(330, 17.5), (323, 21), (323, 19), (321, 19), (321, 16), (323, 16), (323, 14)],
]


def draw(name, label, background, under=None):
    """The picture <name> (<name>.json, orig/<name>.png): its words above
    and under the keys of the colour label, on background; under: other
    words under keys ({key: word})"""
    p = Picture(os.path.join(HERE, name + '.json'), os.path.join(HERE, 'orig/%s.png' % name),
                float(sys.argv[2]) if len(sys.argv) > 2 else 2)
    S = p.S
    im = p.im.copy()
    # the colours of the picture (single odd pixels as the nearest of them)
    cols, counts = np.unique(im.reshape(-1, 3), axis=0, return_counts=True)
    pure = cols[counts >= 30]
    near = ((im[:, :, None, :] - pure[None, None]) ** 2).sum(-1).argmin(-1)
    im = pure[near]
    p.im = im

    legends = {}
    for i, (l, s, a, u) in enumerate(ROW1):
        legends[i] = dict(letter=l, keyword=None, symbol=s, above=a, below=u)
    for row, first in ((ROW2, 10), (ROW3, 20), (ROW4, 31)):
        for i, (l, k, s, a, u) in enumerate(row):
            legends[first + i] = dict(letter=l, keyword=k, symbol=s, above=a, below=u)
    for k, w in (under or {}).items():
        legends[k]['below'] = w

    texts = []      # (kind, ids, text)
    new_id = [10000]
    erase = []      # (box, colour): pixels of a text, to take out of the frames

    def blobs(colour, x0, y0, x1, y1):
        return [w for w in p.words.values() if w['c'] == list(colour)
                and x0 <= w['x'] and w['x2'] <= x1 and y0 <= w['y'] and w['y2'] <= y1]

    def add(kind, ws, t):
        if not ws or not t:
            return
        if t.endswith('$') and kind == 'label':
            # the $ was not found as a text: it touches a line under it.
            # Its pixels: right of the word, a pixel higher and lower
            x2, y0, y2 = max(w['x2'] for w in ws), min(w['y'] for w in ws), max(w['y2'] for w in ws)
            part = im[y0 - 1:y2 + 1, x2:x2 + 7]
            m = (part == ws[0]['c']).all(-1)
            m[m.all(1)] = False
            ys, xs = np.where(m)
            if len(xs):
                w = dict(id=new_id[0], c=ws[0]['c'], x=x2 + xs.min(), y=y0 - 1 + ys.min(),
                         x2=x2 + xs.max() + 1, y2=y0 - 1 + ys.max() + 1)
                p.words[new_id[0]] = w
                new_id[0] += 1
                ws = ws + [w]
        ids = tuple(w['id'] for w in ws)
        texts.append((kind, ids if len(ids) > 1 else ids[0], t))
        for w in ws:
            erase.append(((w['x'], w['y'], w['x2'], w['y2']), tuple(w['c'])))

    def region(colour, x0, y0, x1, y1):
        # a text of the pixels of a colour in a part of the picture
        m = (im[y0:y1, x0:x1] == colour).all(-1)
        ys, xs = np.where(m)
        if not len(ys):
            return None
        w = dict(id=new_id[0], c=list(colour), x=x0 + xs.min(), y=y0 + ys.min(),
                 x2=x0 + xs.max() + 1, y2=y0 + ys.max() + 1)
        p.words[new_id[0]] = w
        new_id[0] += 1
        return w

    for k, (fx, fy, fw, fh) in enumerate(p.keys):
        if k in LARGE:
            colour = BAR if k == 38 else (0, 0, 0)
            ws = sorted(blobs(colour, fx, fy, fx + fw, fy + fh), key=lambda w: w['y'])
            lines = []
            for w in ws:
                if lines and w['y'] < lines[-1][-1]['y2']:
                    lines[-1].append(w)
                else:
                    lines.append([w])
            for ln, t in zip(lines, LARGE[k]):
                add('large', ln, t)
            continue
        if k not in legends:
            continue
        lg = legends[k]
        if k < 10:
            # the digit (next to the block graphic: not one text with it)
            w = region((0, 0, 0), fx + 3, fy, fx + 9, fy + 7)
            add('letter', [w] if w else [], lg['letter'])
        else:
            add('letter', [w for w in blobs((0, 0, 0), fx, fy, fx + 10, fy + 7) if w['x2'] - w['x'] <= 6],
                lg['letter'])
            add('keyword', blobs((0, 0, 0), fx, fy + 6, fx + fw, fy + 12), lg['keyword'])
        add('symbol', blobs(BAR, fx, fy, fx + fw, fy + fh), lg['symbol'])
        add('label', blobs(label, fx - 9, fy - 16, fx + fw + 9, fy - 3), lg['above'])
        add('label', blobs(label, fx - 9, fy + fh + 3, fx + fw + 9, fy + fh + 17), lg['below'])
    # the colour names
    strip = sorted([w for w in p.words.values() if w['y2'] <= 12 and w['c'] != list(background)],
                   key=lambda w: w['x'])
    for w, t in zip(strip, NAMES):
        add('label', [w], t)
    # PERSONAL COLOR COMPUTER
    add('bottom', blobs(label, 0, 195, 110, 223), 'PERSONAL COLOR COMPUTER')

    # The frames, lines, keys and block graphics: the picture without its
    # texts and arrows, of its pixels
    clean = im.copy()
    for (x0, y0, x1, y1), colour in erase:
        part = clean[y0 - 1:y1 + 1, x0 - 1:x1 + 1]
        m = (part == colour).all(-1)
        others = part[~m].reshape(-1, 3)
        c, n = np.unique(others, axis=0, return_counts=True)
        fill = c[n.argmax()]
        inner = m.copy()
        inner[0, :] = inner[-1, :] = False
        inner[:, 0] = inner[:, -1] = False
        part[inner] = fill
    for pts in ARROWS:
        xs = [a for a, b in pts]
        ys = [b for a, b in pts]
        part = clean[int(min(ys)):int(np.ceil(max(ys))), int(min(xs)):int(np.ceil(max(xs)))]
        part[(part == label).all(-1)] = background
    E = lambda v: round(v * S)
    for c in np.unique(clean.reshape(-1, 3), axis=0):
        if tuple(c) == tuple(background):
            continue
        m = (clean == c).all(-1)
        rects = []
        prev = {}
        for y in range(p.h + 1):
            runs = {}
            if y < p.h:
                row = m[y]
                x = 0
                while x < p.w:
                    if row[x]:
                        e = x
                        while e < p.w and row[e]:
                            e += 1
                        runs[(x, e)] = y
                        x = e
                    else:
                        x += 1
            # runs going on from the row above grow downwards
            for r, y0 in prev.items():
                if r in runs:
                    runs[r] = y0
                else:
                    rects.append((r[0], y0, r[1], y))
            prev = runs
        p.items.append('<path fill="%s" d="%s"/>' % (hexcol(c), ''.join(
            'M%d %dH%dV%dH%dz' % (E(a), E(b), E(c2), E(d), E(a)) for a, b, c2, d in rects)))
    for pts in ARROWS:
        p.items.append('<polygon fill="%s" points="%s"/>'
                       % (hexcol(label), ' '.join('%g,%g' % (a * S, b * S) for a, b in pts)))

    # The texts
    for kind, ids, t in texts:
        if kind == 'letter':
            p.text(ids, t, fonts=('herosb',))
            if t == '0':
                # with its slash, from the top right to the bottom left
                X, Y, W, H = p.moved(p.box(ids))
                p.items.append('<line x1="%g" y1="%g" x2="%g" y2="%g" stroke="#000000" stroke-width="%g"/>'
                               % (X + W - 0.4 * S, Y + 0.4 * S, X + 0.4 * S, Y + H - 0.4 * S, 0.9 * S))
        elif kind in ('keyword', 'symbol') and (len(t) > 1 and t.isalpha()):
            p.word(ids, t, 'cnb', 4)
        elif kind in ('keyword', 'symbol'):
            p.text(ids, t, fonts=('herosb', 'cnb'))
        elif t == '~':
            # a thin wave (a letter of the font would be thick at this size)
            X, Y, W, H = p.moved(p.box(ids))
            pt = lambda u, v: '%.2f %.2f' % (X + u * W, Y + v * H)
            p.items.append('<path fill="none" stroke="%s" stroke-width="%g" stroke-linecap="round" '
                           'd="M%s C%s %s %s S%s %s"/>'
                           % (hexcol(label), 0.9 * S, pt(0.06, 0.7), pt(0.18, 0.1), pt(0.34, 0.12),
                              pt(0.5, 0.5), pt(0.82, 0.9), pt(0.94, 0.3)))
        elif t == '\\':
            # a thin line too, as the wave
            X, Y, W, H = p.moved(p.box(ids))
            p.items.append('<line x1="%.2f" y1="%.2f" x2="%.2f" y2="%.2f" stroke="%s" stroke-width="%g"/>'
                           % (X + 0.4 * S, Y + 0.3 * S, X + W - 0.4 * S, Y + H - 0.3 * S, hexcol(label), 0.9 * S))
        elif kind == 'large':
            p.word(ids, t, 'cnb', 5)
        elif kind == 'bottom':
            p.word(ids, t, 'cnb', 5)
        elif len(t) > 1 and any(ch.isalpha() for ch in t):
            p.word(ids, t, 'cnb', 5, over=True)
        else:
            p.text(ids, t, fonts=('herosb', 'cnb'))

    print('%s, not drawn: %s' % (name, [i for i in p.unused() if p.words[i]['c'] != list(FACE)]),
          file=sys.stderr)
    open(sys.argv[1], 'w').write(p.svg(background))
