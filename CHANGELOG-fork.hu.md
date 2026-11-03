# A fork változásai

[English](CHANGELOG-fork.md)

A `hungarian-windows` ág változásai a
[charlierobson/EightyOne](https://github.com/charlierobson/EightyOne)
eredetihez képest.

## Kiadatlan

### Billentyűzet

- **A Windows billentyűzetkiosztásának használata** (Options > Keyboard,
  alapból bekapcsolva): a gépelés azt a karaktert követi, amit a Windows
  kiosztása ad, nem a billentyűk helyét, így bármilyen kiosztás működik (a
  magyaron a Shift+2 `"`, ami a ZX81-en SHIFT+P). Minden gépen: ZX80, ZX81,
  Lambda, Spectrum és Jupiter Ace.
  - A betűk billentyűk maradnak: a Shift+betű a gép SHIFT+betűje
    (kulcsszavak és jelek a ZX80-on, ZX81-en és Lambdán, nagybetűk a
    Spectrumon és az Ace-en).
  - A jeleket az emulált gép billentyűkombinációjával üti le, az AltGr-eseket
    is (`# & @ [ ] { } < >`...), amelyik a gépen megvan; amihez a gépnek
    nincs billentyűje, azt figyelmen kívül hagyja.
  - Az ékezetes betűk ékezet nélkül mennek át.
  - A bal Alt és egy számbillentyű SHIFT + azt a számot adja (EDIT, AND,
    THEN, kurzorok, GRAPHICS, RUBOUT...).
  - A karakter nélküli billentyűk (kurzorok, Enter, Backspace, Ctrl), a
    numerikus billentyűzet és a joystick billentyűi a régi módon működnek.
  - Az AltGr-rel együtt küldött Ctrl-t nem adja tovább a gépnek.
