/****************************************************************************
* 
*  Copyright (C) 2002-2011 Helta Kft. / NociSoft Software Solutions
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
#ifndef NCREPORTDATASOURCE_H
#define NCREPORTDATASOURCE_H

#include <QObject>
#include <QStringList>
#include <QVector>
#include <QHash>
#include <QVariant>

#include "sharedlib_global.h"

class NCReportError;
//class QSqlDatabase;
class NCReportXMLReader;
class NCReportXMLWriter;
class NCReportEvaluator;
class NCReportDataFieldInfo;

/*!
This abstract class represents the NCReport's data source. 
 */
class NCREPORT_LIB_API NCReportDataSource : public QObject
{
	Q_OBJECT
public:
	NCReportDataSource( QObject *parent=0 );
	virtual ~NCReportDataSource();
	
	enum DataSourceType { Sql=0, Text, XML, StringList, ItemModel, Custom };
	enum LocationType { Static=0, File, Http, Ftp, Parameter };
	enum ConnectionType { Internal=0, External };
	enum OpenRoles { BeginReport=0, ChildDataSource, GroupChange, SubReport };
	
	void setID( const QString& );
	QString id() const;

        //! Assign a brief description for the data source.
        void setBriefDescription( const QString& description);

        //! Returns the brief description.
        QString briefDescription() const;

        //! Assign a long description for the data source.
        void setLongDescription( const QString& description);

        //! Returns the long description.
        QString longDescription() const;

	void setDataSourceType( DataSourceType );
	void setConnectionType( ConnectionType );
	void setLocationType( LocationType );
	void setFileName( const QString& );
	void setEncoding( const QString& );
	void setRecordFilter( const QString& );
	void setSpecText( const QString& );
	void setErrorHandler( NCReportError* );
	/*! Sets the parent datasource ID*/
	void setParentID( const QString& );
	QString parentID() const;
	/*! Sets a parent datasource */
	void setParentDataSource( NCReportDataSource* );
	NCReportDataSource* parentDataSource() const;
	DataSourceType dataSourceType() const;
	ConnectionType connectionType() const;
	LocationType locationType() const;
	QString specText() const;
	QString fileName() const;
	QString rawFileName() const;
	QString recordFilter() const;
	/*! \return Number of data rows (records) */
	virtual int rowCount() const;
	/*! \return Number of data columns */
	virtual int columnCount() const;
	/*! returns if empty data is allowed or not */
	bool isEmptyForbidden();
	/*! set if empty data is allowed or not */
	void setEmptyForbidden( bool );
	
	virtual bool read( NCReportXMLReader* );
	virtual bool write( NCReportXMLWriter* );

	/*!Opens the datasource and fetches data from it*/
	virtual bool open() =0;
	/*!Closes the datasource opened by open()*/
	virtual bool close() =0;
	// data record navigation
	virtual bool first() =0;
	virtual bool last() =0;
	virtual bool next() =0;
	virtual bool previous() =0;
	/*! Returns true if it has next data row. */
	virtual bool hasNext() =0;
	/*! Jumps to the data row/record at position index, if available, and positions the query on the retrieved record.
	The first record is at position 0. If index = -1 the record positioning is skipped (keeps current record)*/
	virtual bool seek( int index )=0;

	/*! Returns the size of the datasource (number of rows/records) */
	virtual int size() const =0;
	/*! Reloads the data if applicable */
	virtual bool reQuery();
	
	//HRaba
	virtual QVariant value( const QString& columnname, bool* ok=0, int nQTDataRole = -1 ) const =0;
	virtual QVariant value( int column, bool* ok=0, int nQTDataRole = -1 ) const =0;
	virtual void evaluate( NCReportEvaluator* ) =0;
	virtual bool isValid() const =0;

	virtual QStringList keywordList();
	virtual QStringList columnNames();
       //! Returns a vector with the information about the data fields.
	virtual QVector<NCReportDataFieldInfo> fieldInfo() const;
	virtual QString columnName(int column) const;
	virtual QString columnTitle(int column) const;
	virtual QString rowTitle(int row) const;
	virtual bool hasColumn(int column) const;
	virtual bool hasColumn(const QString& columnname) const;
	virtual int columnIndexByName(const QString& columnname) const;
	/*! Returns if data source can contain parameters. This is good for XML data source*/
	virtual bool hasParameters() const;
	/*! Returns parameter hash if applicable */
	virtual QHash<QString,QVariant>& parameters();

	/*! Internal function for setting done flag */
	void setDone( bool );
	bool isDone() const;
	bool isOpened();
	void setOpenRole( OpenRoles );
	OpenRoles openRole() const;
	void setAvailable( bool set );
	bool isAvailable() const;

signals:
	void updateRequest( const QString& dsID, const QString& spec );

protected:
	DataSourceType datasourcetype;
	ConnectionType connection;
	LocationType location;
	OpenRoles m_openRole;
	//BaseTypes type;
	QString specification;
	QString _id;
	QString alias;
	QString recordfilter;
	QString filename;
	QString encoding;
	NCReportError *error;
	QString m_parentID;
	NCReportDataSource *m_parent;
	
	int recno;
	bool flagEnd, flagBegin;
	bool prevRecordPending;
	bool emptyForbidden;
	bool m_done;
	bool m_opened;
	bool m_available;

private:
	QString _briefDescription;
	QString _longDescription;

	QHash<QString,QVariant> _parameters;
};


#endif
