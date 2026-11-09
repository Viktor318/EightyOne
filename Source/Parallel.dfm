object ParallelPort: TParallelPort
  Left = 402
  Top = 104
  BorderIcons = [biSystemMenu]
  BorderStyle = bsToolWindow
  Caption = 'Printer Port'
  ClientHeight = 166
  ClientWidth = 277
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Scaled = False
  PixelsPerInch = 96
  TextHeight = 15
  object Label1: TLabel
    Left = 41
    Top = 22
    Width = 65
    Height = 15
    Alignment = taRightJustify
    Caption = 'Connect To:'
  end
  object ComPortList: TComboBox
    Left = 111
    Top = 17
    Width = 152
    Height = 23
    Style = csDropDownList
    ItemHeight = 15
    TabOrder = 0
    OnChange = ComPortListChange
  end
  object FilePanel: TPanel
    Left = 13
    Top = 44
    Width = 254
    Height = 79
    BevelOuter = bvNone
    TabOrder = 1
    object Label3: TLabel
      Left = 34
      Top = 35
      Width = 59
      Height = 15
      Alignment = taRightJustify
      Caption = 'Output File:'
    end
    object OutputFileEdit: TEdit
      Left = 98
      Top = 31
      Width = 125
      Height = 23
      TabOrder = 0
      Text = 'c:\parallel.txt'
    end
    object OutputFileButton: TButton
      Left = 229
      Top = 31
      Width = 18
      Height = 22
      Caption = '...'
      TabOrder = 1
    end
  end
  object TCPPanel: TPanel
    Left = 13
    Top = 44
    Width = 254
    Height = 84
    BevelOuter = bvNone
    TabOrder = 2
    object Label9: TLabel
      Left = 0
      Top = 13
      Width = 94
      Height = 15
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Address:'
    end
    object Label10: TLabel
      Left = 0
      Top = 48
      Width = 94
      Height = 15
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Port:'
    end
    object TCPAddress: TEdit
      Left = 98
      Top = 9
      Width = 147
      Height = 23
      TabOrder = 0
    end
    object TCPPort: TEdit
      Left = 98
      Top = 44
      Width = 49
      Height = 23
      TabOrder = 1
    end
  end
  object PortPanel: TPanel
    Left = 13
    Top = 44
    Width = 254
    Height = 84
    BevelOuter = bvNone
    TabOrder = 3
    object Label5: TLabel
      Left = 0
      Top = 9
      Width = 94
      Height = 15
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Baud Rate:'
    end
    object Label6: TLabel
      Left = 0
      Top = 35
      Width = 94
      Height = 15
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Data Bits:'
    end
    object Label7: TLabel
      Left = 150
      Top = 35
      Width = 49
      Height = 15
      Caption = 'Stop Bits:'
    end
    object Label8: TLabel
      Left = 0
      Top = 61
      Width = 94
      Height = 15
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'Parity:'
    end
    object BaudRate: TComboBox
      Left = 98
      Top = 4
      Width = 152
      Height = 23
      Style = csDropDownList
      ItemHeight = 15
      TabOrder = 0
      Items.Strings = (
        '50'
        '110'
        '300'
        '600'
        '1200'
        '2400'
        '4800'
        '9600'
        '14400'
        '19200'
        '38400'
        '56000'
        '115200')
    end
    object DataBits: TComboBox
      Left = 98
      Top = 31
      Width = 45
      Height = 23
      Style = csDropDownList
      ItemHeight = 15
      TabOrder = 1
      Items.Strings = (
        '5'
        '6'
        '7'
        '8')
    end
    object StopBits: TComboBox
      Left = 205
      Top = 31
      Width = 45
      Height = 23
      Style = csDropDownList
      ItemHeight = 15
      TabOrder = 2
      Items.Strings = (
        '1'
        '1.5'
        '2')
    end
    object Parity: TComboBox
      Left = 98
      Top = 57
      Width = 152
      Height = 23
      Style = csDropDownList
      ItemHeight = 15
      TabOrder = 3
      Items.Strings = (
        'None'
        'Odd'
        'Even'
        'Mark'
        'Space')
    end
  end
  object OK: TButton
    Left = 183
    Top = 131
    Width = 82
    Height = 27
    Caption = 'OK'
    TabOrder = 4
    OnClick = OKClick
  end
  object ComPort: TComPort
    BaudRate = br9600
    Port = 'COM1'
    Parity.Bits = prNone
    StopBits = sbOneStopBit
    DataBits = dbEight
    Events = [evRxChar, evTxEmpty, evRxFlag, evRing, evBreak, evCTS, evDSR, evError, evRLSD, evRx80Full]
    FlowControl.OutCTSFlow = False
    FlowControl.OutDSRFlow = False
    FlowControl.ControlDTR = dtrDisable
    FlowControl.ControlRTS = rtsDisable
    FlowControl.XonXoffOut = False
    FlowControl.XonXoffIn = False
    Top = 131
  end
  object ClientSocket: TClientSocket
    Active = False
    ClientType = ctNonBlocking
    Port = 0
    Left = 35
    Top = 131
  end
  object SaveDialog: TSaveDialog
    Filter = 'All Files|*.*'
    Left = 70
    Top = 131
  end
end
