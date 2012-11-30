#ifndef NCREPORTDATASOURCEFACTORY_H
#define NCREPORTDATASOURCEFACTORY_H

#include "sharedlib_global.h"

class NCReportDataSource;

class NCREPORT_LIB_API NCReportDataSourceFactory
{
public:
    NCReportDataSourceFactory();
	virtual ~NCReportDataSourceFactory();
	virtual NCReportDataSource * createCustomDataSource( const QString& dsID ) = 0;
};

#endif // NCREPORTDATASOURCEFACTORY_H
