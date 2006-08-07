#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>

#include <x264.h>

#include "private.h"
#include "private_x264.h"
#include "matroska.h"
#include "encode.h"


typedef void *hnd_t;


#define DATA_MAX 3000000
uint8_t data[DATA_MAX];

int64_t x264_mdate (void);


/* input interface */
static int p_open_infile     (Avs3_Data *data, x264_param_t *p_param);
static int p_get_frame_total (Avs3_Data *data);
static int p_read_frame      (Avs3_Data *data, x264_picture_t *p_pic, int i_frame);

/* output interface */
static int open_file_bsf( char *psz_filename, hnd_t *p_handle );
static int set_param_bsf( hnd_t handle, x264_param_t *p_param );
static int write_nalu_bsf( hnd_t handle, uint8_t *p_nalu, int i_size );
static int set_eop_bsf( hnd_t handle,  x264_picture_t *p_picture );
static int close_file_bsf( hnd_t handle );

static int open_file_mkv( char *psz_filename, hnd_t *p_handle );
static int set_param_mkv( hnd_t handle, x264_param_t *p_param );
static int write_nalu_mkv( hnd_t handle, uint8_t *p_nalu, int i_size );
static int set_eop_mkv( hnd_t handle, x264_picture_t *p_picture );
static int close_file_mkv( hnd_t handle );

#ifdef MP4_OUTPUT
static int open_file_mp4( char *psz_filename, hnd_t *p_handle );
static int set_param_mp4( hnd_t handle, x264_param_t *p_param );
static int write_nalu_mp4( hnd_t handle, uint8_t *p_nalu, int i_size );
static int set_eop_mp4( hnd_t handle, x264_picture_t *p_picture );
static int close_file_mp4( hnd_t handle );
#endif /* MP4_OUTPUT */

static int (*p_open_outfile)      (char *filename, void **handle);
static int (*p_set_outfile_param) (void *handle, x264_param_t *p_param);
static int (*p_write_nalu)        (void *handle, uint8_t *p_nal, int i_size);
static int (*p_set_eop)           (void *handle, x264_picture_t *p_picture);
static int (*p_close_outfile)     (void *handle);

static int _set_drivers  (gint out_container);
static int _encode_frame (x264_t *h, void *handle, x264_picture_t *pic);


void
avs3_encode (Avs3_Thread_Data *thread_data)
{
  GIOStatus       status;
  gsize           size;
  Avs3_Pipe_Data  pipe_data;
  x264_param_t   *param;
  x264_picture_t  pic;
  x264_t         *h;
  hnd_t           hout;
  int             i_frame;
  int             i_frame_total;
  int64_t         i_start;
  int64_t         i_end;
  int64_t         i_file;
  int             i_frame_size;
  int             i_progress;
  int             err;

  param = thread_data->param;
  err = _set_drivers (thread_data->container);
  if (err < 0) {
    GtkWidget *no_driver;

    no_driver = gtk_message_dialog_new (GTK_WINDOW(thread_data->dialog),
                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                        GTK_MESSAGE_ERROR,
                                        GTK_BUTTONS_CLOSE,
                                        "Error: unknown output file type");
    gtk_dialog_run (GTK_DIALOG (no_driver));
    gtk_widget_destroy (no_driver);

    return;
  }

  if (p_open_infile (thread_data->data, param)) {
    GtkWidget *no_driver;

    no_driver = gtk_message_dialog_new (GTK_WINDOW(thread_data->dialog),
                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                        GTK_MESSAGE_ERROR,
                                        GTK_BUTTONS_CLOSE,
                                        "Error: unsupported colorspace");
    gtk_dialog_run (GTK_DIALOG (no_driver));
    gtk_widget_destroy (no_driver);

    return;
  }

  p_open_outfile ((char *)thread_data->file_output, &hout);

  i_frame_total = p_get_frame_total (thread_data->data);
  if (((i_frame_total == 0) || (param->i_frame_total < i_frame_total)) &&
      (param->i_frame_total > 0))
    i_frame_total = param->i_frame_total;
  param->i_frame_total = i_frame_total;

  if ((h = x264_encoder_open (param)) == NULL) {
    fprintf (stderr, "x264_encoder_open failed\n");
    p_close_outfile (hout);
    g_free (param);

    return;
  }

  if (p_set_outfile_param (hout, param)) {
    fprintf (stderr, "can't set outfile param\n");
    p_close_outfile (hout);
    g_free (param);

    return;
  }

  /* Create a new pic */
  x264_picture_alloc (&pic, X264_CSP_I420, param->i_width, param->i_height );

  i_start = x264_mdate();

  /* Encode frames */
  for (i_frame = 0, i_file = 0, i_progress = 0;
       ((i_frame < i_frame_total) || (i_frame_total == 0)); ) {
    if (p_read_frame (thread_data->data, &pic, i_frame))
      break;

    pic.i_pts = (int64_t)i_frame * param->i_fps_den;

    i_file += _encode_frame (h, hout, &pic);

    i_frame++;

    /* update status line (up to 1000 times per input file) */
    if (param->i_log_level < X264_LOG_DEBUG &&
        (i_frame_total ? i_frame * 1000 / i_frame_total > i_progress
         : i_frame % 10 == 0)) {
      int64_t i_elapsed = x264_mdate () - i_start;

      if (i_frame_total) {
        pipe_data.frame = i_frame;
        pipe_data.frame_total = i_frame_total;
        pipe_data.file = i_file;
        pipe_data.elapsed = i_elapsed;
        status = g_io_channel_write_chars (thread_data->io_write,
                                           (const gchar *)&pipe_data,
                                           sizeof (Avs3_Pipe_Data),
                                           &size, NULL);
        if (status != G_IO_STATUS_NORMAL) {
          g_print ("Error ! %d %d %d\n", status, sizeof (Avs3_Pipe_Data), size);
        }
        else {
          /* we force the GIOChannel to write to the pipeline */
          status = g_io_channel_flush (thread_data->io_write,
                                       NULL);
          if (status != G_IO_STATUS_NORMAL) {
            g_print ("Error ! %d\n", status);
          }
        }
      }
    }
  }
  /* Flush delayed B-frames */
  do {
    i_file += i_frame_size = _encode_frame (h, hout, NULL);
  } while (i_frame_size);

  i_end = x264_mdate ();
  memset (&pic, 0, sizeof (x264_picture_t));
  x264_encoder_close (h);
  fprintf (stderr, "\n");

  p_close_outfile (hout);

  if (i_frame > 0) {
    double fps = (double)i_frame * (double)1000000 /
      (double)(i_end - i_start);

    fprintf (stderr, "encoded %d frames, %.2f fps, %.2f kb/s\n",
             i_frame, fps,
             (double) i_file * 8 * param->i_fps_num /
             ((double) param->i_fps_den * i_frame * 1000));
  }

  gtk_widget_set_sensitive (thread_data->end_button, TRUE);
  gtk_widget_hide (thread_data->button);

  return;
}


static int
p_open_infile (Avs3_Data *data, x264_param_t *p_param)
{
  AVS_VideoInfo  *info;
  AVS_ColorSpace *csp;
  ColorSpace_Id  csp_id;

  if (!data->env)
    data->env = avs_environment_new (10000000);
  data->clip = avs_clip_new_from_file (data->filename, data->env);

  info = avs_clip_videoinfo_get (data->clip);
  if (!avs_videoinfo_has_video (info)) {
    fprintf (stderr, "avs3 [error]: no video\n");

    return -1;
  }

  /* check input format */
  csp = avs_videoinfo_colorspace_get (info);
  csp_id = avs_colorspace_id_get (csp);
  if (csp_id != I_YV12) {
    char *csp_name;
    switch (csp_id) {
    case I_EXTERNAL:
      csp_name = "external";
    case I_RGB24:
      csp_name = "RGB24";
    case I_RGB32:
      csp_name = "RGB32";
    case I_YUY2:
      csp_name = "YUY2";
    case I_YV24:
      csp_name = "YV24";
    default:
      csp_name = "unknown";
    }
    fprintf (stderr, "avs3 [error]: unsupported input format (%s)\n", csp_name);

    return -1;
  }

  fprintf (stderr, "avs3 [info]: %dx%d @ %.2f fps (%d frames)\n",
           p_param->i_width, p_param->i_height,
           (double)p_param->i_fps_num / (double)p_param->i_fps_den,
           (int)avs_videoinfo_framecount_get (info));

  avs_colorspace_delete (csp);
  avs_videoinfo_delete (info);

  return 0;
}

static int
p_get_frame_total (Avs3_Data *data)
{
  AVS_VideoInfo *info;
  int            frame_count;

  info = avs_clip_videoinfo_get (data->clip);
  frame_count = avs_videoinfo_framecount_get (info);
  avs_videoinfo_delete (info);

  return frame_count;
}

static int
p_read_frame (Avs3_Data *data, x264_picture_t *p_pic, int i_frame)
{
  AVS_VideoFrame      *frame;

  frame = avs_clip_videoframe_get (data->clip, i_frame);
  if (!frame)
    return -1;

  p_pic->img.i_csp = X264_CSP_YV12;
  p_pic->img.plane[0] = (uint8_t *)avs_videoframe_plane_get (frame, 'Y');
  p_pic->img.plane[1] = (uint8_t *)avs_videoframe_plane_get (frame, 'U');
  p_pic->img.plane[2] = (uint8_t *)avs_videoframe_plane_get (frame, 'V');

  avs_videoframe_delete (frame);

  return 0;
}

static int
_set_drivers (gint container)
{
  switch (container) {
  case 0:
/*     Raw ES output file driver */
    p_open_outfile = open_file_bsf;
    p_set_outfile_param = set_param_bsf;
    p_write_nalu = write_nalu_bsf;
    p_set_eop = set_eop_bsf;
    p_close_outfile = close_file_bsf;
    break;
  case 1:
/*     Matroska output file driver */
    p_open_outfile = open_file_mkv;
    p_set_outfile_param = set_param_mkv;
    p_write_nalu = write_nalu_mkv;
    p_set_eop = set_eop_mkv;
    p_close_outfile = close_file_mkv;
    break;
#ifdef MP4_OUTPUT
  case 2:
    p_open_outfile = open_file_mp4;
    p_set_outfile_param = set_param_mp4;
    p_write_nalu = write_nalu_mp4;
    p_set_eop = set_eop_mp4;
    p_close_outfile = close_file_mp4;
    break;
#endif /* MP4_OUTPUT */
  default:
    return -1;
  }

  return 1;
}

static int
_encode_frame (x264_t *h, void *handle, x264_picture_t *pic)
{
  x264_picture_t pic_out;
  x264_nal_t    *nal;
  int            i_nal;
  int            i;
  int            i_file = 0;

  /* Do not force any parameters */
  if (pic)
    {
      pic->i_type = X264_TYPE_AUTO;
      pic->i_qpplus1 = 0;
    }
  if (x264_encoder_encode (h, &nal, &i_nal, pic, &pic_out) < 0)
    {
      fprintf (stderr, "x264_encoder_encode failed\n");
    }

  for (i = 0; i < i_nal; i++)
    {
      int i_size;
      int i_data;

      i_data = DATA_MAX;
      if ((i_size = x264_nal_encode (data, &i_data, 1, &nal[i])) > 0 )
        {
          i_file += p_write_nalu (handle, data, i_size);
        }
      else if (i_size < 0)
        {
          fprintf (stderr, "need to increase buffer size (size=%d)\n", -i_size);
        }
    }
  if (i_nal)
    p_set_eop (handle, &pic_out);

  return i_file;
}

/* -- bsf muxing support ------------------------------------------------- */
static int open_file_bsf( char *psz_filename, hnd_t *p_handle )
{
    if ((*p_handle = fopen(psz_filename, "w+b")) == NULL)
        return -1;

    return 0;
}

static int set_param_bsf( hnd_t handle, x264_param_t *p_param )
{
    return 0;
}

static int write_nalu_bsf( hnd_t handle, uint8_t *p_nalu, int i_size )
{
    if (fwrite(p_nalu, (size_t)i_size, 1, (FILE *)handle) > 0)
        return i_size;
    return -1;
}

static int set_eop_bsf( hnd_t handle,  x264_picture_t *p_picture )
{
    return 0;
}

static int close_file_bsf( hnd_t handle )
{
    if ((handle == NULL) || (handle == stdout))
        return 0;

    return fclose((FILE *)handle);
}

/* -- mkv muxing support ------------------------------------------------- */
typedef struct
{
    mk_Writer *w;

    uint8_t   *sps, *pps;
    int       sps_len, pps_len;

    int       width, height, d_width, d_height;

    int64_t   frame_duration;
    int       fps_num;

    int       b_header_written;
    char      b_writing_frame;
} mkv_t;

static int write_header_mkv( mkv_t *p_mkv )
{
    int       ret;
    uint8_t   *avcC;
    int  avcC_len;

    if( p_mkv->sps == NULL || p_mkv->pps == NULL ||
        p_mkv->width == 0 || p_mkv->height == 0 ||
        p_mkv->d_width == 0 || p_mkv->d_height == 0)
        return -1;

    avcC_len = 5 + 1 + 2 + p_mkv->sps_len + 1 + 2 + p_mkv->pps_len;
    avcC = malloc(avcC_len);
    if (avcC == NULL)
        return -1;

    avcC[0] = 1;
    avcC[1] = p_mkv->sps[1];
    avcC[2] = p_mkv->sps[2];
    avcC[3] = p_mkv->sps[3];
    avcC[4] = 0xff; /* nalu size length is four bytes */
    avcC[5] = 0xe1; /* one sps */

    avcC[6] = p_mkv->sps_len >> 8;
    avcC[7] = p_mkv->sps_len;

    memcpy(avcC+8, p_mkv->sps, p_mkv->sps_len);

    avcC[8+p_mkv->sps_len] = 1; /* one pps */
    avcC[9+p_mkv->sps_len] = p_mkv->pps_len >> 8;
    avcC[10+p_mkv->sps_len] = p_mkv->pps_len;

    memcpy( avcC+11+p_mkv->sps_len, p_mkv->pps, p_mkv->pps_len );

    ret = mk_writeHeader( p_mkv->w, "x264", "V_MPEG4/ISO/AVC",
                          avcC, avcC_len, p_mkv->frame_duration, 50000,
                          p_mkv->width, p_mkv->height,
                          p_mkv->d_width, p_mkv->d_height );

    free( avcC );

    p_mkv->b_header_written = 1;

    return ret;
}

static int open_file_mkv( char *psz_filename, hnd_t *p_handle )
{
    mkv_t *p_mkv;

    *p_handle = NULL;

    p_mkv  = malloc(sizeof(*p_mkv));
    if (p_mkv == NULL)
        return -1;

    memset(p_mkv, 0, sizeof(*p_mkv));

    p_mkv->w = mk_createWriter(psz_filename);
    if (p_mkv->w == NULL)
    {
        free(p_mkv);
        return -1;
    }

    *p_handle = p_mkv;

    return 0;
}

static int set_param_mkv( hnd_t handle, x264_param_t *p_param )
{
    mkv_t   *p_mkv = handle;
    int64_t dw, dh;

    if( p_param->i_fps_num > 0 )
    {
        p_mkv->frame_duration = (int64_t)p_param->i_fps_den *
                                (int64_t)1000000000 / p_param->i_fps_num;
        p_mkv->fps_num = p_param->i_fps_num;
    }
    else
    {
        p_mkv->frame_duration = 0;
        p_mkv->fps_num = 1;
    }

    p_mkv->width = p_param->i_width;
    p_mkv->height = p_param->i_height;

    if( p_param->vui.i_sar_width && p_param->vui.i_sar_height )
    {
        dw = (int64_t)p_param->i_width  * p_param->vui.i_sar_width;
        dh = (int64_t)p_param->i_height * p_param->vui.i_sar_height;
    }
    else
    {
        dw = p_param->i_width;
        dh = p_param->i_height;
    }

    if( dw > 0 && dh > 0 )
    {
        int64_t a = dw, b = dh;

        for (;;)
        {
            int64_t c = a % b;
            if( c == 0 )
              break;
            a = b;
            b = c;
        }

        dw /= b;
        dh /= b;
    }

    p_mkv->d_width = (int)dw;
    p_mkv->d_height = (int)dh;

    return 0;
}

static int write_nalu_mkv( hnd_t handle, uint8_t *p_nalu, int i_size )
{
    mkv_t *p_mkv = handle;
    uint8_t type = p_nalu[4] & 0x1f;
    uint8_t dsize[4];
    int psize;

    switch( type )
    {
    /* sps */
    case 0x07:
        if( !p_mkv->sps )
        {
            p_mkv->sps = malloc(i_size - 4);
            if (p_mkv->sps == NULL)
                return -1;
            p_mkv->sps_len = i_size - 4;
            memcpy(p_mkv->sps, p_nalu + 4, i_size - 4);
        }
        break;

    /* pps */
    case 0x08:
        if( !p_mkv->pps )
        {
            p_mkv->pps = malloc(i_size - 4);
            if (p_mkv->pps == NULL)
                return -1;
            p_mkv->pps_len = i_size - 4;
            memcpy(p_mkv->pps, p_nalu + 4, i_size - 4);
        }
        break;

    /* slice, sei */
    case 0x1:
    case 0x5:
    case 0x6:
        if( !p_mkv->b_writing_frame )
        {
            if( mk_startFrame(p_mkv->w) < 0 )
                return -1;
            p_mkv->b_writing_frame = 1;
        }
        psize = i_size - 4 ;
        dsize[0] = psize >> 24;
        dsize[1] = psize >> 16;
        dsize[2] = psize >> 8;
        dsize[3] = psize;
        if( mk_addFrameData(p_mkv->w, dsize, 4) < 0 ||
            mk_addFrameData(p_mkv->w, p_nalu + 4, i_size - 4) < 0 )
            return -1;
        break;

    default:
        break;
    }

    if( !p_mkv->b_header_written && p_mkv->pps && p_mkv->sps &&
        write_header_mkv(p_mkv) < 0 )
        return -1;

    return i_size;
}

static int set_eop_mkv( hnd_t handle, x264_picture_t *p_picture )
{
    mkv_t *p_mkv = handle;
    int64_t i_stamp = (int64_t)(p_picture->i_pts * 1e9 / p_mkv->fps_num);

    p_mkv->b_writing_frame = 0;

    return mk_setFrameFlags( p_mkv->w, i_stamp,
                             p_picture->i_type == X264_TYPE_IDR );
}

static int close_file_mkv( hnd_t handle )
{
    mkv_t *p_mkv = handle;
    int   ret;

    if( p_mkv->sps )
        free( p_mkv->sps );
    if( p_mkv->pps )
        free( p_mkv->pps );

    ret = mk_close(p_mkv->w);

    free( p_mkv );

    return ret;
}

/* -- mp4 muxing support ------------------------------------------------- */
#ifdef MP4_OUTPUT

typedef struct
{
    GF_ISOFile *p_file;
    GF_AVCConfig *p_config;
    GF_ISOSample *p_sample;
    int i_track;
    uint32_t i_descidx;
    int i_time_inc;
    int i_time_res;
    int i_numframe;
    int i_init_delay;
    uint8_t b_sps;
    uint8_t b_pps;
} mp4_t;


static void recompute_bitrate_mp4(GF_ISOFile *p_file, int i_track)
{
    u32 i, count, di, timescale, time_wnd, rate;
    u64 offset;
    Double br;
    GF_ESD *esd;

    esd = gf_isom_get_esd(p_file, i_track, 1);
    if (!esd) return;

    esd->decoderConfig->avgBitrate = 0;
    esd->decoderConfig->maxBitrate = 0;
    rate = time_wnd = 0;

    timescale = gf_isom_get_media_timescale(p_file, i_track);
    count = gf_isom_get_sample_count(p_file, i_track);
    for (i=0; i<count; i++) {
        GF_ISOSample *samp = gf_isom_get_sample_info(p_file, i_track, i+1, &di, &offset);

        if (samp->dataLength>esd->decoderConfig->bufferSizeDB) esd->decoderConfig->bufferSizeDB = samp->dataLength;

        if (esd->decoderConfig->bufferSizeDB < samp->dataLength) esd->decoderConfig->bufferSizeDB = samp->dataLength;
        esd->decoderConfig->avgBitrate += samp->dataLength;
        rate += samp->dataLength;
        if (samp->DTS > time_wnd + timescale) {
            if (rate > esd->decoderConfig->maxBitrate) esd->decoderConfig->maxBitrate = rate;
            time_wnd = samp->DTS;
            rate = 0;
        }

        gf_isom_sample_del(&samp);
    }

    br = (Double) (s64) gf_isom_get_media_duration(p_file, i_track);
    br /= timescale;
    esd->decoderConfig->avgBitrate = (u32) (esd->decoderConfig->avgBitrate / br);
    /*move to bps*/
    esd->decoderConfig->avgBitrate *= 8;
    esd->decoderConfig->maxBitrate *= 8;

    gf_isom_change_mpeg4_description(p_file, i_track, 1, esd);
    gf_odf_desc_del((GF_Descriptor *) esd);
}


static int close_file_mp4( hnd_t handle )
{
    mp4_t *p_mp4 = (mp4_t *)handle;

    if (p_mp4 == NULL)
        return 0;

    if (p_mp4->p_config)
        gf_odf_avc_cfg_del(p_mp4->p_config);

    if (p_mp4->p_sample)
    {
        if (p_mp4->p_sample->data)
            free(p_mp4->p_sample->data);

        gf_isom_sample_del(&p_mp4->p_sample);
    }

    if (p_mp4->p_file)
    {
        recompute_bitrate_mp4(p_mp4->p_file, p_mp4->i_track);
        gf_isom_set_pl_indication(p_mp4->p_file, GF_ISOM_PL_VISUAL, 0x15);
        gf_isom_set_storage_mode(p_mp4->p_file, GF_ISOM_STORE_FLAT);
        gf_isom_close(p_mp4->p_file);
    }

    free(p_mp4);

    return 0;
}

static int open_file_mp4( char *psz_filename, hnd_t *p_handle )
{
    mp4_t *p_mp4;

    *p_handle = NULL;

    if ((p_mp4 = (mp4_t *)malloc(sizeof(mp4_t))) == NULL)
        return -1;

    memset(p_mp4, 0, sizeof(mp4_t));
    p_mp4->p_file = gf_isom_open(psz_filename, GF_ISOM_OPEN_WRITE, NULL);

    if ((p_mp4->p_sample = gf_isom_sample_new()) == NULL)
    {
        close_file_mp4( p_mp4 );
        return -1;
    }

    gf_isom_set_brand_info(p_mp4->p_file, GF_ISOM_BRAND_AVC1, 0);

    *p_handle = p_mp4;

    return 0;
}


static int set_param_mp4( hnd_t handle, x264_param_t *p_param )
{
    mp4_t *p_mp4 = (mp4_t *)handle;

    p_mp4->i_track = gf_isom_new_track(p_mp4->p_file, 0, GF_ISOM_MEDIA_VISUAL,
        p_param->i_fps_num);

    p_mp4->p_config = gf_odf_avc_cfg_new();
    gf_isom_avc_config_new(p_mp4->p_file, p_mp4->i_track, p_mp4->p_config,
        NULL, NULL, &p_mp4->i_descidx);

    gf_isom_set_track_enabled(p_mp4->p_file, p_mp4->i_track, 1);

    gf_isom_set_visual_info(p_mp4->p_file, p_mp4->i_track, p_mp4->i_descidx,
        p_param->i_width, p_param->i_height);

    p_mp4->p_sample->data = (char *)malloc(p_param->i_width * p_param->i_height * 3 / 2);
    if (p_mp4->p_sample->data == NULL)
        return -1;

    p_mp4->i_time_res = p_param->i_fps_num;
    p_mp4->i_time_inc = p_param->i_fps_den;
    p_mp4->i_init_delay = p_param->i_bframe ? (p_param->b_bframe_pyramid ? 2 : 1) : 0;
    p_mp4->i_init_delay *= p_mp4->i_time_inc;
    fprintf(stderr, "mp4 [info]: initial delay %d (scale %d)\n",
        p_mp4->i_init_delay, p_mp4->i_time_res);

    return 0;
}


static int write_nalu_mp4( hnd_t handle, uint8_t *p_nalu, int i_size )
{
    mp4_t *p_mp4 = (mp4_t *)handle;
    GF_AVCConfigSlot *p_slot;
    uint8_t type = p_nalu[4] & 0x1f;
    int psize;

    switch(type)
    {
    /* sps */
    case 0x07:
        if (!p_mp4->b_sps)
        {
            p_mp4->p_config->configurationVersion = 1;
            p_mp4->p_config->AVCProfileIndication = p_nalu[5];
            p_mp4->p_config->profile_compatibility = p_nalu[6];
            p_mp4->p_config->AVCLevelIndication = p_nalu[7];
            p_slot = (GF_AVCConfigSlot *)malloc(sizeof(GF_AVCConfigSlot));
            p_slot->size = i_size - 4;
            p_slot->data = (char *)malloc(p_slot->size);
            memcpy(p_slot->data, p_nalu + 4, i_size - 4);
            gf_list_add(p_mp4->p_config->sequenceParameterSets, p_slot);
            p_slot = NULL;
            p_mp4->b_sps = 1;
        }
        break;

    /* pps */
    case 0x08:
        if (!p_mp4->b_pps)
        {
            p_slot = (GF_AVCConfigSlot *)malloc(sizeof(GF_AVCConfigSlot));
            p_slot->size = i_size - 4;
            p_slot->data = (char *)malloc(p_slot->size);
            memcpy(p_slot->data, p_nalu + 4, i_size - 4);
            gf_list_add(p_mp4->p_config->pictureParameterSets, p_slot);
            p_slot = NULL;
            p_mp4->b_pps = 1;
            if (p_mp4->b_sps)
                gf_isom_avc_config_update(p_mp4->p_file, p_mp4->i_track, 1, p_mp4->p_config);
        }
        break;

    /* slice, sei */
    case 0x1:
    case 0x5:
    case 0x6:
        psize = i_size - 4 ;
        memcpy(p_mp4->p_sample->data + p_mp4->p_sample->dataLength, p_nalu, i_size);
        p_mp4->p_sample->data[p_mp4->p_sample->dataLength + 0] = (psize >> 24) & 0xff;
        p_mp4->p_sample->data[p_mp4->p_sample->dataLength + 1] = (psize >> 16) & 0xff;
        p_mp4->p_sample->data[p_mp4->p_sample->dataLength + 2] = (psize >> 8) & 0xff;
        p_mp4->p_sample->data[p_mp4->p_sample->dataLength + 3] = (psize >> 0) & 0xff;
        p_mp4->p_sample->dataLength += i_size;
        break;
    }

    return i_size;
}

static int set_eop_mp4( hnd_t handle, x264_picture_t *p_picture )
{
    mp4_t *p_mp4 = (mp4_t *)handle;
    uint64_t dts = (uint64_t)p_mp4->i_numframe * p_mp4->i_time_inc;
    uint64_t pts = (uint64_t)p_picture->i_pts;
    int32_t offset = p_mp4->i_init_delay + pts - dts;

    p_mp4->p_sample->IsRAP = p_picture->i_type == X264_TYPE_IDR ? 1 : 0;
    p_mp4->p_sample->DTS = dts;
    p_mp4->p_sample->CTS_Offset = offset;
    gf_isom_add_sample(p_mp4->p_file, p_mp4->i_track, p_mp4->i_descidx, p_mp4->p_sample);

    p_mp4->p_sample->dataLength = 0;
    p_mp4->i_numframe++;

    return 0;
}

#endif /* MP4_OUTPUT */
