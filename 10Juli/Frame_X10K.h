//---------------------------------------------------------------------------

#ifndef Frame_X10KH
#define Frame_X10KH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "DBAccess.hpp"
#include "MSAccess.hpp"
#include "NxCustomGrid.hpp"
#include "NxCustomGridControl.hpp"
#include "NxDBGrid.hpp"
#include "NxScrollControl.hpp"
#include <Data.DB.hpp>
#include "NxColumns.hpp"
#include "NxDBColumns.hpp"
#include <Vcl.DBGrids.hpp>
#include <Vcl.Grids.hpp>
#include "PBNumEdit.hpp"
#include <Vcl.ExtCtrls.hpp>
#include "Gradient.hpp"
#include <Vcl.Graphics.hpp>
#include <Vcl.Buttons.hpp>
#include "PJDropFiles.hpp"
#include "Frame_AttachmentManager.h"
#include <memory>
#include <vector>
//class TFormXML;
//---------------------------------------------------------------------------
#include "Frame_AttachmentManager.h"
class TFrameX10K : public TFrame
{
__published:	// Von der IDE verwaltete Komponenten
	TMSDataSource *DS;
	TLabel *Label1;
	TLabel *Label3;
	TLabel *Label5;
	TLabel *Label7;
	TPBNumEdit *NuEdTempU;
	TPBNumEdit *NuEdFeuchte;
	TCheckBox *CBKuMess;
	TLabel *Label2;
	TComboBox *ComBoxMessg;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *Label10;
	TGradient *GRD;
	TLabel *Label4;
	TEdit *EditOrt;
	TLabel *Label6;
	TPBNumEdit *NuEdTempDfg;
	TLabel *Label11;
	TMSDataSource *DS1;
	TMSDataSource *DS2;
	TMSDataSource *DS3;
	TPanel *PnlHeader;
	TSpeedButton *SpBtnC1;
	TSpeedButton *SpBtnMA;
	TSpeedButton *SpBtnTAP;
	TNextDBGrid *table1;
	TNextDBGrid *table2;
	TNextDBGrid *table3;
	TSpeedButton *SpBtnC1_Delete;
	TSpeedButton *SpBtnMA_Delete;
	TSpeedButton *SpBtnTA_Delete;
	TMemo *Memo1;
	TSpeedButton *table1RecZurDB;
	TSpeedButton *table2RecZurDB;
	TSpeedButton *table3RecZurDB;
	TSpeedButton *spbToXML;
	TComboBox *ComBoxAufbStatic;
	TMemo *memoInfo;
	TLabel *Label12;
	TLabel *lbTable1CountOfXMLUnused;
	TLabel *lbTable2CountOfXMLUnused;
	TLabel *lbTable3CountOfXMLUnused;
	TFrameAttachmentManager *FRattachm;
	void __fastcall CBKuMessClick(TObject *Sender);
	void __fastcall table4DblClick(TObject *Sender);
	void __fastcall SpBtnC1Click(TObject *Sender);
	void __fastcall SpBtnMAClick(TObject *Sender);
	void __fastcall SpBtnTAPClick(TObject *Sender);
	void __fastcall spbToXML1Click(TObject *Sender);
	void __fastcall table1OnCellFormating(TObject *Sender, int ACol, int ARow, WideString Value,
          TColor &TextColor, TFontStyles &FontStyle, TCellState CellState);
	void __fastcall table2OnCellFormating(TObject *Sender, int ACol, int ARow, WideString Value,
          TColor &TextColor, TFontStyles &FontStyle, TCellState CellState);
	void __fastcall table3OnCellFormating(TObject *Sender, int ACol, int ARow, WideString Value,
          TColor &TextColor, TFontStyles &FontStyle, TCellState CellState);
	void __fastcall table1OnCellClick(TObject *Sender, int ACol, int ARow);
	void __fastcall table2OnCellClick(TObject *Sender, int ACol, int ARow);
	void __fastcall table3OnCellClick(TObject *Sender, int ACol, int ARow);
	void __fastcall SpBtnC1Click_Delete(TObject *Sender);
	void __fastcall SpBtnMAClick_Delete(TObject *Sender);
	void __fastcall SpBtnTAPClick_Delete(TObject *Sender);
	void __fastcall table1OnSelectCell(TObject *Sender, int ACol, int ARow);
	void __fastcall Table1OnCellColoring(TObject *Sender, int ACol, int ARow, TColor &CellColor,
          TColor &GridColor, TCellState CellState);
	void __fastcall table1OnBeforeEdit(TObject *Sender, int ACol, int ARow, bool &Accept);
	void __fastcall table1OnEditAccept(TObject *Sender, int ACol, int ARow, WideString Value,
          bool &Accept);
	void __fastcall table1OnAfterEdit(TObject *Sender, int ACol, int ARow, WideString Value);
	void __fastcall table1RecZurDBOnClick(TObject *Sender);
	void __fastcall table2RecZurDBOnClick(TObject *Sender);
	void __fastcall table3RecZurDBOnClick(TObject *Sender);
	void __fastcall ComBoxMessgOnClick(TObject *Sender);
	void __fastcall ComBoxAufStaticOnClick(TObject *Sender);
	void __fastcall EditOrtOnChange(TObject *Sender);
	void __fastcall NuEdTempUOnChange(TObject *Sender);
	void __fastcall NuEdTempDfgOnChange(TObject *Sender);
	void __fastcall NuEdFeuchteOnChange(TObject *Sender);
	void __fastcall AttachManagerOnDropFiles(TObject *Sender);
	void __fastcall Memo1OnChange(TObject *Sender);
	void __fastcall table2OnSelectCell(TObject *Sender, int ACol, int ARow);
	void __fastcall table3OnSelectCell(TObject *Sender, int ACol, int ARow);
	void __fastcall Table2OnCellColoring(TObject *Sender, int ACol, int ARow, TColor &CellColor,
          TColor &GridColor, TCellState CellState);
	void __fastcall Table3OnCellColoring(TObject *Sender, int ACol, int ARow, TColor &CellColor,
          TColor &GridColor, TCellState CellState);
	void __fastcall table2OnBeforeEdit(TObject *Sender, int ACol, int ARow, bool &Accept);
	void __fastcall table3OnBeforeEdit(TObject *Sender, int ACol, int ARow, bool &Accept);
	void __fastcall table2OnEditAccept(TObject *Sender, int ACol, int ARow, WideString Value,
          bool &Accept);
	void __fastcall table3OnEditAccept(TObject *Sender, int ACol, int ARow, WideString Value,
          bool &Accept);
	void __fastcall table2OnAfterEdit(TObject *Sender, int ACol, int ARow, WideString Value);
	void __fastcall table3OnAfterEdit(TObject *Sender, int ACol, int ARow, WideString Value);
	void __fastcall table1OnVerticalScroll(TObject *Sender, int Position);
	void __fastcall table2OnVerticalScroll(TObject *Sender, int Position);
	void __fastcall table3OnVerticalScroll(TObject *Sender, int Position);







private:	// Benutzer-Deklarationen
public:		// Benutzer-Deklarationen
	__fastcall TFrameX10K(TComponent* Owner);
	__fastcall ~TFrameX10K();

	//TFormXML* FormXML; //RRR42

	int RowTable1;
	int ColTable1;
	int Col4RedTable1;
	int RowTable2;
	int ColTable2;
	int Col4RedTable2;
	int RowTable3;
	int ColTable3;
	int Col4RedTable3;

	int RowEditTable1;
	int RowEditTable2;
	int RowEditTable3;

    //Nur für die Datenermittling kein Einsatz in der GUI
	int RowSelectTable1;
	int ColSelectTable1;
	int RowSelectTable2;
	int ColSelectTable2;
	int RowSelectTable3;
	int ColSelectTable3;

	bool xml4table1GUI;
	bool xml4table2GUI;
	bool xml4table3GUI;

	bool xml4table1OneIsSelected;
	bool xml4table2OneIsSelected;
	bool xml4table3OneIsSelected;

	UnicodeString ustrXMLFile;

	//Fuer die GUI: Situation: Merken was im Grid von xml kommend je table steht
    //first: wenn noch nicht in die Datenbank verschoben wurde = 1; second wenn noch nicht editiert wurde dann 1
	//Zur Zeit aber second nicht genutzt
	std::vector < std::pair < int, int > > table1RememberOfXML;
	std::vector < std::pair < int, int > > table2RememberOfXML;
	std::vector < std::pair < int, int > > table3RememberOfXML;

	std::vector < std::pair < std::wstring, std::wstring > > XML_Name2DataBase_Name;

	//TEST erste wie oft; zweite zahlenwert z.B. SelectedCell
	std::vector < std::pair < int, int > > table1TESTHowOftenThis ;
	//TEST so zu ist; Beispiel Index in einer Schleife
	std::vector < std::pair < int, int > > table1TESTSollIst;
	//TEST so ein Wert mit einer Zeichenkette z.B. Funktionsname
	std::vector < std::pair < int, String > > table1TESTValueWithDescription;
	//TEST

	void __fastcall ShowFormXML();
	void __fastcall CreateAndShowForm();
	void __fastcall MarkAsDelete4Table(int TestArt);

	void __fastcall PracticalHeaderName(int tableNr);
	void __fastcall HandleIfDelete(int tableNr);
	int __fastcall HandleCountOfUnusedXMLRecords(int tableNr);
	int __fastcall GetCountOfUnusedXMLRecords(int tableNr);
	void __fastcall DragImportFile();

	void __fastcall IniTable1RememberOfXML();
	void __fastcall IniTable2RememberOfXML();
	void __fastcall IniTable3RememberOfXML();

	//Damit weniger Compilierzeit freiverfügbare während TestPhase
	void __fastcall GeneratePairs4Header();
	void __fastcall CompleteXMLAndSend(int Testart);

	void __fastcall MyMessageBox();

	TMSQuery *QC1;    // TAB-Inhalt
	TMSQuery *QMA;    // MA-Inhalt
	TMSQuery *QTAP;   // TAP-Inhalt
	TMSQuery *QH;     // Header-Inhalt

	void __fastcall Activate            (int ID, String OwnerFormName);         // Die Funktion Activate ist mehrmalig im Gegensatz zum
																				// Konstruktor. Erst nachdem alles im Konstruktor inititalisiert
																				// wird, werden mit Aktivate die Daten für das Protokoll vorbereitet.
	void __fastcall Ini4Activate();

	bool	 Initialized;
	bool ReadOnly;          // Wird als Flag für schreibgeschütze Protokolle verwendet. Aktuell in Vorbereitung.
							// Hierrüber wäre es auch möglich das Frame mit der Option "Enable" beim Lesen der Daten zu sperren.


	int  ProtokolID;         // Die von MS Server zugewiesene Protokollnummer aus der Protokolltabelle. Wird über die globale
							 // Struktur aus der ProdLaufkarte über die Variable PDS.REC_NR an das Frame X10K übergeben.

	void __fastcall CreateNewRecord     (int ID, int TestArt, TMSQuery *QTestArt);   // Kann verwendet werden um eine neue Zeile über Code zu generieren, hierbei wird
	void __fastcall CreateNewRecordDB     (int ID, int TestArt, TMSQuery *QTestArt);   // Kann verwendet werden um eine neue Zeile über Code zu generieren, hierbei wird

																					 // die Prod_Id übergeben
	void __fastcall ImportDataFromDB    ();     		 // Importiert den Datensatz aus der MS Tabelle. Die ID Nummer ist durch die Variable
															 // ProtokollID bekannt.

	void __fastcall ExportDataToDB      ();				// Exportiert den Datensatz aus der GUI in die MS Tabelle. Die ID Nummer ist durch
															// die Variable ProtokollID bekannt. Der Komplette Datensatz wird als Ganzes geschrieben.

	void __fastcall SetChangeFlag       ();     		//  Wird sofort altiv wenn geklickt oder geändert wird.

	bool __fastcall CheckAllFieldsSet   ();     		// Alle Pflichtfelder gesetzt ?

	void __fastcall InitTheHeader   ();

	void __fastcall CheckReadOnly   	(bool ReadOnly);     		// Feldfreigabe zum Editieren

};
//---------------------------------------------------------------------------
extern PACKAGE TFrameX10K *FrameX10K;
//---------------------------------------------------------------------------
#endif
