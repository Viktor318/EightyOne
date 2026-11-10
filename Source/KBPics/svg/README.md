# Keyboard pictures drawn again

The keyboard pictures of EightyOne (*Help > Keyboard Map*) were drawn in
pixels at 505 × 223, and look blurred when the window is enlarged. The
pictures here are the same pictures drawn again as vector graphics, so they
are sharp at any size:

| Drawing | Picture in the program | Original |
|---|---|---|
| `spectrum48.py` | `Keyboard_.dfm`, `spec48kb` | `orig/spectrum48.png` |
| `spectrum16.py` | `Keyboard_.dfm`, `spec16kb` | `orig/spectrum16.png` |
| `spectrum128.py` | `Keyboard_.dfm`, `spec128kb` | `orig/spectrum128.png` |
| `keypad128.py` | `Keypad_.dfm`, `KeypadUk` | `orig/keypad128uk.png` |
| `zx81.py` | `Keyboard_.dfm`, `zx81kb` | `orig/zx81.png` |
| `zx81zxpand.py` | `Keyboard_.dfm`, `zx81zxpandkb` | `orig/zx81zxpand.png` |
| `ts1000.py` | `Keyboard_.dfm`, `ts1000kb` | `orig/ts1000.png` |
| `ts1000zxpand.py` | `Keyboard_.dfm`, `ts1000zxpandkb` | `orig/ts1000zxpand.png` |
| `zx97.py` | `Keyboard_.dfm`, `zx97kb` | `orig/zx97.png` |
| `zx80.py` | `Keyboard_.dfm`, `zx80kb` | `orig/zx80.png` |
| `zx80zxpand.py` | `Keyboard_.dfm`, `zx80zxpandkb` | `orig/zx80zxpand.png` |
| `spectrumplus2.py` | `Keyboard_.dfm`, `specPlus2kb` | `orig/spectrumplus2.png` |
| `spectrumplus3.py` | `Keyboard_.dfm`, `specPlus3kb` | `orig/spectrumplus3.png` |
| `tc2048.py` | `Keyboard_.dfm`, `tc2048kb` | `orig/tc2048.png` |
| `ts2068.py` | `Keyboard_.dfm`, `ts2068kb` | `orig/ts2068.png` |
| `ts1500.py` | `Keyboard_.dfm`, `ts1500kb` | `orig/ts1500.png` |
| `ts1500zxpand.py` | `Keyboard_.dfm`, `ts1500zxpandkb` | `orig/ts1500zxpand.png` |

The program holds them at 2.5x (1263 × 558, the keypad 375 × 558), the
largest size of the window; smaller sizes are scaled down from it smoothly.

## How they are made

The places of everything come from the original picture: `analyze.py`
finds the keys and the texts (pixels of one colour), and writes them to
`<name>.json`, numbered; `<name>_ids.png` shows the numbers. The drawing
script of a picture says what each numbered text is (a table), and draws:

- the keys (rounded rectangles; the dishes of the 128 and the keypad as
  smooth lines through the middle of their pixels),
- the texts in TeX Gyre Heros Bold and Heros Cn Bold (GUST Font License),
  as outlines, so the SVG needs no fonts. The words of a picture are of one
  size and in the font's own spacing (the 48K and the ZX81: as high as
  EDIT; the 128 and the keypad: as LOCK, BORDR and DEL), on the base lines
  of the original;
  a word is made a little narrower only where it would touch something,
- the block graphics and the straight signs (− + = _) of their pixels, the
  arrows, the rainbow and a few signs as shapes. The dots of the grey
  block graphics of the ZX81 (a chequer of single pixels) are spread
  evenly over their squares, so they are of one size at every scale.

The edges of the keys and squares fall on whole pixels at every scale
(1.5x and 2.5x too), so they are sharp. The 48K picture has its key rows a
little further apart than the original (2 pixels each, from the free
space under the last row). Its words above the keys reach over their keys'
right edges as little as they can: CAPS LOCK and TRUE VIDEO with half a
space between their two words and 1 pixel over, INV.VIDEO and GRAPHICS at
93.5% of their width, RESTORE and MAGENTA at 98.2% (Viktor's choice). The
16K picture is the 48K's with light grey keys (`spectrum48.py` draws both,
with the places of `spectrum48.json`). On the ZX81 the white words, 6 pixels high in
the original, are 7 like the others: those above a key row grow upwards,
those under one downwards, so they stay 2 pixels from the keys. The
relatives of the ZX81 (ZXpand, TS1000, ZX97) are drawn by `zx81.py` too:
the texts they share with the ZX81 are found by their pixels, and each
script gives only its own texts. On the ZXpand pictures LPRINT and LLIST
stay small under ZXPAND and CONFIG (5 pixels high, they were 4). The ZX80
(and its ZXpand picture, by `zx80.py` too) has its words as the ZX81
(7 pixels), those above the keys in the middle of their keys, and its
letters, digits and signs thin (Heros), as in the original, whose letters
are smoothed: `analyze.py` takes its colours as the nearest of its four.
The +2 and the +3 (the same picture with dark keys, both by
`spectrumplus2.py`) keep the keys of the original, a top framed by a line
and lines from its corners to the corners of the key, and their slanted
letters (Heros Bold Italic); the words all 5 pixels high, as most of them.
The Timex TC2048 and TS2068 (one picture in two colourings, with five
words of its own under the keys: `timex.py`) are made of lines of whole
pixels: their frames, lines, keys and block graphics are drawn of their
pixels, and only the texts are drawn again (found on and around each key,
by a table of the legends of the keys): above and under the keys and on
the large keys 5 pixels high, on the keys 4 (as in the original), the
arrows as shapes, ~ and \ as thin lines. The TS1500 (and its ZXpand
picture, by `ts1500.py` too) keeps its keys, a light edge and a darker
rounded top, in even rows; its words all 6 pixels high, as most of them,
a little narrower on the tops where a block graphic is next to them.

## Making them

Needs Python 3 with numpy, scipy, scikit-image, Pillow and fontTools, the
TeX Gyre Heros fonts (TeX Live, or `fonts-texgyre`; another folder can be
given in `TEXGYRE`) and `rsvg-convert` (librsvg).

    python3 make.py [scale ...]

writes `<name>.svg` (2.5x, kept here) and `out/<name>_<scale>x.png` for the
scales given (2.5 if none), and `out/<name>.bmp`: the 2.5x picture in 256
colours (the colours of the surfaces exactly) for the forms. A picture goes
into its form with

    python3 dfmpicture.py ../../Keyboard_.dfm spec48kb out/spectrum48.bmp
    python3 dfmpicture.py ../../Keyboard_.dfm spec16kb out/spectrum16.bmp
    python3 dfmpicture.py ../../Keyboard_.dfm spec128kb out/spectrum128.bmp
    python3 dfmpicture.py ../../Keypad_.dfm KeypadUk out/keypad128.bmp
    python3 dfmpicture.py ../../Keyboard_.dfm zx81kb out/zx81.bmp
    python3 dfmpicture.py ../../Keyboard_.dfm zx81zxpandkb out/zx81zxpand.bmp
    python3 dfmpicture.py ../../Keyboard_.dfm ts1000kb out/ts1000.bmp
    python3 dfmpicture.py ../../Keyboard_.dfm ts1000zxpandkb out/ts1000zxpand.bmp
    python3 dfmpicture.py ../../Keyboard_.dfm zx97kb out/zx97.bmp
    python3 dfmpicture.py ../../Keyboard_.dfm zx80kb out/zx80.bmp
    python3 dfmpicture.py ../../Keyboard_.dfm zx80zxpandkb out/zx80zxpand.bmp
    python3 dfmpicture.py ../../Keyboard_.dfm specPlus2kb out/spectrumplus2.bmp
    python3 dfmpicture.py ../../Keyboard_.dfm specPlus3kb out/spectrumplus3.bmp
    python3 dfmpicture.py ../../Keyboard_.dfm tc2048kb out/tc2048.bmp
    python3 dfmpicture.py ../../Keyboard_.dfm ts2068kb out/ts2068.bmp
    python3 dfmpicture.py ../../Keyboard_.dfm ts1500kb out/ts1500.bmp
    python3 dfmpicture.py ../../Keyboard_.dfm ts1500zxpandkb out/ts1500zxpand.bmp

(then Build in C++Builder, as the forms changed). `analyze.py` needs to run
again only if an original changes:

    python3 analyze.py orig/spectrum48.png spectrum48 87,128,140 4
    python3 analyze.py orig/spectrum128.png spectrum128 32,32,32/64,64,64 1 0,0,0
    python3 analyze.py orig/keypad128uk.png keypad128 32,32,32/64,64,64 2 0,0,0
    python3 analyze.py orig/<name>.png <name> 252,252,252 2 0,0,0    (the ZX81 and its relatives)
    python3 analyze.py orig/<name>.png <name> 72,42,254 2 0,0,0 0,0,0/72,42,254/255,201,33/255,255,255    (the ZX80s)
    python3 analyze.py orig/spectrumplus2.png spectrumplus2 146,150,158/64,64,64 2 0,0,0
    python3 analyze.py orig/tc2048.png tc2048 166,166,164 2 0,0,0 small=8,8
    python3 analyze.py orig/ts2068.png ts2068 166,166,164 2 210,210,214 small=8,8
    python3 analyze.py orig/<name>.png <name> 211,209,209 2 123,123,123 123,123,123/211,209,209/145,143,143/255,255,255/0,0,0    (the TS1500s)
