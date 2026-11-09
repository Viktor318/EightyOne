object IF1: TIF1
  Left = 191
  Top = 170
  BorderIcons = []
  BorderStyle = bsToolWindow
  Caption = 'ZX Interface 1 RS232'
  ClientHeight = 183
  ClientWidth = 280
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Scaled = False
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 15
  object GroupSerial: TGroupBox
    Left = 9
    Top = 9
    Width = 262
    Height = 132
    Caption = 'Serial Port'
    TabOrder = 0
    object Label1: TLabel
      Left = 33
      Top = 22
      Width = 64
      Height = 15
      Alignment = taRightJustify
      Caption = 'Connect To:'
    end
    object PortPanel: TPanel
      Left = 4
      Top = 44
      Width = 254
      Height = 84
      BevelOuter = bvNone
      TabOrder = 2
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
          'Auto'
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
    object TCPPanel: TPanel
      Left = 4
      Top = 44
      Width = 254
      Height = 84
      BevelOuter = bvNone
      TabOrder = 3
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
    object FilePanel: TPanel
      Left = 4
      Top = 44
      Width = 254
      Height = 79
      BevelOuter = bvNone
      TabOrder = 1
      object Label2: TLabel
        Left = 42
        Top = 9
        Width = 51
        Height = 15
        Alignment = taRightJustify
        Caption = 'Input File:'
      end
      object Label3: TLabel
        Left = 34
        Top = 35
        Width = 59
        Height = 15
        Alignment = taRightJustify
        Caption = 'Output File:'
      end
      object InputFileEdit: TEdit
        Left = 98
        Top = 4
        Width = 125
        Height = 23
        TabOrder = 0
        Text = 'c:\if1_in.txt'
      end
      object InputFileBrowse: TButton
        Left = 229
        Top = 4
        Width = 18
        Height = 23
        Caption = '...'
        TabOrder = 1
        OnClick = InputFileBrowseClick
      end
      object OutputFileEdit: TEdit
        Left = 98
        Top = 31
        Width = 125
        Height = 23
        TabOrder = 2
        Text = 'c:\if1_out.txt'
      end
      object OutputFileButton: TButton
        Left = 229
        Top = 31
        Width = 18
        Height = 22
        Caption = '...'
        TabOrder = 3
        OnClick = OutputFileButtonClick
      end
    end
    object ComPortList: TComboBox
      Left = 102
      Top = 17
      Width = 152
      Height = 23
      Style = csDropDownList
      ItemHeight = 15
      TabOrder = 0
      OnChange = ComPortListChange
    end
  end
  object OK: TButton
    Left = 190
    Top = 148
    Width = 81
    Height = 28
    Anchors = [akRight, akBottom]
    Caption = 'OK'
    TabOrder = 1
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
    OnRxChar = ComPortRxChar
    OnTxEmpty = ComPortTxEmpty
    Left = 9
    Top = 148
  end
  object ClientSocket: TClientSocket
    Active = False
    ClientType = ctNonBlocking
    Port = 0
    OnError = ClientSocketError
    Left = 44
    Top = 148
  end
  object OpenDialog: TOpenDialog
    Filter = 'All Files|*.*'
    Options = [ofHideReadOnly, ofFileMustExist, ofEnableSizing]
    Left = 87
    Top = 148
  end
  object SaveDialog: TSaveDialog
    Filter = 'All Files|*.*'
    Left = 122
    Top = 148
  end
end
