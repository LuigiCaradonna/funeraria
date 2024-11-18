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
     * @param   const QString&  date_sql - The date in Sql format
     *
     * @return  QString - A date in dd/mm/yyyy format or an empty string if the given date was not valid.
     */
	static QString dateSqlToIta(const QString& date_sql);

    /*
     * Converts month's ordinal number to its name.
     *
     * @param   const int number - The number of the month
     *
     * @return  QString - The name of the month.
     */
    static QString monthNumberToName(const int number);

    /*
     * Checks if the given date has the format dd/mm/yyyy.
     *
     * @param   const QString&  date - The date to check
     *
     * @return  bollean true if the date is valid, false otherwise.
     */
    static bool isValidItaDate(const QString& date);

    /*
     * Compares two dates having the format dd/mm/yyyy.
     *
     * @param   const QString&  date - The date to check
     *
     * @return  int -1, 0 or 1 if the first date is earlier, same or later than the second respectively.
     */
    static int compareItaDates(const QString& first_date, const QString& second_date);
};
