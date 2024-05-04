Temp

void __fastcall TFrameX10K::table2OnCellFormating(TObject *Sender, int ACol, int ARow,
		  WideString Value, TColor &TextColor, TFontStyles &FontStyle, TCellState CellState)

{
	if ((ARow >= 0) && (!xml4table1GUI)) { // Überprüfe, ob es sich um die erste Zeile (Überschriftenzeile) handelt
		FontStyle = TFontStyles();
		//TextColor = clSkyBlue;
	}

	if((ARow >= 0) && (xml4table1GUI)){
		FontStyle = TFontStyles();
		TextColor = clWebGold; //clCream;
		return;
	}

	//if(xml4table2GUI)
	//	TextColor = clCream;
	//Fall wenn angeklickt um zu löschen
	if((prepare4table2GUI) && (RowTable2 > -1) && (ARow == RowTable2) )
		TextColor = clFuchsia;

	if ((prepare4table2GUI) && (Col4RedTable2 > -1) && (RowTable2 > -1) && (ARow == RowTable2) && (ACol == Col4RedTable2))  // Überprüfe, ob es sich um die erste Zeile (Überschriftenzeile) handelt
		TextColor = clRed;
}

//Vorher
void __fastcall TFrameX10K::table1OnCellFormating(TObject *Sender, int ACol, int ARow,
		  WideString Value, TColor &TextColor, TFontStyles &FontStyle, TCellState CellState)

{
	FontStyle = TFontStyles();

	if((ARow >= 0) && (xml4table1GUI)){

		TextColor = clWebGold; //clCream;
		FontStyle = TFontStyles();//table1->RefreshRow(ARow);
		return;
	}

	if ((ARow >= 0) && (ARow != RowTable1)) {  //(RowTable1 > -1) &&
		TextColor = clBlack; //RRR42
		FontStyle = TFontStyles() << fsBold; // >> fsBold;
		//table1->RefreshRow(ARow);
		return;
	}

	/*
	//Fall wenn angeklickt um zu löschen
	else if((prepare4table1GUI) && (RowTable1 > -1) && (ARow == RowTable1) ){
		FontStyle = TFontStyles(); // << fsBold;
		TextColor = clOlive; //clFuchsia;
			if ((prepare4table1GUI) && (Col4RedTable1 > -1) && (RowTable1 > -1) && (ARow == RowTable1) && (ACol == Col4RedTable1)) {  // Überprüfe, ob es sich um die erste Zeile (Überschriftenzeile) handelt
				FontStyle = TFontStyles(); // << fsBold;
				TextColor = clRed;
		}
	} */
	/*else if ((ARow >= 0) && (ARow != RowTable1)) {  //(RowTable1 > -1) &&
		FontStyle = TFontStyles(); // >> fsBold;
	} */

}