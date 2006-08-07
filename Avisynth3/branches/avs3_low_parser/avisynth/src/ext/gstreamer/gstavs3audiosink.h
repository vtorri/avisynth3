/* GStreamer
 * Copyright (C) 2006 Vincent Torri <vtorri@univ-evry.fr>
 *
 * gstavs3sink.h:
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */


#ifndef __GST_AVS3_AUDIO_SINK_H__
#define __GST_AVS3_AUDIO_SINK_H__

#include <gst/gst.h>
#include <gst/base/gstbasesink.h>

G_BEGIN_DECLS


#define GST_TYPE_AVS3_AUDIO_SINK \
  (gst_avs3_audio_sink_get_type())
#define GST_AVS3_AUDIO_SINK(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_AVS3_AUDIO_SINK,GstAvs3AudioSink))
#define GST_AVS3_AUDIO_SINK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_AVS3_AUDIO_SINK,GstAvs3AudioSinkClass))
#define GST_IS_AVS3_AUDIO_SINK(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_AVS3_AUDIO_SINK))
#define GST_IS_AVS3_AUDIO_SINK_CLASS(obj) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_AVS3_AUDIO_SINK))


typedef struct _GstAvs3AudioSink GstAvs3AudioSink;
typedef struct _GstAvs3AudioSinkClass GstAvs3AudioSinkClass;

/**
 * GstAvs3Sink:
 *
 * The opaque #GstAvs3AudioSink data structure.
 */
struct _GstAvs3AudioSink {
  GstBaseSink           element;

  gint                  sample_rate;
  gint64                sample_start;
  gint                  samples_count;
  GstBuffer           * buffer;

  /* Amount of bits used and allocated per sample */
  gint                  width;

  /* lock management */
  gboolean              start;
  gboolean              got_buffer;
  GCond                *condition;
};

struct _GstAvs3AudioSinkClass {
  GstBaseSinkClass parent_class;

  /* element methods */
  void (*seek) (GstAvs3AudioSink *sink, gint64 sample_start, gint samples_count, GstBuffer **buffer);
};

GType gst_avs3_audio_sink_get_type (void);

G_END_DECLS

#endif /* __GST_AVS3_AUDIO_SINK_H__ */
