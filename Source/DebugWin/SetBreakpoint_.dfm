object SetBreakpoint: TSetBreakpoint
  Left = 108
  Top = 101
  BorderStyle = bsToolWindow
  Caption = 'Configure Breakpoint'
  ClientHeight = 98
  ClientWidth = 507
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Scaled = False
  OnCreate = FormCreate
  OnKeyPress = FormKeyPress
  PixelsPerInch = 96
  TextHeight = 15
  object LabelBreakAddress: TLabel
    Left = 160
    Top = 9
    Width = 44
    Height = 15
    Caption = 'Address:'
  end
  object LabelConditionValue: TLabel
    Left = 229
    Top = 9
    Width = 51
    Height = 15
    Caption = 'Condition:'
  end
  object LabelBreakValue: TLabel
    Left = 294
    Top = 9
    Width = 33
    Height = 15
    Caption = 'Value:'
  end
  object LabelType: TLabel
    Left = 9
    Top = 9
    Width = 29
    Height = 15
    Caption = 'Type:'
  end
  object LabelConditionAddr: TLabel
    Left = 95
    Top = 9
    Width = 51
    Height = 15
    Caption = 'Condition:'
  end
  object LabelBreakEnabled: TLabel
    Left = 428
    Top = 9
    Width = 46
    Height = 15
    Caption = 'Enabled:'
  end
  object LabelBreakHitcount: TLabel
    Left = 363
    Top = 9
    Width = 23
    Height = 15
    Caption = 'Hits:'
  end
  object FlagList: TComboBox
    Left = 160
    Top = 26
    Width = 62
    Height = 23
    Style = csDropDownList
    ItemHeight = 15
    TabOrder = 2
    Visible = False
    Items.Strings = (
      '0: C'
      '1: N'
      '2: P/V'
      '3: -'
      '4: H'
      '5: -'
      '6 :Z'
      '7: S')
  end
  object RegisterList: TComboBox
    Left = 160
    Top = 26
    Width = 62
    Height = 23
    Style = csDropDownList
    DropDownCount = 18
    ItemHeight = 15
    TabOrder = 1
    Visible = False
    OnChange = RegisterListChange
    Items.Strings = (
      'BC'
      'DE'
      'HL'
      'IX'
      'IY'
      'PC'
      'SP'
      'BC'#39
      'DE'#39
      'HL'#39
      'A'
      'B'
      'C'
      'D'
      'E'
      'H'
      'L'
      'I'
      'R'
      'IXh'
      'IXl'
      'IYh'
      'IYl'
      'A'#39
      'B'#39
      'C'#39
      'D'#39
      'E'#39
      'H'#39
      'L'#39)
  end
  object OK: TButton
    Left = 168
    Top = 61
    Width = 80
    Height = 27
    Caption = 'OK'
    Default = True
    TabOrder = 7
    OnClick = OKClick
  end
  object Cancel: TButton
    Left = 259
    Top = 61
    Width = 80
    Height = 27
    Caption = 'Cancel'
    TabOrder = 8
    OnClick = CancelClick
  end
  object BreakAddress: TEdit
    Left = 160
    Top = 26
    Width = 62
    Height = 23
    CharCase = ecUpperCase
    MaxLength = 5
    TabOrder = 0
    Text = '0'
    OnChange = BreakAddressChange
  end
  object BreakValue: TEdit
    Left = 294
    Top = 26
    Width = 62
    Height = 23
    CharCase = ecUpperCase
    MaxLength = 5
    TabOrder = 4
    Text = '0'
    OnChange = BreakValueChange
  end
  object BreakConditionAddr: TComboBox
    Left = 95
    Top = 26
    Width = 58
    Height = 23
    Style = csDropDownList
    DropDownCount = 5
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Segoe UI'
    Font.Style = []
    ItemHeight = 15
    ParentFont = False
    TabOrder = 10
    OnChange = BreakConditionAddrChange
  end
  object BreakType: TComboBox
    Left = 9
    Top = 26
    Width = 79
    Height = 23
    Style = csDropDownList
    DropDownCount = 13
    ItemHeight = 15
    TabOrder = 9
    OnChange = BreakTypeChange
    Items.Strings = (
      'Execute'
      'Read'
      'Write'
      'Out A15-0'
      'Out A15-8'
      'Out A7-0'
      'In A15-0'
      'In A15-8'
      'In A7-0'
      'T-Cycles'
      'Memory'
      'Register'
      'Flag'
      'BASIC')
  end
  object BreakConditionValue: TComboBox
    Left = 229
    Top = 26
    Width = 58
    Height = 23
    Style = csDropDownList
    ItemHeight = 15
    TabOrder = 3
  end
  object ComboBoxBreakEnabled: TComboBox
    Left = 428
    Top = 26
    Width = 70
    Height = 23
    Style = csDropDownList
    ItemHeight = 15
    TabOrder = 6
    Items.Strings = (
      'Yes'
      'No')
  end
  object BreakHitCount: TComboBox
    Left = 363
    Top = 26
    Width = 58
    Height = 23
    Style = csDropDownList
    ItemHeight = 15
    TabOrder = 5
  end
end
