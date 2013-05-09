#include "stdafx.h"
#include "ThumbnailIconProvider.h"
#include "Logger.h"

ThumbnailIconProvider::ThumbnailIconProvider()
    : QFileIconProvider()
{

}

ThumbnailIconProvider::~ThumbnailIconProvider()
{

}

QIcon ThumbnailIconProvider::icon( const QFileInfo &info ) const
{
    throw std::exception("The method or operation is not implemented.");

    return QIcon();
}
