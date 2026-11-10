# spectrum128.py <out.svg> [scale]: the ZX Spectrum 128 keyboard picture, drawn again at a scale (2 if not given)
import os
import sys
from kbdraw import Picture, hexcol, arrow_points, ring_paths

HERE = os.path.dirname(os.path.abspath(__file__))
p = Picture(os.path.join(HERE, 'spectrum128.json'), os.path.join(HERE, 'orig/spectrum128.png'), float(sys.argv[2]) if len(sys.argv) > 2 else 2)
S = p.S
KEY, RING, WH = (32, 32, 32), (64, 64, 64), (255, 255, 255)

# Keys: rectangles; ENTER is an L
for k in p.newkeys:
    # edges on whole pixels
    x, y = round(k[0] * S), round(k[1] * S)
    w, h = round((k[0] + k[2]) * S) - x, round((k[1] + k[3]) * S) - y
    if k == [441, 47, 62, 85]:
        pts = [(468, 47), (503, 47), (503, 132), (441, 132), (441, 91), (468, 91)]
        p.items.append('<polygon fill="%s" points="%s"/>'
                       % (hexcol(KEY), ' '.join('%g,%g' % (round(a * S), round(b * S)) for a, b in pts)))
        continue
    p.items.append('<rect x="%g" y="%g" width="%g" height="%g" fill="%s"/>' % (x, y, w, h, hexcol(KEY)))

# The dishes
for d in ring_paths(p.im, RING, S, S):
    p.items.append('<path fill="none" stroke="%s" stroke-width="%g" d="%s"/>' % (hexcol(RING), S, d))

texts = {
    # 1 - 0, BREAK
    21: 'TRUE', 40: 'VIDEO', 22: 'INV', 41: 'VIDEO',
    0: 'BLUE', 8: 'DEF', 9: 'FN', 24: '!', 44: '1',
    1: 'RED', 10: 'FN', 26: '@', 45: '2',
    2: 'MGNTA', 11: 'LINE', 28: '#', 46: '3',
    3: 'GREEN', 12: 'OPEN', 13: '#', 30: '$', 47: '4',
    4: 'CYAN', 14: 'CLOSE', 15: '#', 32: '%', 48: '5',
    5: 'YELLOW', 16: 'MOVE', 34: '&', 49: '6',
    6: 'WHITE', 17: 'ERASE', 36: "'", 50: '7',
    18: 'POINT', 38: '(', 51: '8',
    19: 'CAT', 39: ')', 52: '9',
    7: 'BLACK', 20: 'FORMAT', 53: '0',
    43: 'BREAK',
    # DELETE, GRAPH, Q - P
    84: 'DELETE', 85: 'GRAPH',
    56: 'SIN', 64: 'ASN', 74: 'PLOT', 86: '<=', 95: 'Q',
    57: 'COS', 65: 'ACS', 75: 'DRAW', (87, 88): '<>', 96: 'W',
    58: 'TAN', 66: 'ATN', 76: 'REM', 89: '>=', 97: 'E',
    59: 'INT', 67: 'VERIFY', 77: 'RUN', 90: '<', 98: 'R',
    60: 'RND', 68: 'MERGE', 78: 'RAND', 91: '>', 99: 'T',
    54: 'STR$', 69: '[', 79: 'RETRN', 92: 'AND', 100: 'Y',
    55: 'CHR$', 70: ']', 80: 'IF', 93: 'OR', 101: 'U',
    61: 'CODE', 71: 'IN', 81: 'INPUT', 94: 'AT', 102: 'I',
    62: 'PEEK', 72: 'OUT', 82: 'POKE', 103: 'O',
    63: 'TAB', 73: '(C)', 83: 'PRINT', 104: 'P',
    142: 'ENTER',
    # EXTEND MODE, EDIT, A - L
    123: 'EXTEND', 133: 'MODE', 134: 'EDIT',
    105: 'READ', 114: '~', 124: 'NEW', 135: 'STOP', 145: 'A',
    106: 'RESTR', 125: 'SAVE', 136: 'NOT', 146: 'S',
    107: 'DATA', 116: '\\', 126: 'DIM', 137: 'STEP', 147: 'D',
    108: 'SGN', 117: '{', 127: 'FOR', 138: 'TO', 148: 'F',
    109: 'ABS', 118: '}', 128: 'GOTO', 139: 'THEN', 149: 'G',
    110: 'SQR', 119: 'CIRCLE', 129: 'GOSUB', 150: 'H',
    111: 'VAL', 120: 'VAL$', 130: 'LOAD', 151: 'J',
    112: 'LEN', 121: 'SCRN$', 131: 'LIST', 152: 'K',
    113: 'USR', 122: 'ATTR', 132: 'LET', 153: 'L',
    # CAPS SHIFT, CAPS LOCK, Z - M, ., CAPS SHIFT
    176: 'CAPS', 177: 'SHIFT', 168: 'CAPS', 178: 'LOCK',
    154: 'LN', 161: 'BEEP', 186: 'Z',
    155: 'EXP', 162: 'INK', 170: 'CLEAR', 179: '£', 187: 'X',
    156: 'LPRINT', 163: 'PAPER', 171: 'CONT', 180: '?', 188: 'C',
    157: 'LLIST', 164: 'FLASH', 172: 'CLS', 181: '/', 189: 'V',
    158: 'BIN', 165: 'BRIGHT', 173: 'BORDR', 190: 'B',
    159: 'INKEY$', 166: 'OVER', 174: 'NEXT', 191: 'N',
    160: 'PI', 167: 'INVERS', 175: 'PAUSE', 192: 'M',
    184: 'CAPS', 185: 'SHIFT',
    # bottom row
    193: 'SYMBL', 198: 'SHIFT', 199: ';', 194: '"', 203: ',', 197: 'SYMBL', 202: 'SHIFT',
}
big = set([44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 95, 96, 97, 98, 99, 100, 101, 102, 103,
           104, 145, 146, 147, 148, 149, 150, 151, 152, 153, 186, 187, 188, 189, 190, 191, 192])
# The words: one size and spacing, those of LOCK and BORDR (Heros Bold,
# capitals 5 pixels high, the font's own spacing); words on one line
# together
import re
for a, b, t in ((8, 9, 'DEF FN'), (12, 13, 'OPEN #'), (14, 15, 'CLOSE #'),
                (176, 177, 'CAPS SHIFT'), (184, 185, 'CAPS SHIFT')):
    del texts[a], texts[b]
    texts[(a, b)] = t
# COPY and : of Z were found as one
p.used.add(169)
p.words[1000] = dict(id=1000, c=[255, 255, 255], x=132, y=151, x2=152, y2=156)
texts[1000] = 'COPY'
p.fit(':', 150 * S, 158 * S, 1 * S, 3 * S, WH)

for ids, t in texts.items():
    if ids not in big and len(re.findall('[A-Z]', t)) >= 2 and t != '(C)':
        p.word(ids, t, 'herosb', 5)
    elif t == '~':
        p.text(ids, t, fonts=('cn',))
    elif ids in big:
        p.text(ids, t, fonts=('herosb', 'cnb'))
    else:
        p.text(ids, t)

# Block graphics, bars and small signs: made of squares
for i in (23, 25, 27, 29, 31, 33, 35, 37, 42, 115, 140, 141, 143, 144, 182, 183):
    p.pixels(i)

# The cursor arrows
for i, direction in ((200, 'left'), (201, 'right'), (195, 'up'), (196, 'down')):
    b = p.box(i)
    X, Y, W, H = p.moved(b, False)
    pts = arrow_points(X, Y, W, H, direction, S, head=0.38, s0=0.3, s1=0.7)
    p.items.append('<polygon fill="none" stroke="#ffffff" stroke-width="%g" points="%s"/>'
                   % (S, ' '.join('%g,%g' % q for q in pts)))

print('not drawn:', p.unused(), file=sys.stderr)
open(sys.argv[1], 'w').write(p.svg((0, 0, 0)))
