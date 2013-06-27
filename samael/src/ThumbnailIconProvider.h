#ifndef THUMBNAILICONPROVIDER_H
#define THUMBNAILICONPROVIDER_H

#include <QFileIconProvider>
class QIcon;
class QFileInfo;

class ThumbnailIconProvider : public QFileIconProvider
{
public:
    ThumbnailIconProvider();
    ~ThumbnailIconProvider();

    virtual QIcon icon( const QFileInfo &info ) const;

private:
    QRegExp m_Filters;
};

#endif // THUMBNAILICONPROVIDER_H
