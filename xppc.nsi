; Script generated with the Venis Install Wizard

; Define your application name
!define APPNAME "xppc"
!define APPNAMEANDVERSION "xppc 0.57"

; Main Install settings
Name "${APPNAMEANDVERSION}"
InstallDir "$PROGRAMFILES\xppc"
InstallDirRegKey HKLM "Software\${APPNAME}" ""
OutFile "setup.exe"

; Use compression
SetCompressor LZMA

; Modern interface settings
!include "MUI.nsh"

!define MUI_ABORTWARNING
!define MUI_FINISHPAGE_RUN "$INSTDIR\GuardianApp.exe"

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

; Set languages (first is default language)
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "Russian"
!insertmacro MUI_RESERVEFILE_LANGDLL

Section "xppc" Section1

	; Set Section properties
	SetOverwrite on

	; Set Section Files and Shortcuts
	SetOutPath "$INSTDIR\"
	File "xppc\GuardianApp.exe"
	File "xppc\libgcc_s_sjlj-1.dll"
	File "xppc\libpq.dll"
	File "xppc\libstdc++-6.dll"
	File "xppc\libwinpthread-1.dll"
	File "xppc\Qt3Support4.dll"
	File "xppc\QtCLucene4.dll"
	File "xppc\QtCore4.dll"
	File "xppc\QtDeclarative4.dll"
	File "xppc\QtDesigner4.dll"
	File "xppc\QtDesignerComponents4.dll"
	File "xppc\QtGui4.dll"
	File "xppc\QtHelp4.dll"
	File "xppc\QtMultimedia4.dll"
	File "xppc\QtNetwork4.dll"
	File "xppc\QtOpenGL4.dll"
	File "xppc\QtScript4.dll"
	File "xppc\QtScriptTools4.dll"
	File "xppc\QtSql4.dll"
	File "xppc\QtSvg4.dll"
	File "xppc\QtTest4.dll"
	File "xppc\QtWebKit4.dll"
	File "xppc\QtXml4.dll"
	File "xppc\QtXmlPatterns4.dll"
	File "xppc\xppc.exe"
	SetOutPath "$INSTDIR\Reports\"
	File "xppc\Reports\jobslist.dotx"
	File "xppc\Reports\ticket.dotx"
	SetOutPath "$INSTDIR\sqldrivers\"
	File "xppc\sqldrivers\qsqlpsql4.dll"
	SetOutPath "$PROFILE\"
	File "xppc\guardiansettings.ini"
	SetOutPath "$INSTDIR\"
	CreateShortCut "$DESKTOP\ витанции 2013.lnk" "$INSTDIR\GuardianApp.exe"
	CreateDirectory "$SMPROGRAMS\xppc"
	CreateShortCut "$SMPROGRAMS\xppc\ витанции 2013.lnk" "$INSTDIR\GuardianApp.exe"
	CreateShortCut "$SMPROGRAMS\xppc\Uninstall.lnk" "$INSTDIR\uninstall.exe"

SectionEnd

Section -FinishSection

	WriteRegStr HKLM "Software\${APPNAME}" "" "$INSTDIR"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayName" "${APPNAME}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "UninstallString" "$INSTDIR\uninstall.exe"
	WriteUninstaller "$INSTDIR\uninstall.exe"

SectionEnd

; Modern install component descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
	!insertmacro MUI_DESCRIPTION_TEXT ${Section1} ""
!insertmacro MUI_FUNCTION_DESCRIPTION_END

;Uninstall section
Section Uninstall

	;Remove from registry...
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}"
	DeleteRegKey HKLM "SOFTWARE\${APPNAME}"

	; Delete self
	Delete "$INSTDIR\uninstall.exe"

	; Delete Shortcuts
	Delete "$DESKTOP\xppc.lnk"
	Delete "$SMPROGRAMS\xppc\xppc.lnk"
	Delete "$SMPROGRAMS\xppc\Uninstall.lnk"

	; Clean up xppc
	Delete "$INSTDIR\GuardianApp.exe"
	Delete "$INSTDIR\libgcc_s_sjlj-1.dll"
	Delete "$INSTDIR\libpq.dll"
	Delete "$INSTDIR\libstdc++-6.dll"
	Delete "$INSTDIR\libwinpthread-1.dll"
	Delete "$INSTDIR\Qt3Support4.dll"
	Delete "$INSTDIR\QtCLucene4.dll"
	Delete "$INSTDIR\QtCore4.dll"
	Delete "$INSTDIR\QtDeclarative4.dll"
	Delete "$INSTDIR\QtDesigner4.dll"
	Delete "$INSTDIR\QtDesignerComponents4.dll"
	Delete "$INSTDIR\QtGui4.dll"
	Delete "$INSTDIR\QtHelp4.dll"
	Delete "$INSTDIR\QtMultimedia4.dll"
	Delete "$INSTDIR\QtNetwork4.dll"
	Delete "$INSTDIR\QtOpenGL4.dll"
	Delete "$INSTDIR\QtScript4.dll"
	Delete "$INSTDIR\QtScriptTools4.dll"
	Delete "$INSTDIR\QtSql4.dll"
	Delete "$INSTDIR\QtSvg4.dll"
	Delete "$INSTDIR\QtTest4.dll"
	Delete "$INSTDIR\QtWebKit4.dll"
	Delete "$INSTDIR\QtXml4.dll"
	Delete "$INSTDIR\QtXmlPatterns4.dll"
	Delete "$INSTDIR\xppc.exe"
	Delete "$INSTDIR\Reports\jobslist.dotx"
	Delete "$INSTDIR\Reports\ticket.dotx"
	Delete "$INSTDIR\Reports\ticket2.dotx"
	Delete "$INSTDIR\Reports\ticketd.docx"
	Delete "$INSTDIR\sqldrivers\qsqlpsql4.dll"
	Delete "$DOCUMENTS|\guardiansettings.ini"

	; Remove remaining directories
	RMDir "$SMPROGRAMS\xppc"
	RMDir "$INSTDIR\sqldrivers\"
	RMDir "$INSTDIR\Reports\"
	RMDir "$INSTDIR\"
	RMDir "$DOCUMENTS|\"

SectionEnd

; On initialization
Function .onInit

	!insertmacro MUI_LANGDLL_DISPLAY

FunctionEnd

; eof