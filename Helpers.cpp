#include "Helpers.h"

QString Helpers::dateSqlToIta(const QString& dateSql)
{
    QRegularExpression re("\\d{4}-\\d{2}-\\d{2}");
    QRegularExpressionMatch match = re.match(dateSql.trimmed());

    if (!match.hasMatch()) {
        return "";
    }

    QStringList date = dateSql.split(u'-');

    return date[2] + "/" + date[1] + "/" + date[0];
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
