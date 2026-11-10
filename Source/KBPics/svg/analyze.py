# analyze.py <picture> <prefix> <key colours r,g,b[/r,g,b]> [gap] [background r,g,b]
#            [colours r,g,b/r,g,b/...]:
# the keys and the texts of a keyboard picture (texts: pixels of one colour,
# letters at most <gap> apart). Writes <prefix>.json and <prefix>_ids.png
# (4x, with the numbers of the texts, for the tables of the drawings).
# Colours: the colours of the picture, for pictures with smoothed letters
# (the ZX80): any other colour is taken as the nearest of them.
# small=w,h (anywhere after the picture): only parts at most w x h pixels
# are letters; larger ones (lines, frames: the Timex pictures) are not texts
import sys, json
import numpy as np
from PIL import Image, ImageDraw, ImageFont
from scipy import ndimage

small = None
for a in list(sys.argv):
    if a.startswith('small='):
        small = [int(v) for v in a[6:].split(',')]
        sys.argv.remove(a)
src, prefix = sys.argv[1], sys.argv[2]
keycols = [tuple(int(v) for v in c.split(',')) for c in sys.argv[3].split('/')]
keycol = keycols[0]
gap = int(sys.argv[4]) if len(sys.argv) > 4 else 2
im = np.array(Image.open(src).convert('RGB')).astype(int)
h, w, _ = im.shape
if len(sys.argv) > 6:
    pure = np.array([[int(v) for v in c.split(',')] for c in sys.argv[6].split('/')])
    near = ((im[:, :, None, :] - pure[None, None]) ** 2).sum(-1).argmin(-1)
    im = pure[near]
key = np.zeros(im.shape[:2], bool)
for kc in keycols: key |= (im == kc).all(-1)

# keys: connected regions of the key colour (with what they hold: holes filled)
# (smaller parts of the key colour are texts: the white words of the ZX81)
lab, n = ndimage.label(ndimage.binary_fill_holes(key))
keys = []
inkey = np.zeros(key.shape, bool)
for i, sl in enumerate(ndimage.find_objects(lab)):
    ys, xs = sl
    if (ys.stop - ys.start) < 16 or (xs.stop - xs.start) < 16: continue
    keys.append([xs.start, ys.start, xs.stop - xs.start, ys.stop - ys.start])
    inkey |= lab == i + 1
keys.sort(key=lambda k: (k[1] // 10, k[0]))

# ink: everything that is not the background (most common) or a key colour
flat = im.reshape(-1, 3)
cols, counts = np.unique(flat, axis=0, return_counts=True)
bg = tuple(cols[counts.argmax()]) if len(sys.argv) <= 5 else tuple(int(v) for v in sys.argv[5].split(','))
blobs = []
for col in cols:
    col = tuple(col)
    m = (im == col).all(-1)
    if col == bg:
        # the background colour on a key is a text (the black letters of
        # the ZX81)
        m &= inkey
    elif col in keycols:
        m &= ~inkey
    if not m.any(): continue
    l, k = ndimage.label(m, structure=np.ones((3, 3)))
    for sl in ndimage.find_objects(l):
        ys, xs = sl
        if small and (xs.stop - xs.start > small[0] or ys.stop - ys.start > small[1]):
            continue
        blobs.append(dict(c=[int(v) for v in col], x=xs.start, y=ys.start, x2=xs.stop, y2=ys.stop))

# group into words: same colour, overlapping rows, gap <= gap
def merge(bs):
    changed = True
    while changed:
        changed = False
        out = []
        for b in sorted(bs, key=lambda b: (b['c'], b['x'])):
            for o in out:
                if o['c'] == b['c'] and min(o['y2'], b['y2']) - max(o['y'], b['y']) > 0 \
                        and b['x'] - o['x2'] <= gap and o['x'] - b['x2'] <= gap:
                    o['x'] = min(o['x'], b['x']); o['x2'] = max(o['x2'], b['x2'])
                    o['y'] = min(o['y'], b['y']); o['y2'] = max(o['y2'], b['y2'])
                    changed = True
                    break
            else:
                out.append(dict(b))
        bs = out
    return bs
# small parts (dots of ! ; : %, bars of =) join a neighbour of their colour
# above or below
def merge_small(bs):
    out = [dict(b) for b in bs]
    changed = True
    while changed:
        changed = False
        for a in out:
            for b in out:
                if a is b or a['c'] != b['c']: continue
                small = min(a['y2'] - a['y'], b['y2'] - b['y']) <= 2 or min(a['x2'] - a['x'], b['x2'] - b['x']) <= 2 and min(a['y2'] - a['y'], b['y2'] - b['y']) <= 3
                if not small: continue
                hov = min(a['x2'], b['x2']) - max(a['x'], b['x']) > 0
                vgap = max(a['y'], b['y']) - min(a['y2'], b['y2'])
                if hov and vgap <= 2:
                    a['x'] = min(a['x'], b['x']); a['x2'] = max(a['x2'], b['x2'])
                    a['y'] = min(a['y'], b['y']); a['y2'] = max(a['y2'], b['y2'])
                    out.remove(b); changed = True; break
            if changed: break
    return out
words = merge(merge_small(merge(blobs)))
words.sort(key=lambda b: (b['y'] // 6, b['x']))
for i, b in enumerate(words): b['id'] = i
json.dump(dict(size=[w, h], bg=[int(v) for v in bg], key=list(keycol), keys=keys, words=words),
          open(prefix + '.json', 'w'), indent=0)

# numbered picture, 4x
S = 4
big = Image.fromarray(im.astype(np.uint8)).resize((w * S, h * S), Image.NEAREST)
d = ImageDraw.Draw(big)
f = ImageFont.truetype('/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf', 11)
for b in words:
    d.rectangle([b['x'] * S, b['y'] * S, b['x2'] * S - 1, b['y2'] * S - 1], outline=(255, 128, 0))
    d.text((b['x'] * S, b['y'] * S - 11), str(b['id']), fill=(255, 128, 0), font=f)
big.save(prefix + '_ids.png')
print(len(keys), 'keys,', len(words), 'words, bg', bg)
