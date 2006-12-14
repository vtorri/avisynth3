/* GStreamer
 * Copyright (C) 2006 Vincent Torri <vtorri@univ-evry.fr>
 *
 * gstavs3audiosink.c:
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
/**
 * SECTION:element-avs3audiosink
 * @short_description: black hole for data
 *
 * Dummy sink that swallows everything.
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include "gstavs3audiosink.h"


#if HAVE___ATTRIBUTE__
#  define __UNUSED__ __attribute__((unused))
#else
#  define __UNUSED__
#endif

#define PACKAGE "Avisynth 3.0"

#ifdef G_ENABLE_DEBUG
#define g_marshal_value_peek_int(v)      g_value_get_int (v)
#define g_marshal_value_peek_int64(v)    g_value_get_int64 (v)
#define g_marshal_value_peek_pointer(v)  g_value_get_pointer (v)
#else /* !G_ENABLE_DEBUG */
#define g_marshal_value_peek_int(v)      (v)->data[0].v_int
#define g_marshal_value_peek_int64(v)    (v)->data[0].v_int64
#define g_marshal_value_peek_pointer(v)  (v)->data[0].v_pointer
#endif /* !G_ENABLE_DEBUG */


static GstStaticPadTemplate avs3_audio_sink_template = GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("audio/x-raw-int, "
        "rate = (int) [ 1, 2147483647 ];"
                     "audio/x-raw-float, "
        "rate = (int) [ 1, 2147483647 ]")
    );

GST_DEBUG_CATEGORY_STATIC (gst_avs3_audio_sink_debug);
#define GST_CAT_DEFAULT gst_avs3_audio_sink_debug

static GstElementDetails gst_avs3_audio_sink_details =
GST_ELEMENT_DETAILS ("Avisynth 3.0 Sink",
    "Sink",
    "Seek-specific audio sink element",
    "Vincent Torri <vtorri@univ-evry.fr>");


/* Avs3AudioSink signals and args */
enum
{
  /* FILL ME */
  SIGNAL_SEEK,
  LAST_SIGNAL
};

enum
{
  PROP_0
};

#define _do_init(bla) \
    GST_DEBUG_CATEGORY_INIT (gst_avs3_audio_sink_debug, "avs3audiosink", 0, "avs3audiosink element");

GST_BOILERPLATE_FULL (GstAvs3AudioSink, gst_avs3_audio_sink, GstBaseSink,
                      GST_TYPE_BASE_SINK, _do_init)


static guint gst_avs3_audio_sink_signals[LAST_SIGNAL] = { 0 };


/*******************/
/*                 */
/*   Declaration   */
/*                 */
/*******************/

/* avs3audiosink methods */

static GstStateChangeReturn gst_avs3_audio_sink_change_state (GstElement * element,
    GstStateChange transition);

static void gst_avs3_audio_sink_seek (GstAvs3AudioSink * sink, gint64 sample_start, gint samples_count, GstBuffer **buffer);

/* basesink methods */

static GstCaps *     gst_avs3_audio_sink_getcaps (GstBaseSink  * bsink);

static gboolean      gst_avs3_audio_sink_setcaps (GstBaseSink  * bsink,
                                                  GstCaps      * caps);

static GstFlowReturn gst_avs3_audio_sink_preroll  (GstBaseSink * bsink,
                                                   GstBuffer   * buffer);
static GstFlowReturn gst_avs3_audio_sink_render   (GstBaseSink * bsink,
                                                   GstBuffer   * buffer);

static void gst_marshal_VOID__INT64_INT_POINTER (GClosure     *closure,
                                                 GValue       *return_value,
                                                 guint         n_param_values,
                                                 const GValue *param_values,
                                                 gpointer      invocation_hint,
                                                 gpointer      marshal_data);

/*******************/
/*                 */
/* Implementation  */
/*                 */
/*******************/

/* marshaller */
/* VOID:INT64,INT,POINTER */
static void
gst_marshal_VOID__INT64_INT_POINTER (GClosure     *closure,
                                     GValue       *return_value __UNUSED__,
                                     guint         n_param_values,
                                     const GValue *param_values,
                                     gpointer      invocation_hint __UNUSED__,
                                     gpointer      marshal_data)
{
  typedef void (*GMarshalFunc_VOID__INT64_INT_POINTER) (gpointer     data1,
                                                        gint64       arg_1,
                                                        gint         arg_2,
                                                        gpointer     arg_3,
                                                        gpointer     data2);
  register GMarshalFunc_VOID__INT64_INT_POINTER callback;
  register GCClosure *cc = (GCClosure*) closure;
  register gpointer data1, data2;

  g_return_if_fail (n_param_values == 4);

  if (G_CCLOSURE_SWAP_DATA (closure))
    {
      data1 = closure->data;
      data2 = g_value_peek_pointer (param_values + 0);
    }
  else
    {
      data1 = g_value_peek_pointer (param_values + 0);
      data2 = closure->data;
    }
  callback = (GMarshalFunc_VOID__INT64_INT_POINTER) (marshal_data ? marshal_data : cc->callback);

  callback (data1,
            g_marshal_value_peek_int64 (param_values + 1),
            g_marshal_value_peek_int (param_values + 2),
            g_marshal_value_peek_pointer (param_values + 3),
            data2);
}

/* avs3audiosink methods */

static void
gst_avs3_audio_sink_init (GstAvs3AudioSink      *sink,
                          GstAvs3AudioSinkClass *g_class __UNUSED__)
{
  sink->sample_rate = 0;
  sink->sample_start = 0;
  sink->samples_count = 0;
  sink->buffer = NULL;
  sink->width = 0;
  sink->start = FALSE;
  sink->got_buffer = FALSE;
  sink->condition = g_cond_new ();
}

static void
gst_avs3_audio_sink_base_init (gpointer g_class)
{
  GstElementClass *gstelement_class = GST_ELEMENT_CLASS (g_class);

  gst_element_class_add_pad_template (gstelement_class,
      gst_static_pad_template_get (&avs3_audio_sink_template));
  gst_element_class_set_details (gstelement_class, &gst_avs3_audio_sink_details);
}

static void
gst_avs3_audio_sink_class_init (GstAvs3AudioSinkClass * klass)
{
  GObjectClass *gobject_class;
  GstElementClass *gstelement_class;
  GstBaseSinkClass *gstbase_sink_class;

  gobject_class = (GObjectClass *) klass;
  gstelement_class = (GstElementClass *) klass;
  gstbase_sink_class = (GstBaseSinkClass *) klass;

  gstelement_class->change_state = GST_DEBUG_FUNCPTR (gst_avs3_audio_sink_change_state);

  gst_avs3_audio_sink_signals[SIGNAL_SEEK] =
    g_signal_new ("seek",
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_RUN_LAST,
                  G_STRUCT_OFFSET (GstAvs3AudioSinkClass, seek),
                  NULL,
                  NULL,
                  gst_marshal_VOID__INT64_INT_POINTER,
                  G_TYPE_NONE,
                  3, G_TYPE_INT64, G_TYPE_INT, G_TYPE_POINTER);

  gstbase_sink_class->get_caps = GST_DEBUG_FUNCPTR (gst_avs3_audio_sink_getcaps);
  gstbase_sink_class->set_caps = GST_DEBUG_FUNCPTR (gst_avs3_audio_sink_setcaps);

  gstbase_sink_class->preroll = GST_DEBUG_FUNCPTR (gst_avs3_audio_sink_preroll);
  gstbase_sink_class->render = GST_DEBUG_FUNCPTR (gst_avs3_audio_sink_render);

  klass->seek = gst_avs3_audio_sink_seek;
}

/* we don't own the returned buffer */
static void
gst_avs3_audio_sink_seek (GstAvs3AudioSink * sink, gint64 sample_start, gint samples_count, GstBuffer **buffer)
{
  GstClockTime time;
  g_message ("Seek audio !");
  if (!sink->start)
    sink->start = TRUE;

  g_message ("Start sample : %lld", sample_start);
  if (sample_start != sink->sample_start)
    sink->sample_start = sample_start;

  g_message ("Sample count : %d", samples_count);
  if (samples_count != sink->samples_count)
    sink->samples_count = samples_count;

  time = (GST_SECOND * sink->sample_start) / sink->sample_rate;
  g_message ("Time         : %lld", time);
  gst_element_seek (GST_ELEMENT (sink),
                    1.0,
                    GST_FORMAT_TIME,
                    (GstSeekFlags)(GST_SEEK_FLAG_ACCURATE | GST_SEEK_FLAG_FLUSH),
                    GST_SEEK_TYPE_SET,
                    (gint64)time,
                    GST_SEEK_TYPE_NONE,
                    -1LL);

  GST_OBJECT_LOCK (sink);

  while (!sink->got_buffer)
    g_cond_wait (sink->condition, GST_OBJECT_GET_LOCK (sink));
  *buffer = sink->buffer;
  sink->got_buffer = FALSE;
  GST_OBJECT_UNLOCK (sink);
}

/* basesink methods */

static GstCaps *
gst_avs3_audio_sink_getcaps (GstBaseSink * bsink)
{
  GstCaps *caps;

  caps = gst_caps_copy (gst_pad_get_pad_template_caps (bsink->sinkpad));

  return caps;
}

static gboolean
gst_avs3_audio_sink_setcaps (GstBaseSink * bsink, GstCaps * caps)
{
  GstAvs3AudioSink *sink = GST_AVS3_AUDIO_SINK (bsink);
  GstStructure     *structure;

  structure = gst_caps_get_structure (caps, 0);

  if (!gst_structure_get_int (structure, "rate", &sink->sample_rate))
    return FALSE;

  if (!gst_structure_get_int (structure, "width", &sink->width))
    return FALSE;

  return TRUE;
}

static GstFlowReturn
gst_avs3_audio_sink_preroll (GstBaseSink * bsink, GstBuffer * buffer)
{
  GstAvs3AudioSink *sink = GST_AVS3_AUDIO_SINK (bsink);

  /* we get the buffer on if gst_avs3_audio_sink_seek has been called once */
  if (!sink->start) {
    return GST_FLOW_OK;
  }

  GST_OBJECT_LOCK (sink);

  g_message ("Size         : %d", GST_BUFFER_SIZE (buffer) / (sink->width >> 3));
  g_message ("Size reques. : %d", sink->samples_count);
  g_message ("Timestamp    : %lld", GST_BUFFER_TIMESTAMP (buffer));
  g_message ("Stream time  : %lld",
           gst_segment_to_stream_time (&bsink->segment,
                                       GST_FORMAT_TIME,
                                       (gint64)GST_BUFFER_TIMESTAMP (buffer)));
  g_message ("seek to      : %lld", (GST_SECOND * sink->sample_start) / sink->sample_rate);

  if (GST_BUFFER_TIMESTAMP_IS_VALID (buffer) &&
      (((gint64)GST_BUFFER_TIMESTAMP (buffer) * sink->sample_rate) >= (GST_SECOND * sink->sample_start))) {
    sink->buffer = buffer;
    sink->got_buffer = TRUE;
  }
  g_cond_broadcast (sink->condition);
  GST_OBJECT_UNLOCK (sink);

  return GST_FLOW_OK;
}

/* just in case we are in PLAY state */
static GstFlowReturn
gst_avs3_audio_sink_render (GstBaseSink *bsink __UNUSED__,
                            GstBuffer   *buf __UNUSED__)
{
  return GST_FLOW_UNEXPECTED;
}

static GstStateChangeReturn
gst_avs3_audio_sink_change_state (GstElement * element, GstStateChange transition)
{
  GstStateChangeReturn ret = GST_STATE_CHANGE_SUCCESS;
  GstAvs3AudioSink *sink = GST_AVS3_AUDIO_SINK (element);

  ret = GST_ELEMENT_CLASS (parent_class)->change_state (element, transition);

  switch (transition) {
    case GST_STATE_CHANGE_PAUSED_TO_READY:
      sink->sample_rate = 0;
      sink->sample_start = 0;
      sink->samples_count = 0;
      sink->width = 0;
      sink->start = FALSE;
      sink->got_buffer = FALSE;
      break;
    case GST_STATE_CHANGE_READY_TO_NULL:
      GST_OBJECT_LOCK (sink);
      g_cond_free (sink->condition);
      GST_OBJECT_UNLOCK (sink);
      break;
    default:
      break;
  }

  return ret;
}

static gboolean
plugin_init (GstPlugin * plugin)
{
  return gst_element_register (plugin, "avs3audiosink", GST_RANK_NONE,
                               GST_TYPE_AVS3_AUDIO_SINK);
}

GST_PLUGIN_DEFINE (0,
    10,
    "avs3audiosink",
    "audio sink plugin for Avisynth 3.0",
    plugin_init, "0.10", "GPL", PACKAGE, "http://avisynth3.unite-video.com/")

/* See gstavs3videosink.c for comments */

/* <wtay> take the segment info,send flush start/stop on the basesink sinkpad, send a newsegment with the saved segment info */
