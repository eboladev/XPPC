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
#ifndef NCREPORTITEM_H
#define NCREPORTITEM_H

#include "ncreportdata.h"
#include <QGraphicsItem>

QT_BEGIN_NAMESPACE

class QGraphicsSceneMouseEvent;
class QPaintDevice;

QT_END_NAMESPACE

class NCReportXMLReader;
class NCReportXMLWriter;
class NCReportOutput;
class QGraphicsSceneMouseEvent;
class NCReportSection;
class QPaintDevice;
class NCReportDef;
class NCReportEvaluator;
class NCReportDirector;

#define SELECTION_BOXSIZE		6

/*!
This class represents the base (abstract) report item's data class
 */
class NCReportItemData : public NCReportData
{
public:
	NCReportItemData()
	 : objectType(0), ist(0), fitRole(0),
	 mPos(QPointF(0,0)), mOffset(QPointF(0,0)), mSize(QSizeF(0,0)), mRealSize(QSizeF(0,0)), mSectionSize(QSizeF(0,0)), mZoneSize(QSizeF()),
	 currentSize(QSizeF(0,0)),
	   autoHeight(false), templateMode(false),	 zoneID(0), adjusted(false), printWhenResult(true), upToDate(false), locked(false), breakToPage(false),
	   reportDef(0), director(0)
	{}

	virtual ~NCReportItemData() {}

	uint objectType;
	uint ist;
	uint fitRole;
	
	// metric position/size in mm
	QPointF mPos;
	QPointF mOffset;		// offset pos for Zone handling
	QSizeF  mSize;			// fixed size in mm
	QSizeF  mRealSize;		// real or required size in mm (for Label, Text)
	QSizeF  mSectionSize;	// size of the current section
	QSizeF  mZoneSize;		// size of the current zone (inside the section)
	QSizeF  currentSize;

	bool autoHeight;
	bool templateMode;
	int zoneID;
	bool adjusted;
	bool printWhenResult;	// for internal usage
	bool upToDate;
	bool locked;
	bool breakToPage;	// able to break to available space
	NCReportDef *reportDef;
	NCReportDirector* director;

	QString tagname;
	QString text;
	QString printWhen;
	QString description;
	QRectF boundingRectOfLastPart;
	//QPointF currentScenePos, lastScenePos;

};

/*!
This class represents the base (abstract) report item class
 */
class NCReportItem : public QGraphicsItem
{
public:
 	NCReportItem( QGraphicsItem* parent =0 );
 	virtual ~NCReportItem();
	
	enum SelectionDirection { TopLeft=0, Top, TopRight, Right, BottomRight, Bottom, BottomLeft, Left, LinePoint1, LinePoint2, None };
	enum ObjectType { Label, Field, Text, Line, Rectangle, Ellipse, Image, Barcode, Graph, CrossTab, Chart, Invalid };
	enum ItemSourceType { Static=0, DataSource, File, FileNameFromDataSource, Parameter, Url, UrlFromDataSource, InternalDocument };
	enum FitRole { NoFit=0, FitHeightToSection, FitHeightToZone };

	inline NCReportData::DataType dataType() const
	{ return d->dataType(); }

	inline void setDataType( NCReportData::DataType dt )
	{ d->setDataType(dt); }

	inline QVariant value() const
	{ return d->value(); }

	inline QVariant & valueRef()
	{ return d->valueRef(); }

	inline QString id() const
	{ return d->id(); }
	inline void setID( const QString& id )
	{ d->setID( id ); }

	inline void setValue( const QVariant& value )
	{ d->setValue( value ); }

	inline ObjectType objectType() const
	{ return (ObjectType)d->objectType; }

	void setObjectType( ObjectType t )
	{ d->objectType = t; }

	inline ItemSourceType itemSourceType() const
	{ return (ItemSourceType)d->ist; }

	void setItemSourceType( ItemSourceType ist)
	{ d->ist = ist; }

	inline QString tagname() const
	{ return d->tagname; }

	virtual QString text() const
	{ return d->text; }

	virtual void setText( const QString& text )
	{ d->text = text; }

	inline QString printWhen() const
	{ return d->printWhen; }

	inline void setPrintWhen( const QString& condition )
	{ d->printWhen = condition; }

	inline bool printWhenResult() const
	{ return d->printWhenResult; }

	void setprintWhen( const QString& logicalExpr )
	{ d->printWhen = logicalExpr; }

	inline QString description() const
	{ return d->description; }

	void setDescription(const QString& descr)
	{ d->description = descr ; }

	//inline bool isPrintDone() const
	//{ return d->; }

	inline bool isAutoHeight() const
	{ return d->autoHeight; }

	inline void setAutoHeight( bool set )
	{ d->autoHeight = set; }

	inline FitRole fitRole() const
	{ return (FitRole)d->fitRole; }

	void setFitRole( FitRole role )
	{ d->fitRole = role; }

	// metric position/size in mm
	inline QPointF metricPos() const
	{ return d->mPos; }

	inline void setMetricPos( const QPointF& point )
	{ d->mPos = point ; }

	inline void setMetricX( qreal x )
	{ d->mPos.setX(x); }

	inline void setMetricY( qreal y )
	{ d->mPos.setY(y); }

	/*! Returns zone position offset */
	inline QPointF metricOffset() const		// offset pos for Zone handling
	{ return d->mOffset; }

	/*! Sets zone position offset */
	void setMetricOffset( const QPointF& point )
	{ d->mOffset = point ; }

	inline QSizeF metricSize() const			// fixed size in mm
	{ return d->mSize; }

	void setMetricSize( const QSizeF& size )
	{ d->mSize = size ; }

	inline void setMetricWidth( qreal w )
	{ d->mSize.setWidth(w); }

	inline void setMetricHeight( qreal h )
	{ d->mSize.setHeight(h); }

	void setRealMetricSize( const QSizeF& size )
	{ d->mRealSize = size ; }

	virtual QSizeF sectionMetricSize() const	// size of the current section
	{ return d->mSectionSize; }

	void setSectionMetricSize( const QSizeF& size )
	{ d->mSectionSize = size; }

	virtual QSizeF zoneMetricSize() const		// size of the current zone (inside the section)
	{ return d->mZoneSize; }

	void setZoneMetricSize( const QSizeF& size )
	{ d->mZoneSize = size; }

	inline bool isTemplateMode() const
	{ return d->templateMode; }

	void setTemplateMode( bool set )
	{ d->templateMode = set; }

	inline int zoneID() const
	{ return d->zoneID; }

	void setZoneID( int id )
	{ d->zoneID = id; }

	inline bool isAdjusted() const
	{ return d->adjusted; }
	void setAdjusted( bool set )
	{ d->adjusted = set; }

	inline bool isUpToDate() const
	{ return d->upToDate; }
	void setUpToDate( bool set )
	{ d->upToDate = set; }

	inline bool isLocked() const
	{ return d->locked; }
	void setLocked( bool set )
	{ d->locked = set; }

	inline NCReportDef *reportDef() const
	{ return d->reportDef; }

	virtual void setReportDef( NCReportDef* rdef )
	{ d->reportDef = rdef; }

	inline NCReportDirector *director() const
	{ return d->director; }

	void setDirector( NCReportDirector* director )
	{ d->director = director; }

	inline QRectF boundingRectOfLastPart() const
	{ return d->boundingRectOfLastPart; }

	void setBoundingRectOfLastPart( const QRectF& rect )
	{ d->boundingRectOfLastPart = rect; }

	NCReportSection *sectionScene() const;
	virtual QRectF boundingRect() const;
	virtual int type() const =0;
	virtual void setSnapPos( const QPointF& );
	virtual void resize( const QSizeF& );
	virtual void resizeInMM( const QSizeF& );
	virtual QSizeF currentSize() const;
	//virtual const QSizeF lastSize() const;
	//virtual const QPointF lastPos() const;
	SelectionDirection selectionHoverState() const;
	/*!Size hint for designer*/
	virtual QSizeF sizeHint() const;
	/*! adjust size for designer */
	virtual void adjustSize();
	/*! adjust size for report rendering */
	virtual void adjustSize( NCReportOutput* );
	/*! Returns the real/required size in mm for item. Use adjustSize() before. */
	virtual QSizeF realMetricSize() const;
	virtual void offsetPastePos();
	
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) =0;
	virtual bool read( NCReportXMLReader* );
	virtual bool write( NCReportXMLWriter* );
	/*!Set default settings for editor when add as new item*/
	virtual void setDefaultForEditor()=0;
	/*!Updates its settings for editor after load*/
	virtual void updateForEditor();
	/*!Updates the metric settings before editor saves it's data*/
	virtual void updateForSave();
	/*!Refreshes the size in the scene by metric size*/
	virtual void updateSize();
	/*!Refreshes the position in the scene by metric position*/
	virtual void updatePosition();
	virtual void updateValue( NCReportEvaluator* );
	/*! 
	Print/paint method for rendering item in high resolution device such as QPrinter
	*/
	virtual void paint( NCReportOutput*, const QPointF& )=0;
	/*!
	Dummy paint method for double pass mode calculations.
	*/
	virtual void dummyPaint( NCReportOutput*, const QPointF& );
	/*! Returns position in item was selected */
	QPointF selectPos() const;
	QSizeF selectSize() const;
	/*! Saves the state (position, size etc.) for undo */
	virtual void saveSelectState();
	/*! Returns if item is dynamic or not. */
	virtual bool isDynamic();
	/*! Returns if item is empty or not. Virtual. */
	virtual bool isEmpty();
	virtual void enableOffsetPosition();
	virtual void disableOffsetPosition();
	virtual QPainterPath shape() const;

	virtual bool isAbleToBreak() const;
	virtual void setAbleToBreak( bool set );

	virtual qreal breakTo( qreal heightMM, NCReportOutput* output );
	virtual bool isBroken() const;
	virtual bool hasSizeAndPosition() const;

protected:
	friend class NCReportEvaluator;

	NCReportItemData *d;
	SelectionDirection m_selectionHoverState;
	QPointF m_selectPos;
	QSizeF m_selectSize;
	//bool m_isMoving;
	
	virtual void readBaseProperties( NCReportXMLReader* );
	virtual void preparePaintSelection(QPainter *painter);
	virtual void paintSelection(QPainter *painter);
	virtual QRectF selectionRect( const SelectionDirection, const QSizeF& itemsize ) const;
	virtual void updateCursor(const SelectionDirection dir);
	
	virtual QVariant itemChange( GraphicsItemChange change, const QVariant & value );
	virtual void itemPositionChange( const QPointF& );
	virtual void hoverMoveEvent( QGraphicsSceneHoverEvent * event );
	virtual void mouseMoveEvent( QGraphicsSceneMouseEvent * event );
	virtual void mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event );
	virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent * event );
};

#endif
