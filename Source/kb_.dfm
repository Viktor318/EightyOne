object Kb: TKb
  Left = 281
  Top = 214
  BorderIcons = []
  BorderStyle = bsToolWindow
  Caption = 'Keyboard'
  ClientHeight = 209
  ClientWidth = 428
  Color = clBtnFace
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
  object CursorKeysLabel: TLabel
    Left = 9
    Top = 22
    Width = 63
    Height = 15
    Caption = 'Cursor keys:'
  end
  object CtrlKeyMapsToLabel: TLabel
    Left = 9
    Top = 61
    Width = 101
    Height = 15
    Caption = 'CTRL Key Maps to:'
  end
  object CursorMode: TComboBox
    Left = 104
    Top = 20
    Width = 149
    Height = 23
    Style = csDropDownList
    ItemHeight = 15
    TabOrder = 0
    OnChange = CursorModeChange
    Items.Strings = (
      '5, 6, 7, 8'
      '5, 6, 7, 8 (Shifted)'
      'C,V,B,N'
      'C,V,B,N (Shifted)'
      'Custom')
  end
  object OK: TButton
    Left = 330
    Top = 175
    Width = 88
    Height = 27
    Caption = 'OK'
    Default = True
    TabOrder = 13
    OnClick = OKClick
  end
  object CustomUp: TEdit
    Left = 375
    Top = 9
    Width = 18
    Height = 23
    Enabled = False
    TabOrder = 3
    Text = '7'
  end
  object CustomDown: TEdit
    Left = 375
    Top = 35
    Width = 18
    Height = 23
    Enabled = False
    TabOrder = 4
    Text = '6'
  end
  object CustomLeft: TEdit
    Left = 349
    Top = 20
    Width = 18
    Height = 23
    Enabled = False
    TabOrder = 1
    Text = '5'
  end
  object CustomRight: TEdit
    Left = 401
    Top = 20
    Width = 18
    Height = 23
    Enabled = False
    TabOrder = 2
    Text = '8'
  end
  object RadioButton1: TRadioButton
    Left = 122
    Top = 61
    Width = 80
    Height = 19
    BiDiMode = bdLeftToRight
    Caption = 'Function'
    ParentBiDiMode = False
    TabOrder = 6
  end
  object RadioButton2: TRadioButton
    Left = 209
    Top = 61
    Width = 80
    Height = 19
    Caption = '0 (Zero)'
    TabOrder = 7
  end
  object UseRightShiftCheckBox: TCheckBox
    Left = 9
    Top = 61
    Width = 193
    Height = 19
    Caption = 'Use Right Shift as Symbol Shift'
    TabOrder = 5
  end
  object UseNumericPadForJoystickCheckBox: TCheckBox
    Left = 9
    Top = 87
    Width = 410
    Height = 19
    Caption = 'Use numeric keypad for joystick input (when Num Lock is on):'
    Checked = True
    State = cbChecked
    TabOrder = 8
    OnClick = UseNumericPadForJoystickCheckBoxClick
  end
  object UseNumericPadForJoystick1: TRadioButton
    Left = 26
    Top = 113
    Width = 89
    Height = 19
    Caption = 'Joystick 1'
    Checked = True
    TabOrder = 9
    TabStop = True
    OnClick = UseNumericPadForJoystick1Click
  end
  object UseNumericPadForJoystick2: TRadioButton
    Left = 122
    Top = 113
    Width = 89
    Height = 19
    Caption = 'Joystick 2'
    TabOrder = 10
    TabStop = True
    OnClick = UseNumericPadForJoystick2Click
  end
  object WindowsLayoutCheckBox: TCheckBox
    Left = 9
    Top = 140
    Width = 245
    Height = 18
    Caption = 'Use the Windows keyboard layout'
    Checked = True
    State = cbChecked
    TabOrder = 11
  end
  object AltShiftCheckBox: TCheckBox
    Left = 9
    Top = 161
    Width = 250
    Height = 19
    Caption = 'Use left Alt as Shift with number keys'
    Checked = True
    State = cbChecked
    TabOrder = 12
  end
  object KeyRepeatCheckBox: TCheckBox
    Left = 9
    Top = 183
    Width = 250
    Height = 19
    Caption = 'Key repeat on ZX80/81'
    TabOrder = 13
  end
end
