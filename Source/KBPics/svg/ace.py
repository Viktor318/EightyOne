# ace.py <out.svg> [scale]: the Jupiter Ace keyboard picture, drawn again at a scale (2 if not given)
import os
import sys
from kbdraw import Picture, hexcol

HERE = os.path.dirname(os.path.abspath(__file__))
KEY, WHITE, BLACK = (30, 23, 9), (255, 255, 255), (0, 0, 0)

TEXTS = {
    # 1 - 0
    9: '1', 10: '2', 11: '3', 12: '4', 13: '5', 14: '6', 15: '7', 16: '8', 17: '9',
    0: '!', 1: '@', 2: '#', 3: '$', 4: '%', 5: '&', 6: "'", 7: '(', 8: ')',
    28: 'DELETE', 35: 'LINE', 29: 'CAPS', 36: 'LOCK', 30: 'INVERSE', 37: 'VIDEO', 33: 'GRAPHICS',
    34: 'DELETE',
    # Q - P
    47: 'Q', 48: 'W', 49: 'E', 50: 'R', 51: 'T', 52: 'Y', 53: 'U', 54: 'I', 55: 'O', 56: 'P',
    40: '<', 41: '>', 42: '[', 43: ']', 44: '©', 45: ';', 46: '"',
    # A - ENTER
    66: 'A', 67: 'S', 68: 'D', 69: 'F', 70: 'G', 71: 'H', 72: 'J', 73: 'K', 74: 'L', 75: 'ENTER',
    60: '{', 61: '}', 62: '↑',
    # SHIFT - SPACE
    86: 'Z', 87: 'X', 88: 'C', 89: 'V', 90: 'B', 91: 'N', 92: 'M',
    77: '£', 78: '?', 81: ',', 85: 'SHIFT', 83: 'SYMBOL', 93: 'SHIFT', 84: 'BREAK',
}
BIG = set(list(range(9, 18)) + list(range(47, 57)) + list(range(66, 75)) + list(range(86, 93)))


def main():
    p = Picture(os.path.join(HERE, 'ace.json'), os.path.join(HERE, 'orig/ace.png'),
                float(sys.argv[2]) if len(sys.argv) > 2 else 2)
    S = p.S

    # Keys: one pitch and width a row (the last row: SPACE keeps its own),
    # on one line
    p.regular_rows(wide=(39,))
    for k in p.newkeys:
        if k[1] > 170:
            k[1], k[3] = 175, 32
    p.draw_keys(KEY, 2.5)

    # The words: all as most of them (Heros Bold, 5 pixels high), those
    # under the keys in the middle of them; SPACE larger, as in the
    # original. The letters and digits bold, the signs thin, as there
    for ids, t in TEXTS.items():
        if ids in BIG:
            p.text(ids, t, fonts=('herosb',))
        elif len(t) > 1:
            # under the top row: in the middle of the key above
            b = p.words[ids]
            over = True
            if 50 <= b['y'] <= 65:
                over = max(range(10), key=lambda i: min(b['x2'], p.keys[i][0] + p.keys[i][2]) - max(b['x'], p.keys[i][0]))
            p.word(ids, t, 'herosb', 5, over=over)
        else:
            p.text(ids, t, fonts=('heros',))
    p.word(94, 'SPACE', 'herosb', 7)

    # 0 with its slash, from the top right to the bottom left
    b = p.box(18)
    X, Y, W, H = p.moved(b)
    p.fit('0', X + 1 * S, Y, W - 2 * S, H, WHITE, fonts=('herosb',))
    p.items.append('<line x1="%g" y1="%g" x2="%g" y2="%g" stroke="#ffffff" stroke-width="%g"/>'
                   % (X + W - 0.5 * S, Y + 0.5 * S, X + 0.5 * S, Y + H - 0.5 * S, 1.2 * S))

    # Block graphics and the straight signs: squares
    for i in (20, 21, 22, 23, 24, 25, 26, 27, 19, 58, 63, 64, 65, 76, 82):
        p.pixels(i)

    def line(x1, y1, x2, y2, colour=WHITE, w=1.0):
        p.items.append('<line x1="%.2f" y1="%.2f" x2="%.2f" y2="%.2f" stroke="%s" stroke-width="%g"/>'
                       % (x1, y1, x2, y2, hexcol(colour), w * S))

    # \ and /: thin lines
    b = p.box(59)
    X, Y, W, H = p.moved(b)
    line(X + 0.4 * S, Y + 0.3 * S, X + W - 0.4 * S, Y + H - 0.3 * S)
    b = p.box(79)
    X, Y, W, H = p.moved(b)
    line(X + 0.4 * S, Y + H - 0.3 * S, X + W - 0.4 * S, Y + 0.3 * S)

    # ~: a thin wave
    b = p.box(57)
    X, Y, W, H = p.moved(b)
    pt = lambda u, v: '%.2f %.2f' % (X + u * W, Y + v * H)
    p.items.append('<path fill="none" stroke="#ffffff" stroke-width="%g" stroke-linecap="round" '
                   'd="M%s C%s %s %s S%s %s"/>'
                   % (S, pt(0.05, 0.85), pt(0.15, 0.0), pt(0.4, 0.05), pt(0.55, 0.6),
                      pt(0.85, 1.0), pt(0.95, 0.15)))

    # * on B: thin strokes, as in the original
    b = p.box(80)
    X, Y, W, H = p.moved(b)
    cx = X + W / 2
    line(cx, Y, cx, Y + 3.5 * S)
    line(X, Y + 2.5 * S, X + W, Y + 2.5 * S)
    line(cx, Y + 3 * S, X + 1 * S, Y + H)
    line(cx, Y + 3 * S, X + W - 1 * S, Y + H)

    # The arrows under 5 - 8: a thin shaft and a filled head
    for i, direction in ((38, 'left'), (31, 'up'), (32, 'down'), (39, 'right')):
        b = p.box(i)
        X, Y, W, H = p.moved(b)
        if direction in ('left', 'right'):
            head, cy = 0.45 * W, Y + H / 2
            tip, base = (X, X + head) if direction == 'left' else (X + W, X + W - head)
            end = X + W if direction == 'left' else X
            pts = [(tip, cy), (base, Y), (base, Y + H)]
            line(base, cy, end, cy, BLACK)
        else:
            head, cx = 0.45 * H, X + W / 2
            tip, base = (Y, Y + head) if direction == 'up' else (Y + H, Y + H - head)
            end = Y + H if direction == 'up' else Y
            pts = [(cx, tip), (X, base), (X + W, base)]
            line(cx, base, cx, end, BLACK)
        p.items.append('<polygon fill="#000000" points="%s"/>' % ' '.join('%.2f,%.2f' % q for q in pts))

    print('ace, not drawn: %s' % p.unused(), file=sys.stderr)
    open(sys.argv[1], 'w').write(p.svg(WHITE))


if __name__ == '__main__':
    main()
