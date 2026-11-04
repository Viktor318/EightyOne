object MidiForm: TMidiForm
  Left = 223
  Top = 190
  BorderIcons = [biSystemMenu]
  BorderStyle = bsToolWindow
  Caption = 'Sound Settings'
  ClientHeight = 494
  ClientWidth = 640
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
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 15
  object GroupBox2: TGroupBox
    Left = 8
    Top = 8
    Width = 624
    Height = 378
    Caption = 'Mix'
    TabOrder = 0
    object StereoLabel: TLabel
      Left = 12
      Top = 24
      Width = 101
      Height = 15
      Caption = 'Stereo mode (AY):'
    end
    object HelpLabel: TLabel
      Left = 12
      Top = 354
      Width = 600
      Height = 15
      AutoSize = False
      Caption =
        'Double click a slider: default value.   Mouse wheel or arrow key' +
        's: 1%, with Shift: 10%.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGrayText
      Font.Height = -12
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
    end
    object StereoBox: TComboBox
      Left = 130
      Top = 20
      Width = 80
      Height = 23
      Style = csDropDownList
      ItemHeight = 15
      TabOrder = 0
      OnChange = StereoBoxChange
    end
    object DefaultsButton: TButton
      Left = 512
      Top = 19
      Width = 100
      Height = 25
      Caption = 'Reset'
      TabOrder = 1
      OnClick = DefaultsButtonClick
    end
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 394
    Width = 300
    Height = 56
    Caption = 'Send MIDI Output to:'
    TabOrder = 1
    object MidiBox: TComboBox
      Left = 12
      Top = 22
      Width = 276
      Height = 23
      Style = csDropDownList
      ItemHeight = 15
      TabOrder = 0
      OnChange = MidiChange
    end
  end
  object VideoSoundGroupBox: TGroupBox
    Left = 316
    Top = 394
    Width = 316
    Height = 56
    Caption = 'Video Sound Settings'
    Enabled = False
    TabOrder = 2
    object BeeperExcludeHSyncs: TCheckBox
      Left = 12
      Top = 24
      Width = 296
      Height = 19
      Caption = 'Exclude ZX81 HSync Pulses'
      TabOrder = 0
      OnClick = BeeperExcludeHSyncsClick
    end
  end
  object OK: TButton
    Left = 552
    Top = 460
    Width = 80
    Height = 25
    Caption = 'OK'
    TabOrder = 3
    OnClick = OKClick
  end
end
