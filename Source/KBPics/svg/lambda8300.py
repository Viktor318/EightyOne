# lambda8300.py <out.svg> [scale]: the Lambda 8300 keyboard picture, drawn again at a scale (2 if not given)
#
# Its keys are those of the TS1500 (drawn by ts1500.keys()), in other
# colours, with yellow graphics.
import os
import sys
import numpy as np
from kbdraw import Picture, hexcol, font
import ts1500

HERE = os.path.dirname(os.path.abspath(__file__))
BACK, KEY, FACE = (102, 102, 102), (149, 179, 174), (104, 150, 142)
WHITE, YELLOW, BLACK = (255, 255, 255), (242, 207, 65), (0, 0, 0)

TEXTS = {
    # 1 - 0
    18: '1', 19: '2', 20: '3', 21: '4', 22: '5', 23: '6', 24: '7', 25: '8', 26: '9',
    10: '$', 11: '"', (12, 15): ';', 16: "'", 13: '(', 14: ')',
    # Q - P
    31: 'SIN', 32: 'COS', 33: 'TAN', 34: 'LOG', 35: 'SGN', 36: 'SQR', 37: 'RND', 38: 'PI',
    52: 'Q', 54: 'W', 56: 'E', 58: 'R', 60: 'T', 62: 'Y', 64: 'U', 66: 'I', 68: 'O', 69: 'P',
    50: '<', 51: '>',
    # A - ENTER
    70: 'ASN', 71: 'ACS', 72: 'ATN', 73: 'EXP', 74: 'ABS', 75: 'GRAPHICS',
    91: 'A', 92: 'S', 93: 'D', 94: 'F', 95: 'G', 96: 'H', 97: 'J', 98: 'K', 99: 'L',
    # SHIFT - SPACE
    105: 'PRINT', (106, 107): 'LINE NO', 112: 'EDIT', 113: 'DELETE', 114: 'BREAK',
    126: 'Z', 127: 'X', 128: 'C', 129: 'V', 130: 'B', 131: 'N', 132: 'M', 140: '.',
}
BIG = set([18, 19, 20, 21, 22, 23, 24, 25, 26, 52, 54, 56, 58, 60, 62, 64, 66, 68, 69,
           91, 92, 93, 94, 95, 96, 97, 98, 99, 126, 127, 128, 129, 130, 131, 132])
# the arrows above C, V, B and N: their outlines in their boxes
ARROWS = {
    108: [(10 / 22, 0), (12 / 22, 0), (1, 1), (0, 1)],
    109: [(0, 0), (1, 0), (12 / 22, 1), (10 / 22, 1)],
    110: [(0, 2 / 6), (7.5 / 11, 0), (1, 0), (1, 1), (7.5 / 11, 1), (0, 4 / 6)],
    111: [(1, 2 / 6), (3.5 / 11, 0), (0, 0), (0, 1), (3.5 / 11, 1), (1, 4 / 6)],
}


def main():
    p = Picture(os.path.join(HERE, 'lambda8300.json'), os.path.join(HERE, 'orig/lambda8300.png'),
                float(sys.argv[2]) if len(sys.argv) > 2 else 2)
    S = p.S
    # the colours of the picture: the greys that are almost one are one
    pure = np.array([BACK, KEY, FACE, WHITE, YELLOW, BLACK])
    p.im = pure[((p.im[:, :, None, :] - pure[None, None]) ** 2).sum(-1).argmin(-1)]

    faces = ts1500.keys(p, KEY, FACE, (30, 39))

    def on_face(ids):
        b = p.box(ids)
        p.used.difference_update(ids if isinstance(ids, tuple) else (ids,))
        return any(f['x'] <= b['x'] and b['x2'] <= f['x2'] and f['y'] <= b['y'] and b['y2'] <= f['y2']
                   for f in faces.values())

    # The words: all of one size, as most of them (6 pixels high), in the
    # font's own spacing; those on the tops kept off their edges
    for ids, t in TEXTS.items():
        if ids in BIG:
            p.text(ids, t, fonts=('herosb',))
        elif len(t) > 1:
            p.word(ids, t, 'cnb', 6, margin=5 if on_face(ids) else 2)
        else:
            p.text(ids, t, fonts=('herosb', 'cnb'))

    # ENTER and SPACE: larger (Viktor's choice), in the middle of their tops
    for ids, t, k, cap in ((90, 'ENTER', 29, 7.5), (133, 'SPACE', 39, 10)):
        p.used.add(ids)
        f, key, nk = faces[k], p.keys[k], p.newkeys[k]
        fx = f['x'] + nk[0] - key[0]
        fw = f['x2'] - f['x'] + nk[2] - key[2]
        cx, cy = (fx + fw / 2) * S, (f['y'] + f['y2']) / 2 * S
        fn = font('cnb')
        d, (x0, y0, x1, y1) = fn.outline(t)
        sc = cap * S / fn.outline('H')[1][3]
        p.items.append('<path fill="#ffffff" transform="matrix(%.5f 0 0 %.5f %.3f %.3f)" d="%s"/>'
                       % (sc, -sc, cx - sc * (x0 + x1) / 2, cy + cap * S / 2, d))

    # SHIFT: big, as in the original, in the middle of its top
    b = p.box(125)
    f, key, nk = faces[30], p.keys[30], p.newkeys[30]
    fx = f['x'] + nk[0] - key[0]
    fw = f['x2'] - f['x'] + nk[2] - key[2]
    W, H = (b['x2'] - b['x']) * S, (b['y2'] - b['y']) * S
    p.fit('SHIFT', (fx + fw / 2) * S - W / 2, (f['y'] + f['y2']) / 2 * S - H / 2, W, H, BLACK,
          fonts=('herosb', 'cnb'))

    # 0 with its slash
    b = p.box(27)
    X, Y, W, H = p.moved(b)
    p.fit('0', X, Y, W, H, WHITE, fonts=('herosb',))
    p.items.append('<line x1="%g" y1="%g" x2="%g" y2="%g" stroke="#ffffff" stroke-width="%g"/>'
                   % (X + W - 0.6 * S, Y + 0.6 * S, X + 0.6 * S, Y + H - 0.6 * S, 1.3 * S))

    # / on H: a line; * on J: as on the ZX81
    b = p.box(82)
    X, Y, W, H = p.moved(b)
    p.items.append('<line x1="%g" y1="%g" x2="%g" y2="%g" stroke="#ffffff" stroke-width="%g"/>'
                   % (X + 0.5 * S, Y + H - 0.2 * S, X + W - 0.5 * S, Y + 0.2 * S, 1.25 * S))
    b = p.box(88)
    X, Y, W, H = p.moved(b)
    ts1500.star(p, X, Y, W, H, WHITE)

    # The graphics (squares, and the figures on 1, 2, 3 and A) of their
    # pixels; the straight signs too
    for i in (28, 29, 30, 100, 53, 55, 57, 59, 61, 63, 65, 67, 134, 135, 136, 137, 138, 139,
              17, 86, 89):
        p.pixels(i)

    # The half squares on S, D, F, G: the frame of its pixels, the half a
    # triangle
    for i in (101, 102, 103, 104):
        b = p.box(i)
        X, Y, W, H = p.moved(b)
        x0, y0, x1, y1 = b['x'], b['y'], b['x2'], b['y2']
        m = (p.im[y0 + 1:y1 - 1, x0 + 1:x1 - 1] == YELLOW).all(-1)
        # which corner the triangle is in: the half that matches its pixels
        # best
        hh, ww = m.shape
        yy, xx = np.mgrid[0:hh, 0:ww]
        u, v = (xx + 0.5) / ww, (yy + 0.5) / hh
        halves = {'tr': u >= v, 'tl': u + v <= 1, 'br': u + v >= 1, 'bl': u <= v}
        c = max(halves, key=lambda k: (halves[k] == m).sum())
        E = lambda v: round(v * S)
        l, t, r, bt = E(X / S), E(Y / S), E(X / S + W / S), E(Y / S + H / S)
        il, it, ir, ib = E(X / S + 1), E(Y / S + 1), E(X / S + W / S - 1), E(Y / S + H / S - 1)
        tri = {'tr': [(il, it), (ir, it), (ir, ib)], 'tl': [(il, it), (ir, it), (il, ib)],
               'br': [(ir, it), (ir, ib), (il, ib)], 'bl': [(il, it), (ir, ib), (il, ib)]}[c]
        # the frame (1 pixel)
        p.items.append('<path fill="%s" fill-rule="evenodd" d="M%d %dH%dV%dH%dz M%d %dH%dV%dH%dz"/>'
                       % (hexcol(YELLOW), l, t, r, bt, l, il, it, ir, ib, il))
        p.items.append('<polygon fill="%s" points="%s"/>'
                       % (hexcol(YELLOW), ' '.join('%.2f,%.2f' % q for q in tri)))

    # The arrows above C, V, B and N
    for i, pts in ARROWS.items():
        b = p.box(i)
        X, Y, W, H = p.moved(b)
        p.items.append('<polygon fill="#000000" points="%s"/>'
                       % ' '.join('%.2f,%.2f' % (X + u * W, Y + v * H) for u, v in pts))

    print('lambda8300, not drawn: %s' % [i for i in p.unused() if p.words[i]['c'] != list(BACK)],
          file=sys.stderr)
    open(sys.argv[1], 'w').write(p.svg(BACK))


if __name__ == '__main__':
    main()
