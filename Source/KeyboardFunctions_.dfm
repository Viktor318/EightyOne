object KeyboardFunctions: TKeyboardFunctions
  Left = 742
  Top = 135
  VertScrollBar.Visible = False
  AutoSize = True
  BorderIcons = []
  BorderStyle = bsToolWindow
  Caption = 'Functions'
  ClientHeight = 54
  ClientWidth = 130
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnKeyDown = FormKeyDown
  OnKeyPress = FormKeyPress
  OnKeyUp = FormKeyUp
  Scaled = False
  PixelsPerInch = 96
  TextHeight = 15
  object zx80IntegralFunctions: TImage
    Left = 0
    Top = 0
    Width = 130
    Height = 54
    Stretch = True
    OnDblClick = zx80IntegralFunctionsDblClick
  end
  object Tracker: TTimer
    Enabled = False
    Interval = 25
    OnTimer = TrackerTimer
    Left = 96
    Top = 24
  end
end
