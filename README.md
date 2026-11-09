# EightyOne – Hungarian Windows edition

**English** | [Magyar](README.hu.md)

This is a fork of [EightyOne](https://github.com/charlierobson/EightyOne)
(version 1.41), the Windows emulator of the Sinclair ZX80, ZX81 and ZX
Spectrum, their clones and the Jupiter Ace, by Michael D Wynne, Charlie
Robson and Paul Farrow. It adds a Hungarian user interface, typing with the
Windows keyboard layout, cleaner sound through WASAPI with a redesigned
Sound Settings window, 9 point text in every window, a 300% zoom, safer
loading of damaged files and several smaller fixes.

The emulation itself is the original EightyOne. The changes of this fork
are listed in [CHANGELOG-fork.md](CHANGELOG-fork.md), those of EightyOne
itself in `Release Files/Release history.txt`.

## Download

Download `EightyOne-1.41-hu.1-windows.zip` from the
[Releases](https://github.com/Viktor318/EightyOne/releases) page, unpack it
into a folder of your choice and start `EightyOne.exe`. Nothing has to be
installed. The `ROMs` and `ROM Cartridges` folders must stay next to the
program.

## What this edition adds

### Hungarian or English user interface

- **Options > Language**: *Automatic* (follows the language of Windows),
  *English* or *Magyar*. The language can be changed while running.
- Menus, windows and messages are translated; machine and product names
  and abbreviations (ROM, RAM, NMI, AY...) stay in English.
- Snapshots and the settings keep the English names of the hardware
  choices, so they work with either language and with the original
  EightyOne.

### Keyboard

- **Use the Windows keyboard layout** (Options > Keyboard; on by default
  with the Hungarian interface): what you type follows the characters of
  your Windows keyboard layout, not the positions of the keys, on every
  machine (ZX80, ZX81, Lambda, Spectrum, Jupiter Ace). On a Hungarian
  keyboard Shift+2 gives `"`; AltGr symbols (`# & @ [ ] { } < >`...) work
  where the machine has them, and the Spectrum's extended mode symbols
  (`[ ] { } \ | ~ ©`) are typed with the right key sequence. Accented
  letters are typed without the accent.
- **Use left Alt as Shift with number keys** (on by default): left Alt +
  number gives SHIFT + number (CAPS SHIFT on the Spectrum: EDIT, cursors,
  DELETE...).
- **Key repeat on ZX80/81** (off by default): a key held for 0.7 s repeats
  about 8 times a second, which the ZX80 and ZX81 ROMs don't do.

### Sound

- Sound through **WASAPI** at the sample rate of the Windows mixer, with
  lower latency (about 30 ms); DirectSound stays as a fallback and can be
  chosen. The sound follows the default device and survives unplugging it;
  EightyOne also runs without any sound device.
- The **Sound Settings** window is redesigned: a main volume, tall faders
  (0–100%) with level meters, muting of each AY channel and seven AY stereo
  modes (ABC, ACB, BAC, BCA, CAB, CBA, Mono).
- **Cleaner sound**: the beeper and the AY are band limited (no whistling
  or metallic tones beside high notes), the AY envelope has all 16 steps,
  digital drums and samples played through the AY volume register are
  heard, steady tones no longer waver and loud sounds no longer crackle.
  DirectSound output is continuous on Windows 10/11.

### Windows and display

- **9 point Segoe UI** text in every window; windows are wider where the
  English or Hungarian captions need it.
- **300% zoom** (View > Zoom, Ctrl+F3); with 100–400% chosen, resizing the
  main window jumps between these sizes. The main window appears at its
  saved size once it is ready, and its status bar stays at the bottom.
- **Help > Keyboard Map** can be enlarged in steps (1x to 2x) with a double
  click.
- **ZX Printer**: double click for double or triple size; the printout is
  saved as PNG (or BMP). **File > Save Screenshot** saves PNG too.

### Loading files

Damaged or deliberately malformed files (tapes, WAV, CSW, `.z80`, `.z81`,
`.ace`, Microdrive, hard disk images, ZIP...) can no longer crash or hang
the emulator or overwrite its memory; they load as far as they can, or a
message says that they could not be loaded. Saving an `.ace` snapshot no
longer damages the Jupiter Ace's ROM.

### Settings

The settings are kept where the original EightyOne keeps them:
`%APPDATA%\EightyOne\EightyOne.ini` (type `%APPDATA%\EightyOne` into the
address bar of Explorer). Settings of the original EightyOne are taken
over.

## Building from source

EightyOne is built with **Borland C++ Builder 5** (Update Pack 1), which
also runs on Windows 11. How to set up the development environment and the
components is described in
`Documentation/EightyOne Development Environment.docx`. Open
`Source/EightyOne.bpr` and use *Project > Build EightyOne*; `EightyOne.exe`
is made in the `Source` folder and runs from there.

The Hungarian texts are in `Translations/hu.tsv`;
`python3 Translations/make_table.py` makes `Source/LanguageHu.cpp` from it.

## Licence

EightyOne is free software under the GNU General Public License, version 2
or later (see `Release Files/gpl.txt`); so is this edition.

## Thanks

Michael D Wynne, Charlie Robson, Paul Farrow and everyone who made and
keeps developing [EightyOne](https://github.com/charlierobson/EightyOne).
