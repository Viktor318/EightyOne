# tk85.py <out.svg> [scale]: the TK85 keyboard picture (the ZX81's, coloured otherwise: dark
# keys, white letters, yellow and blue), drawn again at a scale (2 if not given)
import zx81

GREY, WHITE, LABEL, BLUE, YELLOW = (68, 68, 68), (255, 255, 255), (236, 252, 234), (57, 114, 197), (239, 252, 0)

# The texts of the ZX81 that are here otherwise (their letters a little
# other, or moved): {ZX81 number: number here}
MORE = {0: 0, 1: 1, 2: 2, 3: 3, 8: 8, 9: 9, 40: 40, 41: 41, 69: 69, 75: 75, 77: 77, 82: 83, 83: 84,
        87: 90, 88: 91, 89: 92, 93: 96, 94: 97, 106: 97, 118: 119, 124: 125,
        131: 132, 140: 142, 146: 148, 147: 149, 148: 150, 149: 151, 150: 152, 153: 155,
        155: 157, 159: 161}



def larger(p):
    """NEW LINE and SPACE larger (Viktor's choice); NEW LINE in the middle
    of its key, SPACE where it was, at its right"""
    S = p.S
    fn = zx81.font('cnb')
    cap0 = fn.outline('H')[1][3]

    def put(t, cx, base, cap, right=None):
        d, (x0, y0, x1, y1) = fn.outline(t)
        sc = cap * S / cap0
        left = (right - sc * (x1 - x0)) if right is not None else cx - sc * (x1 - x0) / 2
        p.items.append('<path fill="#ffffff" transform="matrix(%.5f 0 0 %.5f %.3f %.3f)" d="%s"/>'
                       % (sc, -sc, left - sc * x0, base, d))

    b = p.box((108, 115))
    k = p.key_of(b)
    x, y, w, h = p.newkeys[k]
    cap, gap = 8, 3.5
    cx, cy = (x + w / 2) * S, (y + h / 2) * S
    top = cy - (2 * cap + gap) * S / 2
    put('NEW', cx, top + cap * S, cap)
    put('LINE', cx, top + (2 * cap + gap) * S, cap)
    b = p.box(154)
    X, Y, W, H = p.moved(b)
    put('SPACE', 0, Y + H, 9, right=X + W)


# FUNCTION is above NEW LINE here; INKEY$ was found as one word; a few
# single odd pixels are no texts
zx81.draw('tk85', own={88: 'FUNCTION', 158: 'INKEY$'}, ignore=(78, 89, 133, 159),
          colours=dict(key=GREY, label=LABEL, red=YELLOW, black=WHITE),
          palette=[(0, 0, 0), GREY, WHITE, LABEL, BLUE, YELLOW], more=MORE, extra=larger)
