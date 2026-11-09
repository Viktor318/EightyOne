object EditFlag: TEditFlag
  Left = 461
  Top = 127
  BorderIcons = [biHelp]
  BorderStyle = bsToolWindow
  Caption = 'EditFlag'
  ClientHeight = 89
  ClientWidth = 153
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  OldCreateOrder = False
  Scaled = False
  PixelsPerInch = 96
  TextHeight = 15
  object Label1: TLabel
    Left = 11
    Top = 9
    Width = 9
    Height = 17
    Caption = 'S'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -14
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 28
    Top = 9
    Width = 9
    Height = 17
    Caption = 'Z'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -14
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
  end
  object Label4: TLabel
    Left = 63
    Top = 9
    Width = 9
    Height = 17
    Caption = 'H'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -14
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
  end
  object Label6: TLabel
    Left = 98
    Top = 9
    Width = 9
    Height = 17
    Caption = 'V'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -14
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
  end
  object Label7: TLabel
    Left = 116
    Top = 9
    Width = 8
    Height = 17
    Caption = 'N'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -14
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
  end
  object F7: TCheckBox
    Left = 9
    Top = 26
    Width = 18
    Height = 19
    TabOrder = 0
  end
  object F6: TCheckBox
    Left = 26
    Top = 26
    Width = 19
    Height = 19
    TabOrder = 1
  end
  object F5: TCheckBox
    Left = 44
    Top = 26
    Width = 18
    Height = 19
    TabOrder = 2
  end
  object F4: TCheckBox
    Left = 61
    Top = 26
    Width = 19
    Height = 19
    TabOrder = 3
  end
  object F3: TCheckBox
    Left = 79
    Top = 26
    Width = 18
    Height = 19
    TabOrder = 4
  end
  object F2: TCheckBox
    Left = 96
    Top = 26
    Width = 19
    Height = 19
    TabOrder = 5
  end
  object F1: TCheckBox
    Left = 113
    Top = 26
    Width = 19
    Height = 19
    TabOrder = 6
  end
  object F0: TCheckBox
    Left = 131
    Top = 26
    Width = 18
    Height = 19
    TabOrder = 7
  end
  object OK: TButton
    Left = 9
    Top = 52
    Width = 62
    Height = 28
    Caption = 'OK'
    TabOrder = 8
    OnClick = OKClick
  end
  object Cancel: TButton
    Left = 79
    Top = 52
    Width = 66
    Height = 28
    Caption = 'Cancel'
    TabOrder = 9
    OnClick = CancelClick
  end
end
