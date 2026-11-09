object BasicLister: TBasicLister
  Left = 242
  Top = 124
  BorderIcons = [biSystemMenu]
  BorderStyle = bsToolWindow
  Caption = 'BASIC Listing'
  ClientHeight = 355
  ClientWidth = 298
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  FormStyle = fsStayOnTop
  Icon.Data = {
    0000010002002020100000000000E80200002600000010101000000000002801
    00000E0300002800000020000000400000000100040000000000800200000000
    0000000000000000000000000000000000000000800000800000008080008000
    0000800080008080000080808000C0C0C0000000FF0000FF000000FFFF00FF00
    0000FF00FF00FFFF0000FFFFFF00000000000000000000000000000000007777
    7777777777777777777777777777000000000000000000000000000000000000
    0000000000000000000000000000000070070070070070070070070070000000
    7007007007007007007007007000000000000000000000000000000000000000
    0700700700700700700700700700000007007007007007007007007007000000
    0000000000000000000000000000000070070070070070070070070070000000
    7007007007007007007007007000000000000000000000000000000000000007
    0070070070070070070070070000000700700700700700700700700700000000
    0000000000000000000000000000000000000000000000000000000000000777
    7777777777777777777777777770000000000000000000000000000000000099
    9919091999090000000000000000000900019109090900000000000000000000
    9001910191190000000000000000009999190919990900000000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    0000000000000000000000000000000000000000000000000000000000000770
    7077777777700000000000000000077777777070777700000000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    0000000000000000000000000000FFFFFFFF0000000000000000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    000000000000FFFFFFFFFFFFFFFF280000001000000020000000010004000000
    0000C00000000000000000000000000000000000000000000000000080000080
    00000080800080000000800080008080000080808000C0C0C0000000FF0000FF
    000000FFFF00FF000000FF00FF00FFFF0000FFFFFF0077777777777777770000
    000000000000088878878878878000F7FF7FF7FF7FF00088788788788780F7FF
    7FF7FF7FF0000000000000000000077777777777777000000000000000000999
    0999900000000090909090000000099909999000000000000000000000000000
    0000000000007777777700000000000000000000000000000000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    000000000000000000000000000000000000FFFF0000}
  OldCreateOrder = False
  Scaled = False
  OnClose = FormClose
  OnMouseDown = FormMouseDown
  OnMouseWheel = FormMouseWheel
  OnPaint = FormPaint
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 15
  object ScrollBar: TScrollBar
    Left = 279
    Top = 33
    Width = 19
    Height = 301
    Align = alRight
    Kind = sbVertical
    PageSize = 0
    TabOrder = 0
    OnChange = ScrollBarChange
  end
  object StatusBar: TStatusBar
    Left = 0
    Top = 334
    Width = 298
    Height = 21
    Panels = <
      item
        Width = 153
      end
      item
        Width = 148
      end>
    SimplePanel = False
    SizeGrip = False
  end
  object ToolBar: TToolBar
    Left = 0
    Top = 0
    Width = 298
    Height = 33
    ButtonHeight = 23
    ButtonWidth = 59
    Caption = 'ToolBar'
    EdgeBorders = [ebTop, ebBottom]
    ShowCaptions = True
    TabOrder = 2
    Wrapable = False
    object ToolButtonRefresh: TToolButton
      Left = 0
      Top = 2
      AutoSize = True
      Caption = ' Refresh '
      ImageIndex = 0
      OnClick = ToolButtonRefreshClick
    end
    object ToolButton1: TToolButton
      Left = 59
      Top = 2
      Width = 9
      Caption = 'ToolButton1'
      ImageIndex = 1
      Style = tbsDivider
    end
    object ToolButtonSettings: TToolButton
      Left = 68
      Top = 2
      AutoSize = True
      Caption = 'Settings'
      ImageIndex = 2
      OnClick = ToolButtonSettingsClick
    end
    object ToolButton3: TToolButton
      Left = 121
      Top = 2
      Width = 9
      Caption = 'ToolButton3'
      ImageIndex = 3
      Style = tbsDivider
    end
    object ToolButtonSave: TToolButton
      Left = 130
      Top = 2
      AutoSize = True
      Caption = 'Save'
      ImageIndex = 4
      OnClick = ToolButtonSaveClick
    end
    object ToolButton4: TToolButton
      Left = 189
      Top = 2
      Width = 8
      Caption = 'ToolButton4'
      ImageIndex = 5
      Style = tbsDivider
    end
    object ToolButtonLineEnds: TToolButton
      Left = 197
      Top = 2
      AutoSize = True
      Caption = 'Line Ends'
      ImageIndex = 6
      Style = tbsCheck
      OnClick = ToolButtonLineEndsClick
    end
    object ToolButton5: TToolButton
      Left = 261
      Top = 2
      Width = 6
      Caption = 'ToolButton5'
      ImageIndex = 7
      Style = tbsDivider
    end
    object ToolButtonInfo: TToolButton
      Left = 267
      Top = 2
      AutoSize = True
      Caption = 'Info'
      ImageIndex = 8
      OnClick = ToolButtonInfoClick
    end
  end
  object SaveDialog: TSaveDialog
    Left = 9
    Top = 44
  end
end
