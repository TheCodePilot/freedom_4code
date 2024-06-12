INSERT INTO Tabelle (Spalte1, Spalte2)
VALUES (Wert1, Wert2);

INSERT INTO Tabelle (Spalte1_Float, Spalte2_Integer)
VALUES (CAST('123.45' AS FLOAT), CAST('678' AS INTEGER));

void __fastcall TForm1::FormShow(TObject *Sender)
{
    if (/*Bedingung*/)
    {
        this->Close();
    }
}


Form1->Visible = false;

if (/*Bedingung*/)
{
    Form1->Visible = true;
    Form1->ShowModal();
}
