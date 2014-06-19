#include "fileoutputvideostream.h"

FileOutputVideoStream::FileOutputVideoStream(QString const &_filename, VideoHeader const &_header) :
    filename(_filename),
    filepath( _DEFAULT_RECORDS_FOLDER + _filename + _DEFAULT_VIDEO_EXT),
    header(_header),
    filebuf(new char[1024 * 2048])
{
    this->fd = fopen(this->filepath.toStdString().c_str(), "w+");
    setvbuf(this->fd, this->filebuf, _IOFBF, 1024 * 2048);
    if (!this->fd) {
        throw std::exception();
    }

    fwrite(&(this->header), sizeof(VideoHeader), 1, this->fd);
}

FileOutputVideoStream::~FileOutputVideoStream()
{
    this->writeFramesIndex();
    fclose(this->fd);

    delete this->filebuf;
}

void FileOutputVideoStream::FileOutputVideoStream::pushFrameData(VideoFrameData *videoFrameData)
{
    videoFrameData->header.data_adress = this->header.content_length;
    this->header.content_length += videoFrameData->header.content_length;

    fwrite(videoFrameData->data, videoFrameData->header.content_length, 1, this->fd );

    delete videoFrameData->data;
    videoFrameData->data = NULL;
    this->frames.push_back(videoFrameData);
}

void FileOutputVideoStream::writeFramesIndex(void)
{
    fflush(this->fd);

    fseek(this->fd, 0, SEEK_SET);
    this->header.frames_count = this->frames.size();

    fwrite(&(this->header), sizeof(VideoHeader), 1, this->fd);

    fflush(this->fd);

    fseek(this->fd, sizeof(VideoHeader) + this->header.content_length, SEEK_SET);

    std::vector<VideoFrameData*>::iterator it = this->frames.begin();
    for(;it != this->frames.end();++it) {
        fwrite(&((*it)->header), sizeof(VideoFrameHeader), 1, this->fd);
    }

    fflush(this->fd);
}
