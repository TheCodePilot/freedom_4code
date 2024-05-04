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
#include <memory>
#include <vector>
class TFormXML;
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
	TComboBox *ComBoxAufb;
	TLabel *Label11;
	TMSDataSource *DS1;
	TMSDataSource *DS2;
	TMSDataSource *DS3;
	TPanel *PnlHeader;
	TSpeedButton *SpBtnC1;
	TSpeedButton *SpBtnMA;
	TSpeedButton *SpBtnTAP;
	TSpeedButton *spbToXML;
	TNextDBGrid *table1;
	TNextDBGrid *table2;
	TNextDBGrid *table3;
	TSpeedButton *SpBtnC1_Delete;
	TSpeedButton *SpBtnMA_Delete;
	TSpeedButton *SpBtnTA_Delete;
	TSpeedButton *SpBtnC1_Prepare;
	TSpeedButton *SpBtnMA_Prepare;
	TSpeedButton *SpBtnTAP_Prepare;
	TPJDropFiles *PJDropFiles;
	TMemo *Memo1;
	TSpeedButton *table1RecZurDB;
	TSpeedButton *table2RecZurDB;
	TSpeedButton *table3RecZurDB;
	TSpeedButton *TBuBear;
	void __fastcall CBKuMessClick(TObject *Sender);
	void __fastcall table4DblClick(TObject *Sender);
	void __fastcall SpBtnC1Click(TObject *Sender);
	void __fastcall SpBtnMAClick(TObject *Sender);
	void __fastcall SpBtnTAPClick(TObject *Sender);
	void __fastcall spbToXMLClick(TObject *Sender);
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
	void __fastcall SpBtnC1Click_Prepare(TObject *Sender);
	void __fastcall SpBtnMAClick_Prepare(TObject *Sender);
	void __fastcall SpBtnTAPClick_Prepare(TObject *Sender);
	void __fastcall table1OnSelectCell(TObject *Sender, int ACol, int ARow);
	void __fastcall Table1OnCellColoring(TObject *Sender, int ACol, int ARow, TColor &CellColor,
          TColor &GridColor, TCellState CellState);
	void __fastcall table1OnBeforeEdit(TObject *Sender, int ACol, int ARow, bool &Accept);
	void __fastcall table1OnEditAccept(TObject *Sender, int ACol, int ARow, WideString Value,
          bool &Accept);
	void __fastcall table1OnDblClick(TObject *Sender);
	void __fastcall table1OnEditExit(TObject *Sender);
	void __fastcall table1OnEndEditing(TObject *Sender, int ACol, int ARow, WideString &Value,
          bool &Accept);
	void __fastcall table1OnAfterEdit(TObject *Sender, int ACol, int ARow, WideString Value);
	void __fastcall table1RecZurDBOnClick(TObject *Sender);
	void __fastcall table2RecZurDBOnClick(TObject *Sender);
	void __fastcall table3RecZurDBOnClick(TObject *Sender);






private:	// Benutzer-Deklarationen
public:		// Benutzer-Deklarationen
	__fastcall TFrameX10K(TComponent* Owner);
	__fastcall ~TFrameX10K();

	TFormXML* Form4XML; //RRR42

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
	int ColEditTable1;
	int RowEditTable2;
	int ColEditTable2;
	int RowEditTable3;
	int ColEditTable3;

	bool xml4table1GUI;
	bool xml4table2GUI;
	bool xml4table3GUI;

	bool prepare4table1GUI;
	bool prepare4table2GUI;
	bool prepare4table3GUI;

	//Fuer die GUI: Situation: Merken was im Grid von xml kommend je table steht
	std::vector < std::pair < int, int > > table1RememberOfXML;
	std::vector < std::pair < int, int > > table2RememberOfXML;
	std::vector < std::pair < int, int > > table3RememberOfXML;

	std::vector < std::pair < std::wstring, std::wstring > > XML_Name2DataBase_Name;

	//Für Attachment
	std::unique_ptr<TFrameAttachmentManager> FRattachm;

	void __fastcall IchBin();
	void __fastcall ShowFormXML();
	void __fastcall CreateAndShowForm();
	void __fastcall MarkAsDelete4Table(int TestArt);

	bool EditAccept;

	void __fastcall PracticalHeaderName();
	void __fastcall PracticalValues(int tableNumber);
	void __fastcall DragImportFile();

	void __fastcall IniTable1RememberOfXML();
	void __fastcall IniTable2RememberOfXML();
	void __fastcall IniTable3RememberOfXML();

	//Damit weniger Compilierzeit freiverfügbare während TestPhase
	void __fastcall FindIDColumn();
	void __fastcall GeneratePairs4Header();
	void __fastcall CompleteXMLAndSend(int Testart);
	std::wstring __fastcall GetPracticalName(std::wstring xml_Name);
	std::wstring __fastcall GetDataBaseName(std::wstring db_Name);
	std::wstring __fastcall GetDataBaseNameOverIndex(int index);



	void __fastcall FTest4();
	void __fastcall FTest5();
	void __fastcall FTest6();

	int valueTest1;
	int valueTest2;
	int valueTest3;


	TMSQuery *QC1;    // TAB-Inhalt
	TMSQuery *QMA;    // MA-Inhalt
	TMSQuery *QTAP;   // TAP-Inhalt
	TMSQuery *QH;     // Header-Inhalt

	void __fastcall Activate            (int ID, String OwnerFormName);         // Die Funktion Activate ist mehrmalig im Gegensatz zum
																				// Konstruktor. Erst nachdem alles im Konstruktor inititalisiert
																				// wird, werden mit Aktivate die Daten für das Protokoll vorbereitet.

	bool	 Initialized;


	bool Changed;           // Bei Änderungen (oder beim Ereignis Klick in das Tabellenfeld) wird das Flag auf True gesetzt
							// Textfelder haben den graphischen (Ereignis) dass sie Änderungen melden. Nachteil ist, dass auch bei
							// Programmänderungen im Code das Flag gesetzt wird.

	bool ReadOnly;          // Wird als Flag für schreibgeschütze Protokolle verwendet. Aktuell in Vorbereitung.
							// Hierrüber wäre es auch möglich das Frame mit der Option "Enable" beim Lesen der Daten zu sperren.


	int  ProtokolID;         // Die von MS Server zugewiesene Protokollnummer aus der Protokolltabelle. Wird über die globale
							 // Struktur aus der ProdLaufkarte über die Variable PDS.REC_NR an das Frame X10K übergeben.

	void __fastcall CreateNewRecord     (int ID, int TestArt, TMSQuery *QTestArt, int markedASdestroyed);   // Kann verwendet werden um eine neue Zeile über Code zu generieren, hierbei wird
																					 // die Prod_Id übergeben


	void __fastcall ImportDataFromDB    (void);     		 // Importiert den Datensatz aus der MS Tabelle. Die ID Nummer ist durch die Variable
															 // ProtokollID bekannt.

	void __fastcall ExportDataToDB      (void);				// Exportiert den Datensatz aus der GUI in die MS Tabelle. Die ID Nummer ist durch
															// die Variable ProtokollID bekannt. Der Komplette Datensatz wird als Ganzes geschrieben.

	void __fastcall SetChangeFlag       (void);     		//  Wird sofort altiv wenn geklickt oder geändert wird.

	void __fastcall CheckAllFieldsSet   (void);     		// Alle Pflichtfelder gesetzt ?

	void __fastcall BuildDefaultTable   (void);

	void __fastcall CheckReadOnly   	(bool ReadOnly);     		// Feldfreigabe zum Editieren

};
//---------------------------------------------------------------------------
extern PACKAGE TFrameX10K *FrameX10K;
//---------------------------------------------------------------------------
#endif
