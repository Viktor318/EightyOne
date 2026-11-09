object SearchSequence: TSearchSequence
  Left = 302
  Top = 245
  BorderIcons = [biHelp]
  BorderStyle = bsToolWindow
  Caption = 'Find'
  ClientHeight = 88
  ClientWidth = 281
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  FormStyle = fsStayOnTop
  KeyPreview = True
  OldCreateOrder = False
  Scaled = False
  OnKeyPress = FormKeyPress
  PixelsPerInch = 96
  TextHeight = 15
  object Label1: TLabel
    Left = 9
    Top = 17
    Width = 56
    Height = 15
    Caption = 'Sequence:'
  end
  object EditSequence: TEdit
    Left = 79
    Top = 14
    Width = 193
    Height = 23
    TabOrder = 0
  end
  object Cancel: TButton
    Left = 183
    Top = 52
    Width = 91
    Height = 28
    Caption = 'Cancel'
    TabOrder = 2
    OnClick = CancelClick
  end
  object Find: TButton
    Left = 79
    Top = 52
    Width = 88
    Height = 28
    Caption = 'Find'
    Default = True
    TabOrder = 1
    OnClick = FindClick
  end
end
