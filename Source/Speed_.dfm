object Speed: TSpeed
  Left = 189
  Top = 227
  BorderIcons = []
  BorderStyle = bsToolWindow
  Caption = 'Speed'
  ClientHeight = 120
  ClientWidth = 437
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
    Width = 437
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
      Left = 13
      Top = 52
      Width = 12
      Height = 15
      Caption = '0x'
    end
    object Label11: TLabel
      Left = 170
      Top = 52
      Width = 22
      Height = 15
      Caption = '1.0x'
    end
    object Label12: TLabel
      Left = 367
      Top = 52
      Width = 21
      Height = 15
      Caption = '4.0x'
    end
    object Label1: TLabel
      Left = 44
      Top = 52
      Width = 28
      Height = 15
      Caption = '0.25x'
    end
    object Label2: TLabel
      Left = 87
      Top = 52
      Width = 22
      Height = 15
      Caption = '0.5x'
    end
    object Label3: TLabel
      Left = 131
      Top = 52
      Width = 28
      Height = 15
      Caption = '0.75x'
    end
    object Label4: TLabel
      Left = 209
      Top = 52
      Width = 29
      Height = 15
      Caption = '1.25x'
    end
    object Label5: TLabel
      Left = 253
      Top = 52
      Width = 22
      Height = 15
      Caption = '1.5x'
    end
    object Label6: TLabel
      Left = 288
      Top = 52
      Width = 28
      Height = 15
      Caption = '1.75x'
    end
    object Label7: TLabel
      Left = 332
      Top = 52
      Width = 21
      Height = 15
      Caption = '2.0x'
    end
    object Label13: TLabel
      Left = 401
      Top = 52
      Width = 22
      Height = 15
      Caption = '8.0x'
    end
    object AccurateSpeed: TTrackBar
      Left = 9
      Top = 17
      Width = 420
      Height = 28
      Orientation = trHorizontal
      PageSize = 1
      Frequency = 1
      Position = 4
      SelEnd = 0
      SelStart = 0
      TabOrder = 0
      TickMarks = tmBottomRight
      TickStyle = tsAuto
      OnChange = Recalc
    end
  end
  object Button1: TButton
    Left = 332
    Top = 87
    Width = 81
    Height = 28
    Caption = 'OK'
    Default = True
    TabOrder = 2
    OnClick = Button1Click
  end
  object EnableFrameSkip: TCheckBox
    Left = 9
    Top = 92
    Width = 145
    Height = 18
    Caption = 'Enable Frame Skip'
    TabOrder = 0
    OnClick = EnableFrameSkipClick
  end
end
