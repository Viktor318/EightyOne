# EightyOne – magyar Windows-kiadás

[English](README.md) | **Magyar**

Ez az [EightyOne](https://github.com/charlierobson/EightyOne) (1.41-es
verzió, az 1.42-es fejlesztői változatával) egy változata. Az EightyOne Michael D Wynne, Charlie Robson és Paul
Farrow Windowsos emulátora a Sinclair ZX80, ZX81 és ZX Spectrum gépekhez, a
klónjaikhoz és a Jupiter Ace-hez. Ez a kiadás magyar felülettel, a Windows
billentyűkiosztása szerinti gépeléssel, tisztább, WASAPI-n szóló hanggal és
megújult Hangbeállítások ablakkal, minden ablakban 9 pontos betűvel, 300%-os
nagyítással, a sérült fájlok biztonságos betöltésével és több kisebb
javítással egészíti ki az eredetit.

Maga az emuláció az eredeti EightyOne. A következő kiadásának, az
1.42-nek a fejlesztői változata John Stroebel és Paul Farrow munkája, a
[stroebeljc/EightyOne](https://github.com/stroebeljc/EightyOne) tárolóból.
Ennek a változatnak a módosításai a
[CHANGELOG-fork.hu.md](CHANGELOG-fork.hu.md) fájlban, az EightyOne saját
változásai (az 1.42-éi is) a `Release Files/Release history.txt` fájlban
vannak (angolul).

## Letöltés

A [Releases](https://github.com/Viktor318/EightyOne/releases) oldalról
töltsd le az `EightyOne-1.41-hu.2-windows.zip` fájlt, csomagold ki egy
tetszőleges mappába, és indítsd el az `EightyOne.exe`-t. Telepíteni nem kell
semmit. A `ROMs` és a `ROM Cartridges` mappának a program mellett kell
maradnia.

## Mit ad hozzá ez a kiadás

### Az EightyOne 1.42 fejlesztői változatából

- Az emuláció saját szálon fut, továbbra is a hang ütemére: **menü vagy
  párbeszédablak alatt is szól a hang és fut a gép**.
- A Spectrum 128 AY-ja úgy dekódolódik, mint az igazi gépen, így sok
  programban megszólal a korábban néma zene; pontosabb Z80 és
  képkocka-időzítés.
- **Sebesség** 0,25x és 8x között, a valós időnél lassabban is;
  **RZX-lejátszás** Spectrumon; Beta Disk (TR-DOS) javítások, **ZX1541**
  D64 lemezképekkel, Larken; a Lambda 8300, a Timex gépek és a Spectrum
  128 BASIC listája; Memotech és Quicksilva nagy felbontás, Lambda szín,
  a ZX Printer lendülete és görgetősávja, a joystick POV-kalapja.

### Magyar vagy angol felület

- **Beállítások > Nyelv**: *Automatikus* (a Windows nyelvét követi),
  *English* vagy *Magyar*. Futás közben is átváltható.
- A menük, az ablakok és az üzenetek le vannak fordítva; a gépek és
  termékek nevei és a rövidítések (ROM, RAM, NMI, AY...) angolul maradnak.
- A pillanatképek és a beállítások a hardverválasztások angol nevét
  őrzik, így mindkét nyelvvel és az eredeti EightyOne-nal is működnek.

### Billentyűzet

- **Windows billentyűkiosztás használata** (Beállítások > Billentyűzet;
  magyar felülettel alapból bekapcsolva): a gépelés a Windows
  billentyűkiosztásának karaktereit követi, nem a billentyűk helyét, minden
  gépen (ZX80, ZX81, Lambda, Spectrum, Jupiter Ace). Magyar billentyűzeten
  a Shift+2 `"`-t ad; az AltGr-es jelek (`# & @ [ ] { } < >`...) ott
  működnek, ahol a gépen vannak, a Spectrum kiterjesztett módú jeleit
  (`[ ] { } \ | ~ ©`) pedig a program a megfelelő billentyűsorozattal
  gépeli be. Az ékezetes betűk ékezet nélkül mennek át.
- **Bal Alt SHIFT-ként a számbillentyűkkel** (alapból bekapcsolva): a bal
  Alt + szám SHIFT + számot ad (a Spectrumon CAPS SHIFT-et: EDIT,
  kurzorok, DELETE...).
- **Billentyűismétlés ZX80/81-en** (alapból kikapcsolva): a 0,7 másodpercig
  nyomva tartott billentyű másodpercenként kb. 8-szor ismétlődik, amit a
  ZX80 és a ZX81 ROM-ja nem tud.

### Hang

- A hangot a **WASAPI** szólaltatja meg, a Windows keverőjének
  mintavételi frekvenciáján, kisebb késleltetéssel (kb. 30 ms); a
  DirectSound tartaléknak megmaradt, és választható is. A hang követi az
  alapértelmezett eszközt, és az eszköz kihúzását is túléli; az EightyOne
  hangeszköz nélkül is fut.
- Megújult a **Hangbeállítások** ablak: fő hangerő, magas csúszkák
  (0–100%) szintjelzőkkel, az AY csatornái külön némíthatók, és hét AY
  sztereó mód van (ABC, ACB, BAC, BCA, CAB, CBA, Mono).
- **Tisztább hang**: a csipogó és az AY sávkorlátozott (nincs sípoló vagy
  fémes mellékhang a magas hangok mellett), az AY burkológörbéje mind a 16
  lépcsőt bejárja, a hangerő-regiszteren át lejátszott digidobok és minták
  is szólnak, a kitartott hangok nem hullámzanak, a hangos részek nem
  recsegnek. A DirectSound-kimenet Windows 10/11-en is folyamatos.

### Ablakok és megjelenítés

- Minden ablakban **9 pontos Segoe UI** betű; az ablakok ott szélesebbek,
  ahol az angol vagy a magyar felirat megkívánja.
- **300%-os nagyítás** (Nézet > Nagyítás, Ctrl+F3); 100–400% választása
  mellett a főablak átméretezéskor ezek között a méretek között ugrál. A
  főablak csak készen, a mentett méretében jelenik meg, és az állapotsora
  mindig az alján marad.
- A **Súgó > Billentyűzetkiosztás** képe dupla kattintással lépésenként
  nagyítható (1,5x-től 2,5x-ig); a Spectrum 16K, 48K, 128, +2 és +3
  billentyűzete, a 128 numerikus billentyűzete, a ZX81 billentyűzete (a
  ZXpand-os, a TS1000 és a ZX97 változattal együtt) a ZX80 billentyűzete
  (a ZXpand-ossal együtt) a Timex TC2048, TS2068 és TS1500, valamint a
  Jupiter Ace és a Lambda 8300 billentyűzete újra van rajzolva, minden
  méretben éles.
- **ZX Printer**: dupla kattintásra két- vagy háromszoros méret; a
  nyomat PNG-ben (vagy BMP-ben) menthető. A **Fájl > Képernyőkép mentése**
  is ment PNG-t.

### Fájlok betöltése

A sérült vagy szándékosan hibás fájlok (kazetták, WAV, CSW, `.z80`, `.z81`,
`.ace`, Microdrive, merevlemez-képfájlok, ZIP...) nem omlasztják össze és
nem akasztják meg az emulátort, és nem írnak a memóriájába: ameddig lehet,
betöltődnek, vagy egy üzenet jelzi, hogy nem tölthetők be. Az `.ace`
pillanatkép mentése nem rontja el a Jupiter Ace ROM-ját.

### Beállítások

A beállítások ott vannak, ahol az eredeti EightyOne-nál:
`%APPDATA%\EightyOne\EightyOne.ini` (írd be a `%APPDATA%\EightyOne`
címet a Fájlkezelő címsorába). Az eredeti EightyOne beállításait a
program átveszi.

## Fordítás forrásból

Az EightyOne **Borland C++ Builder 5**-tel (Update Pack 1) fordul, amely
Windows 11-en is fut. A fejlesztői környezet és a komponensek telepítését
a `Documentation/EightyOne Development Environment.docx` írja le (angolul).
Nyisd meg a `Source/EightyOne.bpr` projektet, és fordíts a *Project >
Build EightyOne* paranccsal; az `EightyOne.exe` a `Source` mappába kerül,
és onnan fut.

A magyar szövegek a `Translations/hu.tsv` fájlban vannak; a
`python3 Translations/make_table.py` készíti el belőle a
`Source/LanguageHu.cpp` fájlt.

## Licenc

Az EightyOne szabad szoftver, a GNU General Public License 2-es vagy
későbbi változata alatt (lásd `Release Files/gpl.txt`); ez a kiadás is.

## Köszönet

Michael D Wynne-nak, Charlie Robsonnak, Paul Farrow-nak és mindenkinek, aki
az [EightyOne](https://github.com/charlierobson/EightyOne)-t elkészítette
és fejleszti.
