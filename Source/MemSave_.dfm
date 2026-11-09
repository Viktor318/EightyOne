object MemSave: TMemSave
  Left = 400
  Top = 100
  BorderIcons = [biSystemMenu]
  BorderStyle = bsToolWindow
  Caption = 'MemSave'
  ClientHeight = 137
  ClientWidth = 217
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
    Top = 52
    Width = 44
    Height = 15
    Caption = 'Address:'
  end
  object Label2: TLabel
    Left = 113
    Top = 52
    Width = 26
    Height = 15
    Caption = 'Size:'
  end
  object Label3: TLabel
    Left = 9
    Top = 9
    Width = 49
    Height = 15
    Caption = 'Filename:'
  end
  object Address: TEdit
    Left = 9
    Top = 70
    Width = 97
    Height = 23
    TabOrder = 0
    Text = '0'
  end
  object Size: TEdit
    Left = 113
    Top = 70
    Width = 98
    Height = 23
    TabOrder = 1
    Text = '0'
  end
  object Save: TButton
    Left = 62
    Top = 105
    Width = 92
    Height = 27
    Caption = 'Save File...'
    TabOrder = 2
    OnClick = SaveClick
  end
  object Fname: TEdit
    Left = 9
    Top = 26
    Width = 167
    Height = 23
    TabOrder = 3
  end
  object Browse: TButton
    Left = 183
    Top = 26
    Width = 30
    Height = 23
    Caption = '...'
    TabOrder = 4
    OnClick = BrowseClick
  end
  object SaveDialog: TSaveDialog
    DefaultExt = '*.bin'
    Filter = 
      'Binary Files|*.bin|ROM Files|*.rom|Sprectrum Screen|*.scr|All Fi' +
      'les|*.*'
    Title = 'Save Memory Block...'
    Left = 9
    Top = 105
  end
  object OpenDialog: TOpenDialog
    Filter = 'All Files|*.*|Spectrum Screen|*.scr'
    Left = 183
    Top = 105
  end
end
