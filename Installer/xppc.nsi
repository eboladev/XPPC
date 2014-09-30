; Script generated with the Venis Install Wizard
!include "TextFunc.nsh"
!include "StrFunc.nsh"
${StrRep}
!insertmacro ConfigWrite

; Define your application name
!define APPNAME " витанции 2013"
!define APPNAMEANDVERSION "xppc 0.66"

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
	File "xppc\QtCore4.dll"	
	File "xppc\QtGui4.dll"
	File "xppc\QtNetwork4.dll"	
	File "xppc\QtSql4.dll"	
	File "xppc\xppc.exe"
	File "xppc\QtSerialPort.dll"
	File "xppc\libeay32.dll"
	File "xppc\libssl32.dll"
	File "xppc\ssleay32.dll"
	SetOutPath "$INSTDIR\Reports\"
	File "xppc\Reports\Examples\jobslist.dotx"	
	File "xppc\Reports\Examples\cashreceipt.dotx"
	File "xppc\Reports\Examples\ticket.dotx"
	File "xppc\Reports\Examples\ticket2.dotx"
	File "xppc\Reports\Examples\ticket3.dotx"
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

Section
	Var /GLOBAL MYVAR  
	${StrRep} "$MYVAR" "$PROFILE//logs" "\" "//"
	${ConfigWrite} "$PROFILE\guardiansettings.ini" "LogsDir=" "$MYVAR" $R0	
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
	Delete "$DESKTOP\ витанции 2013.lnk"
	Delete "$SMPROGRAMS\xppc\ витанции 2013.lnk"
	Delete "$SMPROGRAMS\xppc\Uninstall.lnk"

	; Clean up xppc
	Delete "$INSTDIR\GuardianApp.exe"
	Delete "$INSTDIR\libgcc_s_sjlj-1.dll"
	Delete "$INSTDIR\libpq.dll"
	Delete "$INSTDIR\libstdc++-6.dll"
	Delete "$INSTDIR\libwinpthread-1.dll"
	Delete "$INSTDIR\QtCore4.dll"	
	Delete "$INSTDIR\QtGui4.dll"	
	Delete "$INSTDIR\QtNetwork4.dll"	
	Delete "$INSTDIR\QtSql4.dll"	
	Delete "$INSTDIR\xppc.exe"
	Delete "$INSTDIR\QtSerialPort.dll"
	File "xppc\Reports\Examples\jobslist.dotx"	
	File "xppc\Reports\Examples\cashreceipt.dotx"
	File "xppc\Reports\Examples\ticket.dotx"
	File "xppc\Reports\Examples\ticket2.dotx"
	File "xppc\Reports\Examples\ticket3.dotx"
	Delete "$INSTDIR\sqldrivers\qsqlpsql4.dll"
	#Delete "$DOCUMENTS|\guardiansettings.ini"

	; Remove remaining directories
	RMDir "$SMPROGRAMS\xppc"
	RMDir "$INSTDIR\sqldrivers\"
	RMDir "$INSTDIR\Reports\Examples"
	RMDir "$INSTDIR\Reports\"
	RMDir "$INSTDIR\"
	RMDir "$DOCUMENTS|\"

SectionEnd

; On initialization
Function .onInit

	!insertmacro MUI_LANGDLL_DISPLAY

FunctionEnd

; eof