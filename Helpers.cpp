#include "Helpers.h"

QString Helpers::dateSqlToIta(const QString& dateSql)
{
    // Some of the dates are set to NULL because they went lost
    if (dateSql == "NULL") {
        return " - ";
    }

    return QDate::fromString(dateSql, "yyyy-MM-dd").toString("dd/MM/yyyy");
}

bool Helpers::isValidItaDate(const QString& date)
{
    QRegularExpression re("\\d{1,2}/\\d{1,2}/\\d{4}");
    QRegularExpressionMatch match = re.match(date.trimmed());

    if (!match.hasMatch()) {
        return false;
    }

    QDate check_date = QDate::fromString(date, "dd/MM/yyyy");
    if (!QDate::isValid(check_date.year(), check_date.month(), check_date.day())) {
        return false;
    }

    return true;
}
