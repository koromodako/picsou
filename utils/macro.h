#ifndef MACRO_H
#define MACRO_H

#include <QDir>
#include <QDebug>

static const QString LOG_LVL_DBG="DBG";
static const QString LOG_LVL_INF="INF";
static const QString LOG_LVL_WRN="WRN";
static const QString LOG_LVL_ERR="ERR";

static const QString LOG_CLR_DBG="32";
static const QString LOG_CLR_INF="34";
static const QString LOG_CLR_WRN="33";
static const QString LOG_CLR_ERR="31";

#define BOOL2STR(b)     ((b)?"true":"false")
#define BASENAME(str)   (QDir(str).dirName())
#define IS_FLAG_SET(field, flag) (((field)&(flag))==(flag))

#ifdef COLORIZE
#    define LOG(log_func, lvl, color, ...) \
    { \
        QDebug debug = log_func(); \
        debug.nospace(); \
        debug.noquote(); \
        debug << "\x1b["<<(color)<<";1m" \
              << "["<<(lvl)<<"](" \
              << (__LINE__)<<"|" \
              << BASENAME(__FILE__)<<"|" \
              << (__func__)<<") - "; \
        debug.quote(); \
        debug << __VA_ARGS__ \
              << "\x1b[0m";
    }
#else /* COLORIZE */
#    define LOG(log_func, lvl, color, ...) \
    { \
        QDebug debug = log_func(); \
        debug.nospace(); \
        debug.noquote(); \
        debug << "["<<(lvl)<<"](" \
              << (__LINE__)<<"|" \
              << BASENAME(__FILE__)<<"|" \
              << (__func__)<<") - "; \
        debug.quote(); \
        debug << __VA_ARGS__; \
    }
#endif /* COLORIZE */

#ifdef QT_DEBUG
#   define LOG_DEBUG(...)   LOG(qDebug, LOG_LVL_DBG, LOG_CLR_DBG, __VA_ARGS__)
#else /* QT_DEBUG */
#   define LOG_DEBUG(...)
#endif /* QT_DEBUG */

#define LOG_INFO(...)       LOG(qInfo, LOG_LVL_INF, LOG_CLR_INF, __VA_ARGS__)
#define LOG_WARNING(...)    LOG(qWarning, LOG_LVL_WRN, LOG_CLR_WRN, __VA_ARGS__)
#define LOG_CRITICAL(...)   LOG(qCritical, LOG_LVL_ERR, LOG_CLR_ERR, __VA_ARGS__)

#define LOG_IN(...)         LOG_DEBUG("<- (" << __VA_ARGS__ << ")")
#define LOG_IN_VOID(...)    LOG_DEBUG("<- (void)")

#define LOG_BOOL_RETURN(bool_expr) \
    { \
        bool result=(bool_expr); \
        LOG_DEBUG("-> "<<BOOL2STR(result)) \
        return result; \
    }
#define LOG_VOID_RETURN(...) \
    LOG_DEBUG("-> (void)") \
    return

#endif // MACRO_H


