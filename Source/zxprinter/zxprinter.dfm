object ZXPrinter: TZXPrinter
  Left = 416
  Top = 115
  Anchors = [akLeft, akTop, akBottom]
  BorderIcons = [biSystemMenu]
  BorderStyle = bsToolWindow
  Caption = 'ZX Printer'
  ClientHeight = 494
  ClientWidth = 298
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
  object DisplayImage: TImage
    Left = 0
    Top = 0
    Width = 279
    Height = 289
  end
  object ScrollBar: TScrollBar
    Left = 279
    Top = 0
    Width = 19
    Height = 289
    Anchors = [akTop, akRight]
    Kind = sbVertical
    PageSize = 0
    TabOrder = 0
    OnChange = ScrollBarChange
  end
  object SaveImage: TButton
    Left = 9
    Top = 297
    Width = 61
    Height = 27
    Caption = 'Save'
    TabOrder = 1
    OnClick = SaveImageClick
  end
  object ClearImage: TButton
    Left = 76
    Top = 297
    Width = 61
    Height = 27
    Caption = 'Clear'
    TabOrder = 2
    OnClick = ClearImageClick
  end
  object OptionsBtn: TButton
    Left = 212
    Top = 297
    Width = 77
    Height = 27
    Caption = 'Options'
    TabOrder = 4
    OnClick = OptionsBtnClick
  end
  object Panel1: TPanel
    Left = 9
    Top = 332
    Width = 280
    Height = 158
    TabOrder = 5
    object BitRateTxt: TLabel
      Left = 26
      Top = 5
      Width = 80
      Height = 15
      Caption = 'Carriage Speed'
    end
    object LineRateTxt: TLabel
      Left = 26
      Top = 67
      Width = 97
      Height = 15
      Caption = 'Paper Feed Speed'
    end
    object Label1: TLabel
      Left = 17
      Top = 132
      Width = 29
      Height = 15
      Caption = 'Style:'
    end
    object BitRate: TTrackBar
      Left = 9
      Top = 23
      Width = 263
      Height = 36
      Max = 100
      Orientation = trHorizontal
      Frequency = 5
      Position = 0
      SelEnd = 0
      SelStart = 0
      TabOrder = 0
      TickMarks = tmBottomRight
      TickStyle = tsAuto
      OnChange = BitRateChange
    end
    object LineRate: TTrackBar
      Left = 9
      Top = 84
      Width = 263
      Height = 36
      Max = 100
      Orientation = trHorizontal
      Frequency = 5
      Position = 0
      SelEnd = 0
      SelStart = 0
      TabOrder = 1
      TickMarks = tmBottomRight
      TickStyle = tsAuto
      OnChange = LineRateChange
    end
    object Style: TComboBox
      Left = 52
      Top = 128
      Width = 106
      Height = 23
      Style = csDropDownList
      ItemHeight = 15
      TabOrder = 2
      OnChange = StyleChange
      Items.Strings = (
        'ZX Printer'
        'Alphacom 32'
        'Plain Paper')
    end
  end
  object Feed: TButton
    Left = 144
    Top = 297
    Width = 61
    Height = 27
    Caption = 'Feed'
    TabOrder = 3
    OnClick = FeedClick
    OnMouseDown = FeedMouseDown
    OnMouseUp = FeedMouseUp
  end
  object SaveDialog: TSaveDialog
    DefaultExt = 'png'
    Filter = 'PNG Image (PNG)|*.png|Windows Bitmap (BMP)|*.bmp'
    Left = 9
    Top = 9
  end
  object FeedTimer: TTimer
    Enabled = False
    Interval = 100
    OnTimer = FeedTimerExpired
    Left = 253
    Top = 454
  end
end
