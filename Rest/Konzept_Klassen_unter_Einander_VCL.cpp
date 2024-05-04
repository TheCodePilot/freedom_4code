
#include "Frame01.h"   // wegen Funktionen  // !!! Wichtig
		class TFrame4XML;   // !!! Wichtig
class TFormXML : public TForm
//...
TFrame4XML* MyFrame;
//...


______________


#include "Neu_XML_TDBGrid.h" //von Forms das Gegenstück   //!!!Wichtig
class TFormXML; //ganz wichtig !!!
class TFrame4XML : public TFrame
{

//...
private:	// Benutzer-Deklarationen
	//TFormXML* Form4XML;
	std::unique_ptr<TFormXML> Form4XML;
//...

_______________________

_fastcall TFrame4XML::TFrame4XML(TComponent* Owner)
	: TFrame(Owner), Form4XML(new TFormXML(this))
{

}
//---------------------------------------------------------------------------
void __fastcall TFrame4XML::cbnXMLOnClick(TObject *Sender)
{
	ShowMyForm();
}
//---------------------------------------------------------------------------

void __fastcall TFrame4XML::ShowMyForm()
{
	Form4XML->MyFrame = this;
	Form4XML->IchFormXML();
	Form4XML->ShowModal();

}

___________________________________________________________

void __fastcall TFormXML::XMLShow(TObject *Sender)
{
	TFormXML_ (this);
	MyFrame->Ichframe();

}	