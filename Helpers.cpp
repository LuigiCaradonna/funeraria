#include "Helpers.h"

QString Helpers::dateSqlToIta(const QString& date_sql)
{
    // Some of the dates are set to NULL because they went lost
    if (date_sql == "NULL" || date_sql == "") {
        return "";
    }

    QString date = QDate::fromString(date_sql, "yyyy-MM-dd").toString("dd/MM/yyyy");

    // If the date provided was not valid
    if (date == "") {
        return "";
    }

    return date;
}

QString Helpers::monthNumberToName(const int number)
{
    QString month = "nd";

    if (number == 1) month = "Gen";
    else if (number == 2) month = "Feb";
    else if (number == 3) month = "Mar";
    else if (number == 4) month = "Apr";
    else if (number == 5) month = "Mag";
    else if (number == 6) month = "Giu";
    else if (number == 7) month = "Lug";
    else if (number == 8) month = "Ago";
    else if (number == 9) month = "Set";
    else if (number == 10) month = "Ott";
    else if (number == 11) month = "Nov";
    else if (number == 12) month = "Dic";

    return month;
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

bool Helpers::isInt(const QString& number)
{
    QRegularExpression re("^\\d+$");
    QRegularExpressionMatch match_number = re.match(number);

    return match_number.hasMatch();
}

bool Helpers::isDecimal(const QString& number)
{
    QRegularExpression re("^\\d+\\.?\\d*$");
    QRegularExpressionMatch match_number = re.match(number);

    return match_number.hasMatch();
}

float Helpers::scaleFactor(const float original, const float target)
{
    return floorf((target / original) * 1000) / 1000;
}

float Helpers::scaledSize(const float original, const float scale_factor)
{
    return floorf((original * scale_factor) * 1000) / 1000;
}

QString Helpers::getStyle(const QString& css_folder)
{
    QFile file(css_folder + "style.css");
    QString css = "";

    if (file.exists()) {
        QString line;

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            while (!stream.atEnd()) {
                line = stream.readLine();
                css += line + "\n";
            }
        }
    }
    file.close();

    return css;
}
