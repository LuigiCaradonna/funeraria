#include "Helpers.h"

QString Helpers::dateSqlToIta(const QString& dateSql)
{
    // Some of the dates are set to NULL because they went lost
    if (dateSql == "NULL" || dateSql == "") {
        return "";
    }

    QString date = QDate::fromString(dateSql, "yyyy-MM-dd").toString("dd/MM/yyyy");

    // If the date provided was not valid
    if (date == "") {
        return "";
    }

    return date;
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

int Helpers::compareItaDates(const QString& first_date, const QString& second_date)
{
    QDate date1 = QDate::fromString(first_date, "dd/MM/yyyy");
    QDate date2 = QDate::fromString(second_date, "dd/MM/yyyy");

    if (date1 < date2) return -1;
    else if (date1 > date2) return 1;
    else return 0;
}
