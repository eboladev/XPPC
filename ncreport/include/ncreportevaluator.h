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
#ifndef NCREPORTEVALUATOR_H
#define NCREPORTEVALUATOR_H

#include <QString>
#include <QVariant>
#include "ncreportdata.h"
#include "sharedlib_global.h"


class NCReportDef;
class NCReportDataSource;
class NCReportFieldItem;
class NCReportVariable;
class NCReportGroup;
class NCReportDirector;
class NCReportItem;
class NCReportSection;
QT_BEGIN_NAMESPACE
class QScriptEngine;
QT_END_NAMESPACE
/*!
This class is responsible for evaluating fields, variables, dynmic items and script expressions.\n
NCReportEvaluator also evaluates the specified logical grouping conditions.
Used by NCReportDirector.
*/
class NCREPORT_LIB_API NCReportEvaluator
{
public:
    NCReportEvaluator();
    NCReportEvaluator( NCReportDef* );
    ~NCReportEvaluator();
	
	enum ExpType { DataSource=0, Parameter, Variable, Field };

	void setReportDef( NCReportDef* );
	void setDirector( NCReportDirector* );
	
	bool evaluate( QString& exp, NCReportSection* section=0 );
	bool evaluate( QString& exp, ExpType, NCReportSection* section=0 );
	bool evaluateScript( QString& exp );
	bool evaluateScan( QString& exp );
	bool evaluateScript( const QString & exp, QString& result );
	bool evaluateScript( const QString & exp, bool& result );
	bool evaluateScript( const QString & exp, double& result );
	bool evaluateScript( const QString & exp, QVariant& result );
	bool evaluateField( NCReportFieldItem* field );
	bool evaluateVariable( NCReportVariable* var );
	bool evaluateGroup( NCReportGroup* g );
	bool evaluatePrintWhen( const QString& expr );
	bool evaluatePrintWhen( NCReportItem* );

	//bool evaluateDynamicItem( NCReportItem* );

	bool evaluateDynamicText( NCReportItem*, QString* target=0 );
	bool evaluateDynamicImage( NCReportItem* );
	bool evaluateDynamicGraph( NCReportItem* );
	bool evaluateDynamicBarcode( NCReportItem* );

	static void formatData( NCReportData::DataType dt, const QVariant& value, QString& result,
							bool formatNum, bool localized, int fieldwidth, char format, int precision, const QChar &fillchar, bool blankIfZero,
							const QString &dateFormat, const QString& arg, int numChars);

	static const QStringList sysVarNames();
	static void tokenizeKeyWords( QStringList& keywordList, ExpType type );
	bool updateFieldFromDataSource( const QString& columnExpr, QVariant& val, NCReportFieldItem* field );

	NCReportDataSource* lastAppliedDataSource() const;
	QVariant dataSourceFunctionValue( NCReportDataSource* ds, const QString& functionName ) const;

private:
	QScriptEngine *engine;
	NCReportDef *rdef;
	NCReportDirector *director;
	NCReportDataSource *currentDataSource;
	int currentDataColumnIndex;
	int currentDataRole;

	bool getDataSourceValue( const QString& name, QVariant& val );
	//HRaba
	int getItemRoleInfo(QString& name);

	bool getParameterValue( const QString& name, QVariant& val );
	bool getVariableValue( const QString& name, QVariant& val );
	bool getSysVarValue( const QString& name, QVariant& val );
	QString getFieldDisplayValue( const QString& id, NCReportSection *section ) const;


	void quoteMarkSafety( QString& exp );
	bool evaluateScriptPrivate( const QString & exp, QVariant& result );
};

#endif
