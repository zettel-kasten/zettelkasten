#ifndef UIHELPER_H
#define UIHELPER_H

#include <QObject>
#include <QWidget>

#include <QPainter>

#include <string>

inline std::string className(const std::string& prettyFunction)
{
    size_t colons = prettyFunction.find("::");
    if (colons == std::string::npos)
        return "::";
    size_t begin = prettyFunction.substr(0,colons).rfind(" ") + 1;
    size_t end = colons - begin;

    return prettyFunction.substr(begin,end);
}

#define __CLASS_NAME__ className(__PRETTY_FUNCTION__)



#endif // UIHELPER_H
