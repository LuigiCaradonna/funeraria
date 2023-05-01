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
