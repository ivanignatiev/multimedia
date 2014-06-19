#ifndef VIDEOLIBRARY_H
#define VIDEOLIBRARY_H

#include <QDir>
#include <QStandardItemModel>
#include <stdio.h>
#include "config.h"
#include "videoframe.h"

class VideoLibrary
{
public:
    VideoLibrary(QString    const & dirpath);

    QFileInfo const &getFileInfoById(int id) const;
    QStandardItemModel *getListOfVideos(void);

    void refresh(void);
private:
    QString     dirpath;
    QStandardItemModel videos;
    QDir        dir;
};

#endif // VIDEOLIBRARY_H
