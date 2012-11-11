#ifndef NCREPORTDUMMYDATASOURCEFACTORY_H
#define NCREPORTDUMMYDATASOURCEFACTORY_H

#include "ncreportdatasourcefactory.h"

class NCReportDummyDataSourceFactory : public NCReportDataSourceFactory
{
public:
    NCReportDummyDataSourceFactory();
	virtual NCReportDataSource * createCustomDataSource( const QString& dsID );
};

#endif // NCREPORTDUMMYDATASOURCEFACTORY_H
