# spectrum16.py <out.svg> [scale]: the ZX Spectrum 16K keyboard picture, drawn again at a scale (2 if not given)
#
# The picture is the 48K's with light grey keys (all else the same, pixel for
# pixel), so it is drawn as the 48K, with the places of spectrum48.json.
import spectrum48

spectrum48.draw('spectrum16', (186, 170, 173))
