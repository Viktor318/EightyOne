object EditTextForm: TEditTextForm
  Left = 382
  Top = 161
  BorderIcons = []
  BorderStyle = bsToolWindow
  Caption = 'Edit Text Block'
  ClientHeight = 98
  ClientWidth = 324
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
    Left = 9
    Top = 9
    Width = 26
    Height = 15
    Caption = 'Text:'
  end
  object Edit: TEdit
    Left = 9
    Top = 26
    Width = 306
    Height = 23
    TabOrder = 0
  end
  object OK: TButton
    Left = 122
    Top = 61
    Width = 82
    Height = 27
    Caption = 'OK'
    Default = True
    TabOrder = 1
    OnClick = OKClick
  end
end
