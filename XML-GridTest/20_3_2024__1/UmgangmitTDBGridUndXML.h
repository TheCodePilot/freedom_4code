 //---------------------------------------------------------------------------

#ifndef UmgangmitTDBGridUndXMLH
#define UmgangmitTDBGridUndXMLH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Data.DB.hpp>
#include <Datasnap.DBClient.hpp>
#include <Vcl.DBGrids.hpp>
#include <Vcl.Grids.hpp>
//---------------------------------------------------------------------------
#include <vector>
class TForm5 : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TDBGrid *DBGrid4XMLin;
	TClientDataSet *ClientDataSet1;
	TDataSource *DataSource1;
private:	// Benutzer-Deklarationen
public:		// Benutzer-Deklarationen
	__fastcall TForm5(TComponent* Owner);
	void __fastcall FillTDBGridFromXML(std::vector<std::pair<std::wstring, std::wstring> > xmlProcess);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm5 *Form5;
//---------------------------------------------------------------------------
#endif
