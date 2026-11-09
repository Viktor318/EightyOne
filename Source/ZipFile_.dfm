object ZipFile: TZipFile
  Left = 432
  Top = 113
  Width = 209
  Height = 268
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSizeToolWin
  Caption = 'ZipFile'
  Color = clBtnFace
  Constraints.MinHeight = 137
  Constraints.MinWidth = 139
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
    Left = 9
    Top = 9
    Width = 74
    Height = 15
    Caption = 'Please Select:'
  end
  object ListBox: TListBox
    Left = 9
    Top = 35
    Width = 183
    Height = 157
    Anchors = [akLeft, akTop, akRight, akBottom]
    ItemHeight = 15
    TabOrder = 0
    OnDblClick = ListBoxDblClick
  end
  object OK: TButton
    Left = 112
    Top = 200
    Width = 82
    Height = 27
    Anchors = [akRight, akBottom]
    Caption = 'OK'
    TabOrder = 1
    OnClick = OKClick
  end
end
