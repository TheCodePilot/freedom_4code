//---------------------------------------------------------------------------

#ifndef Frame01H
#define Frame01H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "NxCustomGrid.hpp"
#include "NxCustomGridControl.hpp"
#include "NxDBGrid.hpp"
#include "NxScrollControl.hpp"
#include "NxGrid.hpp"
#include "NxColumns.hpp"
#include "NxDBColumns.hpp"
#include "DBAccess.hpp"
#include "MemDS.hpp"
#include "MSAccess.hpp"
#include <Data.DB.hpp>
#include <Datasnap.DBClient.hpp>

#include <memory>
//---------------------------------------------------------------------------
#include "Neu_XML_TDBGrid.h" //von Forms das Gegenstück   //!!!Wichtig
class TFormXML; //ganz wichtig !!!
class TFrame4XML : public TFrame
{
__published:	// Von der IDE verwaltete Komponenten
	TButton *Button1;
	TNextGrid *Ng;
	TNextDBGrid *NgDB;
	TClientDataSet *CDS_Test;
	TMSDataSource *DS;
	TMSQuery *QC1;
	TNextDBGrid *NgDB1;
	TMSConnection *MSCon;
	TButton *Button2;
	void __fastcall cbnXMLOnClick(TObject *Sender);
	void __fastcall cbnSQL_Query(TObject *Sender);
private:	// Benutzer-Deklarationen

	TFormXML* Form4XML;
public:		// Benutzer-Deklarationen

	__fastcall ~TFrame4XML();
	__fastcall TFrame4XML(TComponent* Owner);
	void __fastcall CreateAndShowForm();

	__fastcall void IniDB();
	__fastcall void BereitZumSQL_Quest();
	__fastcall int SendSQLLine (AnsiString Line, TMSQuery *SQ, int showmessage = 0);

	//void __fastcall Ichframe(){ShowMessage("Ich bin ein Frame");}
};
//---------------------------------------------------------------------------
extern PACKAGE TFrame4XML *Frame4XML;
//---------------------------------------------------------------------------
#endif
