object CreateHDF: TCreateHDF
  Left = 344
  Top = 84
  BorderIcons = [biSystemMenu]
  BorderStyle = bsToolWindow
  Caption = 'Create HDF File'
  ClientHeight = 165
  ClientWidth = 353
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
  object Label1: TLabel
    Left = 9
    Top = 13
    Width = 54
    Height = 15
    Caption = 'File Name:'
  end
  object Label2: TLabel
    Left = 13
    Top = 48
    Width = 49
    Height = 15
    Alignment = taRightJustify
    Caption = 'Cylinders:'
  end
  object Label3: TLabel
    Left = 25
    Top = 74
    Width = 37
    Height = 15
    Alignment = taRightJustify
    Caption = 'Heads:'
  end
  object Label4: TLabel
    Left = 20
    Top = 100
    Width = 42
    Height = 15
    Alignment = taRightJustify
    Caption = 'Sectors:'
  end
  object Label5: TLabel
    Left = 216
    Top = 74
    Width = 75
    Height = 15
    Alignment = taRightJustify
    Caption = 'Effective Size:'
  end
  object Label6: TLabel
    Left = 203
    Top = 100
    Width = 88
    Height = 15
    Alignment = taRightJustify
    Caption = 'HDF Image Size:'
  end
  object FName: TEdit
    Left = 70
    Top = 9
    Width = 274
    Height = 23
    ReadOnly = True
    TabOrder = 0
    Text = 'FName'
  end
  object CylText: TEdit
    Left = 70
    Top = 44
    Width = 62
    Height = 23
    TabOrder = 1
    Text = '16'
    OnChange = CylTextChange
  end
  object HeadText: TEdit
    Left = 70
    Top = 70
    Width = 62
    Height = 23
    TabOrder = 3
    Text = '16'
    OnChange = HeadTextChange
  end
  object SecText: TEdit
    Left = 70
    Top = 96
    Width = 62
    Height = 23
    TabOrder = 5
    Text = '16'
    OnChange = SecTextChange
  end
  object CylUpDown: TUpDown
    Left = 133
    Top = 44
    Width = 43
    Height = 23
    Min = -32768
    Max = 32767
    Orientation = udHorizontal
    Position = -31745
    TabOrder = 2
    Wrap = False
    OnClick = CylUpDownClick
  end
  object HeadUpDown: TUpDown
    Left = 133
    Top = 70
    Width = 43
    Height = 23
    Min = 1
    Max = 15
    Orientation = udHorizontal
    Position = 8
    TabOrder = 4
    Wrap = False
    OnClick = HeadUpDownClick
  end
  object SecUpDown: TUpDown
    Left = 133
    Top = 96
    Width = 43
    Height = 23
    Min = 1
    Max = 255
    Orientation = udHorizontal
    Position = 128
    TabOrder = 6
    Wrap = False
    OnClick = SecUpDownClick
  end
  object EightBit: TCheckBox
    Left = 205
    Top = 44
    Width = 108
    Height = 18
    Alignment = taLeftJustify
    BiDiMode = bdLeftToRight
    Caption = '8 Bit Image'
    ParentBiDiMode = False
    TabOrder = 7
    OnClick = EightBitClick
  end
  object EffSize: TEdit
    Left = 299
    Top = 70
    Width = 45
    Height = 23
    ReadOnly = True
    TabOrder = 8
    Text = '0'
  end
  object HDFSize: TEdit
    Left = 299
    Top = 96
    Width = 45
    Height = 23
    ReadOnly = True
    TabOrder = 9
    Text = '0'
  end
  object OK: TButton
    Left = 264
    Top = 131
    Width = 82
    Height = 27
    Caption = 'OK'
    TabOrder = 10
    OnClick = OKClick
  end
  object Progress: TProgressBar
    Left = 9
    Top = 135
    Width = 248
    Height = 19
    Min = 0
    Max = 983025
    TabOrder = 11
  end
end
