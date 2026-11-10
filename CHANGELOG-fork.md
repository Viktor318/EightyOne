# Changes of the Hungarian Windows edition

**English** | [Magyar](CHANGELOG-fork.hu.md)

Changes of this fork on top of EightyOne 1.41: the source of
[charlierobson/EightyOne](https://github.com/charlierobson/EightyOne) as
of 1 April 2025, a little after the 1.41 release. The changes of EightyOne
itself are in `Release Files/Release history.txt`.

## Not released yet

### Keyboard pictures

- The pictures of the **Spectrum 16K, 48K, 128, +2 and +3 keyboards, of
  the 128 keypad and of the ZX81 keyboard (also with the ZXpand, the
  TS1000, with and without the ZXpand, and the ZX97), of the ZX80 keyboard
  (also with the ZXpand), and of the Timex TC2048, TS2068 and TS1500 (also
  with the ZXpand) keyboards, and of the Jupiter Ace and Lambda 8300
  keyboards** are drawn again as vector graphics and held at 2.5x (1263 ×
  558, the keypad 375 × 558), so they are sharp: at 2.5x as they are, at
  the other sizes scaled down smoothly. Same layout and colours as before;
  the words of a picture are of one size and in the font's own spacing
  (TeX Gyre Heros), the 48K (and the 16K, the same picture with light grey
  keys) has its key rows a little further apart, and its words above the
  keys reach over them as little as they can. On the ZX81 the keys are in
  even rows, and the grey block graphics keep their dots, of one size at
  every scale; on the ZXpand pictures LPRINT and LLIST stay small under
  ZXPAND and CONFIG. On the ZX80 the words above the keys are in the
  middle of them, and the letters stay thin. The +2 and +3 keep their keys
  as in the original and their slanted letters. The Timex pictures keep
  their frames and keys pixel for pixel (sharp at every size), their texts
  are drawn again. The other keyboards keep their pictures.
- *Help > Keyboard Map* sizes: **1.5x, 1.75x, 2x, 2.25x and 2.5x** (they
  were 1x–2x); double click goes to the next, dragging the frame jumps
  between them. Sizes at which the window, with the keypad or the ZX80
  functions beside it, does not fit the screen are skipped. The keypad and
  the ZX80 functions are scaled smoothly too. The size is kept in
  `[KBMAP] Zoom` (in quarters); a size saved before is taken over (1x and
  1.25x as 1.5x).
- The keypad and the ZX80 functions are 6 pixels from the keyboard window
  as seen (they were 20 pixels away, and on Windows 10 and 11 about 30, as
  the invisible borders of the windows were counted in).
- The drawings and the scripts that make them are in `Source/KBPics/svg`
  (see its README).

### Spectrum 128 keypad

- With the keypad connected (*Control > Connect Spectrum 128 Keypad*), the
  editing keys of the PC give the editing functions of the 128 BASIC
  editor that only the keypad has, as in the Fuse edition: Home / End:
  start / end of the line; Ctrl + Home / End: first / last line of the
  program; Ctrl + Left / Right: previous / next word; Page Up / Page Down:
  10 lines up / down; Delete: the character under the cursor; Shift +
  Backspace / Delete: delete to the start / end of the line; Ctrl +
  Backspace / Delete: delete to the start / end of the word; Insert: the
  Options menu (CMND); Ctrl + Insert: TOGGLE (between the upper and lower
  screen). The keys not listed (Backspace, the cursor keys) work as
  before, and the numeric keypad keeps giving digits. The right Ctrl with
  the keys at the places of the keypad's keys works too, as before.
- The keypad waits longer between the bits it sends (the ROM may be
  interrupted there) and starts with its line idle, as in Fuse; the
  deleting keys send the keypad SHIFT first, so a press in the middle of a
  scan can't move the cursor instead of deleting.
- The menu item is greyed out on the +2A and the +3, whose ROM does not
  poll the keypad (only the 128 and the +2 have the KEYPAD socket).
- With the keypad connected, keys released while the right Shift is held
  are released on the emulated keyboard too; they stayed pressed.

### Recent files

- **Recent files** (*File > Recent Files*), as in the Fuse edition: the
  last 10 files opened, newest first, numbered 1–9, 10, long paths
  shortened in the middle; *Clear List* under them. The files opened with
  *File > Open Tape* and *Load Snapshot*, *Control > RZX > Play RZX File*,
  in the Drives window, the Tape Manager and the Wave Loader, by dragging
  onto the window or on the command line are listed (for a file from a
  `.zip`, the `.zip`). They are opened again as with their menu items: a tape or a
  `.wav` as with *Open Tape*, a snapshot as with *Load Snapshot*, a disk
  image into drive A, a microdrive cartridge into the first microdrive, a
  hard disk image into the first hard disk. A file that is not there any
  more is taken off the list with a message. The list is kept in the
  settings file (`[RECENTFILES]`), written at each change.

### Fixes

- A floppy disk image dragged onto the window goes into drive A; the file
  dialog of drive A came up instead.
- Loading a configuration (Options > Configuration) sets the window to
  the size saved with it (its zoom); it always went back to 100%.

## v1.41-hu.2 – 2026-11-10

The development version of the next EightyOne (1.42, by John Stroebel and
Paul Farrow) is merged: the `develop` branch of
[stroebeljc/EightyOne](https://github.com/stroebeljc/EightyOne) as of
4 September 2026. Its changes are in `Release Files/Release history.txt`
(among them: the AY on port $BFFD/$FFFD as the 128K decodes it, a more
exact Z80, emulation on its own thread, the Speed window from 0x to 8x,
RZX playback, Beta Disk, Larken and ZX1541 disks, BASIC listings of the
Lambda 8300, the Timex machines and the Spectrum 128, Memotech and
Quicksilva hi-res, Lambda colour, joystick POV hats, ZX Printer momentum).

### Merging

- The emulation runs on its own thread, and the **sound is still its
  clock**: the WASAPI and DirectSound outputs signal it for each frame of
  sound they need (they used to send the main window a message), and wait
  for its sound at most 100 ms, playing silence after that. So the sound
  and the emulation **go on while a menu or a dialog is open**.
- The settings are read once all the windows exist (as in 1.42); the main
  window is still shown only once it has its size.
- The new texts are in Hungarian (ZX1541 disks, RZX playback, the Lambda
  colour menu item, the BASIC listing format information), the new and
  changed windows in Segoe UI 9 pt.

### Fixes to the development version

- Files named on the command line, and a settings file named there, were
  sometimes ignored: the command line was freed right after it was split.
- Loading a snapshot (from the menu, the command line, or dropped on the
  window) stops the emulation meanwhile: frames run during loading could
  end in an "Unknown Internal Error" and a hard reset.
- A failed RZX playback no longer leaves the emulation stopped.
- While the main window waits for the emulation thread, it handles the
  messages that thread sends it (status bar, ZX Printer), so the two
  cannot wait for each other.
- The **ZX Printer** draws a line at a time: a message for every dot
  slowed the emulation to about 43 frames/s while printing. Its scroll
  bar works at 2x and 3x size too.

## v1.41-hu.1 – 2026-11-09

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

### Loading files

Damaged or deliberately malformed files can no longer crash or hang the
emulator, or overwrite its memory: every length, count, address and page
number read from a file is checked against the file and against where the
data goes. A damaged file loads as far as it can, or a message says it
could not be loaded.

- **Tapes** (TZX, TAP, T81, P, O, CSW blocks): block lengths are checked
  against what is left of the file (the old code could read far past it),
  symbol tables and pulse sequences of generalised data blocks too; at
  most as many blocks are loaded as the tape can hold; jumps and loops
  pointing outside the tape are ignored, and a tape that keeps changing
  the signal without any time passing is stopped.
- **WAV and CSW files** with a broken header (no channels, a sample rate of
  0) no longer stop the emulator with a "Division by zero" error, and
  chunks running past the end of the file are not read; the message
  "The file could not be loaded." is shown instead. CSW files must have
  the CSW signature and version 1.01.
- **.z80 snapshots**: compressed runs reaching beyond a block or beyond the
  file, and pages of memory the machine does not have, no longer write
  outside the emulated memory (such pages are skipped; what a file cut
  short leaves out is zero). The Spectra's pages are read correctly when
  stored uncompressed too.
- **.z81 snapshots**: counts and addresses in the text stay within the
  memory they belong to (memory, the Chroma's colour memory, the
  Quicksilva character generator); a file of very many sections no longer
  overflows the stack. The Quicksilva character generator was saved one
  byte too long (a byte read from beyond it).
- **.ace snapshots**: runs stay within memory, and a file ending in the
  middle of a run no longer runs away. B' is saved and loaded (B was
  stored in its place). Saving an .ace snapshot no longer writes part of
  the snapshot header into the Jupiter Ace's ROM (at 0284h-029Fh, 052Ch
  and 07C8h), which could make the Ace misbehave afterwards.
- **Microdrive**: loading an .MDV image always wrote beyond the memory set
  aside for it; a file too short for one sector now gives a blank
  cartridge. Saving a cartridge on which no sector can be found no longer
  hangs, and sectors running round the end of the tape are saved right.
- **Hard disks**: an .HDF file whose data seems to start inside its header
  no longer overwrites memory. VHD images with a byte of 128 or more in
  their geometry (large VHDs have 255 sectors per track) got a wrong,
  negative geometry. In the drives window, changing
  the heads or sectors of the second hard disk used the first one's, and
  a 0 there no longer divides by zero.
- **Larken disks**: a track number above 79 no longer reads or writes
  outside the disk, and the disk file is no longer left open.
- **ZIP files**: a member with a name of 256 characters or more no longer
  gives a garbled name; a member whose name has `:` or another character
  Windows does not allow in a file name is extracted under a valid name;
  if a member cannot be extracted (it cannot be written, or the archive is
  broken), nothing is loaded instead of a missing or broken file.
- **File > Load Memory Block**: a Timex .SCR screen (12288 or 12289 bytes)
  puts its second half at the right place (8K after the first, it went
  6K further) and switches to hi-colour or hi-res with its colour (the
  last byte was read from one byte beyond it). Nothing is loaded past
  the top of memory, and the file is closed.
- A Timex dock cartridge (.DCK) that cannot be read is no longer left open.
- The paths of ROMs, fonts and non-volatile memory files can be of any
  length (a deep EightyOne folder overflowed a buffer of 256 characters).

### Windows

- The windows use the **Segoe UI** font of Windows Vista and later instead
  of the old MS Sans Serif, at the same size, so the layout stays as it
  was. The four windows that would have been enlarged by the change of
  font (keyboard picture, keypad, keyboard functions, edit flags) keep
  their size.
- **Every window uses 9 point text** (it was 8 point), as the Sound
  Settings window does. The windows are laid out larger to match, and
  where a caption still did not fit, in English or in Hungarian, the
  window is wider: among others the debugger, TV Emulation, Keyboard,
  Profiler, CPU History, Drives, ZX Interface 1 RS232, Printer Port,
  TS2050, ZX97, the BASIC listing options, the breakpoint and the HDF
  windows. Captions that were cut off (the debugger's buttons and check
  boxes, "Függőleges szinkron", "HDF képfájl mérete:", the Hungarian
  "Karaktergenerátor:" and the longer check boxes of the Hardware window's
  *Advanced Settings*, and others) are shown whole. The windows sized by a
  picture or by the program (keyboard picture, keypad, keyboard
  functions) keep their size; the rows of the tape list are taller.
- The **About** window shows the version of this edition (1.41-hu.1);
  the file version of `EightyOne.exe` is 1.41.0.1.
- The **BASIC Listing** window is at least as wide as its tool bar, so all
  of its buttons are shown. The paragraphs of *Saving a BASIC Listing as
  Text* are put one under the other, so they no longer run into each
  other, and the window is as tall as they need.

### Main window size

- New zoom: **300%** (*View > Zoom*, Ctrl+F3), between 200% and 400%.
- With 100%, 200%, 300% or 400% chosen, the window can be resized too, but
  it jumps between these sizes (the zoom ticked follows). *User Defined*
  still allows any size.
- At start the main window appears only once it is ready: at the size it
  was left at, in its language, with the machine chosen. It used to appear
  during the splash screen at its design size with English menus, and
  jump to its size a second after the machine had started. Files named on
  the command line are loaded straight away (they waited two seconds).
- The status bar stays at the bottom of the window. At 100% the menu bar
  takes two lines (always with the Hungarian menus, on Windows 11 with the
  English ones too), and as the window changed size the status bar could
  be left behind: at start higher up, under the picture (so it looked
  empty), or after switching to 300% or 400% below the window, until the
  zoom was changed again.

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
- When the language is changed, the texts the program puts together also
  change at once (they used to wait for their next update): the tape
  list with its header and the Tape Manager's title, the total RAM in the
  Hardware window, the Wave Loader's title and status, the ZX Printer's
  speeds, the BASIC Lister's status bar, the profiler's plot and the
  game controller menus.
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

### BASIC Listing

- The *Output line numbers above 9999 using 5 digits* setting is kept
  after a restart: it was saved under a misspelt name, so it was always
  read back as off.
