object Speed: TSpeed
  Left = 270
  Top = 168
  BorderIcons = []
  BorderStyle = bsToolWindow
  Caption = 'Speed'
  ClientHeight = 122
  ClientWidth = 237
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
  PixelsPerInch = 96
  TextHeight = 15
  object AccuratePanel: TPanel
    Left = 0
    Top = 0
    Width = 228
    Height = 80
    BevelOuter = bvNone
    TabOrder = 1
    object Label9: TLabel
      Left = 9
      Top = 4
      Width = 37
      Height = 15
      Caption = 'Speed:'
    end
    object Label10: TLabel
      Left = 109
      Top = 52
      Width = 19
      Height = 15
      Caption = '16x'
    end
    object Label11: TLabel
      Left = 17
      Top = 52
      Width = 12
      Height = 15
      Caption = '1x'
    end
    object Label12: TLabel
      Left = 201
      Top = 52
      Width = 18
      Height = 15
      Caption = '32x'
    end
    object AccurateSpeed: TTrackBar
      Left = 9
      Top = 17
      Width = 219
      Height = 28
      Max = 100
      Orientation = trHorizontal
      Frequency = 17
      Position = 0
      SelEnd = 0
      SelStart = 0
      TabOrder = 0
      TickMarks = tmBottomRight
      TickStyle = tsAuto
      OnChange = Recalc
    end
  end
  object Button1: TButton
    Left = 151
    Top = 87
    Width = 82
    Height = 28
    Caption = 'OK'
    Default = True
    TabOrder = 2
    OnClick = Button1Click
  end
  object EnableFrameSkip: TCheckBox
    Left = 9
    Top = 92
    Width = 140
    Height = 18
    Caption = 'Enable Frame Skip'
    TabOrder = 0
    OnClick = EnableFrameSkipClick
  end
end
