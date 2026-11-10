# spectrum48.py <out.svg> [scale]: the ZX Spectrum 48K keyboard picture, drawn again at a scale (2 if not given)
import os
import sys
from kbdraw import Picture, hexcol

HERE = os.path.dirname(os.path.abspath(__file__))
p = Picture(os.path.join(HERE, 'spectrum48.json'), os.path.join(HERE, 'orig/spectrum48.png'), float(sys.argv[2]) if len(sys.argv) > 2 else 2)
S = p.S
W, R, G = (255, 255, 255), (255, 0, 0), (0, 255, 0)

# The rainbow: four stripes, 10 pixels wide, going 1 left for 2 down
for i, c in enumerate([(255, 0, 0), (255, 255, 0), (0, 255, 0), (0, 255, 255)]):
    L = 433 + 10 * i
    pts = [(L - 0.25, 223), (L + 9.75, 223), (L + 9.75 + 111.5, 0), (L - 0.25 + 111.5, 0)]
    p.items.append('<polygon fill="%s" points="%s"/>'
                   % (hexcol(c), ' '.join('%g,%g' % (x * S, y * S) for x, y in pts)))
p.used.update([87, 99, 119, 140, 152, 153, 203, 204, 205, 206])

p.regular_rows()
# More room between the rows (for the red and green words between them):
# the 9 free pixels under the last row shared out, 2 pixels a row
for k in p.newkeys:
    k[1] += {22: 0, 73: 2, 123: 4, 175: 6}[min((22, 73, 123, 175), key=lambda y: abs(y - k[1]))]
p.draw_keys((87, 128, 140), 3)

texts = {
    # colours and the functions above the number keys
    0: 'BLUE', 1: 'RED', 2: 'MAGENTA', 3: 'GREEN', 4: 'CYAN', 5: 'YELLOW', 6: 'WHITE',
    8: 'EDIT', 9: 'CAPS LOCK', 10: 'TRUE VIDEO', 11: 'INV.VIDEO', 16: 'GRAPHICS', 17: 'DELETE',
    # number keys
    18: '1', 20: '2', 22: '3', 24: '4', 26: '5', 28: '6', 30: '7', 32: '8', 34: '9', 35: '0',
    36: '!', 37: '@', 38: '#', 39: '$', 40: '%', 41: '&', 42: '(', 43: ')', 44: "'",
    46: 'DEF', 47: 'FN', 48: 'FN', 49: 'LINE', 50: 'OPEN #', 51: 'CLOSE #', 52: 'MOVE',
    53: 'ERASE', 54: 'POINT', 55: 'CAT', 56: 'FORMAT',
    # Q - P
    57: 'SIN', 58: 'COS', 59: 'TAN', 60: 'INT', 61: 'RND', 62: 'STR$', 63: 'CHR$', 64: 'CODE',
    65: 'PEEK', 66: 'TAB',
    67: 'Q', 68: '<=', 69: 'W', 70: '<>', 71: 'E', 72: '>=', 73: 'R', 74: '<', 75: 'T', 76: '>',
    77: 'Y', 78: 'AND', 79: 'U', 80: 'OR', 81: 'I', 82: 'AT', 83: 'O', (84, 88): ';', 85: 'P',
    86: '"',
    89: 'PLOT', 90: 'DRAW', 91: 'REM', 92: 'RUN', 93: 'RAND', 94: 'RETURN', 95: 'IF',
    96: 'INPUT', 97: 'POKE', 98: 'PRINT',
    100: 'ASN', 101: 'ACS', 102: 'ATN', 103: 'VERIFY', 104: 'MERGE', 105: '[', 106: ']',
    107: 'IN', 108: 'OUT', 109: '©',
    # A - ENTER
    110: 'READ', 111: 'RESTORE', 112: 'DATA', 113: 'SGN', 114: 'ABS', 115: 'SQR', 116: 'VAL',
    117: 'LEN', 118: 'USR',
    120: 'A', 121: 'STOP', 122: 'S', 123: 'NOT', 124: 'D', 125: 'STEP', 126: 'F', 127: 'TO',
    128: 'G', 129: 'THEN', 130: 'H', 131: '↑', 132: 'J', 134: 'K', 136: 'L',
    139: 'ENTER',
    141: 'NEW', 142: 'SAVE', 143: 'DIM', 144: 'FOR', 145: 'GOTO', 146: 'GOSUB', 147: 'LOAD',
    148: 'LIST', 149: 'LET',
    150: 'VAL$', 151: 'SCREEN$', 154: '~', 156: '\\', 157: '{', 158: '}', 159: 'CIRCLE',
    160: 'ATTR',
    # CAPS SHIFT - SPACE
    161: 'LN', 162: 'EXP', 163: 'LPRINT', 164: 'LLIST', 165: 'BIN', 166: 'INKEY$', 167: 'PI',
    169: 'CAPS', 187: 'SHIFT', 170: 'Z', (171, 185): ':', 172: 'X', 173: '£', 174: 'C',
    175: '?', 176: 'V', 177: '/', 178: 'B', 180: 'N', 181: ',', 182: 'M', 183: '.',
    184: 'SYMBOL', 195: 'SHIFT', 168: 'BREAK', 186: 'SPACE',
    188: 'COPY', 189: 'CLEAR', 190: 'CONT', 191: 'CLS', 192: 'BORDER', 193: 'NEXT', 194: 'PAUSE',
    196: 'BEEP', 197: 'INK', 198: 'PAPER', 199: 'FLASH', 200: 'BRIGHT', 201: 'OVER', 202: 'INVERSE',
}
big = set([18, 20, 22, 24, 26, 28, 30, 32, 34, 35, 67, 69, 71, 73, 75, 77, 79, 81, 83, 85,
           120, 122, 124, 126, 128, 130, 132, 134, 136, 170, 172, 174, 176, 178, 180, 182, 186])
# The words (red, green and white, not SPACE): all as high as EDIT (7
# pixels), in the font's own spacing; on the original base line, at the
# side of the original box (left; on a key right or centre, as there)
import re
from kbdraw import font
CN = font('cnb')
CAP = CN.outline('H')[1][3]
WORDS = [i for i, t in texts.items() if isinstance(i, int) and t != 'SPACE'
         and len(re.findall('[A-Z]', t)) >= 2]
texts[(46, 47)] = 'DEF FN'
del texts[46], texts[47]
WORDS = [i for i in WORDS if i not in (46, 47)] + [(46, 47)]
drawn = []

def word(ids, t):
    b = p.box(ids)
    X, Y, Wd, H = p.moved(b, 'x' if b['y'] < 22 else True)
    base = b['y2'] - (1 if '$' in t and b['y2'] - b['y'] > 7 else 0)
    base = Y + (base - b['y']) * S
    k = p.key_of(b)
    kx, ky, kw, kh = p.keys[k]
    inside = kx <= b['x'] and b['x2'] <= kx + kw and ky <= b['y'] and b['y2'] <= ky + kh
    d, (x0, y0, x1, y1) = CN.outline(t)
    sc = 7 * S / CAP
    w = (x1 - x0) * sc
    # Room: up to what is next to it (in the original), so that nothing
    # touches; where there is less, a little narrower
    same = lambda o: o['y'] < b['y2'] and b['y'] < o['y2'] and o is not b \
        and not (isinstance(ids, tuple) and o['id'] in ids)
    if inside:
        lm, rm = b['x'] - kx, kx + kw - b['x2']
        if rm <= 4:
            lefts = [o['x2'] + 3 for o in p.words.values() if same(o) and kx <= o['x'] and o['x2'] <= b['x']]
            room = b['x2'] - max(lefts + [kx + 3])
        else:
            room = kw - 6
    else:
        rights = [o['x'] - 6 for o in p.words.values() if same(o) and o['x'] >= b['x2']]
        room = min(rights + [p.w - 2]) - b['x']
    sx = min(1.0, room * S / w)
    w *= sx
    if inside:
        lm, rm = b['x'] - kx, kx + kw - b['x2']
        if rm <= 4:
            left = X + Wd - w
        elif abs(lm - rm) <= 3:
            nk = p.newkeys[k]
            left = (nk[0] + nk[2] / 2) * S - w / 2
        else:
            left = X
    else:
        left = X
    p.items.append('<path fill="%s" transform="matrix(%.5f 0 0 %.5f %.3f %.3f)" d="%s"/>'
                   % (hexcol(b['c']), sc * sx, -sc, left - sc * sx * x0, base, d))
    if sx < 1:
        print('narrower: %s %.2f' % (t, sx), file=sys.stderr)
    drawn.append((ids, t, left, base - 7 * S, left + w, base, inside, k))

# GRAPHICS and DELETE on the line of EDIT (they were 1 pixel higher)
for i in (16, 17):
    p.words[i]['y'] += 1
    p.words[i]['y2'] += 1

# The two lines above the number keys were drawn on common lines, not with
# their keys (which are 1 pixel off): only sideways with the keys
top = set(range(0, 18))
for ids, t in texts.items():
    if ids in WORDS:
        word(ids, t)
    elif ids in top and ids not in (0, 1, 2, 3, 4, 5, 6):
        p.text(ids, t, follow='x')
    elif ids in (0, 1, 2, 3, 4, 5, 6):
        # the colours: as high as EDIT under them (7 pixels, theirs were
        # 6), the font's own spacing, on the same line at the bottom
        b = p.box(ids)
        X, Y, Wd, H = p.moved(b, 'x')
        p.fit(t, X, Y + H - 7 * S, Wd, 7 * S, b['c'], fonts=('cnb',), natural='left')
    elif t == '~':
        p.text(ids, t, fonts=('cn',))
    elif ids in big:
        p.text(ids, t, fonts=('herosb',))
    else:
        p.text(ids, t)

# The block graphics, and the bars: made of squares
for i in (19, 21, 23, 25, 27, 29, 31, 33, 45, 133, 135, 137, 138, 155):
    p.pixels(i)

# BLACK: black letters on white
b = p.box(7)
X, Y, Wd, H = p.moved(b, 'x')
# the box one pixel higher, for letters as high as the other colours
p.items.append('<rect x="%g" y="%g" width="%g" height="%g" fill="#ffffff"/>' % (X, Y - S, Wd, H + S))
p.fit('BLACK', X, Y, Wd, 7 * S, (0, 0, 0), fonts=('cnb',), natural='centre')

# The cursor arrows above 5 - 8: outlines
def arrow(i, direction):
    b = p.box(i)
    X, Y, Wd, H = p.moved(b, 'x')
    sw = S
    # the arrow pointing left in a unit box, then turned
    pts = [(0, 0.5), (0.42, 0.0), (0.42, 0.28), (1, 0.28), (1, 0.72), (0.42, 0.72), (0.42, 1.0)]
    if direction == 'right':
        pts = [(1 - x, y) for x, y in pts]
    elif direction == 'up':
        pts = [(y, x) for x, y in pts]
    elif direction == 'down':
        pts = [(y, 1 - x) for x, y in pts]
    pts = [(X + sw / 2 + x * (Wd - sw), Y + sw / 2 + y * (H - sw)) for x, y in pts]
    p.items.append('<polygon fill="none" stroke="#ffffff" stroke-width="%g" stroke-linejoin="miter" points="%s"/>'
                   % (sw, ' '.join('%g,%g' % q for q in pts)))
arrow(12, 'left')
arrow(13, 'down')
arrow(14, 'up')
arrow(15, 'right')

# * on B: the star of the keyboard
b = p.box(179)
X, Y, Wd, H = p.moved(b)
cx, cy = X + Wd / 2, Y + H / 2
lines = [(cx, Y, cx, Y + H), (X, Y + 0.1 * H, X + Wd, Y + 0.9 * H), (X, Y + 0.9 * H, X + Wd, Y + 0.1 * H)]
for x1, y1, x2, y2 in lines:
    p.items.append('<line x1="%g" y1="%g" x2="%g" y2="%g" stroke="#ff0000" stroke-width="%g"/>' % (x1, y1, x2, y2, S))
p.items.append('<rect x="%g" y="%g" width="%g" height="%g" fill="#ff0000"/>' % (cx - 2.5 * S, cy - 2 * S, 5 * S, 4 * S))

# words that come too near something else
others = []
for i, b in p.words.items():
    if any(i == w or (isinstance(w, tuple) and i in w) for w in WORDS):
        continue
    X, Y, Wd, H = p.moved(b, 'x' if b['y'] < 22 else True)
    others.append((i, X, Y, X + Wd, Y + H))
for ids, t, l, tp, r, bt, inside, k in drawn:
    nk = [v * S for v in p.newkeys[k]]
    if inside and (l < nk[0] + 2 * S or r > nk[0] + nk[2] - 2 * S):
        print('near the key edge:', t, file=sys.stderr)
    for i, a, bb, c, dd in others:
        if l < c + S and a < r + S and tp < dd and bb < bt:
            print('near:', t, i, file=sys.stderr)
    for ids2, t2, l2, tp2, r2, bt2, _, _ in drawn:
        if ids2 != ids and l < r2 + 2 * S and l2 < r + 2 * S and tp < bt2 and tp2 < bt:
            print('near:', t, t2, file=sys.stderr)
print('not drawn:', p.unused(), file=sys.stderr)
open(sys.argv[1], 'w').write(p.svg((0, 0, 0)))
