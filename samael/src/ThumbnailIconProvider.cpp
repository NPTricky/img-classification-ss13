#include "stdafx.h"
#include "ThumbnailIconProvider.h"
#include "Logger.h"
#include <QIcon>

ThumbnailIconProvider::ThumbnailIconProvider(QRegExp &imageextensions)
    : QFileIconProvider()
    , m_Filters(imageextensions)
{

}

ThumbnailIconProvider::~ThumbnailIconProvider()
{

}

QIcon ThumbnailIconProvider::icon(const QFileInfo &info) const
{
    if (m_Filters.exactMatch(info.suffix()))
        return QIcon(info.absoluteFilePath());

    return QFileIconProvider::icon(info);
}
