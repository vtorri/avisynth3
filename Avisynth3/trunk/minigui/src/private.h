#ifndef __AVS3_PRIVATE_H__
#define __AVS3_PRIVATE_H__


#include "../avisynth_c/clip_c.h"


typedef struct _Avs3_Data Avs3_Data;

struct _Avs3_Data
{
  GtkWidget *window;
  GtkWidget *preview;
  GtkWidget *scale;
  GtkWidget *status;
  
  gchar *filename;
  guint id;

  AVS_Environment *env;
  AVS_Clip        *clip;
  gint             frame_current;
  gint             frames_count;

  GThread *thread;
};

void _data_delete (Avs3_Data *data);
void _dialog_no_clip (GtkWidget *parent);


#endif /* __AVS3_PRIVATE_H__ */
