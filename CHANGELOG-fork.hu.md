# A magyar Windows-kiadás változásai

[English](CHANGELOG-fork.md) | **Magyar**

Ennek a változatnak a módosításai az EightyOne 1.41-hez képest: a
[charlierobson/EightyOne](https://github.com/charlierobson/EightyOne)
2025. április 1-jei forrásához, amely kicsivel az 1.41-es kiadás után
készült. Magának az EightyOne-nak a változásai a
`Release Files/Release history.txt` fájlban vannak (angolul).

## Még nincs kiadva

### BASIC-hibakereső

Az EightyOne 1.42 fejlesztői változatának `BASIC` ágából (John Stroebel),
kiegészítve a Spectrummal:

- A **BASIC lista** (Eszközök menü) a program hibakeresésére is
  használható: **töréspont BASIC-sorra** (jobb kattintás a soron:
  Töréspont hozzáadása, Futtatás eddig a sorig, Bekapcsolva, Összes
  letiltása, Összes törlése; vagy a Hibakereső ablakban, BASIC típussal),
  **Megállítás/Futtatás** és **Lépés** soronként, a következő sor
  kijelölve; a **Változók** ablak futás közben mutatja a program
  változóit (számok, szövegek, tömbök, FOR-ciklusok).
- A ZX80-on és a ZX81-en működik, ebben a kiadásban a **Spectrumon** is:
  16K, 48K, 128, +2, +2A és +3, 48 BASIC-ben és 128 BASIC-ben. A sor
  kezdetét a program a ROM-okban a kódja alapján keresi meg, így minden
  ROM-változattal és nyelvvel működik.
- Javítások: a lista megnyitáskor rögtön mutatja a programot (eddig
  üres maradt, amíg nem írtak be egy sort); a rövid lista megálláskor nem
  csúszik egy sorral feljebb az eszköztár alá; a begépelt parancs (RUN)
  nem számít programsornak, így a Lépés nem áll meg rajta; a töréspont
  ablakában a BASIC-sornál nincs magányos kettőspont.
- Magyarul is, Segoe UI 9 ponttal.

### Javítások

- Konfiguráció betöltésekor (Beállítások > Konfiguráció) az ablak a
  vele mentett méretre (nagyításra) áll; eddig mindig 100%-ra ugrott.

## v1.41-hu.2 – 2026-11-10

A következő EightyOne (1.42, John Stroebel és Paul Farrow) fejlesztői
változata be van olvasztva: a
[stroebeljc/EightyOne](https://github.com/stroebeljc/EightyOne) `develop`
ága 2026. szeptember 4-i állapotában. A változásai a
`Release Files/Release history.txt` fájlban vannak (angolul; köztük: az
AY a $BFFD/$FFFD porton úgy, ahogy a 128K dekódolja, pontosabb Z80, saját
szálon futó emuláció, 0x és 8x közötti Sebesség ablak, RZX-lejátszás,
Beta Disk, Larken és ZX1541 lemezek, a Lambda 8300, a Timex gépek és a
Spectrum 128 BASIC listája, Memotech és Quicksilva nagy felbontás, Lambda
szín, a joystick POV-kalapja, a ZX Printer lendülete).

### A beolvasztás

- Az emuláció saját szálon fut, és **továbbra is a hang az órája**: a
  WASAPI- és a DirectSound-kimenet minden hangkockához jelez neki (eddig
  üzenetet küldött a főablaknak), és legfeljebb 100 ms-ig vár a hangjára,
  utána csendet játszik. Így **menü vagy párbeszédablak alatt is szól a
  hang és fut a gép**.
- A beállítások akkor töltődnek be, amikor már minden ablak megvan (mint
  az 1.42-ben); a főablak továbbra is csak akkor jelenik meg, amikor már
  megvan a mérete.
- Az új szövegek magyarul is megvannak (ZX1541 lemezek, RZX-lejátszás, a
  Lambda szín menüpontja, a BASIC lista formátumleírása), az új és
  megváltozott ablakok Segoe UI 9 pontosak.

### Javítások a fejlesztői változathoz képest

- A parancssorban megadott fájlokat, és az ott megadott beállításfájlt,
  néha figyelmen kívül hagyta: a parancssort a szétbontása után azonnal
  felszabadította.
- Pillanatkép betöltése alatt (menüből, parancssorból vagy az ablakra
  húzva) áll az emuláció: a betöltés közben futó képkockák „Unknown
  Internal Error” hibával és újraindítással végződhettek.
- A sikertelen RZX-lejátszás után nem marad állva az emuláció.
- Amíg a főablak az emuláció szálára vár, kezeli az onnan kapott
  üzeneteket (állapotsor, ZX Printer), így nem várhatnak egymásra.
- A **ZX Printer** soronként rajzol: a pontonkénti üzenet nyomtatás
  közben kb. 43 képkocka/s-ra lassította az emulációt. A görgetősávja 2x
  és 3x méretben is működik.

## v1.41-hu.1 – 2026-11-09

### Hang

- Az AY **burkológörbéje** úgy működik, mint a chipé, 16 lépcsővel. 15
  volt, ezért a lecsengő görbe nem némult el, hanem a legkisebb szinten
  szólt tovább (a 2-es és 3-as alak majdnem a legfelsőn), a felfutó és
  tartó alakok pedig egy lépcsővel alacsonyabban tartottak. A gyors
  burkológörbék (kis periódusú „buzzer” basszusok) is mind a 16 lépcsőt
  bejárják, nem csak annyit, amennyi a periódus; a 0-s periódus 1-nek
  számít.
- Ha egy csatornán **sem hang, sem zaj** nincs bekapcsolva, az AY a
  csatorna hangerejének szintjét adja ki, mint a chip, így a
  hangerő-regiszteren át lejátszott **digidobok és minták** is szólnak
  (eddig némák voltak).
- Az AY kimenetéből ki van szűrve az egyenfeszültség, mert a hangszóró
  is kondenzátoron át kapja: a keverőben kikapcsolt, de felhúzott
  hangerejű csatorna nem veszi el a helyet a hangok elől.
- **Sávkorlátozott AY** is: folytonos időben fut, és a kimenetének minden
  változása (a hangok élei, a zaj, a burkológörbe lépései, a
  regiszterírások, mind a minta azon a részén, ahol történnek)
  sávkorlátozott lépcső. A magas hangok és a burkológörbés „buzzer”
  basszusok mellett nem szólnak fémes, nem harmonikus mellékhangok:
  4 kHz-en ezek −20 dB-en voltak (együtt −13 dB-en), most −100 dB alatt.
  Az AY hangzása egyébként változatlan: a hangerők, a sztereó módok, a
  burkológörbék és a hangerő-regiszterrel lejátszott minták is.
- **Sávkorlátozott csipogó:** minden él a mintavételi frekvencia fele
  alá szűrt lépcsőként kerül a hangba (BLEP), így a magas hangok mellett
  nem szólnak sípoló, nem harmonikus mellékhangok (eddig −22…−43 dB-en
  voltak, most −75 dB alatt). A kattanás utáni halkulás 31 lépcső
  helyett egyenletes, a magas felharmonikusok nem tompulnak, ezért a
  csipogó kicsit fényesebb és tisztább. Kb. 0,35 ms-mal később szól,
  mint eddig.
- A hangot a **WASAPI** szólaltatja meg (Windows Vistától), a Windows
  keverőjének mintavételi frekvenciáján (jellemzően 48000 Hz), így nincs
  átmintavételezés. Kb. 30 ms hangot tart előre, ezért hamarabb hallatszik,
  mint a DirectSounddal (kb. 70 ms). Ha a WASAPI nem használható
  (Windows XP), a DirectSound szól, mint eddig.
- Ha a hangeszközt kihúzzák, vagy leáll, az emuláció a helyes sebességgel
  fut tovább, és a hang visszajön, amint újra megnyitható egy eszköz
  (másodpercenként próbálja). Ha a Windowsban másik eszköz lesz az
  alapértelmezett, a hang átkerül rá.
- Hangeszköz nélkül is fut az emuláció (eddig üres „Hanghiba” üzenetet
  adott, és nem futott).
- A kimenet a Hangbeállítások ablakban választható: *Automatikus*
  (WASAPI, vagy ha az nem használható, DirectSound), *WASAPI* vagy
  *DirectSound*. Azonnal átvált, a hangchipek alaphelyzetbe állítása
  nélkül; a beállítások (és a mentett konfigurációk) megőrzik. Mellette
  látszik, mi szólaltatja meg a hangot, és milyen frekvencián.
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
- A kitartott hangok már nem hullámzanak. Egy képkocka mindig egy
  képsor végéig fut, ezért legfeljebb egy képsornyival hosszabb vagy
  rövidebb a névlegesnél, a hangját viszont úgy helyezte el, mintha
  pontosan a puffere elején kezdődne: a csipogó élei (és az AY, a SpecDrum
  változásai) kb. 3 mintányit ide-oda csúsztak. Most minden képkocka ott
  kezdődik, ahol az előző véget ért, és ami a puffer végén túlra esik, az
  a következőbe kerül.
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

### Fájlok betöltése

Sérült vagy szándékosan hibás fájl már nem tudja összeomlasztani vagy
megakasztani az emulátort, és nem írhat felül a memóriájában: minden
fájlból olvasott hosszt, darabszámot, címet és lapszámot összevet a fájllal
és azzal, ahová az adat kerül. A sérült fájlból annyi töltődik be, amennyi
lehet, vagy üzenet jelzi, hogy nem sikerült betölteni.

- **Kazetták** (TZX, TAP, T81, P, O, CSW-blokkok): a blokkhosszakat a
  fájlból még hátralévő részhez méri (a régi kód messze túlolvashatott
  rajta), az általános adatblokkok szimbólumtábláit és impulzussorozatait
  is; legfeljebb annyi blokk töltődik be, amennyit a kazetta tárolni tud;
  a kazettán kívülre mutató ugrásokat és ciklusokat figyelmen kívül
  hagyja, és megállítja azt a kazettát, amely idő eltelte nélkül
  folyamatosan váltogatja a jelet.
- A hibás fejlécű (csatorna nélküli, 0 mintavételi frekvenciájú) **WAV- és
  CSW-fájlok** már nem állítják meg az emulátort „Division by zero”
  hibával, és a fájl végén túlnyúló részeket nem olvassa; helyette „A
  fájlt nem sikerült betölteni.” üzenet jelenik meg. A CSW-fájlnak CSW
  aláírással és 1.01-es verzióval kell kezdődnie.
- **.z80 pillanatképek**: a blokkon vagy a fájlon túlnyúló tömörített
  ismétlések és a gépben nem létező memórialapok már nem írnak az
  emulált memórián kívülre (az ilyen lapokat kihagyja; amit a csonka fájl
  kihagy, az nulla). A Spectra lapjait tömörítetlen tárolás esetén is
  helyesen olvassa.
- **.z81 pillanatképek**: a szövegben lévő darabszámok és címek abban a
  memóriában maradnak, ahová tartoznak (memória, a Chroma színmemóriája, a
  Quicksilva karaktergenerátor); a nagyon sok szakaszból álló fájl már
  nem tölti túl a vermet. A Quicksilva karaktergenerátort egy bájttal
  hosszabban mentette (egy azon túli bájtot is).
- **.ace pillanatképek**: az ismétlések a memórián belül maradnak, és az
  ismétlés közepén véget érő fájl már nem szalad el. A B' regisztert
  menti és betölti (a helyén a B-t tárolta). Az .ace pillanatkép mentése
  már nem írja a pillanatkép fejlécének egy részét a Jupiter Ace ROM-jába
  (0284h–029Fh, 052Ch és 07C8h), amitől az Ace utána hibásan működhetett.
- **Microdrive**: az .MDV kép betöltése mindig túlírt a neki fenntartott
  memórián; az egy szektornál rövidebb fájlból most üres kazetta lesz. Az
  olyan kazetta mentése, amelyen nincs megtalálható szektor, már nem akad
  meg, és a szalag végén átnyúló szektorokat is helyesen menti.
- **Merevlemezek**: az a .HDF fájl, amelynek adatai a fejlécén belül
  kezdődnének, már nem ír felül memóriát. Azok a VHD-képek, amelyek
  geometriájában van 128-as vagy nagyobb bájt (a nagy VHD-knek sávonként
  255 szektoruk van), hibás, negatív geometriát kaptak. A
  meghajtók ablakában a második merevlemez fej- vagy szektorszámának
  változtatása az elsőét használta, és az ott beállított 0 már nem okoz
  nullával osztást.
- **Larken lemezek**: a 79 fölötti sávszám már nem olvas és nem ír a
  lemezen kívülre, és a lemezfájl már nem marad nyitva.
- **ZIP-fájlok**: a legalább 256 karakteres nevű tag már nem kap
  értelmetlen nevet; az a tag, amelynek nevében `:` vagy más, Windows-fájlnévben
  nem megengedett karakter van, érvényes néven kerül kibontásra; ha egy
  tagot nem lehet kibontani (nem írható, vagy sérült az archívum), semmi
  sem töltődik be hiányzó vagy hibás fájl helyett.
- **File > Load Memory Block**: a Timex .SCR képernyő (12288 vagy 12289
  bájt) második fele a helyére kerül (8K-val az első után, eddig 6K-val
  odébb került), és hi-colour vagy hi-res módba vált a színével (az
  utolsó bájtot eddig eggyel mögüle olvasta). A memória tetején túl semmit
  sem tölt be, és a fájlt bezárja.
- A nem olvasható Timex dokkoló kazetta (.DCK) fájlja már nem marad nyitva.
- A ROM-ok, betűkészletek és nem felejtő memóriafájlok elérési útja
  bármilyen hosszú lehet (a mélyen fekvő EightyOne mappa túlcsordított egy
  256 karakteres puffert).

### Ablakok

- Az ablakok a régi MS Sans Serif helyett a Windows Vista óta használt
  **Segoe UI** betűtípust használják, azonos méretben, így az
  elrendezésük nem változik. Az a négy ablak, amelyet a betűcsere
  felnagyított volna (billentyűzetkép, numerikus billentyűzet,
  billentyűfunkciók, jelzőbitek szerkesztése), megtartja a méretét.
- **Minden ablak 9 pontos betűt használ** (eddig 8 pontosat), mint a
  Hangbeállítások. Az ablakok elrendezése ehhez arányosan nagyobb, és ahol
  egy felirat így sem fért ki – angolul vagy magyarul –, az ablak
  szélesebb lett: többek közt a Hibakereső, a TV-emuláció, a Billentyűzet,
  a Profilozó, a CPU-előzmények, a Meghajtók, a ZX Interface 1 RS232, a
  Nyomtatóport, a TS2050, a ZX97, a BASIC lista beállításai, a Töréspont
  és a HDF-fájl ablaka. A korábban csonka feliratok (a Hibakereső gombjai
  és jelölőnégyzetei, a „Függőleges szinkron”, a „HDF képfájl mérete:”, a
  „Karaktergenerátor:” és a Hardver ablak *Speciális beállítások*
  részének hosszabb jelölőnégyzetei és mások) most teljesen látszanak. A
  képhez vagy a programhoz méretezett ablakok (billentyűzetkép, numerikus
  billentyűzet, billentyűfunkciók) megtartják a méretüket; a kazettalista
  sorai magasabbak.
- A **Névjegy** ablak ennek a kiadásnak a verzióját mutatja (1.41-hu.1);
  az `EightyOne.exe` fájlverziója 1.41.0.1.
- A **BASIC lista** ablak legalább olyan széles, mint az eszköztára, így
  minden gombja látszik. A *BASIC lista mentése szövegként* ablak
  bekezdései egymás alá kerülnek, így nem csúsznak egymásba, és az ablak
  olyan magas, amilyet igényelnek.

### A főablak mérete

- Új nagyítás: **300%** (*View > Zoom*, Ctrl+F3), a 200% és a 400% között.
- A 100%, 200%, 300% vagy 400% nagyításnál az ablak húzással is
  átméretezhető, de csak ezek között a méretek között ugrik (a bejelölt
  nagyítás követi). A *User Defined* továbbra is bármilyen méretet enged.
- Induláskor a főablak csak akkor jelenik meg, amikor már kész: a legutóbbi
  méretében, a választott nyelven és géppel. Eddig a nyitókép alatt
  megjelent a tervezési méretében, angol menüvel, és csak egy másodperccel
  a gép indulása után ugrott a méretére. A parancssorban megadott fájlok
  azonnal betöltődnek (eddig két másodpercet vártak).
- Az állapotsor az ablak alján marad. 100%-nál a menüsor két sorba
  törik (a magyar menüvel mindig, Windows 11-en az angollal is), és az
  ablak átméretezésekor az állapotsor lemaradhatott: induláskor feljebb, a
  kép alatt maradt (ezért üresnek látszott), 300%-ra vagy 400%-ra váltva
  pedig az ablak alá került, amíg a nagyítást újra át nem állították.

### ZX Printer

- A papírra duplán kattintva a nyomat **kétszeres, majd háromszoros
  méretben** látszik (minden pont 2x2, illetve 3x3 képpont), újabb dupla
  kattintásra ismét normál méretben; a gombok és a beállítások követik az
  ablak szélességét, a méretet megjegyzi.
- A nyomat **PNG**-ben mentődik (a BMP továbbra is választható), abban a
  méretben, ahogy az ablakban látszik: 1x, 2x vagy 3x.

### Képernyőkép

- A *Fájl > Képernyőkép mentése* **PNG**-ben is ment, ez az első
  választás (a BMP és Spectrumon az SCR megmarad).

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
- Nyelvváltáskor a program által összerakott szövegek is azonnal váltanak
  (eddig csak a következő frissítésükkor): a kazettalista a fejlécével és
  a Kazettakezelő címe, a Hardver ablak teljes RAM-mérete, a
  Hangfájl-betöltő címe és állapota, a ZX Printer sebességei, a
  BASIC-lista állapotsora, a profilozó grafikonja és a játékvezérlő-menük.
- A pillanatképek és a beállításfájl a hardverválasztások angol szövegét
  tárolják, így bármelyik nyelvvel és az eredeti EightyOne-nal is működnek.
- A gép- és terméknevek és a rövidítések (ROM, RAM, NMI, AY...) angolul
  maradnak.

### Billentyűzet

- **Billentyűismétlés ZX80/81-en** (Options > Keyboard, alapból
  kikapcsolva, a ZX80-nál, a ZX81-nél és klónjainál látszik): a 0,7
  másodpercnél tovább nyomva tartott billentyű másodpercenként kb.
  nyolcszor ismétlődik, amit a ZX80 és a ZX81 ROM-ja magától nem tud. A
  billentyűt az emulátor 4 képkockára felengedettnek, 2-re lenyomottnak
  mutatja, így a ROM mindegyiket új leütésnek veszi; a SHIFT lenyomva
  marad, így a SHIFT-es jelek is ismétlődnek. A Windows-kiosztással és
  anélkül is működik.
- **A Windows billentyűzetkiosztásának használata** (Options > Keyboard;
  magyar felülettel alapból bekapcsolva, angollal kikapcsolva, ott a
  billentyűk úgy működnek, mint az eredeti EightyOne-ban): a gépelés azt a karaktert követi, amit a Windows
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

### BASIC lista

- A *9999 feletti sorszámok 5 számjeggyel* beállítás újraindítás után is
  megmarad: elírt néven mentődött, így visszaolvasva mindig kikapcsolt
  volt.
