# spectrumplus2.py <out.svg> [scale]: the ZX Spectrum +2 keyboard picture, drawn again at a scale (2 if not given)
#
# The +3 picture is the same with dark keys (all else the same, pixel for
# pixel): spectrumplus3.py draws it with draw(), with the places of
# spectrumplus2.json.
import os
import sys
from kbdraw import Picture, hexcol

HERE = os.path.dirname(os.path.abspath(__file__))
LINE, WHITE = (64, 64, 64), (255, 255, 255)

TEXTS = {
    # the top row
    (0, 9): '!', 1: '@', 2: '#', 3: '$', 4: '%', 5: '&', 6: "'", 7: '(', 8: ')',
    13: '1', 14: '2', 15: '3', 16: '4', 17: '5', 18: '6', 19: '7', 20: '8', 21: '9', 22: '0',
    11: 'TRUE', 23: 'VIDEO', 12: 'INV', 24: 'VIDEO', 25: 'BREAK',
    # Q - P
    29: 'DELETE', 30: 'GRAPH', 27: '>', 28: 'CODE',
    31: 'Q', 32: 'W', 33: 'E', 34: 'R', 35: 'T', 36: 'Y', 37: 'U', 38: 'I', 39: 'O', 40: 'P',
    # A - ENTER
    46: 'EXTEND', 47: 'MODE', 48: 'EDIT', 41: '↑',
    49: 'A', 50: 'S', 51: 'D', 52: 'F', 53: 'G', 54: 'H', 55: 'J', 56: 'K', 57: 'L',
    # CAPS SHIFT - CAPS SHIFT
    (64, 65): 'CAPS SHIFT', 66: 'CAPS', 76: 'LOCK', (74, 75): 'CAPS SHIFT',
    (58, 63): ':', 59: '£', 60: '?', 61: '/', 62: '*', 77: '.',
    67: 'Z', 68: 'X', 69: 'C', 70: 'V', 71: 'B', 72: 'N', 73: 'M',
    # the bottom row
    78: 'SYMB', 84: 'SHIFT', 83: 'SYMB', 88: 'SHIFT', 85: ';', 86: '"', 87: ',',
}
BIG = set(range(13, 23)) | set(range(31, 41)) | set(range(49, 58)) | set(range(67, 74))
# ENTER: an L. Its outline, the pixels of its corners, and the pixels of
# the corners of its top
ENTER_OUT = [(468, 47), (503, 47), (503, 132), (441, 132), (441, 91), (468, 91)]
ENTER_CORNERS = [(468, 47), (502, 47), (502, 131), (441, 131), (441, 91), (468, 91)]
ENTER_TOP = [(472, 49), (498, 49), (498, 122), (445, 122), (445, 93), (472, 93)]


def draw(picture='spectrumplus2', key=(146, 150, 158)):
    """The picture (orig/<picture>.png, the places of spectrumplus2.json),
    its keys of the colour key"""
    p = Picture(os.path.join(HERE, 'spectrumplus2.json'), os.path.join(HERE, 'orig/%s.png' % picture),
                float(sys.argv[2]) if len(sys.argv) > 2 else 2)
    S = p.S
    E = lambda v: round(v * S)
    dark = (p.im == LINE).all(-1)

    # The keys, as in the original: the key, its top (a frame of a line) a
    # little higher than its middle, and the edges of the key from the
    # corners of the top to the corners of the key
    lines, frames = [], []

    def line(a, b):
        lines.append('M%.2f %.2fL%.2f %.2f' % ((a[0] + 0.5) * S, (a[1] + 0.5) * S, (b[0] + 0.5) * S, (b[1] + 0.5) * S))

    def frame(pts):
        # a frame of 1 pixel along the corners pts (x, y of the pixels of
        # the line), on whole pixels
        for (x0, y0), (x1, y1) in zip(pts, pts[1:] + pts[:1]):
            l, r = min(x0, x1), max(x0, x1) + 1
            t, b = min(y0, y1), max(y0, y1) + 1
            frames.append('M%d %dH%dV%dH%dz' % (E(l), E(t), E(r), E(b), E(l)))

    for x, y, w, h in p.keys:
        if (x, y) == (441, 47):
            continue
        p.items.append('<rect x="%d" y="%d" width="%d" height="%d" fill="%s"/>'
                       % (E(x), E(y), E(x + w) - E(x), E(y + h) - E(y), hexcol(key)))
        # the top: the rows and columns of the long lines
        d = dark[y:y + h, x:x + w]
        rows = [i for i in range(h) if d[i].sum() > w / 2]
        cols = [i for i in range(w) if d[:, i].sum() > h / 2]
        t, b, l, r = y + rows[0], y + rows[-1], x + cols[0], x + cols[-1]
        frame([(l, t), (r, t), (r, b), (l, b)])
        for a, c in (((x, y), (l, t)), ((x + w - 1, y), (r, t)),
                     ((x + w - 1, y + h - 1), (r, b)), ((x, y + h - 1), (l, b))):
            line(a, c)
    # ENTER
    p.items.append('<polygon fill="%s" points="%s"/>'
                   % (hexcol(key), ' '.join('%d,%d' % (E(a), E(b)) for a, b in ENTER_OUT)))
    frame(ENTER_TOP)
    for a, c in zip(ENTER_CORNERS, ENTER_TOP):
        line(a, c)
    p.items.append('<path fill="%s" d="%s"/>' % (hexcol(LINE), ''.join(frames)))
    p.items.append('<path fill="none" stroke="%s" stroke-width="%g" d="%s"/>'
                   % (hexcol(LINE), 1.0 * S, ''.join(lines)))

    # < and RUN on R were found as one
    p.used.add(26)
    p.words[1001] = dict(id=1001, c=list(WHITE), x=215, y=56, x2=220, y2=61)
    p.words[1002] = dict(id=1002, c=list(WHITE), x=222, y=56, x2=239, y2=61)
    texts = dict(TEXTS)
    texts[1001] = '<'
    texts[1002] = 'RUN'

    # The words: all of one size, as most of them (5 pixels high), slanted
    # as in the original (Heros Bold Italic, the font's own spacing); the
    # letters, digits and signs slanted too
    for ids, t in texts.items():
        if ids in BIG:
            p.text(ids, t, fonts=('herosbi',))
        elif len(t) > 1:
            p.word(ids, t, 'herosbi', 5)
        else:
            p.text(ids, t, fonts=('herosbi',))

    # ENTER and its arrow: a line from the word to the right, and up
    p.used.add(45)
    p.words[1003] = dict(id=1003, c=list(WHITE), x=450, y=113, x2=472, y2=118)
    p.word(1003, 'ENTER', 'herosbi', 5)
    p.items.append('<path fill="none" stroke="#ffffff" stroke-width="%g" d="M%.2f %.2fH%.2fV%.2f"/>'
                   % (S, 475 * S, 115.5 * S, 486.5 * S, 97 * S))

    # Straight signs: squares
    for i in (10, 42, 43, 44):
        p.pixels(i)

    # The cursor keys: chevrons
    def chevron(i, pts):
        b = p.box(i)
        X, Y, W, H = p.moved(b)
        p.items.append('<polyline fill="none" stroke="#ffffff" stroke-width="%g" stroke-linejoin="miter" points="%s"/>'
                       % (1.4 * S, ' '.join('%.2f,%.2f' % (X + u * W, Y + v * H) for u, v in pts)))
    chevron(79, [(0.85, 0.05), (0.15, 0.5), (0.85, 0.95)])
    chevron(80, [(0.15, 0.05), (0.85, 0.5), (0.15, 0.95)])
    chevron(81, [(0.05, 0.85), (0.5, 0.15), (0.95, 0.85)])
    chevron(82, [(0.05, 0.15), (0.5, 0.85), (0.95, 0.15)])

    print('%s, not drawn: %s' % (picture, p.unused()), file=sys.stderr)
    open(sys.argv[1], 'w').write(p.svg((0, 0, 0)))


if __name__ == '__main__':
    draw()
