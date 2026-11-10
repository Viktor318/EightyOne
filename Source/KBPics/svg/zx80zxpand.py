# zx80zxpand.py <out.svg> [scale]: the ZX80 keyboard picture with the ZXpand
# (DELETE, CONFIG, CAT), drawn again at a scale (2 if not given)
import zx80

zx80.draw('zx80zxpand', own={29: 'DELETE', 58: 'CONFIG', 80: 'CAT'})
