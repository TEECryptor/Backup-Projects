; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTesAudioDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "TesAudio.h"

ClassCount=4
Class1=CTesAudioApp
Class2=CTesAudioDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_TESAUDIO_DIALOG

[CLS:CTesAudioApp]
Type=0
HeaderFile=TesAudio.h
ImplementationFile=TesAudio.cpp
Filter=N

[CLS:CTesAudioDlg]
Type=0
HeaderFile=TesAudioDlg.h
ImplementationFile=TesAudioDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=TesAudioDlg.h
ImplementationFile=TesAudioDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_TESAUDIO_DIALOG]
Type=1
Class=CTesAudioDlg
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_BTN_OPEN,button,1342242816
Control4=IDC_BTN_PLAY,button,1342242816
Control5=IDC_BTN_PAUSE,button,1342242816
Control6=IDC_BTN_STOP,button,1342242816
Control7=IDC_STATIC_FILE,static,1342312459

