#ifndef __AVS3_PRIVATE_X264_H__
#define __AVS3_PRIVATE_X264_H__


typedef struct _Avs3_Encode_Data Avs3_Encode_Data;
typedef struct _Avs3_Thread_Data Avs3_Thread_Data;
typedef struct _Avs3_Pipe_Data Avs3_Pipe_Data;

struct _Avs3_Encode_Data
{
  Avs3_Data        *data;

  GtkWidget         *path_output;
  GtkWidget         *file_output;
  GtkWidget         *combo;
};

struct _Avs3_Thread_Data
{
  Avs3_Data        *data;

  GtkWidget         *current_video_frame;
  GtkWidget         *video_data;
  GtkWidget         *video_rendering_rate;
  GtkWidget         *time_elapsed;
  GtkWidget         *time_remaining;
  GtkWidget         *progress;

  GtkWidget         *dialog;
  GtkWidget         *button;
  GtkWidget         *end_button;

  x264_param_t      *param;

  gchar             *file_output;
  gint               container;

  /* file descriptors */
  GIOChannel        *io_read;  /* use it with read */
  GIOChannel        *io_write; /* use it with write */
};

struct _Avs3_Pipe_Data
{
  int     frame;
  int     frame_total;
  int     file;
  int64_t elapsed;
};


#endif /* __AVS3_PRIVATE_X264_H__ */
