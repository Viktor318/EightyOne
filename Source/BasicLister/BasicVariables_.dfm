object BasicVariables: TBasicVariables
  Left = 186
  Top = 187
  HorzScrollBar.ButtonSize = 1
  HorzScrollBar.Margin = 1
  HorzScrollBar.ThumbSize = 1
  Anchors = [akLeft, akTop, akRight, akBottom]
  BorderIcons = []
  BorderStyle = bsToolWindow
  Caption = 'BASIC Variables'
  ClientHeight = 106
  ClientWidth = 340
  Color = clBtnFace
  Constraints.MaxHeight = 1743
  Constraints.MaxWidth = 1090
  Constraints.MinHeight = 85
  Constraints.MinWidth = 87
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnClose = FormClose
  OnMouseDown = FormMouseDown
  OnMouseWheel = FormMouseWheel
  OnPaint = FormPaint
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 15
  object StatusBar: TStatusBar
    Left = 0
    Top = 85
    Width = 340
    Height = 21
    Panels = <
      item
        Width = 164
      end
      item
        Width = 164
      end>
    SimplePanel = False
    SizeGrip = False
  end
  object ScrollBar: TScrollBar
    Left = 322
    Top = 0
    Width = 18
    Height = 85
    Align = alRight
    Kind = sbVertical
    PageSize = 0
    TabOrder = 1
  end
  object BasicVariablesRefreshTimer: TTimer
    Enabled = False
    Interval = 50
    OnTimer = BasicVariablesRefreshTimerTimer
    Left = 35
    Top = 17
  end
end
