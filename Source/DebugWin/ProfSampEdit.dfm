object ProfileSampleEdit: TProfileSampleEdit
  Left = 266
  Top = 398
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Profile Entry'
  ClientHeight = 148
  ClientWidth = 268
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Scaled = False
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 15
  object Label1: TLabel
    Left = 58
    Top = 10
    Width = 21
    Height = 15
    Alignment = taRightJustify
    Caption = 'Tag'
  end
  object Label2: TLabel
    Left = 11
    Top = 36
    Width = 69
    Height = 15
    Alignment = taRightJustify
    Caption = 'Start Address'
  end
  object Label3: TLabel
    Left = 13
    Top = 62
    Width = 66
    Height = 15
    Alignment = taRightJustify
    Caption = 'End Address'
  end
  object Label4: TLabel
    Left = 9
    Top = 95
    Width = 166
    Height = 28
    Caption = 'Specify the addresses of the start and end instructions.'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
    WordWrap = True
  end
  object EditTag: TEdit
    Left = 87
    Top = 9
    Width = 167
    Height = 23
    TabOrder = 0
    Text = 'EditTag'
  end
  object EditStart: TEdit
    Left = 87
    Top = 35
    Width = 167
    Height = 23
    TabOrder = 1
    Text = 'EditStart'
    OnChange = EditStartChange
    OnDblClick = EditDblClick
  end
  object EditEnd: TEdit
    Left = 87
    Top = 61
    Width = 167
    Height = 23
    TabOrder = 2
    Text = 'EditEnd'
    OnChange = EditEndChange
  end
  object ButtonOK: TButton
    Left = 183
    Top = 96
    Width = 73
    Height = 27
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 3
    OnClick = ButtonOKClick
  end
end
