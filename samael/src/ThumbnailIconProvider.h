#ifndef THUMBNAILICONPROVIDER_H
#define THUMBNAILICONPROVIDER_H

#include <QFileIconProvider>

class ThumbnailIconProvider : public QFileIconProvider
{
public:
    ThumbnailIconProvider();
    ~ThumbnailIconProvider();

    virtual QIcon icon( const QFileInfo &info ) const;

private:
    
};

#endif // THUMBNAILICONPROVIDER_H
