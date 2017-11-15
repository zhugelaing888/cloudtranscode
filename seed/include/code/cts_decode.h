#ifndef CTS_DECODE_H_
#define CTS_DECODE_H_

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
};
class CTS_decode
{
public:
	CTS_decode();
	~CTS_decode();	
	CTS_decode(const CTS_decode &tmp);
	CTS_decode & operator=(const CTS_decode &tmp);
	int destroy();
	int dec_init(const char *filename);
	int dec_get_frame(AVFrame **frame, int *got, int *type);
	const AVFormatContext* dec_get_fmt(void);
	int dec_get_height(void);
	int dec_get_width(void);
	AVStream *get_type(const int type);
	int get_video_stream_index();
	int get_audio_stream_index();
	int get_duration();
	int get_bitrate();
	int get_rotate();
	AVCodecContext *get_CodecCtx(const int type);
private:
	AVFormatContext *ifmt_ctx;
	int video_stream_index;
	int audio_stream_index;
	AVFrame *dec_frame;
	AVCodecContext *vCodecCtx;
	AVCodecContext *aCodecCtx;
	AVCodec *vCodec;
	AVCodec *aCodec;
	int duration;
	int bitrate;
	int rotate;
};

#endif