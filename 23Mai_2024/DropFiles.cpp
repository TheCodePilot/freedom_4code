Hallo ChatGP. Kennst Du Dich mit TPJDropFiles im C++ Builder mittels C++ aus?
ChatGPT
Ja, ich kenne mich mit der Verwendung von TPJDropFiles im C++ Builder aus. TPJDropFiles ist eine Komponente, die es ermöglicht, Dateien per Drag & Drop auf ein Formular oder eine andere visuelle Komponente zu ziehen und deren Pfade zu erfassen. Diese Komponente ist besonders nützlich, um Anwendungen benutzerfreundlicher zu gestalten, indem sie Drag & Drop-Funktionalität unterstützt.

Hier ist ein Beispiel, wie Du TPJDropFiles in einem C++ Builder-Projekt verwenden kannst:

Komponente hinzufügen: Stelle sicher, dass Du die TPJDropFiles-Komponente installiert hast. Falls nicht, lade sie von der Projekt-Jedi-Website herunter und installiere sie in Deinem C++ Builder.

Komponente auf Formular platzieren: Ziehe die TPJDropFiles-Komponente auf Dein Formular. Die Komponente ist nicht visuell, daher erscheint sie nur in der Komponentenleiste am unteren Rand des Formulardesigners.

Ereignisbehandlung: Füge einen Event-Handler für das OnDropFiles-Ereignis der TPJDropFiles-Komponente hinzu. Dies ist das Ereignis, das ausgelöst wird, wenn Dateien auf das Formular gezogen und losgelassen werden.

Hier ist ein Beispielcode, der zeigt, wie das gemacht wird:

cpp
Code kopieren
#include <vcl.h>
#pragma hdrstop
#include "Unit1.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

// ---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner) : TForm(Owner)
{
}

// Ereignishandler für das OnDropFiles-Ereignis
void __fastcall TForm1::PJDropFiles1DropFiles(TObject *Sender, const TJPFNAArray FileNames)
{
    // Hier werden die Pfade der gezogenen Dateien verarbeitet
    String message = "Dropped files:\n";
    for (int i = 0; i < FileNames.Length; i++) {
        message += FileNames[i] + "\n";
    }

    // Zeige die Dateipfade in einer Nachricht an
    ShowMessage(message);
}
In diesem Beispiel wird eine Nachricht angezeigt, die die Pfade der auf das Formular gezogenen Dateien enthält.

Schritt-für-Schritt-Anleitung:
Projekt erstellen: Erstelle ein neues VCL Forms Application-Projekt in C++ Builder.

TPJDropFiles hinzufügen: Füge die TPJDropFiles-Komponente zu Deinem Formular hinzu.

Ereignishandler erstellen: Doppelklicke auf die TPJDropFiles-Komponente im Formular, um den Ereignishandler für OnDropFiles zu erstellen.

Code hinzufügen: Füge den oben gezeigten Code in die entsprechende Ereignishandler-Methode (PJDropFiles1DropFiles) ein.

Projekt kompilieren und ausführen: Kompiliere und starte das Projekt. Ziehe einige Dateien auf das Formular und lasse sie los. Du solltest die Pfade der Dateien in einer Nachricht sehen.

Wenn Du spezielle Fragen oder Probleme hast, lass es mich wissen, und ich helfe Dir gerne weiter!

Danke ist es auch möglich wenn im Code eine Pfadangabe bekannt ist das ich ein File erhalte um es per Drag und Drop irgendwo zu öffnen. Beispiel PDF ?
ChatGPT
Ja, es ist möglich, eine Datei aus einem bekannten Pfad per Drag & Drop zu erstellen und sie in eine andere Anwendung zu ziehen, z.B. eine PDF-Datei. Hier ist ein Beispiel, wie Du das in C++ Builder umsetzen kannst.

Schritt-für-Schritt-Anleitung:
Komponente hinzufügen: Füge eine Schaltfläche (TButton) und die TPJDropFiles-Komponente zu Deinem Formular hinzu.

Drag & Drop Initialisierung: Verwende die Windows-API, um Drag & Drop zu initialisieren. Dazu musst Du den Pfad der Datei in ein DROPFILES-Struktur verpacken und die Drag & Drop-Aktionen starten.

Hier ist der Beispielcode:

cpp
Code kopieren
#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include "Unit1.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

// ---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner) : TForm(Owner)
{
}

// Funktion, um eine Datei per Drag & Drop zu ziehen
void __fastcall TForm1::StartDragDrop(String filePath)
{
    // Konvertiere den Pfad in ein WideString
    WideString wideFilePath = filePath;

    // Berechne die Größe der DROPFILES-Struktur und des Dateipfads
    int size = sizeof(DROPFILES) + (wideFilePath.Length() + 1) * sizeof(wchar_t);

    // Erstelle einen globalen Speicherblock für die DROPFILES-Struktur
    HGLOBAL hGlobal = GlobalAlloc(GHND | GMEM_SHARE, size);
    if (hGlobal)
    {
        // Sperre den globalen Speicherblock und erhalte einen Zeiger darauf
        DROPFILES* pDropFiles = (DROPFILES*)GlobalLock(hGlobal);
        if (pDropFiles)
        {
            // Initialisiere die DROPFILES-Struktur
            pDropFiles->pFiles = sizeof(DROPFILES);
            pDropFiles->pt.x = 0;
            pDropFiles->pt.y = 0;
            pDropFiles->fNC = TRUE;
            pDropFiles->fWide = TRUE; // Verwende WideString

            // Kopiere den Dateipfad in den Speicherblock nach der DROPFILES-Struktur
            memcpy((BYTE*)pDropFiles + sizeof(DROPFILES), wideFilePath.c_bstr(), (wideFilePath.Length() + 1) * sizeof(wchar_t));

            // Schließe den globalen Speicherblock
            GlobalUnlock(hGlobal);

            // Starte die Drag & Drop-Operation
            IDataObject* pDataObject;
            IDropSource* pDropSource;
            HRESULT hr;

            // Erstelle ein IDataObject
            hr = CoCreateInstance(CLSID_DataObject, NULL, CLSCTX_INPROC_SERVER, IID_IDataObject, (void**)&pDataObject);
            if (SUCCEEDED(hr))
            {
                // Erstelle ein IDropSource
                hr = CoCreateInstance(CLSID_DropSource, NULL, CLSCTX_INPROC_SERVER, IID_IDropSource, (void**)&pDropSource);
                if (SUCCEEDED(hr))
                {
                    // Füge den globalen Speicherblock zu IDataObject hinzu
                    FORMATETC fmtetc = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
                    STGMEDIUM stgmed = { TYMED_HGLOBAL, { hGlobal }, NULL };
                    pDataObject->SetData(&fmtetc, &stgmed, TRUE);

                    // Starte die Drag & Drop-Operation
                    DWORD dwEffect;
                    hr = DoDragDrop(pDataObject, pDropSource, DROPEFFECT_COPY | DROPEFFECT_MOVE, &dwEffect);

                    // Freigeben von IDropSource
                    pDropSource->Release();
                }
                // Freigeben von IDataObject
                pDataObject->Release();
            }
            // Freigeben des globalen Speicherblocks
            GlobalFree(hGlobal);
        }
    }
}

// Ereignishandler für den Button
void __fastcall TForm1::Button1Click(TObject *Sender)
{
    // Hier den Pfad der Datei angeben, die gezogen werden soll
    String filePath = "C:\\Pfad\\zur\\Datei.pdf";
    StartDragDrop(filePath);
}
Erklärung:
StartDragDrop: Diese Methode initialisiert den Drag & Drop-Vorgang. Sie erstellt eine DROPFILES-Struktur und füllt sie mit dem Pfad der Datei, die gezogen werden soll.

Button1Click: Dieser Ereignishandler wird aufgerufen, wenn die Schaltfläche geklickt wird. Er startet den Drag & Drop-Vorgang für die angegebene Datei.

CoCreateInstance, IDataObject, IDropSource: Diese Funktionen und Schnittstellen werden verwendet, um die Drag & Drop-Datenstruktur zu erstellen und die Drag & Drop-Operation zu starten.

Stelle sicher, dass Dein Projekt so konfiguriert ist, dass es die Windows-API-Funktionen und -Typen unterstützt, und dass die OLE-Initialisierung korrekt erfolgt (z.B. OleInitialize in der WinMain-Funktion).

Mit diesem Code kannst Du eine Datei per Drag & Drop aus Deiner Anwendung herausziehen und in eine andere Anwendung einfügen, z.B. eine PDF-Datei in einen PDF-Viewer.

//-----------------------------
//
//-----------------------------

Code-Beispiel zur Behandlung des OnDropFiles-Ereignisses:
OnDropFiles-Ereignisbehandlung:
cpp
Code kopieren
void __fastcall TForm1::PJDropFiles1DropFiles(TObject *Sender)
{
    TPJDropFiles *dropFiles = dynamic_cast<TPJDropFiles*>(Sender);
    if (dropFiles != nullptr)
    {
        // Erhalte die Liste der gezogenen Dateien
        int fileCount = dropFiles->FileCount;
        String message = "Dropped files:\n";
        for (int i = 0; i < fileCount; i++)
        {
            message += dropFiles->Files[i] + "\n";
        }

        // Zeige die Dateipfade in einer Nachricht an
        ShowMessage(message);
    }
}
Schritt-für-Schritt-Anleitung:
Komponente hinzufügen: Stelle sicher, dass Du die TPJDropFiles-Komponente zu Deinem Formular hinzugefügt hast.

Ereignishandler erstellen: Doppelklicke auf die TPJDropFiles-Komponente im Formular, um den Ereignishandler für OnDropFiles zu erstellen.

Code hinzufügen: Füge den oben gezeigten Code in die entsprechende Ereignishandler-Methode (PJDropFiles1DropFiles) ein.

Erklärung:
TPJDropFiles-Zugriff: Die TPJDropFiles-Komponente wird durch das Sender-Objekt übergeben, das in den Ereignishandler als TPJDropFiles umgewandelt wird.
FileCount und Files: Die FileCount-Eigenschaft gibt die Anzahl der gezogenen Dateien an, und die Files-Eigenschaft enthält die Dateipfade.
Drag & Drop einer bekannten Datei:
Hier ist der Code, um eine Datei aus einem bekannten Pfad per Drag & Drop zu erstellen, wie zuvor beschrieben:

Drag & Drop Initialisierung:
cpp
Code kopieren
#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include "Unit1.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

// ---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner) : TForm(Owner)
{
}

// Funktion, um eine Datei per Drag & Drop zu ziehen
void __fastcall TForm1::StartDragDrop(String filePath)
{
    // Konvertiere den Pfad in ein WideString
    WideString wideFilePath = filePath;

    // Berechne die Größe der DROPFILES-Struktur und des Dateipfads
    int size = sizeof(DROPFILES) + (wideFilePath.Length() + 1) * sizeof(wchar_t);

    // Erstelle einen globalen Speicherblock für die DROPFILES-Struktur
    HGLOBAL hGlobal = GlobalAlloc(GHND | GMEM_SHARE, size);
    if (hGlobal)
    {
        // Sperre den globalen Speicherblock und erhalte einen Zeiger darauf
        DROPFILES* pDropFiles = (DROPFILES*)GlobalLock(hGlobal);
        if (pDropFiles)
        {
            // Initialisiere die DROPFILES-Struktur
            pDropFiles->pFiles = sizeof(DROPFILES);
            pDropFiles->pt.x = 0;
            pDropFiles->pt.y = 0;
            pDropFiles->fNC = TRUE;
            pDropFiles->fWide = TRUE; // Verwende WideString

            // Kopiere den Dateipfad in den Speicherblock nach der DROPFILES-Struktur
            memcpy((BYTE*)pDropFiles + sizeof(DROPFILES), wideFilePath.c_bstr(), (wideFilePath.Length() + 1) * sizeof(wchar_t));

            // Schließe den globalen Speicherblock
            GlobalUnlock(hGlobal);

            // Starte die Drag & Drop-Operation
            IDataObject* pDataObject;
            IDropSource* pDropSource;
            HRESULT hr;

            // Erstelle ein IDataObject
            hr = CoCreateInstance(CLSID_DataObject, NULL, CLSCTX_INPROC_SERVER, IID_IDataObject, (void**)&pDataObject);
            if (SUCCEEDED(hr))
            {
                // Erstelle ein IDropSource
                hr = CoCreateInstance(CLSID_DropSource, NULL, CLSCTX_INPROC_SERVER, IID_IDropSource, (void**)&pDropSource);
                if (SUCCEEDED(hr))
                {
                    // Füge den globalen Speicherblock zu IDataObject hinzu
                    FORMATETC fmtetc = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
                    STGMEDIUM stgmed = { TYMED_HGLOBAL, { hGlobal }, NULL };
                    pDataObject->SetData(&fmtetc, &stgmed, TRUE);

                    // Starte die Drag & Drop-Operation
                    DWORD dwEffect;
                    hr = DoDragDrop(pDataObject, pDropSource, DROPEFFECT_COPY | DROPEFFECT_MOVE, &dwEffect);

                    // Freigeben von IDropSource
                    pDropSource->Release();
                }
                // Freigeben von IDataObject
                pDataObject->Release();
            }
            // Freigeben des globalen Speicherblocks
            GlobalFree(hGlobal);
        }
    }
}

// Ereignishandler für den Button
void __fastcall TForm1::Button1Click(TObject *Sender)
{
    // Hier den Pfad der Datei angeben, die gezogen werden soll
    String filePath = "C:\\Pfad\\zur\\Datei.pdf";
    StartDragDrop(filePath);
}
Zusammenfassung:
OnDropFiles-Ereignis: Handelt korrekt die gezogenen Dateien mit nur einem Parameter Sender.
Drag & Drop: Implementiert eine Drag & Drop-Funktionalität für bekannte Dateipfade.
Viel Erfolg bei Deinem Miniprojekt! Falls Du weitere Fragen hast, stehe ich Dir gerne zur Verfügung.    

//>>>>>>>---------------------------
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

//---------------------------------
#include "Utility.h"
//---------------------------------

#include "Frame_AttachmentManager.h"
#include "Unit00_Main.h"
#include "Unit00_Setup.h"
#include "Unit1_DataBaseLayer.h"
#include "Unit2_Info.h"
#include "Unit71_Camera2.h"
#include "Unit81_Monitor.h"
#include "DebenuPDFLibraryDLL0916.h"
#include "Unit90_Drucken.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#include "_ExternalVars.h"

TFrameAttachmentManager *FrameAttachmentManager;
//====================================================================================================================================
__fastcall TFrameAttachmentManager::TFrameAttachmentManager(TComponent* Owner) : TFrame(Owner)
//====================================================================================================================================
{
	Color           =COL_H_MAIN;
	LIattachm->Color=SetLuminance(COL_H_MAIN,130);
	DboName         ="";
	DboKeyName      ="";
	StoredKeyVal    ="";
	SourcePathsList =new TStringList();
	DocsPathsList   =new TStringList();
	OpenedFilesList =new TStringList();
	IDList          =new TStringList();

	AutoScaleXmax   =0;
	AutoScaleYmax   =0;

	Busy            =false;

//	Image2->Picture->Bitmap->LoadFromResourceName((int)HInstance, "ATTACHMENTS");
}
//====================================================================================================================================
void __fastcall TFrameAttachmentManager::AfterResize(void)
//====================================================================================================================================
{
int nr,maxnr;

	Image2->Width =Width -8;
	PNpdf ->Width =Width -8;

	maxnr=Height/LIattachm->ItemHeight;
	nr=LIattachm->Count; if(nr>maxnr/2) nr=maxnr/2;
	LIattachm->Height=LIattachm->ItemHeight*nr+1;

	if(nr>0) Image2->Height=Height-LIattachm->Height-8;
	else     Image2->Height=Height-8;

	PNpdf ->Top =Image2->Top;
	PNpdf ->Left =Image2->Left;
	PNpdf ->Width =Image2->Width;
	PNpdf ->Height =Image2->Height;

	Label1->Left=(Width -Label1->Width)/2;
	Label1->Top =(Height-Label1->Height)/2;
	Label2->Left=(Width -Label2->Width)/2;
	Label2->Top =(Height-Label2->Height)/2;

	BNdelete->Left=Width-23;
	BNdelete->Top =0;

	BNlink->Left=Width-23;
	BNlink->Top=LIattachm->Top-BNlink->Height-1;

	if(TABMODUS) {PNbottom->Height=Height/4; PNphoto->Width=PNphoto->Height;}
	else         {PNbottom->Height=44;       PNphoto->Width=0;}

	LIattachm->Color=SetLuminance(Gradient->ColorEnd,90);
	LIattachm->Font->Color=clYellow;
}
//====================================================================================================================================
void __fastcall TFrameAttachmentManager::ShowImage(int Index)    // Index<0 setzt ggfs. bisheriges Image auf unsichtbar
//====================================================================================================================================
{
int     w,h,dpi;
double  pw,ph,pl;
bool    landscape;
String  path,suffix;
TColor  col;

	  if(Index<0 || Index>=DocsPathsList->Count)
	  {
		Label1->Visible=true;
		Image2->Visible=false;
		PNpdf-> Visible=false;
		return;
	  }

	  col=Gradient->ColorBegin;
	  Gradient->ColorBegin=clLime;
	  Label2->Visible=true;
	  KPIdle(100);

	  path=DocsPathsList->Strings[Index]; suffix=ExtractFileExt(LIattachm->Items->Strings[Index]);

	  if(SuffixIsOneOf(".BMP,.JPG,.JPEG,.PNG,.TIF","C:\\Dummy"+suffix))
	  {
		RenameFile(path,path+suffix);
		Image2->Picture->LoadFromFile(path+suffix);
		PNpdf->Visible=false; Image2->Visible=true;
		Label1->Visible=false;
		RenameFile(path+suffix,path);
	  }
	  else
	  if(SuffixIsOneOf(".PDF","C:\\Dummy"+suffix))
	  {
		if(Form90->Start_PDFModul())
		{
		   Image2->Visible=false; PNpdf->Visible=true;
		   PDF->NewDocument();
		   if(PDF->LoadFromFile(WideString(path).c_bstr(),L"")==1)
		   {
			 pw=PDF->PageWidth(); ph=PDF->PageHeight();
			 landscape=(pw>ph);
			 if(landscape) dpi=int(40.0*PNpdf->Width *pw/400000.0);
			 else          dpi=int(40.0*PNpdf->Height*ph/400000.0);

			 PDF->RenderPageToDC(dpi,1,GetDC(PNpdf->Handle));       // Seite 1 anzeigen, dpi=60 gilt nur für Canvas-Größe 495 x 700 (A4-Ratio)

			 h=PNpdf->Height;   // den auf PNpdf abgebildeten Thumb jetzt noch auf Image2 kopieren, da PNpdf vom System neu gezeichnet (gelöscht) wird
			 w=h*pw/ph;

			 Image2->Picture->Bitmap->SetSize(w,h);          //

			 BitBlt(Image2->Picture->Bitmap->Canvas->Handle,0,0,w,h,GetDC(PNpdf->Handle),0,0,SRCCOPY);
			 PNpdf->Visible=false; Image2->Visible=true;
		   }
		   else {Image2->Visible=false; PNpdf->Visible=false; }
		}
	  }
	  else {Image2->Visible=false; PNpdf->Visible=false; }

	  Label2->Visible=false;
	  Gradient->ColorBegin=col;
}
//====================================================================================================================================
void __fastcall TFrameAttachmentManager::DeleteOpenedFiles(void)
//====================================================================================================================================
{
   for(int i=0; i<OpenedFilesList->Count; i++) DeleteFile(OpenedFilesList->Strings[i]);
   OpenedFilesList->Clear();
}
//====================================================================================================================================
bool __fastcall TFrameAttachmentManager::LoadList(String DboKeyVal, bool Delayed) // DboKeyVal = ID des übergeordneten Datensatzes
//====================================================================================================================================
{                                          // mit dem Aufruf wird der StoredKeyVal = ID für alle weiteren Aktionen innerhalb des selben
String     filename, txt;                  // Datensatzes gesetzt
AnsiString sql="";                         // DboName muss vorab einmalig explizit von der übergeordneten Form gesetzt werden.
bool       success=true;
TMSQuery  *QQ;

   if(DboKeyVal.Length()) StoredKeyVal=DboKeyVal;
   if(StoredKeyVal.Length()==0 || DboName.Length()==0 || DboKeyName.Length()==0) return false;

   ShowImage(-1);

   if(Delayed)
   {
	  Label1->Visible=false;
	  TIdelay->Enabled=false; TIdelay->Enabled=true; return true;
   }

   if(Busy) return false;
   Busy=true;

   DeleteOpenedFiles();
   IDList->Clear();
   LIattachm->Items->Clear();
   SourcePathsList->Clear();
   DocsPathsList->Clear();

   //---------------------------------------------------------------------------------------------------------
   QQ=new TMSQuery(NULL);

   sql="DBONAME='"+AnsiString(DboName)+"' AND DBOKEYVAL="+StoredKeyVal+" AND (ISDELETED = 'False' or ISDELETED IS NULL)";
   if(Form1->SendSQLLine("SELECT * FROM dbo.AttachedFiles WHERE "+sql+" ORDER BY ID", QQ)) ;
   while(!QQ->Eof)
   {
	  IDList->Add(IntToStr(QQ->FieldByName("ID")->AsInteger));

	  txt=QQ->FieldByName("ORIGINALPATH")->AsString;
	  SourcePathsList->Add(txt);
	  LIattachm->Items->Add(ExtractFileName(txt));

	  txt=DIR_DOCS+QQ->FieldByName("DBONAME")->AsString+"\\"+QQ->FieldByName("DOCSFILENAME")->AsString;
	  DocsPathsList->Add(txt);

	  QQ->Next();
   }
   delete QQ;
   //---------------------------------------------------------------------------------------------------------
   AfterResize();

   int cnt=LIattachm->Count;
   if(cnt>0)
   {
	 BNdelete->Enabled=true;
	 ShowImage(cnt-1);
   }
   else BNdelete->Enabled=false;
   LIattachm->ItemIndex=cnt-1;

   Busy=false;
   return success;
}
//====================================================================================================================================
bool __fastcall TFrameAttachmentManager::AddItem(String Path, TJPEGImage *JPG_Photo, String DboKeyVal, bool LinkOnly)
//====================================================================================================================================
{                                                                    // DboKeyVal wird 1:1 übergeben, er muss ggfs. ' ' enthalten
String filename, sourcepath,targetpath, targetfilename,suffix;
AnsiString sql;
bool success=true;

   if(DboKeyVal.Length()) StoredKeyVal=DboKeyVal;

   if(Path.Length()==0 && JPG_Photo==NULL)           return false;
   if(StoredKeyVal.Length()==0)                      return false;
   if(DboName.Length()==0 || DboKeyName.Length()==0) return false;

   sourcepath="";

   if(Path.Length()>0)
   {
	 sourcepath=Path;
	 if(Form0->OpenForm(this,"Form81",sourcepath)) Form81->CheckForImageDownScale(sourcepath, AutoScaleXmax, AutoScaleYmax);
   }

   targetpath=DIR_DOCS+DboName; if(DirectoryExists(targetpath)==false) success=ForceDirectories(targetpath); // Ziel-Verzeichnis anlegen
   targetfilename=Now().FormatString("yyyy-mm-dd-hh-nn-ss-zzz"); targetpath+="\\"+targetfilename;            // Ziel-Datei-Pfad vorbereiten

   if(sourcepath!="") success=CopyFile(sourcepath.w_str(),targetpath.w_str(),false);
   else if(JPG_Photo)
   {
	  try {JPG_Photo->SaveToFile(targetpath); success=true;} catch (...) {success=false;}
	  sourcepath="\\\\TAB-FOTO\\TAB-Foto "+targetfilename+".jpg"; // ist für die spätere Generierung eines Filenamens erforderlich
   }

   if(!success) FormInfo->ShowMessage("Dokument konnte nicht gespeichtert werden",2,7,"");

   if(success)
   {
	 TMSQuery *QQ=new TMSQuery(NULL);

	 suffix=ExtractFileExt(sourcepath);

	 sql ="INSERT INTO dbo.AttachedFiles (ORIGINALPATH, MDEUSER, DBONAME, DBOKEYNAME, DBOKEYVAL, DOCSFILENAME, DOCSSUFFIX) VALUES (";
	 sql+="'"+AnsiString(sourcepath)    +"', ";
	 sql+="'"+PID.USER4                 +"', ";
	 sql+="'"+AnsiString(DboName)       +"', ";
	 sql+="'"+AnsiString(DboKeyName)    +"', ";
	 sql+=    AnsiString(StoredKeyVal)  +", " ;
	 sql+="'"+AnsiString(targetfilename)+"', ";
	 sql+="'"+AnsiString(suffix)        +"') ";

	 if(!Form1->SendSQLLine(sql, QQ)) {FormInfo->ShowMessage("Dokument konnte nicht registriert werden",2,7,""); success=false;}
	 else                             {filename=ExtractFileName(Path); LIattachm->Items->Add(filename);}

	 delete QQ;
   }

   LoadList(StoredKeyVal, false);

   return success;
}
//====================================================================================================================================
void __fastcall TFrameAttachmentManager::LIattachmClick(TObject *Sender)
//====================================================================================================================================
{
int idx;
String temppath;
String suffix;

   idx=LIattachm->ItemIndex; if(idx<0) return;


   temppath=DIR_MDELIVEDATA_LOCAL+"\\Temp\\"+LIattachm->Items->Strings[idx];
   if(CopyFile(DocsPathsList->Strings[idx].w_str(), temppath.w_str(),false))  OpenedFilesList->Add(temppath);
   else                                                                      {FormInfo->ShowMessage("",4,7004,""); return;}

   if(FileExists(temppath))
   {
	 suffix=ExtractFileExt(temppath).UpperCase();
	 if(String(".JPG .JPEG .PNG .TIF .GIF .BMP ").Pos(suffix)==0) ShellExecute(Handle,L"open",temppath.w_str(), NULL, NULL, SW_SHOWNORMAL);
	 else
	 {
	   ShowImage(LIattachm->ItemIndex);
	   if(Form0->OpenForm(Sender,"Form81","")) Form81->ShowDocument(temppath);
	 }
   }
   else FormInfo->ShowMessage("",4,7004,"");
}
//====================================================================================================================================
void __fastcall TFrameAttachmentManager::Image2MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
//====================================================================================================================================
{
  if(Button==mbRight)
  {
	if(StoredKeyVal.Length()==0) return;

	if(FODialog->Execute())
	if(FODialog->FileName.Length())
	{
	   for(int i=0; i<FODialog->Files->Count; i++) AddItem(FODialog->Files->Strings[i],NULL, StoredKeyVal, false);
	}
  }
  else
  {
	if(LIattachm->Items->Count)
	{
	  if(LIattachm->ItemIndex<0) LIattachm->ItemIndex=0;
	  LIattachmClick(Sender);
	}
  }
}
//====================================================================================================================================
void __fastcall TFrameAttachmentManager::GradientMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
//====================================================================================================================================
{
TColor col;
static bool busy;

	if(busy) return;

	busy=true;
	col=Gradient->ColorBegin;
	Gradient->ColorBegin=clYellow;
	KPIdle(400);
	Gradient->ColorBegin=col;
	busy=false;
}
//====================================================================================================================================
void __fastcall TFrameAttachmentManager::TIdelayTimer(TObject *Sender)
//====================================================================================================================================
{
   TIdelay->Enabled=false;
   LoadList("",false);
}
//====================================================================================================================================
void __fastcall TFrameAttachmentManager::BNdeleteClick(TObject *Sender)
//====================================================================================================================================
{
   if(LIattachm->ItemIndex<0) return;

   Form1->UpdateDboItem("AttachedFiles","ISDELETED='True'", "ID="+IDList->Strings[LIattachm->ItemIndex]);
   LoadList(StoredKeyVal, false);
}
//===================================================================================================================================================
bool __fastcall TFrameAttachmentManager::ConvertImageToGlyph(TObject *Sender, TImage *IM, int Width, int Height, TStream *STarget, String TFileName)
//===================================================================================================================================================
{
   if(IM==NULL || Width==0 || Height==0 || (STarget==NULL && TFileName=="")) return false;

   Graphics::TBitmap *bmp=new Graphics::TBitmap();
   bmp->SetSize(Width,Height);

   //----------------------------------------------------------------
   int w=IM->Width;
   int h=IM->Height;

   if(w>h && Width>Height) bmp->Canvas->StretchDraw(TRect(0,0,Width,Height),IM->Picture->Graphic); else // Orientierung passt: Skalieren
   if(w<h && Width<Height) bmp->Canvas->StretchDraw(TRect(0,0,Width,Height),IM->Picture->Graphic); else // Orientierung passt: Skalieren
						   bmp->Canvas->Draw(0,0,IM->Picture->Graphic);                                 // sonst Ausschnitt
   //----------------------------------------------------------------

   TJPEGImage *jpg=new TJPEGImage();
   jpg->Assign(bmp);
   jpg->CompressionQuality=80;

   if(!TFileName.IsEmpty() && TFileName.UpperCase().Pos(".JP")) jpg->SaveToFile(TFileName);
   if(STarget!=NULL) 											jpg->SaveToStream(STarget);

   delete jpg;
   bmp->FreeImage();
   delete bmp;

}
//===================================================================================================================================================
void __fastcall TFrameAttachmentManager::DropFilesDropFiles(TObject *Sender)
//===================================================================================================================================================
{
String path;

   path=DropFiles->FileName; if(path=="") return;

   int x=DropFiles->DropPoint.X;
   int y=DropFiles->DropPoint.Y;

   if(x>BNlink->Left && y>BNlink->Top && y<(BNlink->Top+BNlink->Height)) // es soll nur ein Link zur gewählten Datei abgelegt werden
   {                                                                     // ist noch nicht umgesetzt
	 Label1->Caption=String(IntToStr(x)+" "+IntToStr(y));
	 AddItem(path,NULL,"",true);
   }

   AddItem(path,NULL,"",false);
}
//===================================================================================================================================================
void __fastcall TFrameAttachmentManager::PNphotoClick(TObject *Sender)
//===================================================================================================================================================
{
   if(Form71!=NULL && Form71->Visible) return; // ein anderes Modul fotografiert gerade ....

   Form0->OpenForm(Sender, "Form71","",0);
   TimerPhoto->Enabled=true;

   if(Form71==NULL) return;

   Form71->JPG_ReadyToPick=false;
}
//===================================================================================================================================================
void __fastcall TFrameAttachmentManager::TimerPhotoTimer(TObject *Sender)  // Wartet auf fertiges Foto aus Form71
//===================================================================================================================================================
{
   if(Form71==NULL)      return;
   if(!Form71->Visible) {TimerPhoto->Enabled=false; return;} // nach Schließen der Foto-Form diese Timer-Verbindung beenden

   if(Form71->JPG_ReadyToPick)
   {
	 Form71->JPG_ReadyToPick=false;
	 AddItem("",Form71->JPG_Photo,"",false);
   }
}
//===================================================================================================================================================








