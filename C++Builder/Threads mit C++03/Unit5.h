//---------------------------------------------------------------------------

#ifndef Unit5H
#define Unit5H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TForm5 : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
private:	// Benutzer-Deklarationen
public:		// Benutzer-Deklarationen
	__fastcall TForm5(TComponent* Owner);
	void TestUmgebung();
    void TestUmgebung_ThreadManuellStarten();
	/*void TestUmgebung1();
	void TestUmgebung2();
	void asLamdaShowThreadMessage();*/
};
//---------------------------------------------------------------------------
extern PACKAGE TForm5 *Form5;
//---------------------------------------------------------------------------
#endif

/*
Vorteile:
Kein direkter Zugriff auf Synchronize erforderlich: Die Methode ist direkt in der TThread-Klasse enthalten.
Saubere Trennung von Hintergrund- und GUI-Operationen.
GUI-Operationen sind tabu: Falls Sie von Execute() aus mit der Benutzeroberfläche interagieren möchten, sollten Sie dies mit Synchronize()
oder Queue() tun, um sicherzustellen, dass die Operation im Hauptthread läuft.
*/
