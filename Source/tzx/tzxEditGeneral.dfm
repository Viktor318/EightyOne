object EditGeneralForm: TEditGeneralForm
  Left = 238
  Top = 28
  Width = 705
  Height = 721
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSizeToolWin
  Caption = 'Edit General Data Block'
  Color = clBtnFace
  Constraints.MinHeight = 409
  Constraints.MinWidth = 689
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Scaled = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 15
  object Label4: TLabel
    Left = 156
    Top = 8
    Width = 127
    Height = 15
    Caption = 'Pilot/Sync Symbol Table'
  end
  object Label5: TLabel
    Left = 156
    Top = 112
    Width = 98
    Height = 15
    Caption = 'Data Symbol Table'
  end
  object Label7: TLabel
    Left = 7
    Top = 659
    Width = 36
    Height = 15
    Anchors = [akLeft, akBottom]
    Caption = 'Pause:'
  end
  object Label8: TLabel
    Left = 8
    Top = 226
    Width = 25
    Height = 15
    Caption = 'Data'
  end
  object Label9: TLabel
    Left = 230
    Top = 659
    Width = 74
    Height = 15
    Anchors = [akLeft, akBottom]
    Caption = 'Character Set:'
  end
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 81
    Height = 15
    Caption = 'Pilot/Sync Data'
  end
  object SymDefP: TMemo
    Left = 156
    Top = 25
    Width = 530
    Height = 80
    Anchors = [akLeft, akTop, akRight]
    Lines.Strings = (
      'SyncPulses')
    ReadOnly = True
    ScrollBars = ssBoth
    TabOrder = 1
  end
  object SymDefD: TMemo
    Left = 156
    Top = 130
    Width = 530
    Height = 88
    Anchors = [akLeft, akTop, akRight]
    Lines.Strings = (
      'Alphabet')
    ReadOnly = True
    ScrollBars = ssBoth
    TabOrder = 2
  end
  object Pause: TEdit
    Left = 56
    Top = 655
    Width = 157
    Height = 23
    Anchors = [akLeft, akBottom]
    TabOrder = 4
    Text = 'Pause'
  end
  object OK: TButton
    Left = 600
    Top = 651
    Width = 81
    Height = 28
    Anchors = [akRight, akBottom]
    Caption = 'OK'
    Default = True
    TabOrder = 7
    OnClick = OKClick
  end
  object Data: TMemo
    Left = 8
    Top = 243
    Width = 679
    Height = 395
    Anchors = [akLeft, akTop, akRight, akBottom]
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Fixedsys'
    Font.Style = []
    Lines.Strings = (
      'Data')
    ParentFont = False
    ReadOnly = True
    ScrollBars = ssVertical
    TabOrder = 3
  end
  object CharSet: TComboBox
    Left = 320
    Top = 655
    Width = 96
    Height = 23
    Style = csDropDownList
    Anchors = [akLeft, akBottom]
    ItemHeight = 15
    TabOrder = 5
    OnChange = CharSetChange
    Items.Strings = (
      'ZX80'
      'ZX81'
      'ASCII')
  end
  object PRLE: TMemo
    Left = 8
    Top = 25
    Width = 139
    Height = 193
    Lines.Strings = (
      'PRLE')
    ScrollBars = ssVertical
    TabOrder = 0
  end
  object ButtonSave: TButton
    Left = 508
    Top = 651
    Width = 82
    Height = 28
    Anchors = [akRight, akBottom]
    Caption = 'Save...'
    TabOrder = 6
    OnClick = ButtonSaveClick
  end
  object SaveDialog1: TSaveDialog
    Left = 454
    Top = 646
  end
end
