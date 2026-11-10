# ts1000.py <out.svg> [scale]: the Timex Sinclair 1000 keyboard picture (the
# ZX81's, with DELETE and ENTER), drawn again at a scale (2 if not given)
import zx81

zx81.draw('ts1000', own={9: 'DELETE', 113: 'ENTER'})
