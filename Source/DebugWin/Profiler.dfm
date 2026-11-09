object Profiler: TProfiler
  Left = 230
  Top = 227
  Width = 632
  Height = 204
  BorderStyle = bsSizeToolWin
  Caption = 'Profiler'
  Color = clBtnFace
  Constraints.MinHeight = 204
  Constraints.MinWidth = 536
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Scaled = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 15
  object ButtonNew: TButton
    Left = 9
    Top = 133
    Width = 78
    Height = 27
    Anchors = [akLeft, akBottom]
    Caption = 'New'
    TabOrder = 1
    OnClick = ButtonNewClick
  end
  object ButtonDelete: TButton
    Left = 173
    Top = 133
    Width = 78
    Height = 27
    Anchors = [akLeft, akBottom]
    Caption = 'Delete'
    Enabled = False
    TabOrder = 3
    OnClick = ButtonDeleteClick
  end
  object ListViewProfileSamples: TListView
    Left = 9
    Top = 9
    Width = 604
    Height = 116
    Anchors = [akLeft, akTop, akRight, akBottom]
    Columns = <
      item
        Caption = 'Tag'
        Width = 144
      end
      item
        Caption = 'Start Addr'
        Width = 80
      end
      item
        Caption = 'End Addr'
        Width = 80
      end
      item
        Caption = 'Min T'
        Width = 76
      end
      item
        Caption = 'Max T'
        Width = 76
      end
      item
        Caption = 'Samples'
        Width = 76
      end>
    ColumnClick = False
    GridLines = True
    HideSelection = False
    RowSelect = True
    TabOrder = 0
    ViewStyle = vsReport
    OnEditing = ListViewProfileSamplesEditing
    OnSelectItem = ListViewProfileSamplesSelectItem
  end
  object ButtonEdit: TButton
    Left = 91
    Top = 133
    Width = 78
    Height = 27
    Anchors = [akLeft, akBottom]
    Caption = 'Edit'
    Enabled = False
    TabOrder = 2
    OnClick = ButtonEditClick
  end
  object ButtonReset: TButton
    Left = 255
    Top = 133
    Width = 78
    Height = 27
    Anchors = [akLeft, akBottom]
    Caption = 'Reset'
    Enabled = False
    TabOrder = 4
    OnClick = ButtonResetClick
  end
  object ButtonRefresh: TButton
    Left = 485
    Top = 133
    Width = 57
    Height = 27
    Anchors = [akRight, akBottom]
    Caption = 'Refresh'
    Enabled = False
    TabOrder = 6
    OnClick = ButtonRefreshClick
  end
  object ButtonPlot: TButton
    Left = 554
    Top = 133
    Width = 60
    Height = 27
    Anchors = [akRight, akBottom]
    Caption = 'Plot'
    Enabled = False
    TabOrder = 7
    OnClick = ButtonPlotClick
  end
  object AutoRefresh: TCheckBox
    Left = 346
    Top = 137
    Width = 134
    Height = 19
    Anchors = [akRight, akBottom]
    Caption = 'Auto Refresh'
    Enabled = False
    TabOrder = 5
    OnClick = AutoRefreshClick
  end
  object Timer: TTimer
    Enabled = False
    OnTimer = TimerTimer
    Left = 201
    Top = 131
  end
end
