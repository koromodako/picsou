#ifndef MACRO_H
#define MACRO_H

#define IS_FLAG_SET(field, flag) (((field)&(flag))==(flag))

#define LOG(log_func, lvl, ...) log_func()<<"["<<(lvl)<<"] - "<<__VA_ARGS__

#ifdef QT_DEBUG
#   define LOG_DEBUG(...)   LOG(qDebug, "DBG", __VA_ARGS__)
#else
#   define LOG_DEBUG(...)
#endif

#define LOG_INFO(...)       LOG(qInfo, "INF", __VA_ARGS__)
#define LOG_WARNING(...)    LOG(qWarning, "WRN", __VA_ARGS__)
#define LOG_CRITICAL(...)   LOG(qCritical, "CTL", __VA_ARGS__)
#define LOG_FATAL(...)      LOG(qFatal, "FTL", __VA_ARGS__)

#endif // MACRO_H
