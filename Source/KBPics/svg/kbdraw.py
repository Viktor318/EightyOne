# kbdraw.py: draws a keyboard picture of EightyOne again, as SVG, at any scale.
# The places come from the original bitmap (analyze.py), the texts from a
# table; texts are outlines of TeX Gyre Heros (GUST Font License) fitted
# into the box of the original pixel text, so the result needs no fonts.
import json, math, os
import numpy as np
from PIL import Image
from fontTools.ttLib import TTFont
from fontTools.pens.svgPathPen import SVGPathPen
from fontTools.pens.boundsPen import BoundsPen
from fontTools.pens.transformPen import TransformPen

# TeX Gyre Heros (in TeX Live, or fonts-texgyre on Debian/Ubuntu); the
# folder can be given in TEXGYRE
FONTDIRS = [os.environ.get('TEXGYRE', ''),
            '/usr/share/texmf/fonts/opentype/public/tex-gyre/',
            '/usr/share/fonts/opentype/texgyre/',
            '/usr/share/texlive/texmf-dist/fonts/opentype/public/tex-gyre/']


def font_file(name):
    for d in FONTDIRS:
        if d and os.path.exists(os.path.join(d, name)):
            return os.path.join(d, name)
    raise SystemExit('%s not found: give its folder in TEXGYRE' % name)


class Font:
    def __init__(self, file):
        self.f = TTFont(font_file(file))
        self.gs = self.f.getGlyphSet()
        self.cmap = self.f.getBestCmap()
        self.hmtx = self.f['hmtx']
        self.upm = self.f['head'].unitsPerEm

    def outline(self, text, tracking=0.0):
        pen = SVGPathPen(self.gs)
        bp = BoundsPen(self.gs)
        x = 0.0
        for i, ch in enumerate(text):
            g = self.cmap.get(ord(ch))
            if g is None:
                raise KeyError('%r not in font' % ch)
            self.gs[g].draw(TransformPen(pen, (1, 0, 0, 1, x, 0)))
            self.gs[g].draw(TransformPen(bp, (1, 0, 0, 1, x, 0)))
            x += self.hmtx[g][0] + tracking
        return pen.getCommands(), bp.bounds


FONTS = {}


def font(name):
    if name not in FONTS:
        FONTS[name] = Font({
            'heros': 'texgyreheros-regular.otf',
            'herosb': 'texgyreheros-bold.otf',
            'cn': 'texgyreheroscn-regular.otf',
            'cnb': 'texgyreheroscn-bold.otf',
        }[name])
    return FONTS[name]


def hexcol(c):
    return '#%02x%02x%02x' % tuple(c)


class Picture:
    def __init__(self, analysis, bmp, scale=2):
        self.a = json.load(open(analysis))
        self.im = np.array(Image.open(bmp).convert('RGB')).astype(int)
        self.S = scale
        self.w, self.h = self.a['size']
        self.words = {b['id']: b for b in self.a['words']}
        self.items = []         # svg elements, in drawing order
        self.keys = [list(k) for k in self.a['keys']]
        self.newkeys = [list(k) for k in self.keys]
        self.used = set()

    # ---- keys -------------------------------------------------------------
    def regular_rows(self, wide=()):
        """Keys of a row at one pitch, one width, one top and height (the
        wide keys keep their places and widths); returns the moves"""
        rows = {}
        for i, k in enumerate(self.keys):
            rows.setdefault(round(k[1] / 20), []).append(i)
        for r, idx in rows.items():
            idx.sort(key=lambda i: self.keys[i][0])
            y = min(self.keys[i][1] for i in idx)
            h = int(np.median([self.keys[i][3] for i in idx]))
            std = [i for i in idx if self.keys[i][2] < 45 and i not in wide]
            w = int(np.median([self.keys[i][2] for i in std])) if std else None
            if len(std) >= 3:
                n = np.array([idx.index(i) for i in std], float)
                xs = np.array([self.keys[i][0] for i in std], float)
                # pitch over the standard keys, counted by place in the row
                p = np.polyfit(n, xs, 1)
            for i in idx:
                k = self.newkeys[i]
                k[1] = y
                k[3] = h
                if i in std and len(std) >= 3:
                    k[0] = round((p[1] + p[0] * idx.index(i)) * self.S) / self.S
                    k[2] = w

    def key_of(self, b):
        """The key a text belongs to: the nearest"""
        cx, cy = (b['x'] + b['x2']) / 2, (b['y'] + b['y2']) / 2
        best, bd = None, 1e9
        for i, k in enumerate(self.keys):
            dx = max(k[0] - cx, 0, cx - (k[0] + k[2]))
            dy = max(k[1] - cy, 0, cy - (k[1] + k[3]))
            d = dx * dx + 4 * dy * dy
            if d < bd:
                best, bd = i, d
        return best

    def moved(self, b, follow=True):
        """The box of a text (x, y, w, h) in the new picture, moved with its
        key"""
        x, y, w, h = b['x'], b['y'], b['x2'] - b['x'], b['y2'] - b['y']
        if follow:
            k = self.key_of(b)
            x += self.newkeys[k][0] - self.keys[k][0]
            if follow != 'x':
                y += self.newkeys[k][1] - self.keys[k][1]
        S = self.S
        return x * S, y * S, w * S, h * S

    def draw_keys(self, fill, rx, outline=None, inset=0, ow=2, orx=None):
        S = self.S
        for k in self.newkeys:
            # edges on whole pixels (at 1.5x or 2.5x too)
            x, y = round(k[0] * S), round(k[1] * S)
            w, h = round((k[0] + k[2]) * S) - x, round((k[1] + k[3]) * S) - y
            self.items.append('<rect x="%g" y="%g" width="%g" height="%g" rx="%g" fill="%s"/>'
                              % (x, y, w, h, rx * S, hexcol(fill)))
            if outline:
                o = inset * S + ow / 2
                self.items.append('<rect x="%g" y="%g" width="%g" height="%g" rx="%g" fill="none" stroke="%s" stroke-width="%g"/>'
                                  % (x + o, y + o, w - 2 * o, h - 2 * o, (orx if orx is not None else rx) * S,
                                     hexcol(outline), ow))

    # ---- texts ------------------------------------------------------------
    def box(self, ids, follow=True):
        if isinstance(ids, int):
            ids = (ids,)
        bs = [self.words[i] for i in ids]
        self.used.update(ids)
        b = dict(x=min(v['x'] for v in bs), y=min(v['y'] for v in bs),
                 x2=max(v['x2'] for v in bs), y2=max(v['y2'] for v in bs), c=bs[0]['c'])
        return b

    def text(self, ids, text, fonts=('cnb', 'herosb'), colour=None, follow=True,
             maxtrack=0.12, pad=(0, 0, 0, 0), stretch=1.0, natural=None):
        b = self.box(ids)
        X, Y, W, H = self.moved(b, follow)
        # pad: grow the box (left, top, right, bottom), in output pixels
        X -= pad[0]; Y -= pad[1]; W += pad[0] + pad[2]; H += pad[1] + pad[3]
        self.fit(text, X, Y, W, H, colour or b['c'], fonts, maxtrack, stretch, natural)

    def fit(self, text, X, Y, W, H, colour, fonts=('cnb', 'herosb'), maxtrack=0.12, stretch=1.0,
            natural=None):
        if natural:
            # the font's own proportions and spacing, at the height of the
            # box, aligned 'left' or 'centre' in it
            f = font(fonts[0])
            d, (x0, y0, x1, y1) = f.outline(text)
            s = H / (y1 - y0)
            left = X if natural == 'left' else X + (W - s * (x1 - x0)) / 2
            self.items.append('<path fill="%s" transform="matrix(%.5f 0 0 %.5f %.3f %.3f)" d="%s"/>'
                              % (hexcol(colour), s, -s, left - s * x0, Y + s * y1, d))
            return
        best = None
        for fn in fonts:
            f = font(fn)
            d, (x0, y0, x1, y1) = f.outline(text)
            s = H / (y1 - y0)
            t = 0.0
            if len(text) > 1 and (x1 - x0) * s < W:
                t = min((W / s - (x1 - x0)) / (len(text) - 1), maxtrack * f.upm)
                d, (x0, y0, x1, y1) = f.outline(text, t)
            a, dd = W / (x1 - x0), H / (y1 - y0)
            score = abs(math.log(a / dd / stretch))
            if best is None or score < best[0]:
                best = (score, d, a, dd, x0, y1)
        _, d, a, dd, x0, y1 = best
        self.items.append('<path fill="%s" transform="matrix(%.5f 0 0 %.5f %.3f %.3f)" d="%s"/>'
                          % (hexcol(colour), a, -dd, X - a * x0, Y + dd * y1, d))

    # ---- pixel shapes -----------------------------------------------------
    def pixels(self, ids, colour=None, follow=True, region=None):
        """The pixels of the text in its box, as squares (for the block
        graphics, which are made of squares)"""
        b = self.box(ids)
        c = colour or b['c']
        col = tuple(b['c'])
        X, Y, _, _ = self.moved(b, follow)
        S = self.S
        parts = []
        for y in range(b['y'], b['y2']):
            x = b['x']
            while x < b['x2']:
                if tuple(self.im[y, x]) == col:
                    x2 = x
                    while x2 < b['x2'] and tuple(self.im[y, x2]) == col:
                        x2 += 1
                    # edges on whole pixels (at 1.5x or 2.5x too)
                    l, t = round(X + (x - b['x']) * S), round(Y + (y - b['y']) * S)
                    r, bt = round(X + (x2 - b['x']) * S), round(Y + (y + 1 - b['y']) * S)
                    parts.append('M%g %gh%gv%gh%gz' % (l, t, r - l, bt - t, -(r - l)))
                    x = x2
                else:
                    x += 1
        self.items.append('<path fill="%s" d="%s"/>' % (hexcol(c), ''.join(parts)))

    def dotted(self, ids, colour=None, follow=True):
        """A block graphic with dotted parts (the grey of the ZX80 and the
        ZX81, a chequer of single pixels): the solid parts (the frame too)
        on whole pixels, as pixels() draws them; the dots spread evenly over
        the rows and columns they fill, so that they are of one size and
        one spacing at every scale"""
        S = self.S
        b = self.box(ids)
        col = tuple(b['c'])
        X, Y, _, _ = self.moved(b, follow)
        x0, x1 = b['x'], b['x2']
        edge = lambda v, o: round(o + v * S)
        solid, dots, band = [], [], []

        def rect(l, t, r, bt, out):
            out.append('M%g %gh%gv%gh%gz' % (l, t, r - l, bt - t, -(r - l)))

        def ink(y):
            return (self.im[y, x0:x1] == col).all(-1)

        for y in list(range(b['y'], b['y2'])) + [None]:
            row = None if y is None else ink(y)
            inner = None if y is None else row[1:-1]
            if y is not None and inner.any() and not (inner[:-1] & inner[1:]).any():
                band.append(y)
                continue
            if band:
                # a band of dotted rows: its pixels split evenly
                l, r = edge(1, X), edge(x1 - x0 - 1, X)
                t, bt = edge(band[0] - b['y'], Y), edge(band[-1] + 1 - b['y'], Y)
                cw, ch = (r - l) / (x1 - x0 - 2), (bt - t) / len(band)
                for n, yy in enumerate(band):
                    for m, on in enumerate(ink(yy)[1:-1]):
                        if on:
                            rect(l + m * cw, t + n * ch, l + (m + 1) * cw, t + (n + 1) * ch, dots)
                    ty, by = edge(yy - b['y'], Y), edge(yy + 1 - b['y'], Y)
                    rect(edge(0, X), ty, l, by, solid)
                    rect(r, ty, edge(x1 - x0, X), by, solid)
                band = []
            if y is None:
                break
            # a solid row: its runs of pixels
            ty, by = edge(y - b['y'], Y), edge(y + 1 - b['y'], Y)
            x = 0
            while x < len(row):
                if row[x]:
                    e = x
                    while e < len(row) and row[e]:
                        e += 1
                    rect(edge(x, X), ty, edge(e, X), by, solid)
                    x = e
                else:
                    x += 1
        self.items.append('<path fill="%s" d="%s"/>' % (hexcol(colour or col), ''.join(solid)))
        self.items.append('<path fill="%s" d="%s"/>' % (hexcol(colour or col), ''.join(dots)))

    def same_texts(self, analysis, picture, dx=2, dy=(-10, 2)):
        """The numbers of another picture's texts in this one: {its number:
        number here}, for the texts of the same pixels and colour, the
        nearest (at most dx sideways and dy up or down)"""
        z = json.load(open(analysis))
        zi = np.array(Image.open(picture).convert('RGB')).astype(int)
        found = {}
        for w in z['words']:
            best = None
            for o in self.words.values():
                ox, oy = o['x'] - w['x'], o['y'] - w['y']
                if o['c'] != w['c'] or abs(ox) > dx or not dy[0] <= oy <= dy[1] \
                        or o['x2'] - o['x'] != w['x2'] - w['x'] or o['y2'] - o['y'] != w['y2'] - w['y']:
                    continue
                if not (self.im[o['y']:o['y2'], o['x']:o['x2']] == zi[w['y']:w['y2'], w['x']:w['x2']]).all():
                    continue
                d = abs(ox) + abs(oy)
                if best is None or d < best[0]:
                    best = (d, o['id'])
            if best:
                found[w['id']] = best[1]
        return found

    def svg(self, background):
        S = self.S
        # the size EightyOne gives its window at this zoom: (505 * quarters
        # + 2) / 4
        W, H = int((self.w * S * 4 + 2) // 4), int((self.h * S * 4 + 2) // 4)
        out = ['<svg xmlns="http://www.w3.org/2000/svg" width="%d" height="%d" viewBox="0 0 %d %d">'
               % (W, H, W, H),
               '<!-- An EightyOne keyboard picture, drawn again by Source/KBPics/svg (see its '
               'README.md) from the original pixel picture. The letters are outlines of '
               'TeX Gyre Heros (GUST Font License). -->',
               '<rect width="100%%" height="100%%" fill="%s"/>' % hexcol(background)]
        out += self.items
        out.append('</svg>')
        return '\n'.join(out)

    def unused(self):
        return [i for i in self.words if i not in self.used]


# ---- shared helpers ---------------------------------------------------------
def arrow_points(X, Y, W, H, direction, sw, head=0.42, s0=0.28, s1=0.72):
    """An outline arrow in a box: the points of its centre line"""
    pts = [(0, 0.5), (head, 0.0), (head, s0), (1, s0), (1, s1), (head, s1), (head, 1.0)]
    if direction == 'right':
        pts = [(1 - x, y) for x, y in pts]
    elif direction == 'up':
        pts = [(y, x) for x, y in pts]
    elif direction == 'down':
        pts = [(y, 1 - x) for x, y in pts]
    return [(X + sw / 2 + x * (W - sw), Y + sw / 2 + y * (H - sw)) for x, y in pts]


def ring_paths(im, colour, scale, width, follow=None):
    """The dish outlines (1 pixel rings) as smooth closed lines through the
    middle of their pixels"""
    from scipy import ndimage
    from skimage import measure, filters
    ring = (im == colour).all(-1)
    lab, n = ndimage.label(ring, structure=np.ones((3, 3)))
    paths = []
    Z = 8
    for i in range(1, n + 1):
        r = lab == i
        f = ndimage.binary_fill_holes(r)
        inner = f & ~r
        ys, xs = np.where(f)
        y0, y1, x0, x1 = ys.min() - 2, ys.max() + 3, xs.min() - 2, xs.max() + 3
        field = (f.astype(float) + inner.astype(float))[y0:y1, x0:x1] / 2
        big = ndimage.zoom(field, Z, order=0)
        big = filters.gaussian(big, sigma=Z * 0.55)
        cs = measure.find_contours(big, 0.5)
        c = max(cs, key=len)
        c = measure.approximate_polygon(c, tolerance=Z * 0.04)
        pts = [((x0 + (q[1] + 0.5) / Z) * scale, (y0 + (q[0] + 0.5) / Z) * scale) for q in c]
        if follow:
            pts = [follow(x, y) for x, y in pts]
        d = 'M' + 'L'.join('%.2f %.2f' % q for q in pts) + 'Z'
        paths.append(d)
    return paths


def add_word_method():
    """Picture.word(): a word at a fixed cap height in a font's own spacing,
    on the original base line, aligned as the original box was in its key
    (left, right or centre); a little narrower only if there is no room"""
    import sys

    def word(self, ids, t, fontname, cap_px, colour=None, follow=True, margin=2, over=False):
        # over: a word above or under a key, in the middle of the key
        S = self.S
        b = self.box(ids)
        X, Y, Wd, H = self.moved(b, follow)
        dollar = '$' in t and (b['y2'] - b['y']) > cap_px + 1
        base = Y + ((b['y2'] - (1 if dollar else 0)) - b['y']) * S
        k = self.key_of(b)
        kx, ky, kw, kh = self.keys[k]
        nk = self.newkeys[k]
        inside = kx <= b['x'] and b['x2'] <= kx + kw and ky <= b['y'] and b['y2'] <= ky + kh
        f = font(fontname)
        cap = f.outline('H')[1][3]
        d, (x0, y0, x1, y1) = f.outline(t)
        sc = cap_px * S / cap
        w = (x1 - x0) * sc
        same = lambda o: o['y'] < b['y2'] and b['y'] < o['y2'] and o is not b \
            and not (isinstance(ids, tuple) and o['id'] in ids)
        align = 'left'
        if inside:
            lm, rm = b['x'] - kx, kx + kw - b['x2']
            if abs(lm - rm) <= 3:
                align = 'centre'
            elif rm <= 4:
                align = 'right'
        if inside:
            # room on the line in the key, up to its other things
            lo = [o['x2'] + 2 for o in self.words.values() if same(o) and kx <= o['x'] and o['x2'] <= b['x']]
            hi = [o['x'] - 2 for o in self.words.values() if same(o) and b['x2'] <= o['x'] and o['x2'] <= kx + kw]
            lo = max(lo + [kx + margin])
            hi = min(hi + [kx + kw - margin])
            if align == 'centre':
                c = (b['x'] + b['x2']) / 2
                room = 2 * min(c - lo, hi - c)
            elif align == 'right':
                room = b['x2'] - lo
            else:
                room = hi - b['x']
        elif over:
            align = 'over'
            room = kw + 8
        else:
            rights = [o['x'] - 6 for o in self.words.values() if same(o) and o['x'] >= b['x2']]
            room = min(rights + [self.w - 2]) - b['x']
        sx = min(1.0, room * S / w)
        w *= sx
        if align == 'centre':
            left = X + Wd / 2 - w / 2
        elif align == 'over':
            left = (nk[0] + nk[2] / 2) * S - w / 2
        elif align == 'right':
            left = X + Wd - w
        else:
            left = X
        self.items.append('<path fill="%s" transform="matrix(%.5f 0 0 %.5f %.3f %.3f)" d="%s"/>'
                          % (hexcol(colour or b['c']), sc * sx, -sc, left - sc * sx * x0, base, d))
        if sx < 1:
            print('narrower: %s %.2f' % (t, sx), file=sys.stderr)
    Picture.word = word


add_word_method()
