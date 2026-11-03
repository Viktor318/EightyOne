# Changes in this fork

[Magyarul](CHANGELOG-fork.hu.md)

The changes of the `hungarian-windows` branch, compared with
[charlierobson/EightyOne](https://github.com/charlierobson/EightyOne).

## Unreleased

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
  - The left Alt and a number key give SHIFT + that number (EDIT, AND, THEN,
    cursors, GRAPHICS, RUBOUT...).
  - Keys without a character (cursors, Enter, Backspace, Ctrl), the numeric
    keypad and the joystick keys work as before.
  - The Ctrl that Windows sends with AltGr is no longer passed on.
