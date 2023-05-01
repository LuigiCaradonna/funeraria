#pragma once
#include "qstring.h"
#include "qstringlist.h"
#include <QRegularExpression>

class Helpers
{
public:
    /*
     * Converts a date string format as yyyy-mm-dd to dd/mm/yyyy.
     *
     * @param   const QString&  dateSql - The date in Sql format
     *
     * @return  QString - A date in dd/mm/yyyy format or an empty string if the given date was not valid.
     */
	static QString dateSqlToIta(const QString& dateSql);
};
