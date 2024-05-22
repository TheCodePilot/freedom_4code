#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <locale>
#include <codecvt>

// Funktion zum Skalieren und Formatieren
std::wstring formatMeasurement(std::wstring symbol, std::wstring unit, int precision, double value) {
    // Definiere die Skalenfaktoren und die entsprechenden Einheiten
    const double factors[] = {1.0, 1e3, 1e6, 1e9, 1e12, 1e15, 1e18, 1e21, 1e24};
    const std::wstring prefixes[] = {L"", L"m", L"µ", L"n", L"p", L"f", L"a", L"z", L"y"};
    const int numFactors = sizeof(factors) / sizeof(factors[0]);
    std::wstring result;

    // Finde den Index des aktuellen Präfixes
    int symbolIndex = 0;
    for (int i = 0; i < numFactors; ++i) {
        if (prefixes[i] == symbol) {
            symbolIndex = i;
            break;
        }
    }

    // Skaliere den Wert
    int scaleIndex = symbolIndex;
    double scaledValue = value;
    while (scaledValue < 1.0 && scaleIndex < numFactors - 1) {
        scaleIndex++;
        scaledValue *= 1e3;
    }

    // Berechne den neuen Präfix-Index
    int newSymbolIndex = symbolIndex + (scaleIndex - symbolIndex);
    if (newSymbolIndex >= numFactors) {
        newSymbolIndex = numFactors - 1; // Begrenze den Index, um nicht über das Array hinauszugehen
    }

    // Formatieren des Wertes
    std::wstringstream ss;
    ss << std::fixed << std::setprecision(std::min(precision, 9)) << scaledValue;
    std::wstring formattedValue = ss.str();

    // Ergebnis zusammenstellen
    result = formattedValue + L" " + prefixes[newSymbolIndex] + unit;
    return result;
}

int main() {
    // Set locale for wide character output
    std::setlocale(LC_ALL, "");

    // Beispielwerte
    std::wstring symbol = L"m"; // Beispiel: Milli
    std::wstring unit = L"m"; // Beispiel: Meter
    int precision = 9;
    double value = 0.00005194;

    // Ergebnis formatieren
    std::wstring result = formatMeasurement(symbol, unit, precision, value);
    std::wcout.imbue(std::locale(""));
    std::wcout << result << std::endl;

    return 0;
}


//--------------------------
////NEU
//--------------------------

#include <iostream>
#include <string>
#include <sstream>

// Funktion zur Konvertierung von std::wstring in int
int wstringToInt(const std::wstring& str) {
    std::wstringstream wss(str);
    int result;
    wss >> result;
    if (wss.fail()) {
        std::wcerr << L"Konvertierungsfehler bei int: " << str << std::endl;
        return 0; // oder eine andere geeignete Fehlerbehandlung
    }
    return result;
}

// Funktion zur Konvertierung von std::wstring in double
double wstringToDouble(const std::wstring& str) {
    std::wstringstream wss(str);
    double result;
    wss >> result;
    if (wss.fail()) {
        std::wcerr << L"Konvertierungsfehler bei double: " << str << std::endl;
        return 0.0; // oder eine andere geeignete Fehlerbehandlung
    }
    return result;
}

//---------------
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>

// Funktion zum Skalieren und Formatieren
std::wstring formatMeasurement(std::wstring symbol, std::wstring unit, int precision, double value) {
    // Definiere die Skalenfaktoren und die entsprechenden Einheiten
    const double factors[] = {1.0, 1e3, 1e6, 1e9, 1e12, 1e15, 1e18, 1e21, 1e24};
    const std::wstring prefixes[] = {L"", L"m", L"µ", L"n", L"p", L"f", L"a", L"z", L"y"};
    const int numFactors = sizeof(factors) / sizeof(factors[0]);
    std::wstring result;

    switch (symbol[0]) {
        case L'%':
            {
                double promilleValue = value * 10; // Umwandlung von Prozent zu Promille
                std::wstringstream ss;
                ss << std::fixed << std::setprecision(std::min(precision, 9)) << promilleValue;
                result = ss.str() + L" ‰"; // Verwende das Promille-Symbol
                break;
            }
        default:
            {
                // Finde die passende Skala
                int scaleIndex = 0;
                double scaledValue = value;
                while (scaledValue < 1.0 && scaleIndex < numFactors - 1) {
                    scaleIndex++;
                    scaledValue *= 1e3;
                }

                // Formatieren des Wertes
                std::wstringstream ss;
                ss << std::fixed << std::setprecision(std::min(precision, 9)) << scaledValue;
                std::wstring formattedValue = ss.str();

                // Ergebnis zusammenstellen
                result = formattedValue + L" " + prefixes[scaleIndex] + symbol + unit;
                break;
            }
    }

    return result;
}

int main() {
    // Beispielwerte
    std::wstring symbol = L"%";
    std::wstring unit = L"A";
    int precision = 9;
    double value = 0.005194; // Beispielwert in Prozent

    // Ergebnis formatieren
    std::wstring result = formatMeasurement(symbol, unit, precision, value);
    std::wcout << result << std::endl;

    return 0;
}
