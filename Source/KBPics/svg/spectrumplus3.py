# spectrumplus3.py <out.svg> [scale]: the ZX Spectrum +3 keyboard picture, drawn again at a scale (2 if not given)
#
# The picture is the +2's with dark keys (all else the same, pixel for
# pixel), so it is drawn as the +2, with the places of spectrumplus2.json.
import spectrumplus2

spectrumplus2.draw('spectrumplus3', (32, 32, 32))
