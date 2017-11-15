#include "code/cts_encode.h"
#include "code/cts_decode.h"
#include "util/cts_log.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;


#define _MSG_LEN 1024
static char _MSG[_MSG_LEN] = {0,};
#undef av_ts2str



static int sec_time = 0;
static inline char *av_ts2str(int64_t ts)
{
    char *buf = _MSG; 
    memset(buf, 0, _MSG_LEN);

    if (ts == AV_NOPTS_VALUE) snprintf(buf, AV_TS_MAX_STRING_SIZE, "NOPTS");
    else                      snprintf(buf, AV_TS_MAX_STRING_SIZE, "%"PRId64, ts);
    return buf;
}

#undef av_ts2timestr
static inline char *av_ts2timestr(int64_t ts, AVRational *tb)
{
    char *buf = _MSG; 
    memset(buf, 0, _MSG_LEN);
    if (ts == AV_NOPTS_VALUE) snprintf(buf, AV_TS_MAX_STRING_SIZE, "NOPTS");
    else                      snprintf(buf, AV_TS_MAX_STRING_SIZE, "%.6g", av_q2d(*tb) * ts);
    return buf;
}

#undef av_err2str
static inline char *av_err2str(int errnum)
{
    char *buf = _MSG; 
    memset(buf, 0, _MSG_LEN);
    av_strerror(errnum, buf, _MSG_LEN);
    return buf;
}


static const char *get_error_text(const int error)
{
    static char error_buffer[255];
    av_strerror(error, error_buffer, sizeof(error_buffer));
    return error_buffer;
}


CTS_OutputStream::CTS_OutputStream():st(NULL),next_pts(0),samples_count(0),frame(NULL),tmp_frame(NULL),t(0),tincr(0),tincr2(0)
	,sws_ctx(NULL),swr_ctx(NULL)
{}
CTS_OutputStream::CTS_OutputStream(const CTS_OutputStream &tmp):st(tmp.st),next_pts(tmp.next_pts),samples_count(tmp.samples_count),frame(tmp.frame),tmp_frame(tmp.tmp_frame),t(tmp.t),tincr(tmp.tincr),tincr2(tmp.tincr2)
	,sws_ctx(tmp.sws_ctx),swr_ctx(tmp.swr_ctx)
{}
CTS_OutputStream & CTS_OutputStream::operator=(const CTS_OutputStream &tmp)
{
	st = tmp.st;
	next_pts = tmp.next_pts;
	samples_count = tmp.samples_count;
	frame = tmp.frame;
	tmp_frame = tmp.tmp_frame;
	t = tmp.t;
	tincr = tmp.tincr;
	tincr2 = tmp.tincr2;
	sws_ctx = tmp.sws_ctx;
	swr_ctx = tmp.swr_ctx;
	return *this;
}
CTS_OutputStream::~CTS_OutputStream()
{
}

int CTS_OutputStream::destroy()
{	
	avcodec_close(st->codec);
    av_frame_free(&frame);
    av_frame_free(&tmp_frame);
    sws_freeContext(sws_ctx);
    swr_free(&swr_ctx);
	return 0;
}
static void log_packet(const AVFormatContext *fmt_ctx, const AVPacket *pkt)
{
    //AVRational *time_base = &fmt_ctx->streams[pkt->stream_index]->time_base;

    /*ERROR("enc pts:%s pts_time:%s dts:%s dts_time:%s duration:%s duration_time:%s stream_index:%d\n",
           av_ts2str(pkt->pts), av_ts2timestr(pkt->pts, time_base),
           av_ts2str(pkt->dts), av_ts2timestr(pkt->dts, time_base),
           av_ts2str(pkt->duration), av_ts2timestr(pkt->duration, time_base),
           pkt->stream_index);
          */

}

static int write_frame(AVFormatContext *fmt_ctx, const AVRational *time_base, AVStream *st, AVPacket *pkt)
{
	av_packet_rescale_ts(pkt, *time_base, st->time_base);	
   	pkt->stream_index = st->index;
	log_packet(fmt_ctx, pkt);	
	return av_interleaved_write_frame(fmt_ctx, pkt);
}

std::vector<std::string> split(std::string str,std::string pattern)  
{  
    std::string::size_type pos;  
    std::vector<std::string> result;  
    str+=pattern;//扩展字符串以方便操作  
    std::string::size_type size=str.size();  
  
    for(std::string::size_type i=0; i < size; i++)  
    {  
        pos=str.find(pattern,i);  
        if(pos<size)  
        {  
            std::string s=str.substr(i,pos-i);  
            result.push_back(s);  
            i=pos+pattern.size()-1;  
        }  
    }  
    return result;  
}

/* Add an audio output stream. */

static int add_audio_stream(CTS_decode *input_video,CTS_OutputStream *ost, AVFormatContext *oc,AVCodec **codec,enum AVCodecID codec_id,const int type, Audio &audio)
{
	AVCodecContext *c;
	AVStream *stream = NULL;
	/* find the encoder */
		*codec = avcodec_find_encoder(codec_id);
	if (!(*codec)) 
	{
		ERROR("Could not find output audio encoder for [%s]",avcodec_get_name(codec_id));
		return 0;
    }
    ost->st = avformat_new_stream(oc, *codec);
    if (!ost->st) 
	{
		ERROR( "Could not allocate output audio stream");
		return 0;
    }
	ost->st->id = oc->nb_streams-1;
	c = ost->st->codec;
	stream							= input_video->get_type(AVMEDIA_TYPE_AUDIO);
	if(audio.channels == 0)
		c->channels 				= stream->codec->channels;
	else
		c->channels					= audio.channels;
	c->channel_layout				= av_get_default_channel_layout(c->channels);
	c->sample_fmt					= (*codec)->sample_fmts[0];
	c->sample_rate					= stream->codec->sample_rate;
	c->bit_rate 					= stream->codec->bit_rate;
	/*if(audio.sample_rate == 0)
		c->sample_rate				= stream->codec->sample_rate;
	else
		c->sample_rate					= audio.sample_rate;
	
	if(audio.bitrate == 0)
		c->bit_rate 				= stream->codec->bit_rate;
	else
		c->bit_rate						= audio.bitrate*1000;*/
	c->strict_std_compliance		= FF_COMPLIANCE_EXPERIMENTAL;
	ost->st->time_base.den			= stream->time_base.den;
	ost->st->time_base.num			= stream->time_base.num;
	if (oc->oformat->flags & AVFMT_GLOBALHEADER)
		c->flags |= CODEC_FLAG_GLOBAL_HEADER;
	/* Some formats want stream headers to be separate. */
	if (oc->oformat->flags & AVFMT_GLOBALHEADER)
		c->flags |= CODEC_FLAG_GLOBAL_HEADER;
	return 1;
}

/* Add an video output stream. */
static int add_video_stream(CTS_decode *input_video,CTS_OutputStream *ost, AVFormatContext *oc,AVCodec **codec,enum AVCodecID codec_id,const int type, Video &video)
{
	AVCodecContext *c;
	AVStream *stream = NULL;
	/* find the encoder */
	*codec = avcodec_find_encoder(codec_id);
	if (!(*codec)) 
	{
		ERROR("Could not find output video encoder for [%s]",avcodec_get_name(codec_id));
		return 0;
    }
    ost->st = avformat_new_stream(oc, *codec);
    if (!ost->st) 
	{    
       	ERROR( "Could not allocate output video stream");
		return 0;
    }
	int rotate = input_video->get_rotate();
	if(rotate == 90)
		av_dict_set(&ost->st->metadata,"rotate","90",0);
	else if(rotate == 180)
		av_dict_set(&ost->st->metadata,"rotate","180",0);
	else if(rotate == 270)
		av_dict_set(&ost->st->metadata,"rotate","270",0);
	else
		av_dict_set(&ost->st->metadata,"rotate","0",0);
    ost->st->id = oc->nb_streams-1;
    c = ost->st->codec;
	stream							= input_video->get_type(AVMEDIA_TYPE_VIDEO);
    c->codec_id						= codec_id;
	if(video.bitrate == 0)
		c->bit_rate					= stream->codec->bit_rate;
	else
		c->bit_rate					= video.bitrate*1000;
	if(video.height == 0)
		c->height					= stream->codec->height;
	else
		c->height					= video.height;
	if(video.width == 0)
		c->width					= stream->codec->width;
	else
		c->width					= video.width;
	if(video.aspect[0] != '\0')
	{
		vector<string> res = split(video.aspect,":");
		c->sample_aspect_ratio.num 	= atoi(res[0].c_str())/4;
		c->sample_aspect_ratio.den 	= atoi(res[1].c_str())/3;
	}
	if(video.lev == mid)
		c->profile 					= FF_PROFILE_H264_MAIN;
	else if(video.lev == high)
		c->profile 					= FF_PROFILE_H264_HIGH;
	ost->st->time_base				= (AVRational){stream->r_frame_rate.den, stream->r_frame_rate.num };
    c->time_base					= ost->st->time_base;
    //c->gop_size						= 12; /* emit one intra frame every twelve frames at most */
    c->pix_fmt						= AV_PIX_FMT_YUV420P;
	c->max_b_frames					= stream->codec->max_b_frames;
    /* Some formats want stream headers to be separate. */
    if (oc->oformat->flags & AVFMT_GLOBALHEADER)
        c->flags |= CODEC_FLAG_GLOBAL_HEADER;
	return 1;
}

/*
	Open output video stream
*/
static AVFrame *alloc_picture(enum AVPixelFormat pix_fmt, int width, int height)
{
    AVFrame *picture;
    int ret;

    picture = av_frame_alloc();
    if (!picture)
    {
    	ERROR("av_frame_alloc error");
        return NULL;
    }

    picture->format = pix_fmt;
    picture->width  = width;
    picture->height = height;

    /* allocate the buffers for the frame data */
    ret = av_frame_get_buffer(picture, 32);
    if (ret < 0) {
        ERROR( "Could not allocate frame data.");
        return NULL;
    }

    return picture;
}
static int  open_video(CTS_decode *input_video,AVFormatContext *oc, AVCodec *codec, CTS_OutputStream *ost, AVDictionary *opt_arg)
{	
	int input_video_width = input_video->dec_get_width();
	int input_video_height = input_video->dec_get_height();
    int ret;
    AVCodecContext *c = ost->st->codec;
    AVDictionary *opt = NULL;	
    av_dict_copy(&opt, opt_arg, 0);	
    /* open the codec */
    ret = avcodec_open2(c, codec, &opt);
    av_dict_free(&opt);
    if (ret < 0) {
		ERROR( "Could not open video codec: [%s]", av_err2str(ret));
		return 0;
    }
    /* allocate and init a re-usable frame */
    ost->frame = alloc_picture(c->pix_fmt, c->width, c->height);
    if (!ost->frame) {
        ERROR( "Could not allocate video frame");
        return 0;
    }
    /* If the output format is not YUV420P, then a temporary YUV420P
     * picture is needed too. It is then converted to the required
     * output format. */
    ost->tmp_frame = NULL;
    if (c->pix_fmt != AV_PIX_FMT_YUV420P) {
        ost->tmp_frame = alloc_picture(AV_PIX_FMT_YUV420P, c->width, c->height);
        if (!ost->tmp_frame) {
            ERROR( "Could not allocate temporary picture");
            return 0;
        }
    }
	AVStream *stream = input_video->get_type(AVMEDIA_TYPE_VIDEO);
    ost->sws_ctx = sws_getContext(input_video_width, input_video_height,
							stream->codec->pix_fmt,
							ost->st->codec->width, ost->st->codec->height,
							ost->st->codec->pix_fmt,
							SWS_BICUBIC, NULL, NULL, NULL);
	if (!ost->sws_ctx) {
		ERROR(  "Could not allocate resample context");
		return 0;
	}
	return 1;
}

/* 
	Write video key frame
	*/
static AVFrame *get_video_frame(CTS_decode *input_video,CTS_OutputStream *ost,AVFrame *frame)
{
	int input_video_height = input_video->dec_get_height();
	sws_scale(ost->sws_ctx,
                  (const uint8_t * const *)frame->data, frame->linesize,
                  0, input_video_height , ost->frame->data, ost->frame->linesize);
    ost->frame->pts = ost->next_pts++;
    return ost->frame;
}

static int write_video_frame(AVFormatContext *oc, CTS_OutputStream *ost, AVFrame *frame)
{
    int ret;
    AVCodecContext *c;   
    int got_packet = 0;
    c = ost->st->codec;
    if (oc->oformat->flags & AVFMT_RAWPICTURE) 
	{
        /* a hack to avoid data copy with some raw video muxers */
        AVPacket pkt;
        av_init_packet(&pkt);
        if (!frame)
            return 0;

        pkt.flags        |= AV_PKT_FLAG_KEY;
        pkt.stream_index  = ost->st->index;
        pkt.data          = (uint8_t *)frame;
        pkt.size          = sizeof(AVPicture);

        pkt.pts = pkt.dts = frame->pts;
        ret = av_interleaved_write_frame(oc, &pkt);
		av_free_packet(&pkt);
    }
	else 
	{
        AVPacket pkt = { 0 };
        av_init_packet(&pkt);
        /* encode the image */
        ret = avcodec_encode_video2(c, &pkt, frame, &got_packet);
        if (ret < 0)
		{
            ERROR( "Error encoding video frame: %s", av_err2str(ret));
            return 0;
        }
        if (got_packet) {
	    	sec_time++;
            ret = write_frame(oc, &c->time_base, ost->st, &pkt);
        } else {
            ret = 0;
        }
		av_free_packet(&pkt);
    }

    if (ret < 0) {
        ERROR( "Error while writing video frame: %s", av_err2str(ret));
        return 0;
    }

    return (frame || got_packet) ? 1 : 0;
}



//////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/*
	Open output audio stream
*/

static int open_audio(CTS_decode *input_video,AVFormatContext *oc, AVCodec *codec, CTS_OutputStream *ost, AVDictionary *opt_arg)
{
    AVCodecContext *output_codec_context;
	AVCodecContext *input_codec_context;
    int ret;
    AVDictionary *opt = NULL;

    output_codec_context = ost->st->codec;
	input_codec_context = input_video->get_CodecCtx(AVMEDIA_TYPE_AUDIO);

    /* open it */
    av_dict_copy(&opt, opt_arg, 0);
    ret = avcodec_open2(output_codec_context, codec, &opt);
    av_dict_free(&opt);
    if (ret < 0) {
        ERROR(  "Could not open audio codec: [%s]", av_err2str(ret));
        return 0;
    }
    int error;
    ost->swr_ctx = swr_alloc_set_opts(NULL,
		av_get_default_channel_layout(output_codec_context->channels),
		output_codec_context->sample_fmt,
		output_codec_context->sample_rate,
		av_get_default_channel_layout(input_codec_context->channels),
		input_codec_context->sample_fmt,
		input_codec_context->sample_rate,
		0, NULL);
	if (!ost->swr_ctx) {
		ERROR(  "Could not allocate resample context");
		return 0;
	}
	//av_assert0(output_codec_context->sample_rate == input_codec_context->sample_rate);
	if ((error = swr_init(ost->swr_ctx)) < 0) {
		ERROR(  "Could not open resample context");
		return 0;
	}
	return 1;
}

/*
	Write audio key frame
*/
/** Initialize a FIFO buffer for the audio samples to be encoded. */
static int init_fifo(AVAudioFifo **fifo, AVCodecContext *output_codec_context)
{
    /** Create the FIFO buffer based on the specified output sample format. */
    if (!(*fifo = av_audio_fifo_alloc(output_codec_context->sample_fmt,
                                      output_codec_context->channels, 1))) {
        ERROR( "Could not allocate FIFO");
        return 0;
    }
    return 1;
}

/** Initialize one data packet for reading or writing. */
static void init_packet(AVPacket *packet)
{
    av_init_packet(packet);
    /** Set the packet data and size so that it is recognized as being empty. */
    packet->data = NULL;
    packet->size = 0;
}


/**
 * Convert the input audio samples into the output sample format.
 * The conversion happens on a per-frame basis, the size of which is specified
 * by frame_size.
 */
static int convert_samples(const uint8_t **input_data,
                           uint8_t **converted_data, const int frame_size,
                           SwrContext *resample_context)
{
    int error;

    /** Convert the samples using the resampler. */
    if ((error = swr_convert(resample_context,
                             converted_data, frame_size,
                             input_data    , frame_size)) < 0) {
        ERROR( "Could not convert input samples (error '%s')",
                get_error_text(error));
        return 0;
    }

    return 1;
}

/** Add converted input audio samples to the FIFO buffer for later processing. */
static int add_samples_to_fifo(AVAudioFifo *fifo,
                               uint8_t **converted_input_samples,
                               const int frame_size)
{
    int error;

    /**
     * Make the FIFO as large as it needs to be to hold both,
     * the old and the new samples.
     */
    if ((error = av_audio_fifo_realloc(fifo, av_audio_fifo_size(fifo) + frame_size)) < 0) {
        ERROR( "Could not reallocate FIFO");
        return 0;
    }

    /** Store the new samples in the FIFO buffer. */
    if (av_audio_fifo_write(fifo, (void **)converted_input_samples,
                            frame_size) < frame_size) {
        ERROR( "Could not write data to FIFO");
        return 0;
    }
    return 1;
}
static int init_converted_samples(uint8_t ***converted_input_samples,
                                  AVCodecContext *output_codec_context,
                                  int frame_size)
{
    int error;

    /**
     * Allocate as many pointers as there are audio channels.
     * Each pointer will later point to the audio samples of the corresponding
     * channels (although it may be NULL for interleaved formats).
     */
    if (!(*converted_input_samples = (uint8_t **)calloc(output_codec_context->channels,
                                            sizeof(**converted_input_samples)))) {
        ERROR( "Could not allocate converted input sample pointers");
        return 0;
    }

    /**
     * Allocate memory for the samples of all channels in one consecutive
     * block for convenience.
     */
    if ((error = av_samples_alloc(*converted_input_samples, NULL,
                                  output_codec_context->channels,
                                  frame_size,
                                  output_codec_context->sample_fmt, 0)) < 0) {
        ERROR("Could not allocate converted input samples (error '%s')",
                get_error_text(error));
       // av_freep(&(*converted_input_samples)[0]);
       // free(*converted_input_samples);
        return 0;
    }
    return 1;
}


/**
 * Read one audio frame from the input file, decodes, converts and stores
 * it in the FIFO buffer.
 */
static int read_decode_convert_and_store(AVAudioFifo *fifo, AVFrame *input_frame,
                                         AVCodecContext *output_codec_context,
                                         SwrContext *resampler_context,
                                         int *finished)
{
    /** Temporary storage of the input samples of the frame read from the file. */
    /** Temporary storage for the converted input samples. */
    uint8_t **converted_input_samples = NULL;
    int ret = 0;

	if (!init_converted_samples(&converted_input_samples, output_codec_context,
		input_frame->nb_samples))
		goto cleanup;
	if (!convert_samples((const uint8_t**)input_frame->extended_data, converted_input_samples,
		input_frame->nb_samples, resampler_context))
		goto cleanup;
	/** Add the converted input samples to the FIFO buffer for later processing. */
	if (!add_samples_to_fifo(fifo, converted_input_samples,
		input_frame->nb_samples))
		goto cleanup;
	ret = 1;
cleanup:
    if (converted_input_samples) {
        av_freep(&converted_input_samples[0]);
        free(converted_input_samples);
    }
    return ret;
}

static int init_output_frame(AVFrame **frame,
                             AVCodecContext *output_codec_context,
                             int frame_size)
{
    int error;

    /** Create a new frame to store the audio samples. */
    if (!(*frame = av_frame_alloc())) {
        ERROR("Could not allocate output frame");
        return 0;
    }

    /**
     * Set the frame's parameters, especially its size and format.
     * av_frame_get_buffer needs this to allocate memory for the
     * audio samples of the frame.
     * Default channel layouts based on the number of channels
     * are assumed for simplicity.
     */
    (*frame)->nb_samples     = frame_size;
    (*frame)->channel_layout = output_codec_context->channel_layout;
    (*frame)->format         = output_codec_context->sample_fmt;
    (*frame)->sample_rate    = output_codec_context->sample_rate;

    /**
     * Allocate the samples of the created frame. This call will make
     * sure that the audio frame can hold as many samples as specified.
     */
    if ((error = av_frame_get_buffer(*frame, 0)) < 0) {
        ERROR( "Could allocate output frame samples (error '%s')",
                get_error_text(error));
        av_frame_free(frame);
        return 0;
    }

    return 1;
}

static int encode_audio_frame(AVFrame *frame,
                              AVFormatContext *output_format_context,
                              AVCodecContext *output_codec_context,
							  CTS_OutputStream *ost,
                              int *data_present)
{
    /** Packet used for temporary storage. */
    AVPacket output_packet;
    int error;
    init_packet(&output_packet);

    ///** Set a timestamp based on the sample rate for the container. */
    if (frame) {
		frame->pts = ost->next_pts;
        ost->next_pts += frame->nb_samples;
    }
    /**
     * Encode the audio frame and store it in the temporary packet.
     * The output audio stream encoder is used to do this.
     */
    if ((error = avcodec_encode_audio2(output_codec_context, &output_packet,
                                       frame, data_present)) < 0) {
        ERROR( "Could not encode frame (error '%s')",
                get_error_text(error));
        av_free_packet(&output_packet);
        return 0;
    }

    /** Write one audio frame from the temporary packet to the output file. */
    if (*data_present) {
		if ((error = write_frame(output_format_context, &output_codec_context->time_base, ost->st, &output_packet)) < 0) {
            ERROR( "Could not write frame (error '%s')",
                    get_error_text(error));
            av_free_packet(&output_packet);
            return 0;
        }

        av_free_packet(&output_packet);
    }

    return 1;
}

static int load_encode_and_write(AVAudioFifo *fifo,
                                 AVFormatContext *output_format_context,
                                 CTS_OutputStream *ost)
{
    /** Temporary storage of the output samples of the frame written to the file. */
    AVFrame *output_frame;
	AVCodecContext *output_codec_context = ost->st->codec;
    /**
     * Use the maximum number of possible samples per frame.
     * If there is less than the maximum possible frame size in the FIFO
     * buffer use this number. Otherwise, use the maximum possible frame size
     */
    const int frame_size = FFMIN(av_audio_fifo_size(fifo),
                                 output_codec_context->frame_size);
    int data_written;

    /** Initialize temporary storage for one output frame. */
    if (!init_output_frame(&output_frame, output_codec_context, frame_size))
    {
    	ERROR("%s","Initialize temporary storage for one output frame error");
        return 0;
    }

    /**
     * Read as many samples from the FIFO buffer as required to fill the frame.
     * The samples are stored in the frame temporarily.
     */
    if (av_audio_fifo_read(fifo, (void **)output_frame->data, frame_size) < frame_size) {
        ERROR("%s","Could not read data from FIFO");
        av_frame_free(&output_frame);
        return 0;
    }
	
    /** Encode one frame worth of audio samples. */
    if (!encode_audio_frame(output_frame, output_format_context,
                           output_codec_context,ost,&data_written)) {
        ERROR("%s","Encode one frame worth of audio samples error");
        av_frame_free(&output_frame);
        return 0;
    }
    av_frame_free(&output_frame);
    return 1;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int64_t CTS_encode::getPts()
{
	return video_st.next_pts;
}


int CTS_encode::getBitrate()
{
	return video_st.st->codec->bit_rate;
}

int CTS_encode::enc_init(CTS_decode *input_video,const char *filename,Video video,Audio audio)
{
	int ret;
    av_register_all();
    avformat_network_init();
    /* allocate the output media context */
    avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, filename);
    if (!ofmt_ctx)
	{
		ERROR("allocate the output media context error - [%s]",filename);
        return 0;
	}
    fmt = ofmt_ctx->oformat;
	if (fmt->video_codec != AV_CODEC_ID_NONE && input_video->get_video_stream_index() != -1) 
	{
        if(add_video_stream(input_video,&video_st,ofmt_ctx,&vCodec, AV_CODEC_ID_H264,AVMEDIA_TYPE_VIDEO,video))
		{
			video_stream_index = input_video->get_video_stream_index();
		}
		else
		{
			return 0;
		}
	}
    if (fmt->audio_codec != AV_CODEC_ID_NONE && input_video->get_audio_stream_index() != -1 )
	{
        if(add_audio_stream(input_video,&audio_st, ofmt_ctx, &aCodec, AV_CODEC_ID_AAC,AVMEDIA_TYPE_AUDIO,audio))
		{
			audio_stream_index = input_video->get_audio_stream_index();
		}
		else
		{
			return 0;
		}
    }
    if (video_stream_index != -1)
	{
		if(!open_video(input_video,ofmt_ctx, vCodec, &video_st, opt))
		{
			return 0;
		}
	}
    if (audio_stream_index != -1)
	{
		if(!open_audio(input_video,ofmt_ctx, aCodec, &audio_st, opt))
		{
			return 0;
		}
		if(!init_fifo(&(fifo), audio_st.st->codec))
		{
			return 0;
		}
	}
    av_dump_format(ofmt_ctx, 0, filename, 1);
    /* open the output file, if needed */
    if (!(fmt->flags & AVFMT_NOFILE)) {
        ret = avio_open(&(ofmt_ctx->pb), filename, AVIO_FLAG_WRITE);
        if (ret < 0) 
		{
			ERROR("Could not open '%s': %s", filename,av_err2str(ret));
            return 0;
        }
    }
    /* Write the stream header, if any. */
    ret = avformat_write_header(ofmt_ctx, &(opt));
    if (ret < 0) {
        ERROR("Error occurred when opening output file: %s",av_err2str(ret));
        return 0;
    }	
    return 1;
}

int CTS_encode::enc_write_frame(CTS_decode *input_video,AVFrame *frame,int type)
{
	AVFrame *frame_filt = NULL;
	int ret = 0;
	if(type == video_stream_index)
	{
		if(frame->width > 0 && frame->height > 0)
		{
			frame_filt = get_video_frame(input_video,&video_st,frame);
			if(frame_filt == NULL)
			{
				return ret;
			}
			ret = write_video_frame(ofmt_ctx, &video_st,frame_filt);
		} 
	}else if(type == audio_stream_index){
		const int output_frame_size = audio_st.st->codec->frame_size;
        int finished                = 0;
		while (av_audio_fifo_size(fifo) < output_frame_size) {
			if (!read_decode_convert_and_store(fifo, frame,
				audio_st.st->codec,
				audio_st.swr_ctx, &finished))
				return ret;
			if (finished)
                break;
        }
		while (av_audio_fifo_size(fifo) >= output_frame_size ||
			(finished && av_audio_fifo_size(fifo) > 0))
			if (!load_encode_and_write(fifo,ofmt_ctx,&audio_st))
				return ret;
		ret = 1;
	}else
		return ret;
	return ret;
}

static int encode_write_frame(AVFrame *filt_frame,AVFormatContext *ofmt_ctx, unsigned int stream_index, int *got_frame) {
    int ret;
    int got_frame_local;
    AVPacket enc_pkt;
    int (*enc_func)(AVCodecContext *, AVPacket *, const AVFrame *, int *) =
        (ofmt_ctx->streams[stream_index]->codec->codec_type ==
         AVMEDIA_TYPE_VIDEO) ? avcodec_encode_video2 : avcodec_encode_audio2;

    if (!got_frame)
        got_frame = &got_frame_local;
    /* encode filtered frame */
    enc_pkt.data = NULL;
    enc_pkt.size = 0;
    av_init_packet(&enc_pkt);
    ret = enc_func(ofmt_ctx->streams[stream_index]->codec, &enc_pkt,
            filt_frame, got_frame);
    av_frame_free(&filt_frame);
    if (ret < 0)
        return ret;
    if (!(*got_frame))
        return 0;

    /* prepare packet for muxing */
    enc_pkt.stream_index = stream_index;
    av_packet_rescale_ts(&enc_pkt,
                         ofmt_ctx->streams[stream_index]->codec->time_base,
                         ofmt_ctx->streams[stream_index]->time_base);
    /* mux encoded frame */
    ret = av_interleaved_write_frame(ofmt_ctx, &enc_pkt);
    return ret;
}

int CTS_encode::flush_encoder(AVFormatContext *ofmt_ctx,unsigned int stream_index)
{
    int ret = 0;
    int got_frame;
	if (!(ofmt_ctx->streams[stream_index]->codec->codec->capabilities &
                CODEC_CAP_DELAY))
        return 0;
    while (1) {
		ret = encode_write_frame(NULL,ofmt_ctx,stream_index, &got_frame);
        if (ret < 0)
            break;
        if (!got_frame)
            return 0;
    }
    return ret;
}

CTS_encode::CTS_encode():video_st(),audio_st(),ofmt_ctx(NULL),fmt(NULL),vCodec(NULL),aCodec(NULL),video_stream_index(-1),audio_stream_index(-1),opt(NULL),fifo(NULL)
{
}

CTS_encode::CTS_encode(const CTS_encode &tmp):video_st(tmp.video_st),audio_st(tmp.audio_st),ofmt_ctx(tmp.ofmt_ctx),fmt(tmp.fmt),vCodec(tmp.vCodec),aCodec(tmp.aCodec),video_stream_index(tmp.video_stream_index),audio_stream_index(tmp.audio_stream_index),opt(tmp.opt),fifo(tmp.fifo)
{
}
CTS_encode &CTS_encode::operator=(const CTS_encode &tmp)
{
	video_st 				= tmp.video_st;
	audio_st 				= tmp.audio_st;
	ofmt_ctx				= tmp.ofmt_ctx;
	fmt						= tmp.fmt;
	vCodec					= tmp.vCodec;
	aCodec					= tmp.aCodec;
	video_stream_index		= tmp.video_stream_index;
	audio_stream_index		= tmp.audio_stream_index;
	opt						= tmp.opt;	
	fifo					= tmp.fifo;
	return *this;
}

CTS_encode::~CTS_encode()
{
	destroy();
}
int CTS_encode::destroy()
{	
	//
    /* Close each codec. */
    if (audio_stream_index != -1)
	{
		audio_st.destroy();
		av_audio_fifo_free(fifo);
	}
	if(video_stream_index != -1)
	{
		video_st.destroy();
	}
    if (!(fmt->flags & AVFMT_NOFILE))
        /* Close the output file. */
        avio_closep(&ofmt_ctx->pb);
    /* free the stream */
    avformat_free_context(ofmt_ctx);
	return 0;
}
AVFormatContext* CTS_encode::enc_get_fmt(void)
{
	return ofmt_ctx;
}
