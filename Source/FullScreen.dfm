object FSSettings: TFSSettings
  Left = 189
  Top = 81
  BorderIcons = []
  BorderStyle = bsToolWindow
  Caption = 'Display Settings'
  ClientHeight = 233
  ClientWidth = 209
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
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 15
  object Label2: TLabel
    Left = 9
    Top = 9
    Width = 51
    Height = 15
    Caption = 'Renderer:'
  end
  object OK: TButton
    Left = 120
    Top = 200
    Width = 82
    Height = 27
    Caption = 'OK'
    TabOrder = 3
    OnClick = OKClick
  end
  object GroupBox1: TGroupBox
    Left = 9
    Top = 52
    Width = 193
    Height = 141
    Caption = 'Full Screen Settings'
    TabOrder = 2
    object Label1: TLabel
      Left = 9
      Top = 26
      Width = 58
      Height = 15
      Caption = 'Resolution:'
    end
    object ModeList: TComboBox
      Left = 9
      Top = 44
      Width = 175
      Height = 23
      Style = csDropDownList
      ItemHeight = 15
      TabOrder = 0
      OnChange = ModeListChange
    end
    object Letterbox: TRadioButton
      Left = 9
      Top = 96
      Width = 120
      Height = 19
      Caption = 'Letterbox'
      Checked = True
      TabOrder = 2
      TabStop = True
      OnClick = ModeListChange
    end
    object Stretch: TRadioButton
      Left = 9
      Top = 79
      Width = 120
      Height = 18
      Caption = 'Stretch Image'
      TabOrder = 1
      OnClick = ModeListChange
    end
    object White: TCheckBox
      Left = 9
      Top = 113
      Width = 120
      Height = 19
      Caption = 'White Letterbox'
      TabOrder = 3
      OnClick = ModeListChange
    end
  end
  object GDIBtn: TRadioButton
    Left = 26
    Top = 26
    Width = 71
    Height = 19
    Caption = 'GDI'
    Checked = True
    TabOrder = 0
    TabStop = True
    OnClick = GDIBtnClick
  end
  object DDrawBtn: TRadioButton
    Left = 105
    Top = 26
    Width = 97
    Height = 19
    Caption = 'DirectDraw'
    TabOrder = 1
    OnClick = GDIBtnClick
  end
end
