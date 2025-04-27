

int main() {
    // Ein Vektor von Vektoren, die aus Integer bestehen
    std::vector<std::vector<int> > myVectorOfVectors; // Beachte das Leerzeichen zwischen >> (wegen C++03!)

    // Füge Zeilen dynamisch hinzu
    std::vector<int> row1;
    row1.push_back(10);
    row1.push_back(20);
    myVectorOfVectors.push_back(row1);

    std::vector<int> row2;
    row2.push_back(30);
    myVectorOfVectors.push_back(row2);

    std::vector<int> row3;
    row3.push_back(40);
    row3.push_back(50);
    myVectorOfVectors.push_back(row3);

    // Gib die Elemente aus
    for (size_t i = 0; i < myVectorOfVectors.size(); ++i) {
        for (size_t j = 0; j < myVectorOfVectors[i].size(); ++j) {
            std::cout << myVectorOfVectors[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}


int main() {
    std::vector<std::vector<int> > myVectorOfVectors;

    // Direkt beim push_back einen neuen Vektor erstellen und füllen
    myVectorOfVectors.push_back(std::vector<int>());
    myVectorOfVectors.back().push_back(10);
    myVectorOfVectors.back().push_back(20);

    myVectorOfVectors.push_back(std::vector<int>());
    myVectorOfVectors.back().push_back(30);

    myVectorOfVectors.push_back(std::vector<int>());
    myVectorOfVectors.back().push_back(40);
    myVectorOfVectors.back().push_back(50);

    // Ausgabe
    for (size_t i = 0; i < myVectorOfVectors.size(); ++i) {
        for (size_t j = 0; j < myVectorOfVectors[i].size(); ++j) {
            std::cout << myVectorOfVectors[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
Kurz erklärt:

push_back(std::vector<int>()) erzeugt eine leere Zeile und hängt sie ans Ende.

back() greift auf die neu hinzugefügte Zeile zu, sodass du direkt Elemente hineinpushen kannst.

Wir benutzen dabei den std::vector-Konstruktor, der einen Vektor mit einer festen Anzahl von Kopien eines Wertes initialisiert. Das kann man ausnutzen:

int main() {
    std::vector<std::vector<int> > myVectorOfVectors;

    // Zeilen direkt mit initialisierten Werten anlegen:
    myVectorOfVectors.push_back(std::vector<int>(2, 10)); // Zwei mal 10
    myVectorOfVectors.push_back(std::vector<int>(1, 30)); // Einmal 30
    myVectorOfVectors.push_back(std::vector<int>(2, 40)); // Zwei mal 40

    // Jetzt gezielt einzelne Elemente anpassen, wenn nötig
    myVectorOfVectors[0][1] = 20; // erstes Zeile, zweites Element -> 20 statt 10
    myVectorOfVectors[2][1] = 50; // dritte Zeile, zweites Element -> 50 statt 40

    // Ausgabe
    for (size_t i = 0; i < myVectorOfVectors.size(); ++i) {
        for (size_t j = 0; j < myVectorOfVectors[i].size(); ++j) {
            std::cout << myVectorOfVectors[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}

Wenn ein Vektor existiert, der ein Element mit Wert 42 enthält → nichts tun.
Ansonsten → einen neuen Vektor anlegen und push_back auf myVectorOfVectors machen.

// Hilfsfunktion: prüft, ob irgendwo die Zahl 42 existiert
bool contains42(const std::vector<std::vector<int> >& vecOfVecs) {
    for (size_t i = 0; i < vecOfVecs.size(); ++i) {
        for (size_t j = 0; j < vecOfVecs[i].size(); ++j) {
            if (vecOfVecs[i][j] == 42) {
                return true;
            }
        }
    }
    return false;
}

// Funktion: fügt eine neue Zeile nur hinzu, wenn keine 42 existiert
void addRowIfNo42(std::vector<std::vector<int> >& vecOfVecs, const std::vector<int>& newRow) {
    if (!contains42(vecOfVecs)) {
        vecOfVecs.push_back(newRow);
    }
}

int main() {
    std::vector<std::vector<int> > myVectorOfVectors;

    // Versuchen, Zeilen hinzuzufügen
    addRowIfNo42(myVectorOfVectors, std::vector<int>(2, 10)); // Zwei mal 10
    addRowIfNo42(myVectorOfVectors, std::vector<int>(1, 30)); // Einmal 30
    addRowIfNo42(myVectorOfVectors, std::vector<int>(2, 40)); // Zwei mal 40

    // Jetzt gezielt einzelne Elemente anpassen, wenn nötig
    if (myVectorOfVectors.size() > 0 && myVectorOfVectors[0].size() > 1)
        myVectorOfVectors[0][1] = 20;

    if (myVectorOfVectors.size() > 2 && myVectorOfVectors[2].size() > 1)
        myVectorOfVectors[2][1] = 50;

    // Beispiel: Jetzt eine Zeile mit 42 erzeugen und hinzufügen
    addRowIfNo42(myVectorOfVectors, std::vector<int>(1, 42)); // Fügt 42 hinzu
    addRowIfNo42(myVectorOfVectors, std::vector<int>(1, 99)); // Wird NICHT hinzugefügt, weil 42 existiert!

    // Ausgabe
    for (size_t i = 0; i < myVectorOfVectors.size(); ++i) {
        for (size_t j = 0; j < myVectorOfVectors[i].size(); ++j) {
            std::cout << myVectorOfVectors[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
Ablauf:
addRowIfNo42 prüft mit contains42, ob irgendwo eine 42 existiert.

Wenn nein, wird die neue Zeile (newRow) hinzugefügt.

Sobald eine 42 existiert, werden keine weiteren Zeilen mehr angefügt.

Ausgabe wäre:

10 20 
30 
40 50 
42 
(Das 99 kommt nicht mehr rein, weil 42 jetzt vorhanden ist.)

// Hilfsfunktion: prüft, ob irgendwo die Zahl 42 existiert, aber nur zwischen fromRow und toRow
bool contains42InRange(const std::vector<std::vector<int> >& vecOfVecs, size_t fromRow, size_t toRow) {
    if (vecOfVecs.empty() || fromRow > toRow || toRow >= vecOfVecs.size()) {
        return false;
    }

    for (size_t i = fromRow; i <= toRow; ++i) {
        for (size_t j = 0; j < vecOfVecs[i].size(); ++j) {
            if (vecOfVecs[i][j] == 42) {
                return true;
            }
        }
    }
    return false;
}

// Funktion: fügt eine neue Zeile nur hinzu, wenn keine 42 in einem Bereich existiert
void addRowIfNo42InRange(std::vector<std::vector<int> >& vecOfVecs, const std::vector<int>& newRow, size_t fromRow, size_t toRow) {
    if (!contains42InRange(vecOfVecs, fromRow, toRow)) {
        vecOfVecs.push_back(newRow);
    }
}

int main() {
    std::vector<std::vector<int> > myVectorOfVectors;

    // Ganz normal Zeilen hinzufügen
    addRowIfNo42InRange(myVectorOfVectors, std::vector<int>(2, 10), 0, 0); // erste Zeile
    addRowIfNo42InRange(myVectorOfVectors, std::vector<int>(1, 30), 0, 1); // zweite Zeile
    addRowIfNo42InRange(myVectorOfVectors, std::vector<int>(2, 40), 0, 2); // dritte Zeile

    // Anpassungen
    if (myVectorOfVectors.size() > 0 && myVectorOfVectors[0].size() > 1)
        myVectorOfVectors[0][1] = 20;
    if (myVectorOfVectors.size() > 2 && myVectorOfVectors[2].size() > 1)
        myVectorOfVectors[2][1] = 50;

    // Jetzt 42 einfügen
    addRowIfNo42InRange(myVectorOfVectors, std::vector<int>(1, 42), 0, 2); // Fügt 42 hinzu

    // Jetzt würde dieser Versuch fehlschlagen, weil 42 im Bereich [0,3] existiert
    addRowIfNo42InRange(myVectorOfVectors, std::vector<int>(1, 99), 0, 3);

    // Ausgabe
    for (size_t i = 0; i < myVectorOfVectors.size(); ++i) {
        for (size_t j = 0; j < myVectorOfVectors[i].size(); ++j) {
            std::cout << myVectorOfVectors[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}

#include <algorithm> // für std::find_if

// Functor: prüft, ob ein Wert 42 ist
struct Is42 {
    bool operator()(int value) const {
        return value == 42;
    }
};

// Functor: prüft, ob ein Vector<int> eine 42 enthält
struct VectorContains42 {
    bool operator()(const std::vector<int>& vec) const {
        return std::find_if(vec.begin(), vec.end(), Is42()) != vec.end();
    }
};

// Funktion: prüft, ob eine 42 in irgendeiner Zeile existiert
bool contains42_findif(const std::vector<std::vector<int> >& vecOfVecs) {
    return std::find_if(vecOfVecs.begin(), vecOfVecs.end(), VectorContains42()) != vecOfVecs.end();
}

// Funktion: fügt eine neue Zeile nur hinzu, wenn keine 42 existiert
void addRowIfNo42_findif(std::vector<std::vector<int> >& vecOfVecs, const std::vector<int>& newRow) {
    if (!contains42_findif(vecOfVecs)) {
        vecOfVecs.push_back(newRow);
    }
}

int main() {
    std::vector<std::vector<int> > myVectorOfVectors;

    addRowIfNo42_findif(myVectorOfVectors, std::vector<int>(2, 10));
    addRowIfNo42_findif(myVectorOfVectors, std::vector<int>(1, 30));
    addRowIfNo42_findif(myVectorOfVectors, std::vector<int>(2, 40));

    // Anpassungen
    if (myVectorOfVectors.size() > 0 && myVectorOfVectors[0].size() > 1)
        myVectorOfVectors[0][1] = 20;
    if (myVectorOfVectors.size() > 2 && myVectorOfVectors[2].size() > 1)
        myVectorOfVectors[2][1] = 50;

    // 42 hinzufügen
    addRowIfNo42_findif(myVectorOfVectors, std::vector<int>(1, 42));
    addRowIfNo42_findif(myVectorOfVectors, std::vector<int>(1, 99)); // sollte scheitern

    // Ausgabe
    for (size_t i = 0; i < myVectorOfVectors.size(); ++i) {
        for (size_t j = 0; j < myVectorOfVectors[i].size(); ++j) {
            std::cout << myVectorOfVectors[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
✨ Zusammengefasst:
Is42: prüft einzelne Werte.

VectorContains42: prüft, ob ein vector<int> eine 42 enthält.

std::find_if: sucht mit Functor nach einer Zeile, die 42 hat.


#include <algorithm> // für std::find_if

// Functor: prüft, ob ein Wert gleich dem gesuchten Wert ist
class IsValue {
private:
    int target_;
public:
    explicit IsValue(int target) : target_(target) {}

    bool operator()(int value) const {
        return value == target_;
    }
};

// Functor: prüft, ob ein Vector<int> den gesuchten Wert enthält
class VectorContainsValue {
private:
    int target_;
public:
    explicit VectorContainsValue(int target) : target_(target) {}

    bool operator()(const std::vector<int>& vec) const {
        return std::find_if(vec.begin(), vec.end(), IsValue(target_)) != vec.end();
    }
};

// Funktion: prüft, ob der Wert irgendwo existiert
bool containsValue_findif(const std::vector<std::vector<int> >& vecOfVecs, int value) {
    return std::find_if(vecOfVecs.begin(), vecOfVecs.end(), VectorContainsValue(value)) != vecOfVecs.end();
}

// Funktion: fügt eine neue Zeile nur hinzu, wenn der Wert nicht existiert
void addRowIfNoValue_findif(std::vector<std::vector<int> >& vecOfVecs, const std::vector<int>& newRow, int value) {
    if (!containsValue_findif(vecOfVecs, value)) {
        vecOfVecs.push_back(newRow);
    }
}

int main() {
    std::vector<std::vector<int> > myVectorOfVectors;

    // Hier prüfen wir auf die Zahl 42
    addRowIfNoValue_findif(myVectorOfVectors, std::vector<int>(2, 10), 42);
    addRowIfNoValue_findif(myVectorOfVectors, std::vector<int>(1, 30), 42);
    addRowIfNoValue_findif(myVectorOfVectors, std::vector<int>(2, 40), 42);

    // Anpassungen
    if (myVectorOfVectors.size() > 0 && myVectorOfVectors[0].size() > 1)
        myVectorOfVectors[0][1] = 20;
    if (myVectorOfVectors.size() > 2 && myVectorOfVectors[2].size() > 1)
        myVectorOfVectors[2][1] = 50;

    // Jetzt 42 einfügen
    addRowIfNoValue_findif(myVectorOfVectors, std::vector<int>(1, 42), 42);

    // Jetzt 99 einfügen → wird wegen 42 NICHT eingefügt
    addRowIfNoValue_findif(myVectorOfVectors, std::vector<int>(1, 99), 42);

    // Ausgabe
    for (size_t i = 0; i < myVectorOfVectors.size(); ++i) {
        for (size_t j = 0; j < myVectorOfVectors[i].size(); ++j) {
            std::cout << myVectorOfVectors[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}



#include <algorithm> // für std::find_if

// Functor: Prüft, ob ein einzelner Wert gleich dem Zielwert ist
class IsValue {
private:
    int target_;
public:
    explicit IsValue(int target) : target_(target) {}

    bool operator()(int value) const {
        return value == target_;
    }
};

// Functor: Prüft, ob ein Vektor den Zielwert enthält
class VectorContainsValue {
private:
    int target_;
public:
    explicit VectorContainsValue(int target) : target_(target) {}

    bool operator()(const std::vector<int>& vec) const {
        return std::find_if(vec.begin(), vec.end(), IsValue(target_)) != vec.end();
    }
};

// Typalias (ganz simpel) für bessere Lesbarkeit
typedef std::vector<int> IntVector;
typedef std::vector<IntVector> IntVector2D;

// Funktion: prüft, ob Wert irgendwo existiert
bool containsValue_findif(const IntVector2D& vecOfVecs, int value) {
    return std::find_if(vecOfVecs.begin(), vecOfVecs.end(), VectorContainsValue(value)) != vecOfVecs.end();
}

// Funktion: fügt eine neue Zeile nur hinzu, wenn der Wert nicht existiert
void addRowIfNoValue_findif(IntVector2D& vecOfVecs, const IntVector& newRow, int value) {
    if (!containsValue_findif(vecOfVecs, value)) {
        vecOfVecs.push_back(newRow);
    }
}

int main() {
    IntVector2D myVectorOfVectors;

    addRowIfNoValue_findif(myVectorOfVectors, IntVector(2, 10), 42);
    addRowIfNoValue_findif(myVectorOfVectors, IntVector(1, 30), 42);
    addRowIfNoValue_findif(myVectorOfVectors, IntVector(2, 40), 42);

    // Anpassungen (direkt ohne const, wegen Kompatibilität)
    if (myVectorOfVectors.size() > 0 && myVectorOfVectors[0].size() > 1)
        myVectorOfVectors[0][1] = 20;
    if (myVectorOfVectors.size() > 2 && myVectorOfVectors[2].size() > 1)
        myVectorOfVectors[2][1] = 50;

    // Jetzt 42 hinzufügen
    addRowIfNoValue_findif(myVectorOfVectors, IntVector(1, 42), 42);

    // Versuch, 99 hinzuzufügen → wird ignoriert
    addRowIfNoValue_findif(myVectorOfVectors, IntVector(1, 99), 42);

    // Ausgabe
    for (size_t i = 0; i < myVectorOfVectors.size(); ++i) {
        for (size_t j = 0; j < myVectorOfVectors[i].size(); ++j) {
            std::cout << myVectorOfVectors[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
Änderungen gegenüber davor:
Typedefs ganz simpel (IntVector, IntVector2D) – nix Verschachteltes.

#include <algorithm>

// --- Forward Declarations ---
namespace MyChecker {
    class IsValue;
    class VectorContainsValue;
}

// --- Definitions ---
namespace MyChecker {

    class IsValue {
    private:
        int target_;
    public:
        explicit IsValue(int target) : target_(target) {}

        bool operator()(int value) const {
            return value == target_;
        }
    };

    class VectorContainsValue {
    private:
        int target_;
    public:
        explicit VectorContainsValue(int target) : target_(target) {}

        bool operator()(const std::vector<int>& vec) const {
            return std::find_if(vec.begin(), vec.end(), IsValue(target_)) != vec.end();
        }
    };
}

// --- Type Aliases ---
typedef std::vector<int> IntVector;
typedef std::vector<IntVector> IntVector2D;

// --- Functions ---
bool containsValue_findif(const IntVector2D& vecOfVecs, int value) {
    return std::find_if(vecOfVecs.begin(), vecOfVecs.end(), MyChecker::VectorContainsValue(value)) != vecOfVecs.end();
}

void addRowIfNoValue_findif(IntVector2D& vecOfVecs, const IntVector& newRow, int value) {
    if (!containsValue_findif(vecOfVecs, value)) {
        vecOfVecs.push_back(newRow);
    }
}

int main() {
    IntVector2D myVectorOfVectors;

    addRowIfNoValue_findif(myVectorOfVectors, IntVector(2, 10), 42);
    addRowIfNoValue_findif(myVectorOfVectors, IntVector(1, 30), 42);
    addRowIfNoValue_findif(myVectorOfVectors, IntVector(2, 40), 42);

    if (myVectorOfVectors.size() > 0 && myVectorOfVectors[0].size() > 1)
        myVectorOfVectors[0][1] = 20;
    if (myVectorOfVectors.size() > 2 && myVectorOfVectors[2].size() > 1)
        myVectorOfVectors[2][1] = 50;

    addRowIfNoValue_findif(myVectorOfVectors, IntVector(1, 42), 42);
    addRowIfNoValue_findif(myVectorOfVectors, IntVector(1, 99), 42);

    for (size_t i = 0; i < myVectorOfVectors.size(); ++i) {
        for (size_t j = 0; j < myVectorOfVectors[i].size(); ++j) {
            std::cout << myVectorOfVectors[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}

Vergleichstyp kann einfach ein enum sein → schön C++03 kompatibel.
Hier die Idee:

namespace MyChecker {

    enum CompareType {
        Equal,
        Less,
        Greater,
        Divisible
    };

    class ValueChecker {
    private:
        int target_;
        CompareType type_;
    public:
        ValueChecker(int target, CompareType type)
            : target_(target), type_(type) {}

        bool operator()(int value) const {
            switch (type_) {
                case Equal:      return value == target_;
                case Less:       return value < target_;
                case Greater:    return value > target_;
                case Divisible:  return (target_ != 0) ? (value % target_ == 0) : false;
                default:         return false;
            }
        }
    };

    class VectorContainsValue {
    private:
        int target_;
        CompareType type_;
    public:
        VectorContainsValue(int target, CompareType type)
            : target_(target), type_(type) {}

        bool operator()(const std::vector<int>& vec) const {
            return std::find_if(vec.begin(), vec.end(), ValueChecker(target_, type_)) != vec.end();
        }
    };
}
Damit könntest du später z.B. machen:

// Prüfen ob irgendwo eine Zahl vorhanden ist, die kleiner als 50 ist
std::find_if(myVectorOfVectors.begin(), myVectorOfVectors.end(), 
             MyChecker::VectorContainsValue(50, MyChecker::Less));
Oder:


// Prüfen ob irgendwo eine Zahl vorhanden ist, die durch 7 teilbar ist
std::find_if(myVectorOfVectors.begin(), myVectorOfVectors.end(), 
             MyChecker::VectorContainsValue(7, MyChecker::Divisible));

//Um den Vector für andere Klassen leicht zu erhalten

class MeineKlasse {
private:
    std::vector<std::vector<int> > myVectorOfVectors;  // C++03: Leerzeichen zwischen >> nötig

public:
    // Getter, der eine veränderbare Referenz zurückgibt
    std::vector<std::vector<int> >& getVectorOfVectors() {
        return myVectorOfVectors;
    }

    // Optional: noch ein const-Getter für Lesefälle
    const std::vector<std::vector<int> >& getVectorOfVectors() const {
        return myVectorOfVectors;
    }
};
Erklärung

Der Rückgabetyp std::vector<std::vector<int> >& ist eine Referenz auf den internen Container.

Aufgerufen z. B. in Deinem Code so:

MeineKlasse obj;
// Über den Getter direkt ändern:
obj.getVectorOfVectors().push_back(std::vector<int>());
obj.getVectorOfVectors().back().push_back(123);
Mit der const-Überladung stellst Du sicher, dass const-Objekte Deiner Klasse den Read-Only-Zugriff nutzen können:


const MeineKlasse constObj;
// constObj.getVectorOfVectors().push_back(...); // Compiler-Fehler
auto& v = constObj.getVectorOfVectors();        // nur Lesen erlaubt
Damit hast Du einen einfachen, C++03-kompatiblen Getter, der alles an Deinem vector veränderbar macht.


const MeineKlasse constObj;
// Nur Lesender Zugriff:
const std::vector<std::vector<int> >& v = constObj.getVectorOfVectors();
// v.push_back(...)  // würde hier einen Compiler-Fehler geben, da v const ist

// Zum Ändern brauchst Du das nicht-const-Objekt:
MeineKlasse obj;
std::vector<std::vector<int> >& mutableV = obj.getVectorOfVectors();
mutableV.push_back(std::vector<int>());        // geht einwandfrei

Getter 1 liefert std::vector<std::vector<int> >& → veränderbar.

Getter 2 liefert const std::vector<std::vector<int> >& → nur lesbar.


Was bedeutet in C++03 explicit vor dem Konstruktor in seiner Deklaration ?
In C++ (ab Version 98, also auch in C++03) wirkt das Schlüsselwort explicit direkt vor der Konstruktor­deklaration so:

class Beispiel {
public:
    explicit Beispiel(int x);
};
Was explicit bewirkt
Verhindert implizite Konvertierungen
Ohne explicit könnte der Ein­argument­konstruktor auch als impliziter Konverter dienen. Zum Beispiel:

cpp
Kopieren
Bearbeiten
class OhneExplicit {
public:
    OhneExplicit(int x) { /* ... */ }
};

void foo(const OhneExplicit& o) { /* ... */ }

foo(42);  // OK: 42 wird automatisch zu OhneExplicit(42)
Mit explicit geht das nicht mehr:

cpp
Kopieren
Bearbeiten
class MitExplicit {
public:
    explicit MitExplicit(int x) { /* ... */ }
};

void bar(const MitExplicit& m) { /* ... */ }

bar(42);            // Fehler! Keine automatische Umwandlung
bar(MitExplicit(42)); // OK: explizit konvertiert
Schützt vor ungewollten Konstruktionen
Gerade bei Klassen, die Mehrdeutigkeiten oder stille Fehler verursachen könnten (z. B. Pfad- oder Ressourcen-Klassen, smart pointers, Wrapper), ist explicit sehr nützlich, um faule Implizit-Konvertierungen zu vermeiden.

Zusammenfassung
Ohne explicit: Ein Ein-Argument-Konstruktor kann automatisch überall dort eingesetzt werden, wo der Zielftyp erwartet wird → implizite Konvertierung.

Mit explicit: Solche Konvertierungen sind nur noch explizit erlaubt, d. h. du musst bewusst Klasse(x) schreiben.

Tipp: In C++11 gilt dasselbe auch für Mehr-Argument-Konstruktoren mit {}-Initializer. In C++03 betrifft es nur Ein-Argument-Konstruktoren.

Bedeutet ich muss bewusst den Konstruktor wie eine Funktion und Eingabeparameter, bei Angabe von  Explicit, aufrufen. Richtig ?


Mit explicit markierte Ein-Parameter-Konstruktoren kannst du nur aufrufen, wenn du den Konstruktor explizit wie eine ganz normale Funktion nutzt:

cpp
Kopieren
Bearbeiten
class MitExplicit {
public:
    explicit MitExplicit(int x) { /* ... */ }
};

MitExplicit a = 5;               // ❌ Fehler: implizite Konvertierung verboten
MitExplicit b(5);                // ✅ direkter Aufruf des Konstruktors
MitExplicit c = MitExplicit(5);  // ✅ ebenfalls erlaubt
Ohne explicit wäre auch MitExplicit a = 5; möglich – mit explicit musst du also bewusst eine der beiden erlaubten Formen (MitExplicit(5) oder Konstruktor-Aufruf in Klammern) verwenden.


"Mehrere Vergleichsbedingungen gleichzeitig" (kombinierte Checks)?








