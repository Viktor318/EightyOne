object HistoryBox: THistoryBox
  Left = 198
  Top = 115
  Width = 560
  Height = 468
  BorderStyle = bsSizeToolWin
  Caption = 'CPU History'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Scaled = False
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 15
  object Text: TRichEdit
    Left = 0
    Top = 35
    Width = 555
    Height = 376
    Anchors = [akLeft, akTop, akRight, akBottom]
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -14
    Font.Name = 'Courier New'
    Font.Style = []
    HideScrollBars = False
    Lines.Strings = (
      'Text')
    ParentFont = False
    PlainText = True
    ReadOnly = True
    ScrollBars = ssVertical
    TabOrder = 0
    WordWrap = False
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 408
    Width = 543
    Height = 21
    Panels = <>
    SimplePanel = False
  end
  object ToolBar1: TToolBar
    Left = 0
    Top = 0
    Width = 543
    Height = 27
    ButtonHeight = 23
    ButtonWidth = 52
    Caption = 'ToolBar1'
    ShowCaptions = True
    TabOrder = 2
    Wrapable = False
    object ToolButtonReload: TToolButton
      Left = 0
      Top = 2
      AutoSize = True
      Caption = 'Reload'
      ImageIndex = 0
      OnClick = ToolButtonReloadClick
    end
    object ToolButton4: TToolButton
      Left = 52
      Top = 2
      Width = 9
      Caption = 'ToolButton4'
      ImageIndex = 2
      Style = tbsDivider
    end
    object ToolButtonClear: TToolButton
      Left = 61
      Top = 2
      AutoSize = True
      Caption = 'Clear'
      ImageIndex = 1
      OnClick = ToolButtonClearClick
    end
    object ToolButton3: TToolButton
      Left = 113
      Top = 2
      Width = 9
      Caption = 'ToolButton3'
      ImageIndex = 2
      Style = tbsDivider
    end
    object ReverseToolButtonReverse: TToolButton
      Left = 122
      Top = 2
      AutoSize = True
      Caption = 'Reverse'
      ImageIndex = 3
      Style = tbsCheck
      OnClick = ReverseToolButtonReverseClick
    end
    object ToolButton2: TToolButton
      Left = 175
      Top = 2
      Width = 8
      Caption = 'ToolButton2'
      ImageIndex = 4
      Style = tbsDivider
    end
    object ToolButton0K8K: TToolButton
      Left = 183
      Top = 2
      AutoSize = True
      Caption = '0K-8K'
      Down = True
      ImageIndex = 5
      Style = tbsCheck
      OnClick = ToolButton0K8KClick
    end
    object ToolButton5: TToolButton
      Left = 236
      Top = 2
      Width = 8
      Caption = 'ToolButton5'
      ImageIndex = 6
      Style = tbsDivider
    end
    object ToolButton8K16K: TToolButton
      Left = 244
      Top = 2
      AutoSize = True
      Caption = '8K-16K'
      Down = True
      ImageIndex = 7
      Style = tbsCheck
      OnClick = ToolButton8K16KClick
    end
    object ToolButton7: TToolButton
      Left = 297
      Top = 2
      Width = 8
      Caption = 'ToolButton7'
      ImageIndex = 8
      Style = tbsDivider
    end
    object ToolButton16K32K: TToolButton
      Left = 305
      Top = 2
      AutoSize = True
      Caption = '16K-32K'
      Down = True
      ImageIndex = 9
      Style = tbsCheck
      OnClick = ToolButton16K32KClick
    end
    object ToolButton6: TToolButton
      Left = 358
      Top = 2
      Width = 9
      Caption = 'ToolButton6'
      ImageIndex = 10
      Style = tbsDivider
    end
    object ToolButton32K48K: TToolButton
      Left = 367
      Top = 2
      AutoSize = True
      Caption = '32K-48K'
      Down = True
      ImageIndex = 11
      Style = tbsCheck
      OnClick = ToolButton32K48KClick
    end
    object ToolButton9: TToolButton
      Left = 419
      Top = 2
      Width = 9
      Caption = 'ToolButton9'
      ImageIndex = 12
      Style = tbsDivider
    end
    object ToolButton48K64K: TToolButton
      Left = 428
      Top = 2
      AutoSize = True
      Caption = '48K-64K'
      Down = True
      ImageIndex = 13
      Style = tbsCheck
      OnClick = ToolButton48K64KClick
    end
    object ToolButton11: TToolButton
      Left = 480
      Top = 2
      Width = 9
      Caption = 'ToolButton11'
      ImageIndex = 14
      Style = tbsDivider
    end
  end
end
