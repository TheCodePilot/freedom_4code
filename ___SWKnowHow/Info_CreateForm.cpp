Units_Freigaben

class TForm3_ProdFreigabe : public TForm

if(Form3_ProdFreigabe==NULL) Application->CreateForm(__classid(TForm3_ProdFreigabe), &Form3_ProdFreigabe);
Ist taatsächlich nicht im Application dabei


/=============================================================================================================================================
void __fastcall TForm3::BNfreigabeELEKClick(TObject *Sender)  // Vorbereitung einer Freigabe aufrufen, wird von mehreren Tasten aufgerufen...
//=============================================================================================================================================
{
bool success=false;

	  if(PID.USER4=="") {FormInfo->ShowMessage("Für diesen Vorgang ist eine MDE-Anmeldung erforderlich !",1,6,""); return;}

	  if(PDS.CHANGED && FormInfo->ShowMessage("Protokoll-",0,8100,"")=='j') FRCCBNsaveClick(Sender);

	  if(Form3_ProdFreigabe==NULL) Application->CreateForm(__classid(TForm3_ProdFreigabe), &Form3_ProdFreigabe);
	  
	  
//----------------------------	  


//=====================================================================================================================================================================
void __fastcall TForm60::SpBtnActionItemClick(TObject *Sender)
//=====================================================================================================================================================================
{
	   if(Form101==NULL) Application->CreateForm(__classid(TForm101), &Form101);
	   if(Form101!=NULL) Form101->OpenForm(Sender,0);
}
//======



//=============================================================================================================================================
void __fastcall TFrameMEMO::btnNcrListClick(TObject *Sender)
//=============================================================================================================================================
{
	   if(FormListeMemo==NULL) Application->CreateForm(__classid(TFormListeMemo), &FormListeMemo);
	   if(FormListeMemo!=NULL) FormListeMemo->OpenForm(Sender,0);
}


























BNfreigabeELEKClick