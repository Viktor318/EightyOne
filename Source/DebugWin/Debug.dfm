object Dbg: TDbg
  Left = 517
  Top = 254
  BorderIcons = [biSystemMenu]
  BorderStyle = bsToolWindow
  Caption = 'Debug Window'
  ClientHeight = 511
  ClientWidth = 810
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  FormStyle = fsStayOnTop
  KeyPreview = True
  OldCreateOrder = False
  Scaled = False
  OnClose = FormClose
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 15
  object GroupBox1: TGroupBox
    Left = 4
    Top = 3
    Width = 363
    Height = 150
    Caption = 'Z80'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Courier New'
    Font.Pitch = fpFixed
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object Label1: TLabel
      Left = 12
      Top = 52
      Width = 27
      Height = 15
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'HL:'
    end
    object Label2: TLabel
      Left = 12
      Top = 17
      Width = 27
      Height = 15
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'BC:'
    end
    object Label3: TLabel
      Left = 12
      Top = 35
      Width = 27
      Height = 15
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'DE:'
    end
    object Label4: TLabel
      Left = 12
      Top = 70
      Width = 27
      Height = 15
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'A:'
    end
    object Label5: TLabel
      Left = 12
      Top = 87
      Width = 27
      Height = 15
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'F:'
    end
    object HL: TLabel
      Left = 44
      Top = 52
      Width = 43
      Height = 15
      AutoSize = False
      Caption = '$FFFF'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
      PopupMenu = MemDumpPopup
      OnClick = HLClick
      OnMouseDown = HLMouseDown
    end
    object BC: TLabel
      Left = 44
      Top = 17
      Width = 43
      Height = 15
      AutoSize = False
      Caption = '$FFFF'
      Color = clBtnFace
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentColor = False
      ParentFont = False
      ParentShowHint = False
      PopupMenu = MemDumpPopup
      ShowHint = False
      OnClick = BCClick
      OnMouseDown = BCMouseDown
    end
    object DE: TLabel
      Left = 44
      Top = 35
      Width = 43
      Height = 15
      AutoSize = False
      Caption = '$FFFF'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
      PopupMenu = MemDumpPopup
      OnClick = DEClick
      OnMouseDown = DEMouseDown
    end
    object A: TLabel
      Left = 44
      Top = 70
      Width = 43
      Height = 15
      AutoSize = False
      Caption = '$FF'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
      OnClick = AClick
    end
    object F: TLabel
      Left = 44
      Top = 87
      Width = 62
      Height = 15
      AutoSize = False
      Caption = '00000000'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
      OnClick = FClick
    end
    object Label6: TLabel
      Left = 129
      Top = 52
      Width = 32
      Height = 15
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'HL'#39':'
    end
    object HL_: TLabel
      Left = 166
      Top = 52
      Width = 43
      Height = 15
      AutoSize = False
      Caption = '$FFFF'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
      PopupMenu = MemDumpPopup
      OnClick = HL_Click
      OnMouseDown = HL_MouseDown
    end
    object BC_: TLabel
      Left = 166
      Top = 17
      Width = 43
      Height = 15
      AutoSize = False
      Caption = '$FFFF'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
      PopupMenu = MemDumpPopup
      OnClick = BC_Click
      OnMouseDown = BC_MouseDown
    end
    object DE_: TLabel
      Left = 166
      Top = 35
      Width = 43
      Height = 15
      AutoSize = False
      Caption = '$FFFF'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
      PopupMenu = MemDumpPopup
      OnClick = DE_Click
      OnMouseDown = DE_MouseDown
    end
    object A_: TLabel
      Left = 166
      Top = 70
      Width = 43
      Height = 15
      AutoSize = False
      Caption = '$FF'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
      OnClick = A_Click
    end
    object F_: TLabel
      Left = 166
      Top = 87
      Width = 62
      Height = 15
      AutoSize = False
      Caption = '00000000'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
      OnClick = F_Click
    end
    object Label12: TLabel
      Left = 129
      Top = 87
      Width = 32
      Height = 15
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'F'#39':'
    end
    object Label13: TLabel
      Left = 129
      Top = 70
      Width = 32
      Height = 15
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'A'#39':'
    end
    object Label14: TLabel
      Left = 129
      Top = 35
      Width = 32
      Height = 15
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'DE'#39':'
    end
    object Label15: TLabel
      Left = 129
      Top = 17
      Width = 32
      Height = 15
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'BC'#39':'
    end
    object Label16: TLabel
      Left = 253
      Top = 17
      Width = 22
      Height = 15
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'IX:'
    end
    object IX: TLabel
      Left = 279
      Top = 17
      Width = 44
      Height = 15
      AutoSize = False
      Caption = '$FFFF'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
      PopupMenu = MemDumpPopup
      OnClick = IXClick
      OnMouseDown = IXMouseDown
    end
    object Label18: TLabel
      Left = 253
      Top = 35
      Width = 22
      Height = 15
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'IY:'
    end
    object IY: TLabel
      Left = 279
      Top = 35
      Width = 44
      Height = 15
      AutoSize = False
      Caption = '$FFFF'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
      PopupMenu = MemDumpPopup
      OnClick = IYClick
      OnMouseDown = IYMouseDown
    end
    object Label20: TLabel
      Left = 253
      Top = 52
      Width = 22
      Height = 15
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'IR:'
    end
    object IR: TLabel
      Left = 279
      Top = 52
      Width = 44
      Height = 15
      AutoSize = False
      Caption = '$FFFF'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
      PopupMenu = MemDumpPopup
      OnClick = IRClick
      OnMouseDown = IRMouseDown
    end
    object Label22: TLabel
      Left = 253
      Top = 70
      Width = 22
      Height = 15
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'SP:'
    end
    object SP: TLabel
      Left = 279
      Top = 70
      Width = 44
      Height = 15
      AutoSize = False
      Caption = '$FFFF'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
      PopupMenu = MemDumpPopup
      OnClick = SPClick
      OnMouseDown = SPMouseDown
    end
    object Label24: TLabel
      Left = 253
      Top = 87
      Width = 22
      Height = 15
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'PC:'
    end
    object PC: TLabel
      Left = 279
      Top = 87
      Width = 44
      Height = 15
      AutoSize = False
      Caption = '$FFFF'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
      PopupMenu = MemDumpPopup
      OnClick = PCClick
      OnMouseDown = PCMouseDown
    end
    object Label7: TLabel
      Left = 284
      Top = 122
      Width = 26
      Height = 15
      AutoSize = False
      Caption = 'IM:'
    end
    object IM: TLabel
      Left = 308
      Top = 122
      Width = 10
      Height = 15
      AutoSize = False
      Caption = '0'
      OnClick = IMClick
    end
    object Label9: TLabel
      Left = 92
      Top = 122
      Width = 102
      Height = 15
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Interrupts:'
    end
    object Interrupts: TLabel
      Left = 197
      Top = 122
      Width = 61
      Height = 15
      Caption = 'Disabled'
      OnClick = InterruptsClick
    end
    object Label11: TLabel
      Left = 12
      Top = 122
      Width = 40
      Height = 15
      AutoSize = False
      Caption = 'Halt:'
    end
    object Halt: TLabel
      Left = 56
      Top = 122
      Width = 30
      Height = 15
      AutoSize = False
      Caption = 'Yes'
    end
    object Label28: TLabel
      Left = 44
      Top = 103
      Width = 61
      Height = 15
      Caption = 'SZ-H-VNC'
      Font.Charset = ANSI_CHARSET
      Font.Color = clGrayText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
    end
    object Label29: TLabel
      Left = 166
      Top = 103
      Width = 61
      Height = 15
      Caption = 'SZ-H-VNC'
      Font.Charset = ANSI_CHARSET
      Font.Color = clGrayText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
    end
  end
  object GroupBox2: TGroupBox
    Left = 4
    Top = 248
    Width = 431
    Height = 259
    Caption = 'Program'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    object Disass0: TLabel
      Left = 9
      Top = 40
      Width = 328
      Height = 16
      Caption = 'LABEL000    $0000  xxxxxxxx  LD HL,(IX+$zz)'
      Color = clBtnFace
      Font.Charset = ANSI_CHARSET
      Font.Color = clGrayText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentColor = False
      ParentFont = False
      ShowAccelChar = False
    end
    object Disass1: TLabel
      Left = 9
      Top = 58
      Width = 76
      Height = 15
      Caption = '<history2>'
      Color = clBtnFace
      Font.Charset = ANSI_CHARSET
      Font.Color = clGrayText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentColor = False
      ParentFont = False
      ShowAccelChar = False
    end
    object Disass2: TLabel
      Left = 9
      Top = 75
      Width = 76
      Height = 16
      Caption = '<history3>'
      Color = clBtnFace
      Font.Charset = ANSI_CHARSET
      Font.Color = clGrayText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentColor = False
      ParentFont = False
      ShowAccelChar = False
    end
    object Disass3: TLabel
      Left = 9
      Top = 93
      Width = 382
      Height = 15
      Caption = '<currentPC> 01234567890123456789012345678901234567'
      Color = clHighlightText
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentColor = False
      ParentFont = False
      PopupMenu = MemDumpPopup
      ShowAccelChar = False
      OnMouseDown = Disass3MouseDown
    end
    object Disass4: TLabel
      Left = 9
      Top = 110
      Width = 7
      Height = 15
      Caption = '"'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
      PopupMenu = MemDumpPopup
      ShowAccelChar = False
      OnMouseDown = Disass3MouseDown
    end
    object Disass5: TLabel
      Left = 9
      Top = 128
      Width = 7
      Height = 15
      Caption = '"'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
      PopupMenu = MemDumpPopup
      ShowAccelChar = False
      OnMouseDown = Disass3MouseDown
    end
    object Disass6: TLabel
      Left = 9
      Top = 145
      Width = 7
      Height = 15
      Caption = '"'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
      PopupMenu = MemDumpPopup
      ShowAccelChar = False
      OnMouseDown = Disass3MouseDown
    end
    object Disass7: TLabel
      Left = 9
      Top = 163
      Width = 7
      Height = 15
      Caption = '"'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
      PopupMenu = MemDumpPopup
      ShowAccelChar = False
      OnMouseDown = Disass3MouseDown
    end
    object Disass8: TLabel
      Left = 9
      Top = 180
      Width = 7
      Height = 15
      Caption = '"'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
      PopupMenu = MemDumpPopup
      ShowAccelChar = False
      OnMouseDown = Disass3MouseDown
    end
    object Disass9: TLabel
      Left = 9
      Top = 197
      Width = 7
      Height = 16
      Caption = '"'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
      PopupMenu = MemDumpPopup
      ShowAccelChar = False
      OnMouseDown = Disass3MouseDown
    end
    object Label8: TLabel
      Left = 9
      Top = 23
      Width = 282
      Height = 15
      Caption = 'Label       Addr   Code      Mnemonic'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
    end
    object Disass10: TLabel
      Left = 9
      Top = 215
      Width = 7
      Height = 15
      Caption = '"'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
      PopupMenu = MemDumpPopup
      ShowAccelChar = False
      OnMouseDown = Disass3MouseDown
    end
    object Disass11: TLabel
      Left = 9
      Top = 232
      Width = 7
      Height = 16
      Caption = '"'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
      PopupMenu = MemDumpPopup
      ShowAccelChar = False
      OnMouseDown = Disass3MouseDown
    end
  end
  object GroupBoxZX81: TGroupBox
    Left = 442
    Top = 377
    Width = 364
    Height = 130
    Caption = 'Display Generation'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    TabOrder = 10
    object Label39: TLabel
      Left = 89
      Top = 20
      Width = 115
      Height = 15
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'NMI Generator:'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
    end
    object Label10: TLabel
      Left = 89
      Top = 37
      Width = 115
      Height = 15
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Sync Output:'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
    end
    object Label19: TLabel
      Left = 89
      Top = 55
      Width = 115
      Height = 15
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Scanline:'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
    end
    object Label21: TLabel
      Left = 89
      Top = 72
      Width = 115
      Height = 15
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Line Counter:'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
    end
    object Label23: TLabel
      Left = 89
      Top = 89
      Width = 115
      Height = 16
      Alignment = taRightJustify
      Caption = 'Shift Register:'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
    end
    object NMIGen: TLabel
      Left = 209
      Top = 20
      Width = 63
      Height = 15
      AutoSize = False
      Caption = 'OFF'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      OnClick = NMIGenClick
    end
    object Scanline: TLabel
      Left = 209
      Top = 55
      Width = 63
      Height = 15
      AutoSize = False
      Caption = '299'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
    end
    object RowCount: TLabel
      Left = 209
      Top = 72
      Width = 63
      Height = 15
      AutoSize = False
      Caption = '7'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
    end
    object ShiftReg: TLabel
      Left = 209
      Top = 89
      Width = 63
      Height = 15
      AutoSize = False
      Caption = '00000000'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
    end
    object SyncOutput: TLabel
      Left = 209
      Top = 37
      Width = 63
      Height = 15
      AutoSize = False
      Caption = 'White'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      OnClick = SyncOutputClick
    end
    object Label27: TLabel
      Left = 135
      Top = 107
      Width = 69
      Height = 15
      Alignment = taRightJustify
      Caption = 'T-Cycles:'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
    end
    object TStates: TLabel
      Left = 209
      Top = 107
      Width = 63
      Height = 15
      AutoSize = False
      Caption = '00000000'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
    end
  end
  object GroupBox4: TGroupBox
    Left = 442
    Top = 3
    Width = 364
    Height = 242
    Caption = 'Breakpoints'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    object Label31: TLabel
      Left = 10
      Top = 17
      Width = 114
      Height = 16
      Caption = 'T-Cycles Count:'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      PopupMenu = MemDumpPopup
      OnClick = Stack0Click
      OnMouseDown = Stack0MouseDown
    end
    object TStatesCount: TLabel
      Left = 131
      Top = 17
      Width = 62
      Height = 16
      AutoSize = False
      Caption = '0'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
      PopupMenu = MemDumpPopup
      OnClick = HL_Click
      OnMouseDown = HL_MouseDown
    end
    object AddBrkBtn: TButton
      Left = 8
      Top = 206
      Width = 84
      Height = 27
      Caption = 'Add'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = AddBrkBtnClick
    end
    object DelBrkBtn: TButton
      Left = 96
      Top = 206
      Width = 84
      Height = 27
      Caption = 'Delete'
      Enabled = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnClick = DelBrkBtnClick
    end
    object BPList: TStringGrid
      Left = 8
      Top = 39
      Width = 348
      Height = 158
      ColCount = 1
      DefaultColWidth = 11
      DefaultRowHeight = 16
      FixedCols = 0
      RowCount = 1
      FixedRows = 0
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goRowSelect]
      PopupMenu = BreakpointWindowPopup
      ScrollBars = ssVertical
      TabOrder = 4
      OnContextPopup = BPListContextPopup
      OnDblClick = BPListDblClick
      OnDrawCell = BPListDrawCell
      OnSelectCell = BPListSelectCell
    end
    object EditBrkBtn: TButton
      Left = 184
      Top = 206
      Width = 84
      Height = 27
      Caption = 'Edit'
      Enabled = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      OnClick = EditBrkBtnClick
    end
    object ResetBrkBtn: TButton
      Left = 272
      Top = 206
      Width = 84
      Height = 27
      Caption = 'Reset'
      Enabled = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
      OnClick = ResetBrkBtnClick
    end
  end
  object GroupBox5: TGroupBox
    Left = 373
    Top = 3
    Width = 62
    Height = 150
    Caption = 'Stack'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    object Stack0: TLabel
      Left = 12
      Top = 17
      Width = 38
      Height = 16
      Caption = '$FFFF'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
      PopupMenu = MemDumpPopup
      OnClick = Stack0Click
      OnMouseDown = Stack0MouseDown
    end
    object Stack1: TLabel
      Left = 12
      Top = 35
      Width = 38
      Height = 15
      Caption = '$FFFF'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      PopupMenu = MemDumpPopup
      OnClick = Stack0Click
      OnMouseDown = Stack0MouseDown
    end
    object Stack2: TLabel
      Left = 12
      Top = 52
      Width = 38
      Height = 16
      Caption = '$FFFF'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      PopupMenu = MemDumpPopup
      OnClick = Stack0Click
      OnMouseDown = Stack0MouseDown
    end
    object Stack3: TLabel
      Left = 12
      Top = 70
      Width = 38
      Height = 15
      Caption = '$FFFF'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      PopupMenu = MemDumpPopup
      OnClick = Stack0Click
      OnMouseDown = Stack0MouseDown
    end
    object Stack4: TLabel
      Left = 12
      Top = 87
      Width = 38
      Height = 16
      Caption = '$FFFF'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      PopupMenu = MemDumpPopup
      OnClick = Stack0Click
      OnMouseDown = Stack0MouseDown
    end
    object Stack5: TLabel
      Left = 12
      Top = 105
      Width = 38
      Height = 15
      Caption = '$FFFF'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      PopupMenu = MemDumpPopup
      OnClick = Stack0Click
      OnMouseDown = Stack0MouseDown
    end
    object Stack6: TLabel
      Left = 12
      Top = 122
      Width = 38
      Height = 15
      Caption = '$FFFF'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      PopupMenu = MemDumpPopup
      OnClick = Stack0Click
      OnMouseDown = Stack0MouseDown
    end
  end
  object GroupBoxAce: TGroupBox
    Left = 442
    Top = 335
    Width = 364
    Height = 172
    Caption = 'Jupiter Ace Forth Stack'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    TabOrder = 7
    object AceStk0: TLabel
      Left = 131
      Top = 22
      Width = 38
      Height = 15
      Caption = '$FFFF'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
      OnClick = AceStk0Click
    end
    object AceStk1: TLabel
      Left = 131
      Top = 39
      Width = 38
      Height = 16
      Caption = '$FFFF'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      OnClick = AceStk0Click
    end
    object AceStk2: TLabel
      Left = 131
      Top = 57
      Width = 38
      Height = 15
      Caption = '$FFFF'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      OnClick = AceStk0Click
    end
    object AceStk3: TLabel
      Left = 131
      Top = 74
      Width = 38
      Height = 15
      Caption = '$FFFF'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      OnClick = AceStk0Click
    end
    object AceStk4: TLabel
      Left = 131
      Top = 92
      Width = 38
      Height = 15
      Caption = '$FFFF'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      OnClick = AceStk0Click
    end
    object AceStk5: TLabel
      Left = 131
      Top = 109
      Width = 38
      Height = 15
      Caption = '$FFFF'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      OnClick = AceStk0Click
    end
    object AceStk6: TLabel
      Left = 131
      Top = 127
      Width = 38
      Height = 15
      Caption = '$FFFF'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      OnClick = AceStk0Click
    end
    object AceStk7: TLabel
      Left = 131
      Top = 144
      Width = 38
      Height = 15
      Caption = '$FFFF'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      OnClick = AceStk0Click
    end
    object Label37: TLabel
      Left = 174
      Top = 22
      Width = 8
      Height = 15
      Caption = ':'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
      OnClick = Stack0Click
    end
    object Label38: TLabel
      Left = 174
      Top = 39
      Width = 8
      Height = 16
      Caption = ':'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
    end
    object Label40: TLabel
      Left = 174
      Top = 57
      Width = 8
      Height = 15
      Caption = ':'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
    end
    object Label41: TLabel
      Left = 174
      Top = 74
      Width = 8
      Height = 15
      Caption = ':'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
    end
    object Label42: TLabel
      Left = 174
      Top = 92
      Width = 8
      Height = 15
      Caption = ':'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
    end
    object Label43: TLabel
      Left = 174
      Top = 109
      Width = 8
      Height = 15
      Caption = ':'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
    end
    object Label44: TLabel
      Left = 174
      Top = 127
      Width = 8
      Height = 15
      Caption = ':'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
    end
    object Label45: TLabel
      Left = 174
      Top = 144
      Width = 8
      Height = 15
      Caption = ':'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
    end
    object AceStkVal0: TLabel
      Left = 192
      Top = 22
      Width = 38
      Height = 15
      Caption = '$FFFF'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
      OnClick = AceStkVal0Click
    end
    object AceStkVal1: TLabel
      Left = 192
      Top = 39
      Width = 38
      Height = 16
      Caption = '$FFFF'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      OnClick = AceStkVal0Click
    end
    object AceStkVal2: TLabel
      Left = 192
      Top = 57
      Width = 38
      Height = 15
      Caption = '$FFFF'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      OnClick = AceStkVal0Click
    end
    object AceStkVal3: TLabel
      Left = 192
      Top = 74
      Width = 38
      Height = 15
      Caption = '$FFFF'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      OnClick = AceStkVal0Click
    end
    object AceStkVal4: TLabel
      Left = 192
      Top = 92
      Width = 38
      Height = 15
      Caption = '$FFFF'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      OnClick = AceStkVal0Click
    end
    object AceStkVal5: TLabel
      Left = 192
      Top = 109
      Width = 38
      Height = 15
      Caption = '$FFFF'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      OnClick = AceStkVal0Click
    end
    object AceStkVal6: TLabel
      Left = 192
      Top = 127
      Width = 38
      Height = 15
      Caption = '$FFFF'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      OnClick = AceStkVal0Click
    end
    object AceStkVal7: TLabel
      Left = 192
      Top = 144
      Width = 38
      Height = 15
      Caption = '$FFFF'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
      OnClick = AceStkVal0Click
    end
  end
  object Execute: TGroupBox
    Left = 4
    Top = 154
    Width = 290
    Height = 91
    Caption = 'Control'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    object SingleStep: TButton
      Left = 10
      Top = 23
      Width = 64
      Height = 27
      Hint = 'Shift+F5'
      Caption = 'Step'
      Enabled = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Segoe UI'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
      OnClick = SingleStepClick
    end
    object StepOver: TButton
      Left = 79
      Top = 23
      Width = 64
      Height = 27
      Hint = 'Shift+F6'
      Caption = 'Over'
      Enabled = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Segoe UI'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 1
      OnClick = StepOverClick
    end
    object RunStop: TButton
      Left = 217
      Top = 23
      Width = 64
      Height = 27
      Hint = 'Shift+F8'
      Caption = 'Stop'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Segoe UI'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 3
      OnClick = RunStopClick
    end
    object Continuous: TCheckBox
      Left = 203
      Top = 61
      Width = 85
      Height = 19
      Caption = 'Continuous'
      Enabled = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Segoe UI'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
      TabOrder = 6
      OnClick = ContinuousClick
    end
    object SkipNMIBtn: TCheckBox
      Left = 10
      Top = 61
      Width = 95
      Height = 19
      Caption = 'Skip NMI'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Segoe UI'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
      TabOrder = 4
      OnClick = SkipNMIBtnClick
    end
    object SkipINTBtn: TCheckBox
      Left = 108
      Top = 61
      Width = 91
      Height = 19
      Caption = 'Skip INT'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Segoe UI'
      Font.Pitch = fpFixed
      Font.Style = []
      ParentFont = False
      TabOrder = 5
      OnClick = SkipINTBtnClick
    end
    object StepOut: TButton
      Left = 148
      Top = 23
      Width = 64
      Height = 27
      Hint = 'Shift+F7'
      Caption = 'Out'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 2
      OnClick = StepOutClick
    end
  end
  object GroupBox3: TGroupBox
    Left = 442
    Top = 248
    Width = 364
    Height = 85
    Caption = 'View'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    TabOrder = 6
    object SymRom: TLabel
      Left = 184
      Top = 58
      Width = 28
      Height = 15
      Hint = 'Indicates whether ROM symbols are loaded'
      Caption = 'ROM'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
    end
    object SymApp: TLabel
      Left = 216
      Top = 58
      Width = 23
      Height = 15
      Hint = 'Indicates whether application symbols are loaded'
      Caption = 'APP'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
    end
    object EnableHistory: TCheckBox
      Left = 8
      Top = 56
      Width = 88
      Height = 18
      Hint = 'Enable history capture'
      Caption = 'Enable'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 1
    end
    object History: TButton
      Left = 8
      Top = 19
      Width = 84
      Height = 27
      Caption = 'History'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = HistoryClick
    end
    object Memory: TButton
      Left = 96
      Top = 19
      Width = 84
      Height = 27
      Caption = 'Memory'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      OnClick = MemoryClick
    end
    object Symbols: TButton
      Left = 184
      Top = 19
      Width = 84
      Height = 27
      Caption = 'Symbols'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
      OnClick = SymbolsClick
    end
    object ButtonProfiler: TButton
      Left = 272
      Top = 19
      Width = 84
      Height = 27
      Caption = 'Profiler'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
      TabOrder = 4
      OnClick = ButtonProfilerClick
    end
    object AutoUpdateMemory: TCheckBox
      Left = 100
      Top = 56
      Width = 70
      Height = 18
      Hint = 'Enable Memory window refreshing'
      Caption = 'Refresh'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 5
      OnClick = AutoUpdateMemoryClick
    end
  end
  object IOPorts: TGroupBox
    Left = 302
    Top = 154
    Width = 133
    Height = 91
    Caption = 'I/O'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    object IOPort0Address: TLabel
      Left = 41
      Top = 17
      Width = 39
      Height = 16
      Caption = '$FFFF'
      PopupMenu = InputContextPopup
      OnMouseDown = IOPort0AddressMouseDown
    end
    object IOPort2Address: TLabel
      Left = 43
      Top = 52
      Width = 38
      Height = 16
      Caption = '$FFFF'
      PopupMenu = InputContextPopup
      OnMouseDown = IOPort2AddressMouseDown
    end
    object IOPort1Address: TLabel
      Left = 43
      Top = 35
      Width = 38
      Height = 15
      Caption = '$FFFF'
      PopupMenu = InputContextPopup
      OnMouseDown = IOPort1AddressMouseDown
    end
    object IOPort3Address: TLabel
      Left = 43
      Top = 70
      Width = 38
      Height = 15
      Caption = '$FFFF'
      PopupMenu = InputContextPopup
      OnMouseDown = IOPort3AddressMouseDown
    end
    object IOPort0Data: TLabel
      Left = 98
      Top = 17
      Width = 23
      Height = 16
      Caption = '$FF'
    end
    object IOPort0Direction: TLabel
      Left = 11
      Top = 17
      Width = 23
      Height = 16
      Caption = 'OUT'
    end
    object IOPort2Data: TLabel
      Left = 98
      Top = 52
      Width = 23
      Height = 16
      Caption = '$FF'
    end
    object IOPort1Data: TLabel
      Left = 98
      Top = 35
      Width = 23
      Height = 15
      Caption = '$FF'
    end
    object IOPort3Data: TLabel
      Left = 98
      Top = 70
      Width = 23
      Height = 15
      Caption = '$FF'
    end
    object IOPort3Direction: TLabel
      Left = 11
      Top = 70
      Width = 23
      Height = 15
      Caption = 'OUT'
    end
    object IOPort1Direction: TLabel
      Left = 11
      Top = 35
      Width = 23
      Height = 15
      Caption = 'OUT'
    end
    object IOPort2Direction: TLabel
      Left = 11
      Top = 52
      Width = 23
      Height = 16
      Caption = 'OUT'
    end
    object Label17: TLabel
      Left = 85
      Top = 17
      Width = 8
      Height = 16
      Caption = ':'
    end
    object Label25: TLabel
      Left = 85
      Top = 35
      Width = 8
      Height = 15
      Caption = ':'
    end
    object Label26: TLabel
      Left = 85
      Top = 52
      Width = 8
      Height = 16
      Caption = ':'
    end
    object Label30: TLabel
      Left = 85
      Top = 70
      Width = 8
      Height = 15
      Caption = ':'
    end
  end
  object GroupBoxChroma: TGroupBox
    Left = 442
    Top = 335
    Width = 178
    Height = 39
    Caption = 'Chroma Colour'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    TabOrder = 11
    object ChromaColourModeLabel: TLabel
      Left = 57
      Top = 17
      Width = 38
      Height = 16
      Alignment = taRightJustify
      Caption = 'Mode:'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
    end
    object ChromaColourMode: TLabel
      Left = 100
      Top = 17
      Width = 23
      Height = 16
      Caption = '$00'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
    end
  end
  object GroupBoxZXC: TGroupBox
    Left = 628
    Top = 335
    Width = 178
    Height = 39
    Caption = 'ZXC Cartridge'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    TabOrder = 9
    object ZXCModeLabel: TLabel
      Left = 57
      Top = 17
      Width = 38
      Height = 16
      Alignment = taRightJustify
      Caption = 'Mode:'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Courier New'
      Font.Style = []
      ParentFont = False
    end
    object ZXCMode: TLabel
      Left = 100
      Top = 17
      Width = 23
      Height = 16
      Caption = '$00'
    end
  end
  object GroupBoxSpectra: TGroupBox
    Left = 442
    Top = 335
    Width = 178
    Height = 39
    Caption = 'Spectra Colour'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    TabOrder = 8
    object SpectraModeLabel: TLabel
      Left = 57
      Top = 17
      Width = 38
      Height = 16
      Caption = 'Mode:'
    end
    object SpectraMode: TLabel
      Left = 100
      Top = 17
      Width = 23
      Height = 16
      Caption = '$00'
    end
  end
  object MemDumpPopup: TPopupMenu
    Left = 220
    Top = 236
    object MemDumpFromHere1: TMenuItem
      Caption = 'Show Memory'
      OnClick = MemDumpFromHere1Click
    end
    object OnExecute1: TMenuItem
      Caption = 'Break On Execute'
      OnClick = AddBreak1Click
    end
    object OnRead1: TMenuItem
      Tag = 1
      Caption = 'Break On Read'
      OnClick = AddBreak1Click
    end
    object OnWrite1: TMenuItem
      Tag = 2
      Caption = 'Break On Write'
      OnClick = AddBreak1Click
    end
  end
  object InputContextPopup: TPopupMenu
    Left = 253
    Top = 236
    object BreakOnInput: TMenuItem
      Tag = 6
      Caption = 'Break on Input A15-0'
      OnClick = BreakonInputOutputClick
    end
    object BreakOnInputHigh: TMenuItem
      Tag = 7
      Caption = 'Break on Input A15-8'
      OnClick = BreakonInputOutputHighClick
    end
    object BreakOnInputLow: TMenuItem
      Tag = 8
      Caption = 'Break on Input A7-0'
      OnClick = BreakonInputOutputLowClick
    end
  end
  object OutputContextPopup: TPopupMenu
    Left = 288
    Top = 236
    object BreakOnOutput: TMenuItem
      Tag = 3
      Caption = 'Break on Output A15-0'
      OnClick = BreakonInputOutputClick
    end
    object BreakOnOutputHigh: TMenuItem
      Tag = 4
      Caption = 'Break on Output A15-8'
      OnClick = BreakonInputOutputHighClick
    end
    object BreakOnOutputLow: TMenuItem
      Tag = 5
      Caption = 'Break on Output A7-0'
      OnClick = BreakonInputOutputLowClick
    end
  end
  object BreakpointWindowPopup: TPopupMenu
    Left = 322
    Top = 236
    object Disable: TMenuItem
      Caption = 'Disable'
      OnClick = DisableClick
    end
    object Enable: TMenuItem
      Caption = 'Enable'
      OnClick = EnableClick
    end
    object ResetHitCount: TMenuItem
      Caption = 'Reset Hit Count'
      OnClick = ResetHitCountClick
    end
    object N1: TMenuItem
      Caption = '-'
    end
    object CurrentHitCount: TMenuItem
      Caption = 'Hit Count = x'
    end
  end
  object MemoryWindowTimer: TTimer
    Enabled = False
    Interval = 10000
    OnTimer = MemoryWindowTimerExpired
    Left = 356
    Top = 236
  end
end
