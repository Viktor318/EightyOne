# Changes in this fork

[Magyarul](CHANGELOG-fork.hu.md)

The changes of the `hungarian-windows` branch, compared with
[charlierobson/EightyOne](https://github.com/charlierobson/EightyOne).

## Unreleased

### Sound

- The AY's **envelope** works as the chip's, with 16 steps. It had 15, so
  a decaying envelope did not fall silent but stayed at the lowest level
  (shapes 2 and 3 even near the top), and the attack-and-hold shapes held
  a step too low. Fast envelopes (envelope "buzzer" basses with a small
  period) have all 16 steps too, not only as many as the period; period
  0 counts as 1.
- With **neither tone nor noise** on a channel, the AY gives out the
  channel's level, as the chip does, so **digital drums and samples**
  played through the volume register are heard (they were silent).
- The AY's output has no DC, as the speaker is coupled through a
  capacitor: a channel switched off in the mixer with its volume left up
  takes no headroom.
- The **AY is band limited** as well: it is run in continuous time, and
  every change of its output (tone edges, noise, envelope steps, register
  writes, each at the fraction of a sample where it happens) is a band
  limited step. High notes and envelope "buzzer" basses no longer bring
  metallic, inharmonic tones with them: at 4 kHz these were at -20 dB
  (-13 dB all together), now below -100 dB. How the AY sounds is otherwise
  the same: the levels, the stereo modes, the envelopes and sample playing
  through the volume registers.
- The **beeper is band limited**: each edge is a step filtered to below
  half the sample rate (BLEP), so high notes no longer bring whistling,
  inharmonic tones with them (they were at -22 to -43 dB, now below
  -75 dB). The fade after each click is smooth instead of 31 steps, and
  the high harmonics are no longer dulled, so the beeper sounds a little
  brighter and cleaner. It is about 0.35 ms later than before.
- Sound is played through **WASAPI** (Windows Vista and later), at the
  sample rate of the Windows mixer (usually 48000 Hz), so it is not
  resampled. About 30 ms of sound is kept waiting, so it is heard sooner
  than with DirectSound (about 70 ms). If WASAPI cannot be used (Windows
  XP), DirectSound plays the sound as before.
- If the sound device is unplugged or stops working, the emulator keeps
  running at the right speed, and the sound comes back when a device can
  be opened again (tried every second). When another device becomes the
  default in Windows, the sound moves to it.
- Without any sound device the emulator still runs (it used to show an
  empty "Sound Error" message and not run).
- The output can be chosen in the Sound Settings window: *Automatic*
  (WASAPI, or if it cannot be used, DirectSound), *WASAPI* or
  *DirectSound*. It changes at once, without resetting the sound chips;
  it is kept in the settings (and in saved configurations). Beside it is
  what is playing the sound, and at what rate.
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
- Steady tones no longer waver. A frame is run until the end of a
  scanline, so it is up to a scanline longer or shorter than a nominal
  frame, but its sound was placed as if it began exactly at the start of
  its buffer: the edges of the beeper (and the AY and SpecDrum changes)
  moved back and forth by up to about 3 samples. Now each frame starts
  where the previous one ended, and what falls after the end of a buffer
  goes into the next one.
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

### Windows

- The windows use the **Segoe UI** font of Windows Vista and later instead
  of the old MS Sans Serif, at the same size, so the layout stays as it
  was. The four windows that would have been enlarged by the change of
  font (keyboard picture, keypad, keyboard functions, edit flags) keep
  their size.

### Main window size

- New zoom: **300%** (*View > Zoom*, Ctrl+F3), between 200% and 400%.
- With 100%, 200%, 300% or 400% chosen, the window can be resized too, but
  it jumps between these sizes (the zoom ticked follows). *User Defined*
  still allows any size.

### ZX Printer

- Double click the paper to see it at **double or triple size** (every
  dot 2x2 or 3x3 pixels) or normal again; the buttons and the options
  follow the width of the window, and the size is remembered.
- The printout is saved as **PNG** (or BMP, still to be chosen), at the
  size shown: 1x, 2x or 3x.

### Screenshots

- *File > Save Screenshot* saves **PNG** too, as the first choice (BMP
  and, on the Spectrum, SCR stay).

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

- **Key repeat on ZX80/81** (Options > Keyboard, off by default, shown
  for the ZX80, ZX81 and its clones): a key held longer than 0.7 s
  repeats about 8 times a second, which the ZX80 and ZX81 ROMs do not do.
  The key is shown to the machine as let go for 4 frames and pressed for
  2, so the ROM takes each as a new key press; SHIFT stays held, so
  shifted keys repeat too. It works with and without the Windows layout.
- **Use the Windows keyboard layout** (Options > Keyboard; on by default
  with the Hungarian interface, off with the English one, where the keys
  go as in the original EightyOne):
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
