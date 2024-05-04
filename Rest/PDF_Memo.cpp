#ifndef Frame_MemoH
#define Frame_MemoH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "Frame_AttachmentManager.h" //!!!
#include <Vcl.Buttons.hpp>
#include <Vcl.DBCtrls.hpp>
#include <Vcl.Mask.hpp>
#include "Gradient.hpp"
#include "DBAccess.hpp"
#include "MSAccess.hpp"
#include <Data.DB.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "Flasher.h"

TFrameAttachmentManager *FRattachm;

________________________________________________
Cpp Aufruf
________________________________________________

FRattachm->AutoScaleXmax=1200; //bezieht:
FRattachm->AutoScaleYmax=1000;

//=============================================================================================================================================
void __fastcall TFrameMEMO::ImportDataFromDB(void)	//Aufruf in:  TFrameMEMO::Activate(int ID, String OwnerFormName) 
//=============================================================================================================================================
	// Um das Attachmentframe nutzen zu können muss der DboName sowie der DboKeyName übergeben werden
	String YYYY=PDS.TIMESTAMP.FormatString("yyyy");
	FRattachm->DboName="PROT_Memo_"+YYYY; FRattachm->DboKeyName="ID";	//Hier PROT_X10K_2024
	FRattachm->LoadList(IntToStr(ProtokolID),true); //void __fastcall TFrameMEMO::Activate(int ID, String OwnerFormName) //aus: FR_Memo ->Activate(PDS.PK_NR,"");
	FRattachm->AfterResize();																							//vgl. FR_X10K ->Activate(PDS.PK_NR,""	

________________________________________________
Frame_AttachmentManager.cpp 
________________________________________________


DboName
DBONAME

PROT_X10K_2024



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


//________________________________________________

//PID.USER4 noch zu setzen








