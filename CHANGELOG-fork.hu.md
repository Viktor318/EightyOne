# A fork változásai

[English](CHANGELOG-fork.md)

A `hungarian-windows` ág változásai a
[charlierobson/EightyOne](https://github.com/charlierobson/EightyOne)
eredetihez képest.

## Kiadatlan

### Billentyűzetkép (Help > Keyboard Map)

- Az ablak átméretezhető; a kép megtartja az arányait, és simán
  (halftone) skálázódik, egészen a negyed méretig. A méretét megjegyzi. A
  dupla kattintás továbbra is a normál és a dupla méret között vált (a ZX80
  funkcióival és a Spectrum 128 numerikus billentyűzetével együtt).

### Magyar felület

- A menük, az ablakok és az üzenetek **magyarul** vagy angolul jelennek
  meg: *Options > Language* (Automatikus, English, Magyar; az Automatikus a
  Windows nyelvét követi). A nyelv futás közben is átállítható.
- Az ablakleírók (.dfm) változatlanok: a feliratokat futás közben cseréli a
  `Translations/hu.tsv` alapján (ebből a `Translations/make_table.py`
  készíti a `Source/LanguageHu.cpp`-t), így az angol változat ugyanaz marad.
- A pillanatképek és a beállításfájl a hardverválasztások angol szövegét
  tárolják, így bármelyik nyelvvel és az eredeti EightyOne-nal is működnek.
- A gép- és terméknevek és a rövidítések (ROM, RAM, NMI, AY...) angolul
  maradnak.

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
  - A Spectrum kiterjesztett módú jeleit (`[ ] { } \ | ~` és `©`) időzített
    billentyűsorozattal üti le (E mód, majd SYMBOL SHIFT + billentyű), a 48K
    és a 128K szerkesztőjében egyaránt; a közben leütött billentyűk
    kivárnak, így a gépelés sorrendje megmarad.
  - A `×` és a `÷` (AltGr a magyar kiosztáson) `*`-ként és `/`-ként megy át.
  - Az ékezetes betűk ékezet nélkül mennek át.
  - A karakter nélküli billentyűk (kurzorok, Enter, Backspace, Ctrl), a
    numerikus billentyűzet és a joystick billentyűi a régi módon működnek.
  - Az AltGr-rel együtt küldött Ctrl-t nem adja tovább a gépnek.
- **A bal Alt SHIFT-ként a számbillentyűkkel** (Options > Keyboard, alapból
  bekapcsolva): a bal Alt és egy számbillentyű SHIFT + azt a számot adja (a
  Spectrumon CAPS SHIFT-et: EDIT, AND, THEN, kurzorok, GRAPHICS, RUBOUT,
  DELETE...), amit a Windows-kiosztással a Shift+szám nem tud. A többi
  billentyűvel az Alt továbbra is a Windowsé (menük).
