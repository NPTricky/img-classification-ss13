#include "stdafx.h"
#include "ThumbnailIconProvider.h"
#include "Logger.h"
#include <QIcon>

ThumbnailIconProvider::ThumbnailIconProvider()
    : QFileIconProvider()
{
    m_Filters = QRegExp("(bmp|dib|jpeg|jpg|jpe|jp2|png|pbm|pgm|ppm|tiff|tif)", Qt::CaseInsensitive);
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
