
Entscheidend ist: privater Konstruktor
	__fastcall TFormXML(TComponent* Owner);
public:		// Benutzer-Deklarationen



wichtig nachträglich keine Pointer nehmen !!!

mit Instanzen reichen Referenzen

#include "NxColumnClasses.hpp"
#include "NxColumns.hpp"
#include "NxCustomGrid.hpp"
#include "NxCustomGridControl.hpp"
#include "NxGrid.hpp"
#include "NxScrollControl.hpp"
#include "NxDBGrid.hpp"
#include "NxDBColumns.hpp"

QW=new TMSQuery(NULL);
	DSRC->DataSet=QW; TABwnr->DataSource=DSRC;
	
	
Insgesamt aus:
//===========================================================================================================================================================================
void __fastcall TForm3_WnrSonder::FormShow(TObject *Sender)
//===========================================================================================================================================================================
{
	// Individuelle Privilegien
	bool IndivPak = Form7->CheckPriv2("SonderPAK",1,0);


	if(Wds==NULL) Wds=new C_WnrDataSet();

	QW=new TMSQuery(NULL);
	DSRC->DataSet=QW; TABwnr->DataSource=DSRC;

	LBservice->Enabled    =( PID.MDEGROUP.Pos("HB")>0 || PID.MDEGROUP.Pos("Q")==1 || PID.BEREICH.Pos("OIT")==1 || PID.MDEGROUP.Pos("S")  ==1);
	BNclearStatus->Enabled=(                             PID.MDEGROUP.Pos("Q")==1 || PID.BEREICH.Pos("OIT")==1 || PID.MDEGROUP=="HRV" || PID.MDEGROUP=="HR");
	PNprotokolle->Enabled =(                             PID.MDEGROUP.Pos("Q")==1 || PID.BEREICH.Pos("OIT")==1);

	// Personenbezogne Ausnahmen (keine Berechtigung für Pakerei)
	PNwnr       ->Enabled = ( !IndivPak);
	PNKennz     ->Enabled = ( !IndivPak);

	QW->ReadOnly          =!PNprotokolle->Enabled;

	Label30->Caption=" Weitere Funktionen für "+IntToStr(AUSW_WNR->Count)+" Produkt(e) / Werknummer(n)";

	Wnr1=AUSW_WNR->Strings[0]; LBwnr->Caption=Wnr1;
	Wnr2="";

	EDnewWnr->Text        = Wnr1;
	EDnewWnr->Enabled     =(AUSW_WNR->Count==1);
	BNchangeWnr->Enabled  =(AUSW_WNR->Count==1);
	BNyy1->Enabled        =(AUSW_WNR->Count==1);
	BNpsi->Enabled        =(AUSW_WNR->Count==1);
	BNqsperren->Enabled   =(AUSW_WNR->Count==1 && Form7->CheckPriv("QMB-","",""));

	EDstatusNeu->Text="";
	EDletzteMassnahmeNeu->Text="";
	EDnaechsteMassnahmeNeu->Text="";
	EDortNeu->Text="";
	EDpsi->Text="";
	StatusNr=0;
	TargetWnrExists=true;
	LBwnrAktTeil->Caption="";
	LBanrAktTeil->Caption="";
	CBallowNewWnr->Checked=false;
	ChangedWnrTable=false;

	if(TABstatus->RowCount<3)
	{
	  int idx=0;
	  TMSQuery *QQ=new TMSQuery(NULL);

	  if(Form1->SendSQLLine("SELECT * FROM dbo.APLprozesse where Active = 'true' order by id",QQ))
	  while(!QQ->Eof)
	  {
		TABstatus->AddRow(1);
		TABstatus->Cell[0][idx]->AsString =QQ->FieldByName("Status")->AsString;
		TABstatus->Cell[1][idx]->AsString =QQ->FieldByName("Name")->AsString;
		TABstatus->Cell[2][idx]->AsInteger=QQ->FieldByName("ID")->AsInteger;
		TABstatus->Cell[3][idx]->AsString =QQ->FieldByName("APLgruppe")->AsString;
		TABstatus->Cell[4][idx]->AsString =QQ->FieldByName("APL")->AsString;
		QQ->Next(); idx++;
	  }

	  delete QQ;
	}

	BringToFront();

	ActiveControl=EDstatusNeu;

	UpdateForm();

	Timer->Enabled=true;
}	


//===========================================================================================================================================================================
void __fastcall TForm3_WnrSonder::FillTABprot(TNextGrid *TAB, String Wnr)
//===========================================================================================================================================================================
{
TMSQuery *QQ=new TMSQuery(NULL);

		if(Form1->ProtListToQuery(QQ,NULL,Wnr,0,"",false,false,false,false)>0) // Protokoll-Bereich füllen
		{
		   int idx=0;
		   TAB->ClearRows();

		   while(!QQ->Eof)
		   {
			  TAB->AddRow(1);
			  TAB->Cell[0][idx]->AsString =QQ->FieldByName("X_TYP")->AsString;
			  TAB->Cell[1][idx]->AsString =Form1->GetKom4Line(TAB->Cell[0][idx]->AsString);
			  TAB->Cell[2][idx]->AsString =QQ->FieldByName("WERKNR")->AsString;
			  TAB->Cell[3][idx]->AsString =QQ->FieldByName("TS")->AsString;
			  TAB->Cell[4][idx]->AsInteger=QQ->FieldByName("ID")->AsInteger;

			  QQ->Next(); idx++;
		   }
		}

		delete QQ;
}

siehe:
C:\CPP_Builder_Sydney\Projekte\00_MDELive_Sydney\Units_Tools\Unit3_WnrSonder.cpp [1]