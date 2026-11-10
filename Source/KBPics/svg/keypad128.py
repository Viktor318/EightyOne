# keypad128.py <out.svg> [scale]: the Spectrum 128 keypad picture (UK), drawn again at a scale (2 if not given)
import os
import sys
from kbdraw import Picture, hexcol, ring_paths

HERE = os.path.dirname(os.path.abspath(__file__))
p = Picture(os.path.join(HERE, 'keypad128.json'), os.path.join(HERE, 'orig/keypad128uk.png'), float(sys.argv[2]) if len(sys.argv) > 2 else 2)
S = p.S
KEY, RING, WH, RED = (32, 32, 32), (64, 64, 64), (255, 255, 255), (255, 0, 0)

for k in p.newkeys:
    # edges on whole pixels
    x, y = round(k[0] * S), round(k[1] * S)
    w, h = round((k[0] + k[2]) * S) - x, round((k[1] + k[3]) * S) - y
    p.items.append('<rect x="%g" y="%g" width="%g" height="%g" fill="%s"/>' % (x, y, w, h, hexcol(KEY)))
for d in ring_paths(p.im, RING, S, S):
    p.items.append('<path fill="none" stroke="%s" stroke-width="%g" d="%s"/>' % (hexcol(RING), S, d))

texts = {
    0: 'DEL', 1: 'DEL', 5: 'TOGGLE', 13: 'CMND', 19: 'DEL', 21: 'DEL', 33: 'DEL', 34: 'DEL',
    43: 'SHIFT', 39: 'E', 40: 'N', 41: 'T', 45: 'E', 46: 'R',
    8: '/', 6: '(', 7: ')',
    14: '7', 15: '8', 16: '9', 26: '4', 27: '5', 28: '6', 36: '1', 37: '2', 38: '3',
}
for ids, t in texts.items():
    if t.isdigit():
        p.text(ids, t, fonts=('herosb',))
    elif ids in (39, 40, 41, 45, 46):
        # ENTER, letter under letter: a little wider than the original
        # letters (6 pixels instead of 5), as high
        b = p.box(ids)
        X, Y, W, H = p.moved(b, False)
        p.fit(t, X + W / 2 - 3 * S, Y, 6 * S, H, b['c'], fonts=('herosb',), maxtrack=0)
    elif t.isalpha():
        # the words and the letters of ENTER: all as DEL at the top left
        # (Heros Bold, capitals 5 pixels high, the font's own spacing)
        p.word(ids, t, 'herosb', 5)
    else:
        p.text(ids, t)

# 0 with its slash
b = p.box(47)
X, Y, W, H = p.moved(b, False)
p.fit('0', X + 0.5 * S, Y, W - 1 * S, H, WH, fonts=('herosb',))
p.items.append('<line x1="%g" y1="%g" x2="%g" y2="%g" stroke="#ffffff" stroke-width="%g"/>'
               % (X + 1.2 * S, Y + 10.6 * S, X + 8.3 * S, Y + 0.4 * S, 1.3 * S))

# Bars and small signs: squares
for i in (17, 29, 48, 49):
    p.pixels(i)

# Red dots
for i in (35, 42):
    b = p.box(i)
    X, Y, W, H = p.moved(b, False)
    p.items.append('<circle cx="%g" cy="%g" r="%g" fill="#ff0000"/>' % (X + W / 2, Y + H / 2, W / 2))

# * : eight spokes and a thicker middle
b = p.box(9)
X, Y, W, H = p.moved(b, False)
cx, cy = X + W / 2, Y + H / 2
for x1, y1, x2, y2 in ((cx, Y, cx, Y + H), (X, cy, X + W, cy),
                       (X + 1.5 * S, Y + 1.5 * S, X + W - 1.5 * S, Y + H - 1.5 * S),
                       (X + 1.5 * S, Y + H - 1.5 * S, X + W - 1.5 * S, Y + 1.5 * S)):
    p.items.append('<line x1="%g" y1="%g" x2="%g" y2="%g" stroke="#ffffff" stroke-width="%g"/>'
                   % (x1, y1, x2, y2, S))
p.items.append('<rect x="%g" y="%g" width="%g" height="%g" fill="#ffffff"/>' % (cx - 1.5 * S, cy - 1.5 * S, 3 * S, 3 * S))


# Arrows: a filled head (3 long, 5 wide) and a shaft 1 thick, in pixels of
# the original (edges), from the tip back to the tail
def arrow(tipx, tipy, direction, length, colour=WH):
    dx, dy = {'left': (1, 0), 'right': (-1, 0), 'up': (0, 1), 'down': (0, -1)}[direction]
    px, py = -dy, dx
    hl, hw, lw = 3.0, 2.5, 1.0
    pts = [(tipx, tipy), (tipx + dx * hl + px * hw, tipy + dy * hl + py * hw),
           (tipx + dx * hl - px * hw, tipy + dy * hl - py * hw)]
    p.items.append('<polygon fill="%s" points="%s"/>'
                   % (hexcol(colour), ' '.join('%g,%g' % (a * S, b * S) for a, b in pts)))
    # the shaft
    x0, y0 = tipx + dx * 1, tipy + dy * 1
    x1, y1 = tipx + dx * length, tipy + dy * length
    p.items.append('<line x1="%g" y1="%g" x2="%g" y2="%g" stroke="%s" stroke-width="%g"/>'
                   % (x0 * S, y0 * S, x1 * S, y1 * S, hexcol(colour), lw * S))


def bar(x0, y0, x1, y1, colour=WH):
    p.items.append('<line x1="%g" y1="%g" x2="%g" y2="%g" stroke="%s" stroke-width="%g"/>'
                   % (x0 * S, y0 * S, x1 * S, y1 * S, hexcol(colour), S))


def at(i):
    p.used.add(i)
    b = p.words[i]
    return b['x'], b['y']


# DEL with an arrow under it: ← and →
x, y = at(2); arrow(x, y + 2.5, 'left', 7)
x, y = at(4); arrow(x + 7, y + 2.5, 'right', 7)
# the cursor keys, in red
x, y = at(3); arrow(x + 2.5, y, 'up', 7, RED)
x, y = at(10); arrow(x, y + 2.5, 'left', 7, RED)
x, y = at(11); arrow(x + 2.5, y + 7, 'down', 7, RED)
x, y = at(12); arrow(x + 7, y + 2.5, 'right', 7, RED)
# words: two arrows
for i in (18, 24):
    x, y = at(i); arrow(x, y + 2.5, 'left', 7)
for i in (20, 25):
    x, y = at(i); arrow(x + 7, y + 2.5, 'right', 7)
# 10 lines: two arrows side by side
x, y = at(22); arrow(x + 2.5, y, 'up', 7); arrow(x + 8.5, y, 'up', 7)
x, y = at(23); arrow(x + 2.5, y + 7, 'down', 7); arrow(x + 8.5, y + 7, 'down', 7)
# start / end of the line, first / last line: two arrows and a bar
x, y = at(30); bar(x + 0.5, y, x + 0.5, y + 11); arrow(x + 1, y + 2.5, 'left', 7); arrow(x + 1, y + 8.5, 'left', 7)
x, y = at(31); bar(x + 7.5, y, x + 7.5, y + 11); arrow(x + 7, y + 2.5, 'right', 7); arrow(x + 7, y + 8.5, 'right', 7)
x, y = at(32); bar(x, y + 0.5, x + 11, y + 0.5); arrow(x + 2.5, y + 1, 'up', 7); arrow(x + 8.5, y + 1, 'up', 7)
x, y = at(44); bar(x, y + 7.5, x + 11, y + 7.5); arrow(x + 2.5, y + 7, 'down', 7); arrow(x + 8.5, y + 7, 'down', 7)

print('not drawn:', p.unused(), file=sys.stderr)
open(sys.argv[1], 'w').write(p.svg((0, 0, 0)))
