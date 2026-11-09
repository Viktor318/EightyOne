object EditArchiveInfo: TEditArchiveInfo
  Left = 466
  Top = 139
  BorderIcons = []
  BorderStyle = bsToolWindow
  Caption = 'Edit Archive Information'
  ClientHeight = 358
  ClientWidth = 377
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Scaled = False
  PixelsPerInch = 96
  TextHeight = 15
  object Label1: TLabel
    Left = 89
    Top = 11
    Width = 26
    Height = 15
    Alignment = taRightJustify
    Caption = 'Title:'
  end
  object Label2: TLabel
    Left = 64
    Top = 37
    Width = 51
    Height = 15
    Alignment = taRightJustify
    Caption = 'Publisher:'
  end
  object Label3: TLabel
    Left = 77
    Top = 63
    Width = 38
    Height = 15
    Alignment = taRightJustify
    Caption = 'Author:'
  end
  object Label4: TLabel
    Left = 14
    Top = 89
    Width = 101
    Height = 15
    Alignment = taRightJustify
    Caption = 'Year of Publication:'
  end
  object Label5: TLabel
    Left = 59
    Top = 116
    Width = 56
    Height = 15
    Alignment = taRightJustify
    Caption = 'Language:'
  end
  object Label6: TLabel
    Left = 85
    Top = 142
    Width = 30
    Height = 15
    Alignment = taRightJustify
    Caption = 'Type:'
  end
  object Label7: TLabel
    Left = 85
    Top = 170
    Width = 30
    Height = 15
    Alignment = taRightJustify
    Caption = 'Price:'
  end
  object Label8: TLabel
    Left = 13
    Top = 194
    Width = 102
    Height = 15
    Alignment = taRightJustify
    Caption = 'Protection Scheme:'
  end
  object Label9: TLabel
    Left = 82
    Top = 223
    Width = 33
    Height = 15
    Alignment = taRightJustify
    Caption = 'Origin:'
  end
  object Label10: TLabel
    Left = 58
    Top = 262
    Width = 57
    Height = 15
    Alignment = taRightJustify
    Caption = 'Comments:'
  end
  object Origin: TEdit
    Left = 122
    Top = 218
    Width = 246
    Height = 23
    TabOrder = 8
  end
  object Protection: TEdit
    Left = 122
    Top = 192
    Width = 246
    Height = 23
    TabOrder = 7
  end
  object Price: TEdit
    Left = 122
    Top = 166
    Width = 246
    Height = 23
    TabOrder = 6
  end
  object Type: TEdit
    Left = 122
    Top = 140
    Width = 246
    Height = 23
    TabOrder = 5
  end
  object Language: TEdit
    Left = 122
    Top = 113
    Width = 246
    Height = 23
    TabOrder = 4
  end
  object Year: TEdit
    Left = 122
    Top = 87
    Width = 246
    Height = 23
    TabOrder = 3
  end
  object Author: TEdit
    Left = 122
    Top = 61
    Width = 246
    Height = 23
    TabOrder = 2
  end
  object Publisher: TEdit
    Left = 122
    Top = 35
    Width = 246
    Height = 23
    TabOrder = 1
  end
  object Title: TEdit
    Left = 122
    Top = 9
    Width = 246
    Height = 23
    TabOrder = 0
  end
  object OK: TButton
    Left = 148
    Top = 323
    Width = 82
    Height = 27
    Caption = 'OK'
    Default = True
    TabOrder = 10
    OnClick = OKClick
  end
  object Comments: TMemo
    Left = 122
    Top = 244
    Width = 246
    Height = 71
    TabOrder = 9
  end
end
