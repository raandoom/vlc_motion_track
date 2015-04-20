#ifndef VLCREADER_H
#define VLCREADER_H

#include <QImage>
#include <QMutex>
#include <QMetaType>

Q_DECLARE_METATYPE(QImage)

#include <QDebug>

#include <vlc/vlc.h>

class VLCReaderImage : public QObject {

    Q_OBJECT

    QMutex  m_mutex;
    QImage  m_image;

public:
    VLCReaderImage(QObject* parent = 0) : QObject(parent)
    {
        qRegisterMetaType<QImage>("QImage");
    }

    void lock()
    {
        m_mutex.lock();
    }

    void unlock()
    {
        m_mutex.unlock();
    }

    QImage& image()
    {
        return m_image;
    }

    void setImage(const QImage& img)
    {
        m_image = img;
    }

    void update()
    {
        emit frameReaded(m_image);
    }

signals:
    void frameReaded(QImage img);
};

class VLCReader : public QObject
{
    Q_OBJECT

    VLCReaderImage vlc_image;

    libvlc_instance_t*      m_vlc_instance;
    libvlc_media_player_t*  m_vlc_media_player;
    libvlc_media_t*         m_vlc_media;

    // video callback
    libvlc_video_lock_cb    lock_cb;
    libvlc_video_unlock_cb  unlock_cb;
    libvlc_video_display_cb display_cb;

    // format callback
    libvlc_video_format_cb  setup_cb;
    libvlc_video_cleanup_cb cleanup_cb;

    void vlc_release(libvlc_instance_t*& m_vlc);
    void vlc_release(libvlc_media_player_t*& m_vlc);
    void vlc_release(libvlc_media_t*& m_vlc);

public:
    explicit VLCReader(QObject *parent = 0);
    ~VLCReader();

    void setSource(QString source);
    void setFile(QString path);

    QImage frame();

signals:
    void frameReaded(QImage img);

public slots:
    void play(bool play = true);
    void stop(bool stop = true);
};

void *      lock(void * opaque, void ** planes);
void        unlock(void * opaque, void * picture, void * const * planes);
void        display(void * opaque, void * picture);
unsigned    setup(void     ** opaque,
                  char     *  chroma,
                  unsigned *  width,
                  unsigned *  height,
                  unsigned *  pitches,
                  unsigned *  lines);
void        cleanup(void * opaque);

#endif // VLCREADER_H
