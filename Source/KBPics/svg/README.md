# Keyboard pictures drawn again

The keyboard pictures of EightyOne (*Help > Keyboard Map*) were drawn in
pixels at 505 × 223, and look blurred when the window is enlarged. The
pictures here are the same pictures drawn again as vector graphics, so they
are sharp at any size:

| Drawing | Picture in the program | Original |
|---|---|---|
| `spectrum48.py` | `Keyboard_.dfm`, `spec48kb` | `orig/spectrum48.png` |
| `spectrum128.py` | `Keyboard_.dfm`, `spec128kb` | `orig/spectrum128.png` |
| `keypad128.py` | `Keypad_.dfm`, `KeypadUk` | `orig/keypad128uk.png` |
| `zx81.py` | `Keyboard_.dfm`, `zx81kb` | `orig/zx81.png` |
| `zx81zxpand.py` | `Keyboard_.dfm`, `zx81zxpandkb` | `orig/zx81zxpand.png` |
| `ts1000.py` | `Keyboard_.dfm`, `ts1000kb` | `orig/ts1000.png` |
| `ts1000zxpand.py` | `Keyboard_.dfm`, `ts1000zxpandkb` | `orig/ts1000zxpand.png` |
| `zx97.py` | `Keyboard_.dfm`, `zx97kb` | `orig/zx97.png` |

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
space under the last row). On the ZX81 the white words, 6 pixels high in
the original, are 7 like the others: those above a key row grow upwards,
those under one downwards, so they stay 2 pixels from the keys. The
relatives of the ZX81 (ZXpand, TS1000, ZX97) are drawn by `zx81.py` too:
the texts they share with the ZX81 are found by their pixels, and each
script gives only its own texts. On the ZXpand pictures LPRINT and LLIST
stay small under ZXPAND and CONFIG (5 pixels high, they were 4).

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
    python3 dfmpicture.py ../../Keyboard_.dfm spec128kb out/spectrum128.bmp
    python3 dfmpicture.py ../../Keypad_.dfm KeypadUk out/keypad128.bmp
    python3 dfmpicture.py ../../Keyboard_.dfm zx81kb out/zx81.bmp
    python3 dfmpicture.py ../../Keyboard_.dfm zx81zxpandkb out/zx81zxpand.bmp
    python3 dfmpicture.py ../../Keyboard_.dfm ts1000kb out/ts1000.bmp
    python3 dfmpicture.py ../../Keyboard_.dfm ts1000zxpandkb out/ts1000zxpand.bmp
    python3 dfmpicture.py ../../Keyboard_.dfm zx97kb out/zx97.bmp

(then Build in C++Builder, as the forms changed). `analyze.py` needs to run
again only if an original changes:

    python3 analyze.py orig/spectrum48.png spectrum48 87,128,140 4
    python3 analyze.py orig/spectrum128.png spectrum128 32,32,32/64,64,64 1 0,0,0
    python3 analyze.py orig/keypad128uk.png keypad128 32,32,32/64,64,64 2 0,0,0
    python3 analyze.py orig/<name>.png <name> 252,252,252 2 0,0,0    (the ZX81 and its relatives)
