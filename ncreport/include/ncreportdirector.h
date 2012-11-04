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
#ifndef NCREPORTDIRECTOR_H
#define NCREPORTDIRECTOR_H

#include <QObject>
#include <QStack>
#include <QQueue>
#include <QTime>
#include <QTextStream>

#include "ncreport.h"
#include "ncreportdatasource.h"

//class NCReport;
class NCReportDef;
class NCReportSection;
class NCReportEvaluator;
//class NCReportDataSource;
class NCReportGroup;
class NCReportItem;
class NCReportSectionDirector;

/*!
This class is the internal engine of NCReport. The NCReportDirector manages the whole report running process.\n
Responsible for running datasources and rendering report.
 */
class NCReportDirector : public QObject
{
	Q_OBJECT
public:
	friend class NCReportSectionDirector;
	enum ProcessState { Initial=0, Finished, PageBegin, PageOnProcess, PageEnd, GroupHeader, GroupFooter,
						PageHeader, PageFooter, ReportHeader, ReportFooter, Detail, Unknown };
	enum PageStatus { Printable=0, Hidden };
	enum PassState { Off=0, TestPass, RealPass };
	enum VariableResetMode { All=0, CurrentDataSourceOnly };
	enum Flags { FlagFinished=0, FlagRHeader, FlagRFooter, FlagNoSpaceInPage, FlagBeginReport, FlagNoUpdate, FlagLastRecord };

    NCReportDirector( NCReport* parent );
    ~NCReportDirector();
	
	void setReportDef( NCReportDef* );
	void setForceCopies( int );
	void setCurrentForceCopy( int );
	void setReportNum( int );
	void setReportCount( int );
	
	bool reportProcess();
	bool reportSubProcess(bool doPageBegin=true, bool doPageEnd=true);
	int pageNum() const;
	int pageCount() const;
	int reportNum() const;
	int reportCount() const;
	int numForceCopies() const;
	int currentForceCopy() const;
	int currentRow() const;
	int numRows() const;
	NCReportEvaluator* evaluator();
	NCReportSection* currentDetail();
	NCReportDataSource *currentDataSource();
	PageStatus pageStatus( int page ) const;
	PageStatus currentPageStatus() const;
	bool flag( Flags ) const;
	bool nextRecord();
	void addSlicedItem( NCReportItem* );
	void setPageBreakStop();
	QPointF paintPosMM() const;
	//QPointF& rPaintPosMM();
	void translatePaintPosY( qreal byMM );
	void setPaintPosY( qreal yMM );
	void setPaintPosX( qreal xMM );
	void anchorBottomPaintPosY(NCReportSection* section);
	/*! Breaks the current page and begins a new one*/
	void pageBreak( bool trimRecord, bool doRepeatGroupHeaders );
	/*! Returns available space to bottom in the current page in mm.*/
	qreal spaceToBottom() const;

signals:
	void dataSourceOpen( const QString& dsID );
	void dataSourceOpened( const QString& dsID, int );
	void dataRowProgress( const QString& dsID, int );
	void dataRowProgress( int row );
	void dataRowCount( int numRows );
	void sectionProgress( const QString& sectionID );
	void pageProgress( int );
	
public slots:
	/*!Cancels report process*/
	void cancel();
	
private:
	NCReport *report;
	NCReportDef *rdef;
	NCReportEvaluator *m_evaluator;
	ProcessState state;
	PageStatus m_pageStatus, m_previousPageStatus;
	PassState m_passState;
	QPointF mPaintPos;
	//qreal mPaintPosX, mPaintPosY;		// painter positions in mm
	bool m_flags[7];
	int pageno;
	int pagecount;
	int reportno;
	int reportcount;
	int detailID; // current processing detail
	int forcecopies, currentforcecopy;
	int currentrow;
	QStack<NCReportGroup*> inside;
	QStack<NCReportGroup*> outside;
	NCReportSection* lastRenderedSection;	// save the last printed section
	QTime m_runtime;
	QQueue<NCReportItem*> m_slicedItems;
	bool m_pageBreakStop;
	bool m_pageBreakIsOnProcess;
	int m_variableCorrectionCallerID;
	NCReportDataSource* m_subReportDS;
	//NCReportSectionDirector *m_sectionDirector;

private:
	//-----------------------------
	// PRIVATE METHODS
	//-----------------------------
	void setFlag( Flags, bool );
	bool openDataSources();
	//bool openDataSources( NCReportDataSource::OpenRoles role );
	bool openDataSource( NCReportDataSource* );
	bool closeDataSources( bool all, NCReportDataSource::OpenRoles role = NCReportDataSource::BeginReport );
	QStringList childDataSources( NCReportDataSource* parent ) const;
	
	void pageBegin();
	void pageEnd();
	void reportHeader();
	void reportFooter();
	bool details();
	bool printOverPageSection();
	bool repeatGroupHeaders();
	void setGroupHeaderPrintLocks( bool enable );
	
	/*!Enters to the next group level and handles it*/
	bool groupIn();	
	/*!Leaves from the group level and handles it*/
	bool groupOut( bool final = false );
	/*! Renders the content of a section*/
	bool renderSection( NCReportSection* );
	
	/*! Updates the fields */
	void updateFields( bool u_datasource, bool u_parameters, bool u_variables, bool u_sysvars, bool u_expressions );
	void updateVariables();
	void updateDynamicItems();
	void updateSectionDynamicItems( NCReportSection * );
	void updateGroups( NCReportSection *d );
	void resetGroup( NCReportGroup* );
	void reprintGroupProtection( bool enable, NCReportSection* =0 );
	void initGroupStacks();
	/*! Reset all variables */
	void resetVariables( VariableResetMode mode );
	
	//bool printTextDocument_obsolete();
	bool documentReportProcess();
	void documentReportProcess_Items( NCReportSection* detail, QTextStream& inputHtml,
									  qreal& documentLeftMarginMM, QSizeF& documentPageSizeMM, bool& sizeAndPosIsModified, QTextDocument* document );

	/*! returns the width of the section */
	qreal sectionWidthMM( NCReportSection* ) const;
	/*! returns true if there are more detail(s) after current */
	bool isNextDetail() const;
	/*! Step to next record in the datasource ds */
	bool nextRecord(NCReportDataSource*) const;
	/*! Step to previous record in the datasource ds */
	bool previousRecord(NCReportDataSource*) const;
	/*! Requeries (sql) datasource by ds */
	bool reQueryDataSource( NCReportDataSource* ) const;
	/*! Requeries (sql) datasource that is assigned to a group by group ID */
	bool reQueryDataSource( const QString& groupID ) const;

	void restoreFields( NCReportSection* section, uint mode );
	void benchmark( const QString& msg );
	void paintUnregText();
	bool prepareNextDetail();
	void fatalError( const QString& msg );
	bool pageBreakIsAllowed( NCReportGroup* );
	void variableCorrections(bool enable, int callerID );

protected:
	virtual void beforePageBreak();
	virtual void afterPageBreak();
};

#endif
