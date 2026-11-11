# Keyboard pictures drawn again

The keyboard pictures of EightyOne (*Help > Keyboard Map*) were drawn in
pixels at 505 × 223, and look blurred when the window is enlarged. The
pictures here are the same pictures drawn again as vector graphics, so they
are sharp at any size:

| Drawing | Shown for (the TImage) | Original |
|---|---|---|
| `spectrum48.py` | `Keyboard_.dfm`, `spec48kb` | `orig/spectrum48.png` |
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
| `ace.py` | `Keyboard_.dfm`, `acekb` | `orig/ace.png` |
| `lambda8300.py` | `Keyboard_.dfm`, `lambdakb` | `orig/lambda8300.png` |
| `tk85.py` | `Keyboard_.dfm`, `tk85kb` | `orig/tk85.png` |
| `r470.py` | `Keyboard_.dfm`, `r470kb` | `orig/r470.png` |

They are drawn at 100% of the window (1.75 times the originals: 884 × 390,
the keypad 263 × 390), and the program holds them at 200%, its largest
size (1768 × 780, the keypad 526 × 780), as PNG pictures in 256 colours
(the colours of the surfaces exactly) in its resources, named after the
TImages they are shown for (`../png/<name>.png`, put into the program by
`../../KBPics1.rc`–`KBPics3.rc`: three files, as the linker of C++Builder
5 fails on a resource file of 1 MB or more). At 200% the program shows
the picture as it is; at the other sizes it makes it smaller itself, each
pixel the average of the part of the picture it covers (at 100% of four
pixels, so it is as sharp as drawn), the same on every Windows.

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

The edges of the keys and squares fall on whole pixels at 100%, and so
on every second pixel at 200%: sharp at both. The 48K picture has its key rows a
little further apart than the original (2 pixels each, from the free
space under the last row). Its words and signs are placed as on the
keyboard of the Spectrum (Viktor's choice, after a photo of it): the
keywords at the bottom right of the keys 3 pixels from the bottom and the
right edge, the red words and signs at the top right 3 pixels from the
right edge and on the line of the tops of the big letters (the signs of
their own sizes; the red signs of the number keys under their block
graphics, in Heros Bold of one size, the capitals 9 pixels high as %
and &, on one base line 4 pixels above the bottom of the keys), the words
and signs above and under the keys 1 pixel from the left edge of their
keys; the bars of − + = _ | all a pixel thick; the punctuation ; : , .
of one size, in Heros Bold with dots 2.5 pixels wide (; a full stop and
a comma halfway between it and the top of the keyword under it), " in
Heros Cn Bold 5 pixels high, * on B the star of Heros Bold 8 pixels
high, / on V and \ under D lines at 45 degrees, 7 pixels high, ~ under A
the line in the middle of the letter it was, all three as thick as the /
of the font, _ on 0 one line as − and =, the tops of all the signs at the
top right on the line of the big letters; ENTER in the middle of its key,
CAPS SHIFT in Heros Bold (wider letters, as on the Spectrum).
The words above the keys reach over their keys' right edges as little as
they can: CAPS LOCK and TRUE VIDEO with
half a space between their two words and 1 pixel over (85% and 82% of
their width), INV.VIDEO and GRAPHICS at 93.5%, MAGENTA at 96%. The
16K shows the 48K picture (Viktor's choice; its own was the 48K's with
light grey keys, pixel for pixel). On the ZX81 the white words, 6 pixels high in
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
a little narrower on the tops where a block graphic is next to them. The
Jupiter Ace has its words 5 pixels high in Heros Bold (wide, as in the
original), those under the top row in the middle of their keys, SPACE
larger; its letters bold and its signs thin, as there. The Lambda 8300 has
the keys of the TS1500 (`ts1500.keys()`) in its own colours: its words 6
pixels high, ENTER (7.5) and SPACE (10) larger and, with SHIFT, in the
middle of their keys; the little figures on 1, 2, 3 and A of their pixels,
the half squares on S - G with straight diagonals. The TK85 is the ZX81's
picture coloured otherwise (dark keys, white letters, yellow and blue), a
few of its letters a little other: `zx81.py` draws it in its colours,
finding the ZX81's texts by their shapes (in any colour) and those that
differ by a table; NEW LINE (8) and SPACE (9) larger, NEW LINE in the
middle of its key. The Ringo R470 keeps its grey keys on light grey, the
blue L-shaped FUNC and ENTER (by their corners, rounded) and the cyan
GRAPHICS key, its keys in even rows; its words all 6 pixels high (black
above the keys, blue under them, yellow on them), as close to the block
graphics after them as in the original (`word(..., gap=2)`), the words of
the large keys as in the original; its stars of three thin lines, as
there. `analyze.py` finds its keys with `apart`, as its blue words touch
its grey keys.

## Making them

Needs Python 3 with numpy, scipy, scikit-image, Pillow and fontTools, the
TeX Gyre Heros fonts (TeX Live, or `fonts-texgyre`; another folder can be
given in `TEXGYRE`) and `rsvg-convert` (librsvg).

    python3 make.py [percent ...]

writes `<name>.svg` (the drawing at 100%, kept here), `../png/<name>.png`
(the 200% picture for the program) and `../../KBPics1.rc`–`KBPics3.rc`,
and for the sizes given (in percent) `out/<name>_<percent>.png`: the
picture as the program shows it (made smaller as there). Then Build in
C++Builder (the resources changed). A new picture goes into the table of
`make.py` (`PICTURES`), and its old picture out of its form, with

    python3 dfmclear.py ../../Keyboard_.dfm <TImage>

`analyze.py` needs to run again only if an original changes:

    python3 analyze.py orig/spectrum48.png spectrum48 87,128,140 4
    python3 analyze.py orig/spectrum128.png spectrum128 32,32,32/64,64,64 1 0,0,0
    python3 analyze.py orig/keypad128uk.png keypad128 32,32,32/64,64,64 2 0,0,0
    python3 analyze.py orig/<name>.png <name> 252,252,252 2 0,0,0    (the ZX81 and its relatives)
    python3 analyze.py orig/<name>.png <name> 72,42,254 2 0,0,0 0,0,0/72,42,254/255,201,33/255,255,255    (the ZX80s)
    python3 analyze.py orig/spectrumplus2.png spectrumplus2 146,150,158/64,64,64 2 0,0,0
    python3 analyze.py orig/tc2048.png tc2048 166,166,164 2 0,0,0 small=8,8
    python3 analyze.py orig/ts2068.png ts2068 166,166,164 2 210,210,214 small=8,8
    python3 analyze.py orig/<name>.png <name> 211,209,209 2 123,123,123 123,123,123/211,209,209/145,143,143/255,255,255/0,0,0    (the TS1500s)
    python3 analyze.py orig/ace.png ace 30,23,9 2 255,255,255
    python3 analyze.py orig/lambda8300.png lambda8300 149,179,174 2 102,102,102 102,102,102/149,179,174/104,150,142/255,255,255/242,207,65/0,0,0
    python3 analyze.py orig/tk85.png tk85 68,68,68 2 0,0,0 0,0,0/68,68,68/255,255,255/236,252,234/57,114,197/239,252,0
    python3 analyze.py orig/r470.png r470 127,127,127/57,114,197/139,246,253 2 207,207,207 207,207,207/127,127,127/57,114,197/240,240,240/53,53,53/255,255,0/139,246,253 apart
