object EditValue: TEditValue
  Left = 241
  Top = 176
  BorderIcons = [biHelp]
  BorderStyle = bsToolWindow
  Caption = 'Edit Value'
  ClientHeight = 80
  ClientWidth = 195
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Scaled = False
  OnKeyPress = FormKeyPress
  PixelsPerInch = 96
  TextHeight = 15
  object Label1: TLabel
    Left = 9
    Top = 13
    Width = 32
    Height = 15
    Caption = 'Value:'
  end
  object EditVal: TEdit
    Left = 52
    Top = 9
    Width = 132
    Height = 23
    CharCase = ecUpperCase
    MaxLength = 17
    TabOrder = 0
    Text = '0'
    OnChange = EditValChange
    OnKeyPress = FormKeyPress
  end
  object OK: TButton
    Left = 9
    Top = 44
    Width = 82
    Height = 27
    Caption = 'OK'
    Default = True
    TabOrder = 1
    OnClick = OKClick
  end
  object Cancel: TButton
    Left = 105
    Top = 44
    Width = 82
    Height = 27
    Caption = 'Cancel'
    TabOrder = 2
    OnClick = CancelClick
  end
end
