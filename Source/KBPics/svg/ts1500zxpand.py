# ts1500zxpand.py <out.svg> [scale]: the Timex Sinclair 1500 keyboard picture with the ZXpand
# (ZXPAND on S, CONFIG on G, CAT/COPY), drawn again at a scale (2 if not given)
import ts1500

# LPRINT and LLIST are small under ZXPAND and CONFIG
ts1500.draw('ts1500zxpand', own={36: 'GRAPHICS', 89: 'COS', 129: 'ZXPAND', 135: 'CONFIG', 154: 'CAT/COPY',
                                 187: '£'},
            small={141: 'LPRINT', 142: 'LLIST'})
