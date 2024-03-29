object Form5: TForm5
  Left = 0
  Top = 0
  Caption = 'Form5'
  ClientHeight = 658
  ClientWidth = 1112
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object DBGrid4XMLin: TDBGrid
    Left = 8
    Top = 8
    Width = 1081
    Height = 329
    TabOrder = 0
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'Tahoma'
    TitleFont.Style = []
    OnCellClick = DBGrid4XMLinOnCellClick
    OnDblClick = DBGrid4XMLinDblclick
    OnEndDrag = DBGrid4XMLinEndDrag
    OnMouseDown = DBGrid4XMLinMouseDown
    OnStartDrag = DBGrid4XMLinStartDrag
  end
  object Edit1: TEdit
    Left = 8
    Top = 368
    Width = 1081
    Height = 21
    DragKind = dkDock
    TabOrder = 1
    Text = 'Edit1'
    OnDragDrop = Edit1DragDrop
    OnDragOver = Edit1DragOver
  end
  object ClientDataSet1: TClientDataSet
    Aggregates = <>
    Params = <>
    Left = 24
    Top = 600
  end
  object DataSource1: TDataSource
    Left = 72
    Top = 608
  end
  object OpenTextFileDialog1: TOpenTextFileDialog
    Left = 984
    Top = 624
  end
end
