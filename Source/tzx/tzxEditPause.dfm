object EditPauseForm: TEditPauseForm
  Left = 203
  Top = 127
  BorderIcons = []
  BorderStyle = bsToolWindow
  Caption = 'Edit Pause Block'
  ClientHeight = 70
  ClientWidth = 151
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
    Left = 17
    Top = 9
    Width = 36
    Height = 15
    Caption = 'Pause:'
  end
  object Label2: TLabel
    Left = 122
    Top = 9
    Width = 14
    Height = 15
    Caption = 'ms'
  end
  object Edit: TEdit
    Left = 61
    Top = 7
    Width = 54
    Height = 23
    TabOrder = 0
    Text = '0'
  end
  object OK: TButton
    Left = 44
    Top = 35
    Width = 81
    Height = 27
    Caption = 'OK'
    Default = True
    TabOrder = 1
    OnClick = OKClick
  end
end
