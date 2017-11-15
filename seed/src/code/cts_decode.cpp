#include "code/cts_decode.h"
#include "util/cts_log.h"
#include <iostream>
using namespace std;



CTS_decode::CTS_decode():ifmt_ctx(NULL),video_stream_index(-1),audio_stream_index(-1),dec_frame(NULL),vCodecCtx(NULL)
	,aCodecCtx(NULL),vCodec(NULL),aCodec(NULL),duration(-1),bitrate(-1),rotate(0)
{
}
CTS_decode::CTS_decode(const CTS_decode &tmp):ifmt_ctx(tmp.ifmt_ctx),video_stream_index(tmp.video_stream_index),audio_stream_index(tmp.audio_stream_index),dec_frame(tmp.dec_frame)
	,vCodecCtx(tmp.vCodecCtx),aCodecCtx(tmp.aCodecCtx),vCodec(tmp.vCodec),aCodec(tmp.aCodec),duration(tmp.duration),bitrate(tmp.bitrate),rotate(tmp.rotate)
{}
CTS_decode &CTS_decode::operator=(const CTS_decode &tmp)
{
	ifmt_ctx = tmp.ifmt_ctx;
	video_stream_index = tmp.video_stream_index;
	audio_stream_index = tmp.audio_stream_index;
	dec_frame = tmp.dec_frame;
	vCodecCtx = tmp.vCodecCtx;
	aCodecCtx = tmp.aCodecCtx;
	vCodec = tmp.vCodec;
	aCodec = tmp.aCodec;
	duration = tmp.duration;
	bitrate = tmp.bitrate;
	rotate = tmp.rotate;
	return *this;
}
CTS_decode::~CTS_decode()
{
	destroy();
}
int CTS_decode::destroy()
{	
	if(video_stream_index != -1)
	{
		avcodec_close(vCodecCtx);
		vCodecCtx			= NULL;
		vCodec				= NULL;
	}
	if(audio_stream_index != -1)
	{
		avcodec_close(aCodecCtx);
		aCodecCtx			= NULL;
		aCodec				= NULL;
	}
	avformat_close_input(&ifmt_ctx);
	return 0;
}
int CTS_decode::dec_init(const char *filename)
{
	int ret;
	avcodec_register_all();
	av_register_all();
	avformat_network_init();
	ret = avformat_open_input(&ifmt_ctx, filename, NULL, NULL);
	if (ret < 0)
	{
		ERROR("av_format_open_input() - [%s]", filename);
		return 0;
	}
	ret = avformat_find_stream_info(ifmt_ctx, NULL);
	if (ret < 0) 
	{
		ERROR("could not find codec parameters - [%s]", filename);
		avformat_close_input(&ifmt_ctx);
		return 0;
	}
	unsigned int i = 0;
	for(i = 0;i < ifmt_ctx->nb_streams;i++)
	{
		if(ifmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			bitrate = ifmt_ctx->streams[i]->r_frame_rate.num/ifmt_ctx->streams[i]->r_frame_rate.den;	
			video_stream_index = i;
		}
		else if(ifmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			audio_stream_index = i;
		}
	}
	if(video_stream_index != -1)
	{
		vCodecCtx = ifmt_ctx->streams[video_stream_index]->codec;
		vCodec = avcodec_find_decoder(vCodecCtx->codec_id);
		if(vCodec == NULL)
		{
			ERROR("could not find video decoder - [%s]", filename);
			return 0;
		}
	}
	else
	{}
	if(audio_stream_index != -1)
	{
		aCodecCtx = ifmt_ctx->streams[audio_stream_index]->codec;
		aCodec = avcodec_find_decoder(aCodecCtx->codec_id);
		if(aCodec == NULL)
		{
			ERROR("could not find audio decoder - [%s]", filename);
			return 0;
		}
	}
	else
	{}
	if(video_stream_index != -1)
	{
		if(avcodec_open2(vCodecCtx,vCodec,NULL) < 0)
		{
			ERROR("video avcodec_open2 error - [%s]", filename);
			return 0;
		}
	}
	else
	{}
	if(audio_stream_index != -1)
	{
		if(avcodec_open2(aCodecCtx,aCodec,NULL) < 0)
		{
			ERROR("audio avcodec_open2 error - [%s]", filename);
			return 0;
		}
	}
	else
	{}
	av_dump_format(ifmt_ctx, 0, filename, 0);
	
	AVDictionaryEntry *tag = NULL;
	tag = av_dict_get(ifmt_ctx->streams[video_stream_index]->metadata, "rotate", tag, 0);
	if (tag == NULL)
	{  
		INFO("rotate %d",0);  
	}
	else
	{ 
    	int angle = atoi(tag->value);  
    	angle %= 360;
		rotate = angle;
		INFO("rotate %d",angle);
	}	
	if (ifmt_ctx->duration != AV_NOPTS_VALUE) 
	{
		int hours, mins, secs, us; 
		secs = ifmt_ctx->duration / AV_TIME_BASE;
		INFO("%s duration : %ld",filename,ifmt_ctx->duration);
		duration = secs*bitrate;
		us = ifmt_ctx->duration % AV_TIME_BASE; 
		mins = secs / 60; 
		secs %= 60; 
		hours = mins / 60; 
		mins %= 60; 
		INFO( "%s time : %02d:%02d:%02d.%02d", filename,hours, mins, secs, (100 * us) / AV_TIME_BASE); 	
	} 
	else 
	{ 
		INFO( "%s time : N/A",filename); 
	}
	return 1;
}


int CTS_decode::dec_get_frame(AVFrame **frame, int *got, int *type)
{
	AVPacket packet;
	av_init_packet(&packet);
	int ret = 0;
	if((ret = av_read_frame(ifmt_ctx,&packet)) < 0)
	{
		av_free_packet(&packet);
		return ret;
	}
	if(packet.stream_index == video_stream_index)
	{
		av_packet_rescale_ts(&packet,
			ifmt_ctx->streams[video_stream_index]->time_base, 
			ifmt_ctx->streams[video_stream_index]->codec->time_base);
		ret = avcodec_decode_video2(vCodecCtx,*frame,got,&packet);
	}
	else if(packet.stream_index == audio_stream_index)
	{
		av_packet_rescale_ts(&packet,
			ifmt_ctx->streams[audio_stream_index]->time_base,
			ifmt_ctx->streams[audio_stream_index]->codec->time_base);
		ret = avcodec_decode_audio4(aCodecCtx,*frame,got,&packet);
	}
	*type = packet.stream_index;
	av_free_packet(&packet);
	return ret;
}
const AVFormatContext* CTS_decode::dec_get_fmt(void)
{
	return CTS_decode::ifmt_ctx;
}
int CTS_decode::dec_get_height(void)
{
	return vCodecCtx->height;
}
int CTS_decode::dec_get_width(void)
{
	return vCodecCtx->width;
}
AVStream *CTS_decode::get_type(const int type)
{
	if(type == AVMEDIA_TYPE_VIDEO)
	{
		return ifmt_ctx->streams[video_stream_index];
	}
	else if(type == AVMEDIA_TYPE_AUDIO)
	{
		return ifmt_ctx->streams[audio_stream_index];
	}
	return NULL;	
}
int CTS_decode::get_video_stream_index()
{
	return video_stream_index;
}
int CTS_decode::get_audio_stream_index()
{
	return audio_stream_index;
}
AVCodecContext *CTS_decode::get_CodecCtx(const int type)
{
	if(type == AVMEDIA_TYPE_VIDEO)
	{
		return vCodecCtx;
	}
	else if(type == AVMEDIA_TYPE_AUDIO)
	{
		return aCodecCtx;
	}
	return NULL;
}
int CTS_decode::get_duration()
{
	return duration;
}
int CTS_decode::get_bitrate()
{
	return bitrate;
}
int CTS_decode::get_rotate()
{
	return rotate;
}