; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMol_SimuView
LastTemplate=CAnimateCtrl
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "mol_simu.h"
LastPage=0

ClassCount=6
Class1=CMainFrame
Class2=CMol_SimuApp
Class3=CAboutDlg
Class4=CMol_SimuDoc
Class5=CMol_SimuView

ResourceCount=2
Resource1=IDR_MAINFRAME
Class6=CParticle
Resource2=IDD_ABOUTBOX

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
LastObject=ID_PolySurf
Filter=T
VirtualFilter=fWC

[CLS:CMol_SimuApp]
Type=0
BaseClass=CWinApp
HeaderFile=Mol_Simu.h
ImplementationFile=Mol_Simu.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=Mol_Simu.cpp
ImplementationFile=Mol_Simu.cpp
LastObject=ID_translate

[CLS:CMol_SimuDoc]
Type=0
BaseClass=CDocument
HeaderFile=Mol_SimuDoc.h
ImplementationFile=Mol_SimuDoc.cpp
LastObject=CMol_SimuDoc
Filter=N
VirtualFilter=DC

[CLS:CMol_SimuView]
Type=0
BaseClass=CView
HeaderFile=Mol_SimuView.h
ImplementationFile=Mol_SimuView.cpp
Filter=C
VirtualFilter=VWC
LastObject=CMol_SimuView

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
Command10=ID_rotate
Command11=ID_translate
Command12=ID_scale
Command13=ID_VDWsurface
Command14=ID_APP_ABOUT
Command15=ID_PolySurf
Command16=ID_RationalPolySurf
Command17=ID_InitialMesh
Command18=ID_p1fSurf
Command19=ID_p2fsurf
Command20=ID_p1p2fSurf
Command21=ID_MENUITEM32781
Command22=ID_MENUITEM32782
CommandCount=22

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[ACL:IDR_MAINFRAME]
Type=1
Class=?
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

[CLS:CParticle]
Type=0
HeaderFile=Particle.h
ImplementationFile=Particle.cpp
BaseClass=CAnimateCtrl
Filter=W

