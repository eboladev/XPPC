/***************************************************************************
 *   Copyright (C) 2010 by NociSoft Software Solutions   
 *   support@nocisoft.com   
 ***************************************************************************/
#ifndef NCREPORTHTMLOUTPUT_H
#define NCREPORTHTMLOUTPUT_H

#include "ncreportoutput.h"
#include <QTextStream>
QT_BEGIN_NAMESPACE
class QFile;
QT_END_NAMESPACE
class NCReportLabelItem;
class NCReportLineItem;
class NCReportRectItem;
class NCReportImageItem;
class NCReportTextItem;

/*!
Html Output class. Result pages are stored in one .html file
*/
class NCREPORT_LIB_API NCReportHtmlOutput : public NCReportOutput
{
	Q_OBJECT
public:
	NCReportHtmlOutput( QObject* parent=0 );
	~NCReportHtmlOutput();

	bool setup();
	void cleanup();
	void begin();
	void end();
	void newPage();

	virtual void renderItem( NCReportItem* item, const QRectF& rect );

protected:
	void writeLabel( NCReportLabelItem *item, const QRectF& rect );
	void writeLine( NCReportLineItem *item, const QRectF& rect );
	void writeRectangle( NCReportRectItem * item, const QRectF& rect );
	void writeImage( NCReportImageItem * item, const QRectF& rect );
	void writeText( NCReportTextItem * item, const QRectF& rect );

private:
	QTextStream m_ts;
	QFile* m_file;
	int m_pagePos;

	void writeHeader();
	void writeFooter();
	QString penStyleName( int style ) const;

};


#endif // NCREPORTHTMLOUTPUT_H
