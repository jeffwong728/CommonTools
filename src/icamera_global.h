#ifndef ICAMERA_GLOBAL_H
#define ICAMERA_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ICAMERA_LIBRARY)
#define ICAMERA_LIBRARY_EXPORT Q_DECL_EXPORT
#else
#define ICAMERA_LIBRARY_EXPORT Q_DECL_IMPORT
#endif

#endif // ICAMERA_GLOBAL_H
