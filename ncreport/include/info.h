/****************************************************************************
* 
*  Copyright (C) 2002-2007 Norbert Szabo /Helta Kft. All rights reserved.
*  nszabo@helta.hu
*  www.helta.hu/ncreport
* 
*  This file is part of the NCReport reporting software
* 
*  This file may be used under the terms of the GNU General Public
*  License version 2.0 as published by the Free Software Foundation
*  and appearing in the file LICENSE.GPL included in the packaging of
*  this file.  Please review the following information to ensure GNU
*  General Public Licensing requirements will be met:
*  http://www.opensource.org/licenses/gpl-license.php
* 
*  If you are unsure which license is appropriate for your use, please
*  review the following information:
*  http://www.helta.hu/ncreport/price.html or contact me at
*  nszabo@helta.hu
* 
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* 
****************************************************************************/
#ifndef INFO_H
#define INFO_H

#include <Qt>

#define NCREPORTAPP_ORG			"NociSoft"
#define NCREPORTAPP_NAME		"NCReport"
#define NCREPORTAPP_LONGNAME	"NCReport Report Generator"
#define NCREPORTAPP_VERSION		"2.8.6"
#ifdef NCREPORT_UNREG
	#define NCREPORTAPP_RELEASENAME	"Eval version"
#else
	#define NCREPORTAPP_RELEASENAME	""
#endif
#define NCREPORTAPP_RELEASEDATE	"2012-07-31"
#define NCREPORTAPP_COPYRIGHT	QString("%1 2006-2012 NociSoft/Helta Kft.").arg(QChar(Qt::Key_copyright))
#define NCREPORTAPP_WEB			"http://www.nocisoft.com"

#define NCREPORTAPP_DEFAULT_IMAGEPATH	"images/"

#endif 
