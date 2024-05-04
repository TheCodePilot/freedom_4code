
NextGrid

Cell[4][2]->AsFloat = 3.4;
Cell[4][2]->AsDateTime = Now();
Cell[4][2]->AsBoolean = false;
Cell[4][2]->AsString = "This is Cell Value"; 

Some of columns are editable (such as Text, Number... columns). To enable editing column set coEditing flag in Options property to True.

Column's header and footer owe own sub-properties with own properties: Header and Footer.

 NextGrid1->AddRow; // Add single row
  NextGrid1->AddRow(4); // Add 4 rows
  
  NextGrid1->AddCells(["Mike", "23", "true", "Lisa", "178", "false"]);
  
  //---------------------------
  
  void __fastcall TForm1::Button1Click(TObject *Sender)
{
  OpenPictureDialog1->DefaultExt = GraphicExtension(__classid(TIcon));
  OpenPictureDialog1->FileName = GraphicFileMask(__classid(TIcon));
  OpenPictureDialog1->Filter = GraphicFilter(__classid(TIcon));
  OpenPictureDialog1->Options.Clear();
  OpenPictureDialog1->Options << ofFileMustExist << ofHideReadOnly << ofNoChangeDir;
  while (true)
  {
	if (OpenPictureDialog1->Execute())
    {
      if (!OpenPictureDialog1->Options.Contains(ofExtensionDifferent))
      {
        Application->Icon->LoadFromFile(OpenPictureDialog1->FileName);
        break;
      }
      else // reset Options to remove ofExtensionDifferent
      {
        OpenPictureDialog1->Options.Clear();
        OpenPictureDialog1->Options << ofFileMustExist << ofHideReadOnly << ofNoChangeDir;
      }
    }
    else // user cancelled
      break;
  }
}
  