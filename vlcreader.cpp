#include "vlcreader.h"

const QImage::Format image_format = QImage::Format_RGB32;
const int bpf = 4;
const char vlc_chroma[] = "RV32";

const char * const vlc_arg[] = {
    "--noxlib",
    "--vout", "vmem",
    "--network-caching=0",
    "--live-caching=0"
};

VLCReader::VLCReader(QObject *parent) :
    QObject(parent),
    m_vlc_instance(0),
    m_vlc_media_player(0),
    m_vlc_media(0),
    // video callback
    lock_cb(lock),
    unlock_cb(unlock),
    display_cb(display),
    // format callback
    setup_cb(setup),
    cleanup_cb(cleanup)
{
    connect(&vlc_image,SIGNAL(frameReaded(QImage)),SIGNAL(frameReaded(QImage)));
    /* Load the VLC engine */
    m_vlc_instance = libvlc_new(sizeof(vlc_arg)/sizeof(const char*), vlc_arg);
}

VLCReader::~VLCReader()
{
    vlc_release(m_vlc_media);
    vlc_release(m_vlc_media_player);
    vlc_release(m_vlc_instance);
}

void VLCReader::setSource(QString source)
{
    if (m_vlc_instance)
    {
        vlc_release(m_vlc_media);
        vlc_release(m_vlc_media_player);

        /* Create a new item */
        m_vlc_media = libvlc_media_new_location(m_vlc_instance,source.toLocal8Bit().constData());

        /* Create a media player playing environement */
        m_vlc_media_player = libvlc_media_player_new_from_media(m_vlc_media);

        libvlc_video_set_callbacks(m_vlc_media_player,lock_cb,unlock_cb,display_cb,&vlc_image);
        libvlc_video_set_format_callbacks(m_vlc_media_player,setup_cb,cleanup_cb);

        /* No need to keep the media now */
        vlc_release(m_vlc_media);
    }
}

void VLCReader::setFile(QString path)
{
    if (m_vlc_instance)
    {
        vlc_release(m_vlc_media);
        vlc_release(m_vlc_media_player);

        /* Create a new item */
        m_vlc_media = libvlc_media_new_path(m_vlc_instance,path.toLocal8Bit().constData());

        /* Create a media player playing environement */
        m_vlc_media_player = libvlc_media_player_new_from_media(m_vlc_media);

        libvlc_video_set_callbacks(m_vlc_media_player,lock_cb,unlock_cb,display_cb,&vlc_image);
        libvlc_video_set_format_callbacks(m_vlc_media_player,setup_cb,cleanup_cb);

        /* No need to keep the media now */
        vlc_release(m_vlc_media);
    }
}

void VLCReader::play(bool play)
{
    if (play)
    {
        if (m_vlc_instance && m_vlc_media_player)
        {
            libvlc_media_player_play(m_vlc_media_player);
        }
    }
    else
    {
        stop();
    }
}

void VLCReader::stop(bool stop)
{
    if (stop)
    {
        if (m_vlc_instance && m_vlc_media_player)
        {
            libvlc_media_player_stop(m_vlc_media_player);
        }
    }
    else
    {
        play();
    }
}

void VLCReader::vlc_release(libvlc_instance_t*& m_vlc)
{
    if (m_vlc)
    {
        libvlc_release(m_vlc);
        m_vlc = 0;
    }
}

void VLCReader::vlc_release(libvlc_media_player_t*& m_vlc)
{
    if (m_vlc)
    {
        libvlc_media_player_stop(m_vlc);
        libvlc_media_player_release(m_vlc);
        m_vlc = 0;
    }
}

void VLCReader::vlc_release(libvlc_media_t*& m_vlc)
{
    if (m_vlc)
    {
        libvlc_media_release(m_vlc);
        m_vlc = 0;
    }
}

// vlc callback
void * lock(void * opaque, void ** planes)
{
    // called before render frame
    VLCReaderImage* vd = (VLCReaderImage*)opaque;
    vd->lock();
    *planes = vd->image().bits();
    return 0;
}

void unlock(void * opaque, void * picture, void * const * planes)
{
    // called after render frame
    Q_UNUSED(picture)
    Q_UNUSED(planes)

    VLCReaderImage* vd = (VLCReaderImage*)opaque;
    vd->update();
    vd->unlock();
}

void display(void *opaque, void *picture)
{
    Q_UNUSED(opaque)
    Q_UNUSED(picture)
}

unsigned setup(void     **  opaque,
               char     *   chroma,
               unsigned *   width,
               unsigned *   height,
               unsigned *   pitches,
               unsigned *   lines)
{
    // called once at playing start
    memcpy(chroma,vlc_chroma,4);
    *pitches = (*width) * bpf;
    *lines = (*height);

    VLCReaderImage* vd = (VLCReaderImage*)*opaque;
    vd->lock();
    vd->setImage(QImage(*width,*height,image_format));
    vd->unlock();
    return 1;
}

void cleanup(void *opaque)
{
    // called once at playing stop
    Q_UNUSED(opaque)
}
