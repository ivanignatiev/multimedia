#include "videolibrary.h"

VideoLibrary::VideoLibrary(QString    const & dirpath) :
    dir(QDir(dirpath))
{

}


QFileInfo const &VideoLibrary::getFileInfoById(int id) const
{
    return this->dir.entryInfoList().at(id);
}

QStandardItemModel *VideoLibrary::getListOfVideos(void)
{
    return &(this->videos);
}

void VideoLibrary::refresh(void)
{
    QStringList filters;
    filters << "*" + QString(_DEFAULT_VIDEO_EXT);
    this->dir.setNameFilters(filters);
    this->dir.refresh();

    QFileInfoList list = this->dir.entryInfoList();
    this->videos.clear();
    for (int i = 0; i < list.size(); ++i) {
          QFileInfo fileInfo = list.at(i);

          FILE *fd = fopen(fileInfo.filePath().toStdString().c_str(), "r+");

          unsigned int h = 0;
          unsigned int m = 0;
          unsigned int s = 0;

          if (fd) {
              VideoHeader header;
              fread(&(header), sizeof(VideoHeader), 1, fd);

              s = header.frames_count / header.fps;

              h = s / 3600;
              m = (s - h * 3600) / 60;
              s = (s - h * 3600 - m * 60);

              fclose(fd);

          }

          this->videos.appendRow(new QStandardItem( QString("").sprintf("[%02u:%02u:%02u] ", h, m, s) + fileInfo.baseName() ));
    }
}
