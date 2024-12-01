#pragma once

#include "qstring.h"
#include "qstringlist.h"
#include <QRegularExpression>
#include <QFileDialog>

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

    /*
     * Checks if a string contains an integer number.
     *
     * @param   const QString&  number - The string to check
     *
     * @return  boolean true if the string contains an integer number, false otherwise.
     */
    static bool isInt(const QString& number);

    /*
     * Checks if a string contains a decimal number.
     *
     * @param   const QString&  number - The string to check
     *
     * @return  boolean true if the string contains a decimla number, false otherwise.
     */
    static bool isDecimal(const QString& number);

    /*
     * Computes the scaling factor given the original and target target size.
     *
     * @param   const float original   - The original size
     * @param   const float target     - The target size
     *
     * @return  float the scaling factor truncated to 3 decimal digits.
     */
    static float scaleFactor(const float original, const float target);

    /*
     * Computes the scaled size given the original size and the scaling factor.
     *
     * @param   const float original   - The original size
     * @param   const float target     - The target size
     *
     * @return  float the scaled size truncated to 3 decimal digits.
     */
    static float scaledSize(const float original, const float scale_factor);
};
