object Form5: TForm5
  Left = 0
  Top = 0
  ClientHeight = 612
  ClientWidth = 1205
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnDestroy = OnDestroyAndClear
  OnMouseEnter = Form5MouseEnter
  PixelsPerInch = 96
  TextHeight = 13
  object DBGrid4XMLin: TDBGrid
    Left = 24
    Top = 24
    Width = 553
    Height = 120
    TabOrder = 0
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'Tahoma'
    TitleFont.Style = []
    OnCellClick = DBGrid4XMLinOnCellClick
    OnDrawColumnCell = DBGrid4XMLinDrawColumnCell
  end
  object DBGrid4XMLCard_Item: TDBGrid
    Left = 24
    Top = 192
    Width = 553
    Height = 249
    TabOrder = 1
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'Tahoma'
    TitleFont.Style = []
    OnCellClick = DBGrid4XMLCard_ItemOnCellKlick
    OnEnter = DBGrid4XMLCard_ItemEnter
    OnMouseEnter = DBGrid4XMLCard_ItemMouseEnter
    OnMouseMove = DBGrid4XMLCard_ItemMouseMove
    OnStartDrag = DBGrid4XMLCard_ItemStartDrag
  end
  object Memo1: TMemo
    Left = 624
    Top = 24
    Width = 553
    Height = 120
    Lines.Strings = (
      'Memo1')
    TabOrder = 2
  end
  object DBGridUserChoice: TDBGrid
    Left = 624
    Top = 192
    Width = 553
    Height = 249
    DragCursor = crHandPoint
    TabOrder = 3
    TitleFont.Charset = DEFAULT_CHARSET
    TitleFont.Color = clWindowText
    TitleFont.Height = -11
    TitleFont.Name = 'Tahoma'
    TitleFont.Style = []
    OnEndDrag = SBDBGridUserChoiceEndDrag
    OnEnter = UserGridEnter
    OnMouseEnter = SBMouseEnterUserGrid
    OnMouseMove = SBUserGridMouseMove
  end
  object ClientDataSet1: TClientDataSet
    Aggregates = <>
    Params = <>
    Left = 848
    Top = 560
  end
  object DataSource1: TDataSource
    Left = 424
    Top = 552
  end
  object OpenTextFileDialog1: TOpenTextFileDialog
    Left = 584
    Top = 440
  end
  object ClientDataSet2: TClientDataSet
    Aggregates = <>
    Params = <>
    Left = 704
    Top = 528
  end
  object DataSource2: TDataSource
    Left = 536
    Top = 536
  end
  object ClientDataSet3: TClientDataSet
    Aggregates = <>
    Params = <>
    Left = 1024
    Top = 560
  end
  object DataSource3: TDataSource
    Left = 1136
    Top = 528
  end
end