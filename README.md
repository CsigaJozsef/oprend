# Operációs rendszerek tárgy

- Tárgyon C nyelvben programozás
- Több processz futtatása
    - processzek között komunikáció

## Beadandó 1:
Rendkívüli időket élünk.

Az "Igyál komám" szőlészetben is kopogtat a tavasz. Mint tudjuk a szőlő szereti a gondoskodást, a munkát, de leginkább a munkást, amit rendszerint jó terméssel hálál meg. Sok segítőre van szüksége a szőlészetnek, így a munkára jelentkezőknek egy alkalmazás készül.

Azok jelentkezését várja a gazdaság akik a tavaszi szezon heteiben tudnak munkát vállalni. A jelentkezés során meg kell adni a nevüket és azt, hogy a hét mely napjain tudnak menni dolgozni. A napokat ilyen formában adják meg: hétfő szerda csütörtök. A jelentkezők helyközzel választják el a napokat. A gazdaságnak megvan, hogy melyik napokon(hétfőnként, keddenként stb), hány munkásra van szüksége. Ha egy napon már betelt a létszám, akkor arra a napra már ne fogadjon el jelentkezőt az alkalmazás.

A jelentkezők adatait fájlban tároljuk, az adatfelvételen túl legyen lehetőségünk az adatokon módosítani, törölni és napi vagy teljes listát készíteni a jelentkezőkről.

## Beadandó 2:
Az "Igyál komám" szőlészetben is kopogtat a tavasz. Mint tudjuk a szőlő szereti a gondoskodást, a munkát, de leginkább a munkást, amit rendszerint jó terméssel hálál meg. Sok segítőre van szüksége a szőlészetnek, így a munkára jelentkezőknek egy alkalmazás készül.

Azok jelentkezését várja a gazdaság akik a tavaszi szezon heteiben tudnak munkát vállalni. Az adatok rendelkezésre állnak, minden módosítást el tudunk végezni.

"Igyál komám" szőlészet (szülő folyamat) minden nap reggel megvizsgálja az aznapi jelentkezéseket, igényeket és elindítja a dolgozókért a munkásjárato(ka)t. (gyerekfolyamat(ok)) Csak kisbusz áll rendelkezésre, ami max 5 főt tud fuvarozni, és két darab kisbusz van. (A szőlészet  kis cég, így nincs több napi munkás igény, mint 10 fő.) A szőlészet elindítja a kisbusz(oka)t, akik, miután készek az indulásra, jelzést küldenek vissza, majd válaszul csövön keresztül megkapják szőlészettől, hogy kiket kell munkára behozni. Ezt az információt a munkásjárat(ok) képernyőre is írják.  Amint beérnek a dolgozókkal, összegzésként üzenetsoron visszaküldik a szőlészetnek, hogy a kért dolgozók közül végül is hányat sikerült behozni. Ezt a szőlészet képernyőre írja, majd befejezik a tevékenységet (munkásjárat gyerekfolyam terminál). A szülő ezt megvárja, és ezzel készen áll a következő napi munkásjárat szervezésére. (Lehet újabb jelentkezőt felvenni, módosítani, vagy egy újabb napi buszindítást kezdeni.)