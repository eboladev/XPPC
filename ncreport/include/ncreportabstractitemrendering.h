/****************************************************************************
* 
*  Copyright (C) 2002-2008 Helta Kft. / NociSoft Software Solutions
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
#ifndef NCREPORTABSTRACTITEMRENDERING_H
#define NCREPORTABSTRACTITEMRENDERING_H

#include <QRectF>
#include <QString>

#include "sharedlib_global.h"
QT_BEGIN_NAMESPACE
class QPainter;
QT_END_NAMESPACE
class NCReportOutput;

/*!
Abstract class for custom contents in NCReport. Ideal for creating graphs, drawings or such contents.\n
*/

class NCREPORT_LIB_API NCReportAbstractItemRendering
{
public:
    NCReportAbstractItemRendering();
    virtual ~NCReportAbstractItemRendering();
	
	void setID( const QString& );
	const QString& id() const;
	/*!
	 Abstract method for painting custom item content into the specified report output
	 @param painter active QPainter object
	 @param output current NCReportOutput
	 @param rect Rectangle of the item. Positions and size come from report definition
	 @param itemdata Specification string, come from report definition
	 */
	virtual void paintItem( QPainter* painter, NCReportOutput* output, const QRectF& rect, const QString& itemdata ) =0;
	/*!
	 Virtual method to calculate heigh of item in millimeters when automatic height is set.
	  */
	virtual qreal calculateHeightMM( NCReportOutput* output ) const;
	/*!
	 Virtual method to calculate size of item in millimeters when automatic height is set.
	  */
	virtual QSizeF calculateSizeMM( NCReportOutput* output, const QSizeF& requested ) const;
	virtual QSizeF calculateSizeMM( NCReportOutput* output, const QSizeF& requested, const QString& itemdata ) const;
private:
	QString classid;
};

#endif
