object SerialConfig: TSerialConfig
  Left = 253
  Top = 226
  BorderIcons = []
  BorderStyle = bsToolWindow
  Caption = 'TS2050 Configuration'
  ClientHeight = 203
  ClientWidth = 283
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
    Left = 73
    Top = 13
    Width = 60
    Height = 15
    Alignment = taRightJustify
    Caption = 'Connect to:'
  end
  object SerialPanel: TPanel
    Left = 17
    Top = 35
    Width = 257
    Height = 97
    BevelOuter = bvNone
    TabOrder = 5
    object Label2: TLabel
      Left = 42
      Top = 13
      Width = 74
      Height = 15
      Alignment = taRightJustify
      Caption = '/1 Baud Rate:'
    end
    object Label3: TLabel
      Left = 36
      Top = 39
      Width = 80
      Height = 15
      Alignment = taRightJustify
      Caption = '/16 Baud Rate:'
    end
    object Label4: TLabel
      Left = 36
      Top = 65
      Width = 80
      Height = 15
      Alignment = taRightJustify
      Caption = '/64 Baud Rate:'
    end
    object X1Baud: TComboBox
      Left = 121
      Top = 9
      Width = 132
      Height = 23
      Style = csDropDownList
      ItemHeight = 15
      TabOrder = 0
      Items.Strings = (
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
        '57600'
        '115200')
    end
    object X16Baud: TComboBox
      Left = 121
      Top = 35
      Width = 132
      Height = 23
      Style = csDropDownList
      ItemHeight = 15
      TabOrder = 1
      Items.Strings = (
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
        '57600'
        '115200')
    end
    object X64Baud: TComboBox
      Left = 121
      Top = 61
      Width = 132
      Height = 23
      Style = csDropDownList
      ItemHeight = 15
      TabOrder = 2
      Items.Strings = (
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
        '57600'
        '115200')
    end
  end
  object FilePanel: TPanel
    Left = 9
    Top = 35
    Width = 265
    Height = 88
    BevelOuter = bvNone
    TabOrder = 1
    object Label5: TLabel
      Left = 9
      Top = 13
      Width = 49
      Height = 15
      Caption = 'Filename:'
    end
    object FileNameBox: TEdit
      Left = 9
      Top = 31
      Width = 248
      Height = 23
      TabOrder = 0
      Text = 'FileNameBox'
    end
    object Browse: TButton
      Left = 175
      Top = 57
      Width = 82
      Height = 18
      Caption = 'Browse'
      TabOrder = 1
      OnClick = BrowseClick
    end
  end
  object ComPortList: TComboBox
    Left = 138
    Top = 9
    Width = 132
    Height = 23
    Style = csDropDownList
    ItemHeight = 15
    TabOrder = 0
    OnChange = ComPortListChange
  end
  object Enabled: TCheckBox
    Left = 17
    Top = 175
    Width = 106
    Height = 18
    Alignment = taLeftJustify
    Caption = 'Connected:'
    TabOrder = 3
  end
  object OK: TButton
    Left = 184
    Top = 170
    Width = 82
    Height = 27
    Caption = 'OK'
    TabOrder = 4
    OnClick = OKClick
  end
  object TCPPanel: TPanel
    Left = 17
    Top = 35
    Width = 257
    Height = 132
    BevelOuter = bvNone
    TabOrder = 2
    object Label6: TLabel
      Left = 9
      Top = 57
      Width = 41
      Height = 15
      Caption = 'Address'
      Enabled = False
    end
    object Label7: TLabel
      Left = 186
      Top = 57
      Width = 21
      Height = 15
      Caption = 'Port'
      Enabled = False
    end
    object TCPOutboundAddress: TEdit
      Left = 9
      Top = 31
      Width = 169
      Height = 23
      TabOrder = 1
    end
    object TCPInboundPort: TEdit
      Left = 9
      Top = 100
      Width = 239
      Height = 23
      Enabled = False
      TabOrder = 4
    end
    object ConnectTo: TRadioButton
      Left = 9
      Top = 9
      Width = 123
      Height = 18
      Caption = 'Connect to:'
      Checked = True
      TabOrder = 0
      TabStop = True
    end
    object ListenOn: TRadioButton
      Left = 9
      Top = 79
      Width = 123
      Height = 18
      Caption = 'Listen on port:'
      Enabled = False
      TabOrder = 3
    end
    object TCPOutboundPort: TEdit
      Left = 186
      Top = 31
      Width = 62
      Height = 23
      TabOrder = 2
    end
  end
  object ComPort1: TComPort
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
    OnRxChar = ComPort1RxChar
    OnTxEmpty = ComPort1TxEmpty
    OnBreak = ComPort1Break
    OnCTSChange = ComPort1CTSChange
    OnDSRChange = ComPort1DSRChange
    OnError = ComPort1Error
  end
  object BrowseDialog: TSaveDialog
    Filter = 'All Files (*.*)|*.*'
    Left = 236
  end
  object ClientSocket: TClientSocket
    Active = False
    ClientType = ctNonBlocking
    Port = 0
    OnError = ClientSocketError
    Top = 26
  end
end
