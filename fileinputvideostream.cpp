#include "fileinputvideostream.h"

FileInputVideoStream::FileInputVideoStream(QString const &_filename) :
    filename(_filename),
    filepath( _DEFAULT_RECORDS_FOLDER + _filename + _DEFAULT_VIDEO_EXT)
{
    this->fd = fopen(this->filepath.toStdString().c_str(), "r+");

    if (!this->fd) {
        throw std::exception();
    }

    fread(&(this->header), sizeof(VideoHeader), 1, this->fd);
}

FileInputVideoStream::~FileInputVideoStream()
{
    fclose(this->fd);
}

VideoFrameData *FileInputVideoStream::getFrameData(unsigned long id)
{
    fseek(this->fd, sizeof(VideoHeader) + this->header.content_length + id * sizeof(VideoFrameHeader), SEEK_SET);
    VideoFrameData *frame = new VideoFrameData();
    fread(&(frame->header), sizeof(VideoFrameHeader), 1, this->fd);

    fseek(this->fd, sizeof(VideoHeader) + frame->header.data_adress, SEEK_SET);
    frame->data = new unsigned char[frame->header.content_length];
    fread(frame->data, sizeof(unsigned char), frame->header.content_length, this->fd);

    return frame;
}

VideoHeader const &FileInputVideoStream::getHeader(void) const
{
    return this->header;
}
