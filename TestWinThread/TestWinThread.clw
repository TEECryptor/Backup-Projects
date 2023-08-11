; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMyThread
LastTemplate=CWinThread
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "TestWinThread.h"

ClassCount=4
Class1=CTestWinThreadApp
Class2=CTestWinThreadDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=CMyThread
Resource3=IDD_TESTWINTHREAD_DIALOG

[CLS:CTestWinThreadApp]
Type=0
HeaderFile=TestWinThread.h
ImplementationFile=TestWinThread.cpp
Filter=N

[CLS:CTestWinThreadDlg]
Type=0
HeaderFile=TestWinThreadDlg.h
ImplementationFile=TestWinThreadDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CTestWinThreadDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=TestWinThreadDlg.h
ImplementationFile=TestWinThreadDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_TESTWINTHREAD_DIALOG]
Type=1
Class=CTestWinThreadDlg
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_BUTTON1,button,1342242816
Control4=IDC_BUTTON2,button,1342242816
Control5=IDC_BUTTON3,button,1342242816
Control6=IDC_BUTTON4,button,1342242816

[CLS:CMyThread]
Type=0
HeaderFile=MyThread.h
ImplementationFile=MyThread.cpp
BaseClass=CWinThread
Filter=N
VirtualFilter=TC
LastObject=CMyThread

