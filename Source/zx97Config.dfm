object ZX97Dialog: TZX97Dialog
  Left = 131
  Top = 101
  BorderIcons = []
  BorderStyle = bsToolWindow
  Caption = 'ZX97 Configuration'
  ClientHeight = 130
  ClientWidth = 383
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
    Width = 40
    Height = 15
    Caption = 'Protect:'
  end
  object Protect08: TCheckBox
    Left = 66
    Top = 9
    Width = 115
    Height = 18
    Caption = '$0000 - $1FFF'
    Checked = True
    State = cbChecked
    TabOrder = 0
  end
  object ProtectAB: TCheckBox
    Left = 66
    Top = 26
    Width = 115
    Height = 19
    Caption = '$A000 - $BFFF'
    TabOrder = 1
  end
  object ProtectB0: TCheckBox
    Left = 66
    Top = 44
    Width = 115
    Height = 18
    Caption = 'Bank 0 (ROM)'
    Checked = True
    State = cbChecked
    TabOrder = 2
  end
  object ProtectB115: TCheckBox
    Left = 66
    Top = 61
    Width = 115
    Height = 19
    Caption = 'Bank 1-15 (RAM)'
    TabOrder = 3
  end
  object SaveRAM: TCheckBox
    Left = 192
    Top = 9
    Width = 183
    Height = 18
    Alignment = taLeftJustify
    BiDiMode = bdLeftToRight
    Caption = 'Save RAM on power down'
    Checked = True
    ParentBiDiMode = False
    State = cbChecked
    TabOrder = 4
  end
  object OK: TButton
    Left = 205
    Top = 96
    Width = 82
    Height = 27
    Caption = 'OK'
    ModalResult = 1
    TabOrder = 6
    OnClick = OKClick
  end
  object SwapRAMROM: TCheckBox
    Left = 252
    Top = 26
    Width = 123
    Height = 19
    Alignment = taLeftJustify
    Caption = 'Swap ROM/RAM'
    TabOrder = 5
  end
  object Cancel: TButton
    Left = 292
    Top = 96
    Width = 82
    Height = 27
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 7
    OnClick = CancelClick
  end
end
