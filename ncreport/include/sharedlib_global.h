/****************************************************************************
*
*  Copyright (C) 2002-2010 Helta Kft. / NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: nszabo@helta.hu, info@nocisoft.com
*  Web: www.nocisoft.com
*
*  This file is part of the NCReport reporting software
*
*  Licensees holding a valid NCReport License Agreement may use this
*  file in accordance with the rights, responsibilities, and obligations
*  contained therein. Please consult your licensing agreement or contact
*  nszabo@helta.hu if any conditions of this licensing are not clear
*  to you.
*
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/
#ifndef SHAREDLIB_GLOBAL_H
#define SHAREDLIB_GLOBAL_H

#include <QtCore/QtGlobal>

#if defined(NCREPORT_EXPORT)
#  define NCREPORT_LIB_API Q_DECL_EXPORT
#elif defined(NCREPORT_IMPORT) 
#  define NCREPORT_LIB_API Q_DECL_IMPORT
#else
#  define NCREPORT_LIB_API
#endif

#endif // SHAREDLIB_GLOBAL_H
