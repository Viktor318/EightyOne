object EditDataForm: TEditDataForm
  Left = 244
  Top = 208
  Width = 700
  Height = 434
  BorderIcons = []
  BorderStyle = bsSizeToolWin
  Caption = 'Edit Data Block'
  Color = clBtnFace
  Constraints.MaxWidth = 700
  Constraints.MinHeight = 349
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
  object Label2: TLabel
    Left = 9
    Top = 9
    Width = 92
    Height = 15
    Caption = 'Pilot Pulse Length'
  end
  object Label3: TLabel
    Left = 9
    Top = 61
    Width = 92
    Height = 15
    Caption = 'No. of Pilot pulses'
  end
  object Label6: TLabel
    Left = 524
    Top = 9
    Width = 102
    Height = 15
    Caption = 'Final Byte Used Bits'
  end
  object Label7: TLabel
    Left = 524
    Top = 61
    Width = 32
    Height = 15
    Caption = 'Pause'
  end
  object Label8: TLabel
    Left = 9
    Top = 113
    Width = 25
    Height = 15
    Caption = 'Data'
  end
  object Label4: TLabel
    Left = 175
    Top = 9
    Width = 106
    Height = 15
    Caption = 'Sync Pulse 1 Length'
  end
  object Label5: TLabel
    Left = 175
    Top = 61
    Width = 106
    Height = 15
    Caption = 'Sync Pulse 2 Length'
  end
  object Label1: TLabel
    Left = 349
    Top = 9
    Width = 62
    Height = 15
    Caption = 'Bit 0 Length'
  end
  object Label9: TLabel
    Left = 349
    Top = 61
    Width = 62
    Height = 15
    Caption = 'Bit 1 Length'
  end
  object Label10: TLabel
    Left = 9
    Top = 364
    Width = 74
    Height = 15
    Anchors = [akLeft, akBottom]
    Caption = 'Character Set:'
  end
  object PPL: TEdit
    Left = 9
    Top = 26
    Width = 149
    Height = 23
    TabOrder = 0
    Text = 'PPL'
  end
  object NoPP: TEdit
    Left = 9
    Top = 79
    Width = 149
    Height = 23
    TabOrder = 1
    Text = 'NoPP'
  end
  object UsedBits: TEdit
    Left = 524
    Top = 26
    Width = 149
    Height = 23
    TabOrder = 6
    Text = 'UsedBits'
  end
  object Pause: TEdit
    Left = 524
    Top = 79
    Width = 149
    Height = 23
    TabOrder = 7
    Text = 'Pause'
  end
  object OK: TButton
    Left = 591
    Top = 357
    Width = 82
    Height = 27
    Anchors = [akRight, akBottom]
    Caption = 'OK'
    Default = True
    TabOrder = 10
    OnClick = OKClick
  end
  object Data: TMemo
    Left = 9
    Top = 131
    Width = 664
    Height = 210
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
    TabOrder = 8
  end
  object Sync1: TEdit
    Left = 175
    Top = 26
    Width = 158
    Height = 23
    TabOrder = 2
    Text = 'Sync1'
  end
  object Sync2: TEdit
    Left = 175
    Top = 79
    Width = 158
    Height = 23
    TabOrder = 3
    Text = 'Sync2'
  end
  object Bit0: TEdit
    Left = 349
    Top = 26
    Width = 158
    Height = 23
    TabOrder = 4
    Text = 'Bit0'
  end
  object Bit1: TEdit
    Left = 349
    Top = 79
    Width = 158
    Height = 23
    TabOrder = 5
    Text = 'Bit1'
  end
  object CharSet: TComboBox
    Left = 102
    Top = 360
    Width = 97
    Height = 23
    Style = csDropDownList
    Anchors = [akLeft, akBottom]
    ItemHeight = 15
    TabOrder = 9
    OnChange = CharSetChange
    Items.Strings = (
      'ZX80'
      'ZX81'
      'ASCII')
  end
end
