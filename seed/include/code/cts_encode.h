#ifndef CTS_ENCODE_H_
#define CTS_ENCODE_H_
#include "code/cts_decode.h"
#include "control/cts_outfile.h"

extern "C"
{
	#include "libavutil/avassert.h"
	#include "libavutil/channel_layout.h"
	#include "libavutil/opt.h"
	#include "libavutil/mathematics.h"
	#include "libavutil/timestamp.h"
	#include "libavformat/avformat.h"
	#include "libswscale/swscale.h"
	#include "libswresample/swresample.h"
	#include "libavutil/audio_fifo.h"
	#include "libavutil/avassert.h"
	#include "libavutil/avstring.h"
};

class CTS_OutputStream
{
public:
	CTS_OutputStream();
	CTS_OutputStream(const CTS_OutputStream &tmp);
	CTS_OutputStream & operator=(const CTS_OutputStream &tmp);
	~CTS_OutputStream();
	int destroy();
public:
    AVStream *st;
    int64_t next_pts;
    int samples_count;
    AVFrame *frame;
    AVFrame *tmp_frame;
    float t, tincr, tincr2;
    struct SwsContext *sws_ctx;
    struct SwrContext *swr_ctx;
};

class CTS_encode
{
public:
	CTS_encode();
	CTS_encode(const CTS_encode &tmp);
	CTS_encode & operator=(const CTS_encode &tmp);
	~CTS_encode();
	int destroy();
	AVFormatContext* enc_get_fmt(void);
	int enc_init(CTS_decode *input_video,const char *filename,Video video,Audio audio);
	int enc_write_frame(CTS_decode *input_video,AVFrame *frame,int type);
	int flush_encoder(AVFormatContext *ofmt_ctx,unsigned int stream_index);
	int64_t getPts(); 
	int getBitrate();
private:
	CTS_OutputStream video_st;
	CTS_OutputStream audio_st;
	AVFormatContext *ofmt_ctx;
	AVOutputFormat *fmt;
	AVCodec *vCodec;
	AVCodec *aCodec;
	int video_stream_index;
	int audio_stream_index;
	AVDictionary *opt;	
	AVAudioFifo *fifo;
};

#endif

