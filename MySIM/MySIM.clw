; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMainFrame
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MySIM.h"
LastPage=0

ClassCount=13
Class1=CMySIMApp
Class2=CMySIMDoc
Class3=CMySIMTreeView
Class4=CMainFrame

ResourceCount=8
Resource1=IDR_MAINFRAME
Class5=CAboutDlg
Class6=CMySIMEditView
Resource2=IDD_PIN_DIALOG
Class7=CMySIMReaderDlg
Resource3=IDD_FILE_INFO
Class8=CMySIMCommandDlg
Resource4=IDD_DIR_INFO
Class9=CMySIMPinDlg
Class10=CMySIMListView
Resource5=IDD_COMMAND_DIALOG
Class11=CMySIMDirInfoDlg
Resource6=IDD_READER_DIALOG
Class12=CMySIMFileInfoDlg
Resource7=IDD_ABOUTBOX
Class13=CMySIMEfDataDlg
Resource8=IDD_EF_DATA

[CLS:CMySIMApp]
Type=0
HeaderFile=MySIM.h
ImplementationFile=MySIM.cpp
Filter=N
LastObject=CMySIMApp

[CLS:CMySIMDoc]
Type=0
HeaderFile=MySIMDoc.h
ImplementationFile=MySIMDoc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC
LastObject=CMySIMDoc

[CLS:CMySIMTreeView]
Type=0
HeaderFile=MySIMTreeView.h
ImplementationFile=MySIMTreeView.cpp
Filter=C
LastObject=ID_EDIT_PASTE
BaseClass=CTreeView
VirtualFilter=VWC


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=CMainFrame
BaseClass=CFrameWnd
VirtualFilter=fWC




[CLS:CAboutDlg]
Type=0
HeaderFile=MySIM.cpp
ImplementationFile=MySIM.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_CUT
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_VIEW_TOOLBAR
Command15=ID_VIEW_STATUS_BAR
Command16=ID_APP_ABOUT
CommandCount=16

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_TOOL_SCARD
Command2=ID_TOOL_TREE
Command3=ID_TOOL_COMMAND
Command4=ID_TOOL_PARAM
Command5=ID_TOOL_PIN
Command6=ID_TOOL_CLEAN
Command7=ID_FILE_NEW
Command8=ID_FILE_OPEN
Command9=ID_FILE_SAVE
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_FILE_PRINT
Command14=ID_APP_ABOUT
CommandCount=14

[CLS:CMySIMEditView]
Type=0
HeaderFile=MySIMEditView.h
ImplementationFile=MySIMEditView.cpp
BaseClass=CEditView
Filter=C
LastObject=CMySIMEditView
VirtualFilter=VWC

[DLG:IDD_READER_DIALOG]
Type=1
Class=CMySIMReaderDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST_READER,listbox,1353777411

[CLS:CMySIMReaderDlg]
Type=0
HeaderFile=MySIMReaderDlg.h
ImplementationFile=MySIMReaderDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CMySIMReaderDlg
VirtualFilter=dWC

[DLG:IDD_COMMAND_DIALOG]
Type=1
Class=CMySIMCommandDlg
ControlCount=6
Control1=IDOK,button,1207959553
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_RAWDATA,edit,1350631620
Control5=IDC_BUTTON_OK,button,1342242816
Control6=IDC_COMBO_COMMAND,combobox,1344340226

[CLS:CMySIMCommandDlg]
Type=0
HeaderFile=MySIMCommandDlg.h
ImplementationFile=MySIMCommandDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CMySIMCommandDlg
VirtualFilter=dWC

[DLG:IDD_PIN_DIALOG]
Type=1
Class=CMySIMPinDlg
ControlCount=3
Control1=IDOK,button,1208025089
Control2=IDCANCEL,button,1342242816
Control3=IDC_BUTTON_OK,button,1342242816

[CLS:CMySIMPinDlg]
Type=0
HeaderFile=MySIMPinDlg.h
ImplementationFile=MySIMPinDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CMySIMPinDlg

[CLS:CMySIMListView]
Type=0
HeaderFile=MySIMListView.h
ImplementationFile=MySIMListView.cpp
BaseClass=CListView
Filter=C
LastObject=CMySIMListView
VirtualFilter=VWC

[DLG:IDD_DIR_INFO]
Type=1
Class=CMySIMDirInfoDlg
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDC_LIST_DIR_PARAM,SysListView32,1350631429

[CLS:CMySIMDirInfoDlg]
Type=0
HeaderFile=MySIMDirInfoDlg.h
ImplementationFile=MySIMDirInfoDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_LIST_DIR_PARAM
VirtualFilter=dWC

[DLG:IDD_FILE_INFO]
Type=1
Class=CMySIMFileInfoDlg
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDC_LIST_FILE_PARAM,SysListView32,1350631429

[CLS:CMySIMFileInfoDlg]
Type=0
HeaderFile=MySIMFileInfoDlg.h
ImplementationFile=MySIMFileInfoDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CMySIMFileInfoDlg

[DLG:IDD_EF_DATA]
Type=1
Class=CMySIMEfDataDlg
ControlCount=3
Control1=IDOK,button,1208025089
Control2=IDC_EDIT_EF_DATA,edit,1353777348
Control3=IDC_OK,button,1342242816

[CLS:CMySIMEfDataDlg]
Type=0
HeaderFile=mysimefdatadlg.h
ImplementationFile=mysimefdatadlg.cpp
BaseClass=CDialog
LastObject=CMySIMEfDataDlg
Filter=D
VirtualFilter=dWC

