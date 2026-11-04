# Changes in this fork

[Magyarul](CHANGELOG-fork.hu.md)

The changes of the `hungarian-windows` branch, compared with
[charlierobson/EightyOne](https://github.com/charlierobson/EightyOne).

## Unreleased

### Sound

- The **Sound Settings** window is redesigned: a **main volume** on the
  left, faders twice as tall (0-100%) with a **level meter** beside each
  (after its volume), the percentage under them and above them while
  dragging. Double click a fader for its default (main volume 100%, the
  others 50%); the mouse wheel and the arrow keys move it by 1%, with
  Shift by 10%. *Reset* sets every volume back, unmuted.
- Each AY channel can be muted on its own, and there are **seven AY
  stereo modes**: ABC, ACB, BAC, BCA, CAB, CBA and Mono (was ACB and
  Mono). The AY faders are in the order of the stereo mode: left, centre,
  right.
- The sources are mixed with headroom and the result is clipped after the
  main volume, so loud sounds together no longer wrap round and crackle.
- The old settings are taken over (the 0-31 sliders as percentages).
- The DirectSound output is rewritten, so the sound is continuous on
  Windows 10/11. It filled the half of its short buffer that was already
  playing, so part of each frame was late and old sound was heard
  instead; and the queue could be written past its end.
- The buffer is kept filled about 40 ms beyond DirectSound's write cursor
  (the emulation still follows the sound card's clock). After an underrun
  it continues a little after the write cursor, and if the program is held
  up (a menu, a dialog), silence is heard, not old sound again.
- Changing the sound settings no longer leaks DirectSound objects, and two
  EightyOnes running at once no longer disturb each other's sound.

### Main window size

- New zoom: **300%** (*View > Zoom*, Ctrl+F3), between 200% and 400%.
- With 100%, 200%, 300% or 400% chosen, the window can be resized too, but
  it jumps between these sizes (the zoom ticked follows). *User Defined*
  still allows any size.

### Keyboard picture (Help > Keyboard Map)

- Double click changes the size of the picture in steps: 1x, 1.25x, 1.5x,
  1.75x, 2x and again 1x (it was 1x and 2x), together with the ZX80
  functions and the Spectrum 128 keypad beside it. The picture is scaled
  smoothly (halftone), and its size is remembered. The window can be
  resized too, but it jumps between these sizes.

### Hungarian user interface

- The menus, windows and messages are in **Hungarian** or English:
  *Options > Language* (Automatic, English, Magyar; Automatic follows the
  language of Windows). The language can be changed while running.
- The forms are not changed: their texts are replaced while running from
  `Translations/hu.tsv` (made into `Source/LanguageHu.cpp` by
  `Translations/make_table.py`), so the English version stays the same.
- Snapshots and the settings file keep the English texts of the hardware
  choices, so they work with either language and with the original
  EightyOne.
- Machine and product names and abbreviations (ROM, RAM, NMI, AY...) stay
  in English.

### Keyboard

- **Use the Windows keyboard layout** (Options > Keyboard, on by default):
  typing follows the characters the Windows keyboard layout gives, not the
  positions of the keys, so any layout works (on the Hungarian layout,
  Shift+2 gives `"`, which is SHIFT+P on the ZX81). On every machine: ZX80,
  ZX81, Lambda, Spectrum and Jupiter Ace.
  - Letters stay keys: Shift+letter is the machine's SHIFT+letter (keywords
    and symbols on the ZX80, ZX81 and Lambda, capitals on the Spectrum and
    the Ace).
  - Symbols are typed with the key combination of the emulated machine,
    also the AltGr ones (`# & @ [ ] { } < >`...), where the machine has
    them; characters it has no key for are ignored.
  - The Spectrum symbols of the extended mode (`[ ] { } \ | ~` and `©`)
    are typed with a timed key sequence (E mode, then SYMBOL SHIFT + key),
    in the 48K and the 128K editors alike; keys typed meanwhile wait, so
    the order of typing is kept.
  - `×` and `÷` (AltGr on the Hungarian layout) are typed as `*` and `/`.
  - Accented letters are typed without the accent.
  - Keys without a character (cursors, Enter, Backspace, Ctrl), the numeric
    keypad and the joystick keys work as before.
  - The Ctrl that Windows sends with AltGr is no longer passed on.
- **Use left Alt as Shift with number keys** (Options > Keyboard, on by
  default): the left Alt and a number key give SHIFT + that number (CAPS
  SHIFT on the Spectrum: EDIT, AND, THEN, cursors, GRAPHICS, RUBOUT,
  DELETE...), which Shift+number can't give with the Windows layout. Other
  keys with Alt still go to Windows (menus).
