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
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtDlgs.hpp>
//---------------------------------------------------------------------------
#include <vector>
class XMLProcess;
class TForm5 : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TDBGrid *DBGrid4XMLin;
	TClientDataSet *ClientDataSet1;
	TDataSource *DataSource1;
	TOpenTextFileDialog *OpenTextFileDialog1;
	TEdit *Edit1;
	void __fastcall DBGrid4XMLinEndDrag(TObject *Sender, TObject *Target, int X, int Y);
	void __fastcall Edit1DragDrop(TObject *Sender, TObject *Source, int X, int Y);
	void __fastcall DBGrid4XMLinStartDrag(TObject *Sender, TDragObject *&DragObject);
	void __fastcall Edit1DragOver(TObject *Sender, TObject *Source, int X, int Y, TDragState State,
          bool &Accept);
	void __fastcall DBGrid4XMLinOnCellClick(TColumn *Column);
	void __fastcall DBGrid4XMLinDblclick(TObject *Sender);
	void __fastcall DBGrid4XMLinDrawColumnCell(TObject *Sender, const TRect &Rect, int DataCol,
          TColumn *Column, TGridDrawState State);
	void __fastcall DBGrid4XMLinMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);


private:	// Benutzer-Deklarationen
	void __fastcall FillTDBGridFromXML(XMLProcess* xmlProcess);
public:		// Benutzer-Deklarationen
	__fastcall TForm5(TComponent* Owner);
	void __fastcall XML2Grid(XMLProcess* xmlProcess);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm5 *Form5;
//---------------------------------------------------------------------------
#endif
