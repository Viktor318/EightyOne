# zx81zxpand.py <out.svg> [scale]: the ZX81 keyboard picture with the ZXpand
# (ZXPAND on S, CONFIG on G, CAT/COPY), drawn again at a scale (2 if not given)
import zx81

# 88, 93, 94: white patches over the old texts; LPRINT and LLIST are small
# under ZXPAND and CONFIG
zx81.draw('zx81zxpand', own={89: 'ZXPAND', 92: 'CONFIG', 130: 'CAT/COPY'},
          small={101: 'LPRINT', 105: 'LLIST'}, ignore=(88, 93, 94))
