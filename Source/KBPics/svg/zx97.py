# zx97.py <out.svg> [scale]: the ZX97 keyboard picture (the ZX81's, its top
# row higher, with the ZX97 commands under it), drawn again at a scale (2 if
# not given)
import zx81

zx81.draw('zx97', own={28: 'DIR', 29: 'HLOAD', 30: 'HSAVE', 31: 'F4', 32: 'F5', 33: 'F6',
                       34: 'F7', 35: 'F8', 36: 'F9', 37: 'F0'})
