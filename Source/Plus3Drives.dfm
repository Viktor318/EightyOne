object P3Drive: TP3Drive
  Left = 233
  Top = 134
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsToolWindow
  Caption = 'Drives'
  ClientHeight = 439
  ClientWidth = 434
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
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 15
  object OK: TButton
    Left = 345
    Top = 404
    Width = 81
    Height = 27
    Anchors = [akRight, akBottom]
    Caption = 'OK'
    TabOrder = 3
    OnClick = OKClick
  end
  object FloppyDriveGroup: TGroupBox
    Left = 9
    Top = 9
    Width = 416
    Height = 79
    Caption = 'Floppy Drives'
    TabOrder = 0
    object DriveALabel: TLabel
      Left = 32
      Top = 22
      Width = 41
      Height = 15
      Alignment = taRightJustify
      Caption = 'Drive A:'
    end
    object DriveBLabel: TLabel
      Left = 32
      Top = 48
      Width = 41
      Height = 15
      Alignment = taRightJustify
      Caption = 'Drive B:'
    end
    object DriveAText: TEdit
      Left = 83
      Top = 17
      Width = 202
      Height = 23
      Enabled = False
      ParentShowHint = False
      ReadOnly = True
      ShowHint = True
      TabOrder = 0
      Text = '< Empty >'
    end
    object DriveBText: TEdit
      Left = 83
      Top = 44
      Width = 202
      Height = 23
      Enabled = False
      ReadOnly = True
      TabOrder = 4
      Text = '< Empty >'
    end
    object DriveAFSBtn: TButton
      Left = 288
      Top = 17
      Width = 27
      Height = 26
      Caption = '...'
      TabOrder = 1
      OnClick = DriveAFSBtnClick
    end
    object DriveBFSBtn: TButton
      Left = 288
      Top = 44
      Width = 27
      Height = 25
      Caption = '...'
      TabOrder = 5
      OnClick = DriveBFSBtnClick
    end
    object DriveAEjectBtn: TButton
      Left = 358
      Top = 17
      Width = 49
      Height = 26
      Caption = 'Eject'
      TabOrder = 3
      OnClick = DriveAEjectBtnClick
    end
    object DriveBEjectBtn: TButton
      Left = 358
      Top = 44
      Width = 49
      Height = 25
      Caption = 'Eject'
      TabOrder = 7
      OnClick = DriveBEjectBtnClick
    end
    object DriveANewBtn: TButton
      Left = 319
      Top = 17
      Width = 36
      Height = 26
      Caption = 'New'
      TabOrder = 2
      OnClick = DriveANewBtnClick
    end
    object DriveBNewBtn: TButton
      Left = 319
      Top = 44
      Width = 36
      Height = 25
      Caption = 'New'
      TabOrder = 6
      OnClick = DriveBNewBtnClick
    end
  end
  object HardDriveGroup: TGroupBox
    Left = 9
    Top = 96
    Width = 416
    Height = 254
    Caption = 'Hard Drives'
    TabOrder = 1
    object HD1Label: TLabel
      Left = 42
      Top = 127
      Width = 30
      Height = 15
      Alignment = taRightJustify
      Caption = 'HD 1:'
    end
    object HD1CHS: TLabel
      Left = 35
      Top = 192
      Width = 38
      Height = 15
      Caption = 'C/H/S:'
    end
    object HD0Label: TLabel
      Left = 42
      Top = 31
      Width = 30
      Height = 15
      Alignment = taRightJustify
      Caption = 'HD 0:'
    end
    object HD0CHS: TLabel
      Left = 35
      Top = 92
      Width = 38
      Height = 15
      Caption = 'C/H/S:'
    end
    object HD1Text: TEdit
      Left = 83
      Top = 153
      Width = 202
      Height = 23
      Enabled = False
      ReadOnly = True
      TabOrder = 13
      Text = '< Empty >'
    end
    object HD1FSBtn: TButton
      Left = 288
      Top = 151
      Width = 27
      Height = 25
      Caption = '...'
      TabOrder = 14
      OnClick = HD1FSBtnClick
    end
    object HD1EjectBtn: TButton
      Left = 358
      Top = 151
      Width = 49
      Height = 25
      Caption = 'Eject'
      TabOrder = 16
      OnClick = HD1EjectBtnClick
    end
    object RedetectDrives: TButton
      Left = 228
      Top = 218
      Width = 179
      Height = 25
      Caption = 'Redetect Physical Drives'
      TabOrder = 22
      OnClick = RedetectDrivesClick
    end
    object HD1List: TComboBox
      Left = 83
      Top = 122
      Width = 202
      Height = 23
      Style = csDropDownList
      ItemHeight = 15
      TabOrder = 11
      OnChange = HD1ListChange
    end
    object HD1C: TEdit
      Left = 83
      Top = 188
      Width = 45
      Height = 23
      Enabled = False
      TabOrder = 17
      Text = '0'
    end
    object HD1H: TEdit
      Left = 135
      Top = 188
      Width = 45
      Height = 23
      Enabled = False
      TabOrder = 18
      Text = '0'
    end
    object HD1HUD: TUpDown
      Left = 181
      Top = 188
      Width = 28
      Height = 23
      Min = 0
      Orientation = udHorizontal
      Position = 0
      TabOrder = 19
      Wrap = False
      OnClick = HD1HUDClick
    end
    object HD1S: TEdit
      Left = 214
      Top = 188
      Width = 45
      Height = 23
      Enabled = False
      TabOrder = 20
      Text = '0'
    end
    object HD1SUD: TUpDown
      Left = 260
      Top = 188
      Width = 27
      Height = 23
      Min = 0
      Orientation = udHorizontal
      Position = 0
      TabOrder = 21
      Wrap = False
      OnClick = HD1HUDClick
    end
    object HD0List: TComboBox
      Left = 83
      Top = 26
      Width = 202
      Height = 23
      Style = csDropDownList
      ItemHeight = 15
      TabOrder = 0
      OnChange = HD0ListChange
    end
    object HD0Text: TEdit
      Left = 83
      Top = 57
      Width = 202
      Height = 23
      Enabled = False
      ParentShowHint = False
      ReadOnly = True
      ShowHint = True
      TabOrder = 2
      Text = '< Empty >'
    end
    object HD0FSBtn: TButton
      Left = 288
      Top = 55
      Width = 27
      Height = 25
      Caption = '...'
      TabOrder = 3
      OnClick = HD0FSBtnClick
    end
    object HD0EjectBtn: TButton
      Left = 358
      Top = 55
      Width = 49
      Height = 25
      Caption = 'Eject'
      TabOrder = 5
      OnClick = HD0EjectBtnClick
    end
    object HD0C: TEdit
      Left = 83
      Top = 87
      Width = 45
      Height = 23
      Enabled = False
      TabOrder = 6
      Text = '0'
    end
    object HD0H: TEdit
      Left = 135
      Top = 87
      Width = 45
      Height = 23
      Enabled = False
      TabOrder = 7
      Text = '0'
    end
    object HD0HUD: TUpDown
      Left = 181
      Top = 87
      Width = 28
      Height = 23
      Min = 0
      Orientation = udHorizontal
      Position = 0
      TabOrder = 8
      Wrap = False
      OnClick = HD0HUDClick
    end
    object HD0S: TEdit
      Left = 214
      Top = 87
      Width = 45
      Height = 23
      Enabled = False
      TabOrder = 9
      Text = '0'
    end
    object HD0SUD: TUpDown
      Left = 260
      Top = 87
      Width = 27
      Height = 23
      Min = 0
      Orientation = udHorizontal
      Position = 0
      TabOrder = 10
      Wrap = False
      OnClick = HD0HUDClick
    end
    object HD0ReadOnly: TCheckBox
      Left = 290
      Top = 26
      Width = 104
      Height = 19
      Caption = 'Read Only'
      TabOrder = 1
      OnClick = HD0ReadOnlyClick
    end
    object HD1ReadOnly: TCheckBox
      Left = 290
      Top = 122
      Width = 104
      Height = 19
      Caption = 'Read Only'
      TabOrder = 12
      OnClick = HD1ReadOnlyClick
    end
    object HD0NewBtn: TButton
      Left = 319
      Top = 55
      Width = 36
      Height = 25
      Caption = 'New'
      TabOrder = 4
      OnClick = HD0NewBtnClick
    end
    object HD1NewBtn: TButton
      Left = 319
      Top = 151
      Width = 36
      Height = 25
      Caption = 'New'
      TabOrder = 15
      OnClick = HD1NewBtnClick
    end
  end
  object MicrodriveGroup: TGroupBox
    Left = 9
    Top = 358
    Width = 416
    Height = 237
    Caption = 'Microdrives'
    TabOrder = 2
    object MDV0Label: TLabel
      Left = 34
      Top = 22
      Width = 40
      Height = 15
      Alignment = taRightJustify
      Caption = 'Drive 1:'
    end
    object MDV1Label: TLabel
      Left = 34
      Top = 48
      Width = 40
      Height = 15
      Alignment = taRightJustify
      Caption = 'Drive 2:'
    end
    object MDV3Label: TLabel
      Left = 34
      Top = 100
      Width = 40
      Height = 15
      Alignment = taRightJustify
      Caption = 'Drive 4:'
    end
    object MDV2Label: TLabel
      Left = 34
      Top = 74
      Width = 40
      Height = 15
      Alignment = taRightJustify
      Caption = 'Drive 3:'
    end
    object MDV4Label: TLabel
      Left = 34
      Top = 127
      Width = 40
      Height = 15
      Alignment = taRightJustify
      Caption = 'Drive 5:'
    end
    object MDV5Label: TLabel
      Left = 34
      Top = 153
      Width = 40
      Height = 15
      Alignment = taRightJustify
      Caption = 'Drive 6:'
    end
    object MDV6Label: TLabel
      Left = 34
      Top = 179
      Width = 40
      Height = 15
      Alignment = taRightJustify
      Caption = 'Drive 7:'
    end
    object MDV7Label: TLabel
      Left = 34
      Top = 205
      Width = 40
      Height = 15
      Alignment = taRightJustify
      Caption = 'Drive 8:'
    end
    object MDV0Text: TEdit
      Left = 83
      Top = 17
      Width = 202
      Height = 23
      Enabled = False
      ParentShowHint = False
      ReadOnly = True
      ShowHint = True
      TabOrder = 0
      Text = '< Empty >'
    end
    object MDV0FSBtn: TButton
      Left = 288
      Top = 17
      Width = 27
      Height = 26
      Caption = '...'
      TabOrder = 1
      OnClick = MDVFSBtnClick
    end
    object MDV0EjectBtn: TButton
      Left = 358
      Top = 17
      Width = 49
      Height = 26
      Caption = 'Eject'
      TabOrder = 3
      OnClick = MDVEjectBtnClick
    end
    object MDV1EjectBtn: TButton
      Left = 358
      Top = 44
      Width = 49
      Height = 25
      Caption = 'Eject'
      TabOrder = 7
      OnClick = MDVEjectBtnClick
    end
    object MDV1FSBtn: TButton
      Left = 288
      Top = 44
      Width = 27
      Height = 25
      Caption = '...'
      TabOrder = 5
      OnClick = MDVFSBtnClick
    end
    object MDV1Text: TEdit
      Left = 83
      Top = 44
      Width = 202
      Height = 23
      Enabled = False
      ReadOnly = True
      TabOrder = 4
      Text = '< Empty >'
    end
    object MDV2Text: TEdit
      Left = 83
      Top = 70
      Width = 202
      Height = 23
      Enabled = False
      ParentShowHint = False
      ReadOnly = True
      ShowHint = True
      TabOrder = 8
      Text = '< Empty >'
    end
    object MDV3Text: TEdit
      Left = 83
      Top = 96
      Width = 202
      Height = 23
      Enabled = False
      ReadOnly = True
      TabOrder = 12
      Text = '< Empty >'
    end
    object MDV3FSBtn: TButton
      Left = 288
      Top = 96
      Width = 27
      Height = 25
      Caption = '...'
      TabOrder = 13
      OnClick = MDVFSBtnClick
    end
    object MDV2FSBtn: TButton
      Left = 288
      Top = 70
      Width = 27
      Height = 25
      Caption = '...'
      TabOrder = 9
      OnClick = MDVFSBtnClick
    end
    object MDV2EjectBtn: TButton
      Left = 358
      Top = 70
      Width = 49
      Height = 25
      Caption = 'Eject'
      TabOrder = 11
      OnClick = MDVEjectBtnClick
    end
    object MDV3EjectBtn: TButton
      Left = 358
      Top = 96
      Width = 49
      Height = 25
      Caption = 'Eject'
      TabOrder = 15
      OnClick = MDVEjectBtnClick
    end
    object MDV7Text: TEdit
      Left = 83
      Top = 201
      Width = 202
      Height = 23
      Enabled = False
      ReadOnly = True
      TabOrder = 28
      Text = '< Empty >'
    end
    object MDV6Text: TEdit
      Left = 83
      Top = 175
      Width = 202
      Height = 23
      Enabled = False
      ParentShowHint = False
      ReadOnly = True
      ShowHint = True
      TabOrder = 24
      Text = '< Empty >'
    end
    object MDV5Text: TEdit
      Left = 83
      Top = 148
      Width = 202
      Height = 23
      Enabled = False
      ReadOnly = True
      TabOrder = 20
      Text = '< Empty >'
    end
    object MDV4Text: TEdit
      Left = 83
      Top = 122
      Width = 202
      Height = 23
      Enabled = False
      ParentShowHint = False
      ReadOnly = True
      ShowHint = True
      TabOrder = 16
      Text = '< Empty >'
    end
    object MDV4FSBtn: TButton
      Left = 288
      Top = 122
      Width = 27
      Height = 25
      Caption = '...'
      TabOrder = 17
      OnClick = MDVFSBtnClick
    end
    object MDV5FSBtn: TButton
      Left = 288
      Top = 148
      Width = 27
      Height = 25
      Caption = '...'
      TabOrder = 21
      OnClick = MDVFSBtnClick
    end
    object MDV6FSBtn: TButton
      Left = 288
      Top = 175
      Width = 27
      Height = 25
      Caption = '...'
      TabOrder = 25
      OnClick = MDVFSBtnClick
    end
    object MDV7FSBtn: TButton
      Left = 288
      Top = 201
      Width = 27
      Height = 25
      Caption = '...'
      TabOrder = 29
      OnClick = MDVFSBtnClick
    end
    object MDV7EjectBtn: TButton
      Left = 358
      Top = 201
      Width = 49
      Height = 25
      Caption = 'Eject'
      TabOrder = 31
      OnClick = MDVEjectBtnClick
    end
    object MDV6EjectBtn: TButton
      Left = 358
      Top = 175
      Width = 49
      Height = 25
      Caption = 'Eject'
      TabOrder = 27
      OnClick = MDVEjectBtnClick
    end
    object MDV5EjectBtn: TButton
      Left = 358
      Top = 148
      Width = 49
      Height = 25
      Caption = 'Eject'
      TabOrder = 23
      OnClick = MDVEjectBtnClick
    end
    object MDV4EjectBtn: TButton
      Left = 358
      Top = 122
      Width = 49
      Height = 25
      Caption = 'Eject'
      TabOrder = 19
      OnClick = MDVEjectBtnClick
    end
    object MDV0NewBtn: TButton
      Left = 319
      Top = 17
      Width = 36
      Height = 26
      Caption = 'New'
      TabOrder = 2
      OnClick = MDVNewBtnClick
    end
    object MDV1NewBtn: TButton
      Left = 319
      Top = 44
      Width = 36
      Height = 25
      Caption = 'New'
      TabOrder = 6
      OnClick = MDVNewBtnClick
    end
    object MDV2NewBtn: TButton
      Left = 319
      Top = 70
      Width = 36
      Height = 25
      Caption = 'New'
      TabOrder = 10
      OnClick = MDVNewBtnClick
    end
    object MDV3NewBtn: TButton
      Left = 319
      Top = 96
      Width = 36
      Height = 25
      Caption = 'New'
      TabOrder = 14
      OnClick = MDVNewBtnClick
    end
    object MDV4NewBtn: TButton
      Left = 319
      Top = 122
      Width = 36
      Height = 25
      Caption = 'New'
      TabOrder = 18
      OnClick = MDVNewBtnClick
    end
    object MDV5NewBtn: TButton
      Left = 319
      Top = 148
      Width = 36
      Height = 25
      Caption = 'New'
      TabOrder = 22
      OnClick = MDVNewBtnClick
    end
    object MDV6NewBtn: TButton
      Left = 319
      Top = 175
      Width = 36
      Height = 25
      Caption = 'New'
      TabOrder = 26
      OnClick = MDVNewBtnClick
    end
    object MDV7NewBtn: TButton
      Left = 319
      Top = 201
      Width = 36
      Height = 25
      Caption = 'New'
      TabOrder = 30
      OnClick = MDVNewBtnClick
    end
  end
  object OpenDialogFloppyDiskImage: TOpenDialog
    Filter = 
      'Disk Images|*.dsk;*.mgt;*.img;*.opd;*.opu;*.trd;*.lar;*.zip|+3 D' +
      'isk Images (*.dsk)|*.dsk|Plus D/Disciple Disk Images (*.mgt, *.i' +
      'mg)|*.mgt;*.img|Opus Discovery Disk Images (*.opd, *.opu)|*.opd;' +
      '*.opu|TR-DOS Disk Images (*.trd)|*.trd|Larken Disk Images (*.lar' +
      ')|*.lar|Compressed Disk Images (*.zip)|*.zip'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofPathMustExist, ofCreatePrompt, ofEnableSizing]
    Title = 'Select Floppy Disk Image'
    Left = 44
    Top = 602
  end
  object OpenDialogHardDriveImage: TOpenDialog
    DefaultExt = '.hdf'
    Filter = 
      'All Hard Drive Images (*.hdf; *.vhd)|*.hdf;*.vhd|ZX Hard Drive I' +
      'mages (*.hdf)|*.hdf|VirtualPC Uncompressed Images (*.vhd)|*.vhd'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofPathMustExist, ofEnableSizing]
    Title = 'Select Hard Drive Image'
    Left = 122
    Top = 602
  end
  object SaveDialogNewFloppyDisk: TSaveDialog
    Options = [ofHideReadOnly, ofPathMustExist, ofEnableSizing]
    Left = 9
    Top = 602
  end
  object SaveDialogNewHardDriveImage: TSaveDialog
    DefaultExt = '.hdf'
    Filter = 'ZX Hard Drive Images (*.hdf)|*.hdf'
    Options = [ofHideReadOnly, ofPathMustExist, ofEnableSizing]
    Title = 'Create Hard Drive Image'
    Left = 87
    Top = 602
  end
end
