# A fork változásai

[English](CHANGELOG-fork.md)

A `hungarian-windows` ág változásai a
[charlierobson/EightyOne](https://github.com/charlierobson/EightyOne)
eredetihez képest.

## Kiadatlan

### Hang

- Megújult a **Hangbeállítások** ablak: bal oldalon **fő hangerő**,
  kétszer olyan magas csúszkák (0–100%), mindegyik mellett
  **szintjelzővel** (a hangerő után mér), alattuk a százalék, húzás közben
  fölöttük is. Dupla kattintásra a csúszka az alapértékre áll (fő hangerő
  100%, a többi 50%); az egérgörgő és a nyilak 1%-kal, Shifttel 10%-kal
  állítják. Az *Alaphelyzet* minden hangerőt visszaállít, és a némítást
  is kikapcsolja.
- Az AY csatornái külön is némíthatók, és **hét AY sztereó mód** van:
  ABC, ACB, BAC, BCA, CAB, CBA és Mono (eddig csak ACB és Mono). Az AY
  csúszkái a sztereó mód sorrendjében állnak: bal, közép, jobb.
- A források keverése tartalékkal történik, és a jel a fő hangerő után
  van levágva, így az együtt szóló hangos hangok nem fordulnak át, nem
  recsegnek.
- A régi beállítások megmaradnak (a 0–31-es csúszkák százalékra
  átszámolva).
- Újraírt DirectSound-kimenet: Windows 10/11-en is folyamatos a hang.
  Eddig a rövid puffer éppen szóló felét töltötte, így minden képkocka
  egy része elkésett, és helyette régi hang szólt; a sorba pedig a végén
  túl is írhatott.
- A puffer kb. 40 ms-mal a DirectSound írókurzora előtt jár (az emuláció
  továbbra is a hangkártya órájához igazodik). Kiürülés után kicsivel az
  írókurzor után folytatja, és ha a program megakad (menü, párbeszédablak),
  csend hallatszik, nem újra a régi hang.
- A hangbeállítások változtatásakor már nem maradnak vissza
  DirectSound-objektumok, és két egyszerre futó EightyOne nem zavarja
  egymás hangját.

### A főablak mérete

- Új nagyítás: **300%** (*View > Zoom*, Ctrl+F3), a 200% és a 400% között.
- A 100%, 200%, 300% vagy 400% nagyításnál az ablak húzással is
  átméretezhető, de csak ezek között a méretek között ugrik (a bejelölt
  nagyítás követi). A *User Defined* továbbra is bármilyen méretet enged.

### Billentyűzetkép (Help > Keyboard Map)

- A dupla kattintás lépésenként változtatja a kép méretét: 1x, 1,25x,
  1,5x, 1,75x, 2x, majd újra 1x (eddig csak 1x és 2x volt), a mellette lévő
  ZX80 funkciókkal és Spectrum 128 numerikus billentyűzettel együtt. A kép
  simán (halftone) skálázódik, a méretét megjegyzi. Az ablak húzással is
  átméretezhető, de csak ezek között a méretek között ugrik.

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
