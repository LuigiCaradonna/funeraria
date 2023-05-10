#pragma once
#include "qstring.h"
#include "qstringlist.h"
#include <QRegularExpression>
#include <QDate>

class Helpers
{
public:
    /*
     * Converts a date string formatted as yyyy-mm-dd to dd/mm/yyyy.
     *
     * @param   const QString&  dateSql - The date in Sql format
     *
     * @return  QString - A date in dd/mm/yyyy format or an empty string if the given date was not valid.
     */
	static QString dateSqlToIta(const QString& dateSql);

    /*
     * Checks if the given date has the format dd/mm/yyyy.
     *
     * @param   const QString&  date - The date to check
     *
     * @return  bollean true if the date is valid, false otherwise.
     */
    static bool isValidItaDate(const QString& date);
};
