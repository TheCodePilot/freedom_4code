DBGrid1->Options << dgDragSource << dgRowSelect;
DBGrid1->Options << dgDropTarget;
DBGrid2->Options << dgDragSource << dgRowSelect;
DBGrid2->Options << dgDropTarget;

void __fastcall TForm1::DBGrid2DragDrop(TObject *Sender, TObject *Source, int X, int Y)
{
    // Hier können Sie die Daten aus der Quelle verarbeiten und in das Ziel einfügen
}

void __fastcall TForm1::DBGrid2DragOver(TObject *Sender, TObject *Source, int X, int Y, TDragState State, bool &Accept)
{
    // Überprüfen Sie, ob das Ablegen erlaubt ist, und setzen Sie Accept entsprechend
}

void __fastcall TForm1::DBGrid1StartDrag(TObject *Sender, TDragObject *&DragObject)
{
    // Hier können Sie die Daten vorbereiten, die gezogen werden sollen
}

//-------------------------------------------------------------------------------------------

// Angenommen, Sie haben zwei TDBGrid-Steuerungen namens DBGrid1 und DBGrid2

void __fastcall TForm1::DBGrid1StartDrag(TObject *Sender, TDragObject *&Data)
{
    // Starten Sie das Ziehen des ausgewählten Datensatzes
    Data = new TDragObject();
    Data->Source = DBGrid1;
}

void __fastcall TForm1::DBGrid2DragOver(TObject *Sender, TObject *Source, int X, int Y, TDragState State, bool &Accept)
{
    // Akzeptieren Sie das Ziehen, wenn die Quelle DBGrid1 ist
    Accept = (Source == DBGrid1);
}

void __fastcall TForm1::DBGrid2DragDrop(TObject *Sender, TObject *Source, int X, int Y)
{
    // Fügen Sie den gezogenen Datensatz zu DBGrid2 hinzu
    if (Source == DBGrid1)
    {
        TDataSet *srcDataSet = DBGrid1->DataSource->DataSet;
        TDataSet *dstDataSet = DBGrid2->DataSource->DataSet;

        dstDataSet->Append();
        for (int i = 0; i < srcDataSet->FieldCount; ++i)
        {
            dstDataSet->Fields->Fields[i]->Value = srcDataSet->Fields->Fields[i]->Value;
        }
        dstDataSet->Post();
    }
}