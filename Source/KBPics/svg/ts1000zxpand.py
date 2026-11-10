# ts1000zxpand.py <out.svg> [scale]: the Timex Sinclair 1000 keyboard picture
# with the ZXpand, drawn again at a scale (2 if not given)
import zx81

# 88, 93, 94: white patches over the old texts; LPRINT and LLIST are small
# under ZXPAND and CONFIG
zx81.draw('ts1000zxpand', own={9: 'DELETE', 118: 'ENTER', 89: 'ZXPAND', 92: 'CONFIG',
                               129: 'CAT/COPY'},
          small={101: 'LPRINT', 105: 'LLIST'}, ignore=(88, 93, 94))
