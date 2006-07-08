/* GStreamer
 * Copyright (C) 2006 Vincent Torri <vtorri@univ-evry.fr>
 *
 * gstavs3videosink.c:
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
 * SECTION:element-avs3videosink
 * @short_description: black hole for data
 * @see_also: #GstAvs3Src
 *
 * Dummy sink that swallows everything.
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include "gstavs3videosink.h"

#define PACKAGE "Avisynth 3.0"


static GstStaticPadTemplate avs3_video_sink_template = GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("video/x-raw-rgb, "
        "framerate = (fraction) [ 0, MAX ];"
                     "video/x-raw-yuv, "
        "framerate = (fraction) [ 0, MAX ]")
    );

GST_DEBUG_CATEGORY_STATIC (gst_avs3_video_sink_debug);
#define GST_CAT_DEFAULT gst_avs3_video_sink_debug

static GstElementDetails gst_avs3_video_sink_details =
GST_ELEMENT_DETAILS ("Avisynth 3.0 Sink",
    "Sink",
    "Seek-specific video sink element",
    "Vincent Torri <vtorri@univ-evry.fr>");


/* Avs3VideoSink signals and args */
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
    GST_DEBUG_CATEGORY_INIT (gst_avs3_video_sink_debug, "avs3videosink", 0, "avs3videosink element");

GST_BOILERPLATE_FULL (GstAvs3VideoSink, gst_avs3_video_sink, GstBaseSink,
    GST_TYPE_BASE_SINK, _do_init)


static guint gst_avs3_video_sink_signals[LAST_SIGNAL] = { 0 };


/*******************/
/*                 */
/*   Declaration   */
/*                 */
/*******************/

/* avs3videosink methods */

static GstStateChangeReturn gst_avs3_video_sink_change_state (GstElement * element,
    GstStateChange transition);

static void gst_avs3_video_sink_seek (GstAvs3VideoSink * sink, guint frame_nbr, GstBuffer **buffer);

/* basesink methods */

static GstCaps *     gst_avs3_video_sink_getcaps  (GstBaseSink * bsink);

static gboolean      gst_avs3_video_sink_setcaps  (GstBaseSink * bsink,
                                                   GstCaps     * caps);

static GstFlowReturn gst_avs3_video_sink_preroll  (GstBaseSink * bsink,
                                                   GstBuffer   * buffer);
static GstFlowReturn gst_avs3_video_sink_render   (GstBaseSink * bsink,
                                                   GstBuffer   * buffer);

/*******************/
/*                 */
/* Implementation  */
/*                 */
/*******************/

/* avs3videosink methods */

static void
gst_avs3_video_sink_init (GstAvs3VideoSink * sink, GstAvs3VideoSinkClass * g_class)
{
  sink->frame_nbr = 0;
  sink->buffer = NULL;
  sink->fps_num = 0;
  sink->fps_den = 1;
  sink->start = FALSE;
  sink->got_buffer = FALSE;
  sink->condition = g_cond_new ();
}

static void
gst_avs3_video_sink_base_init (gpointer g_class)
{
  GstElementClass *gstelement_class = GST_ELEMENT_CLASS (g_class);

  gst_element_class_add_pad_template (gstelement_class,
      gst_static_pad_template_get (&avs3_video_sink_template));
  gst_element_class_set_details (gstelement_class, &gst_avs3_video_sink_details);
}

static void
gst_avs3_video_sink_class_init (GstAvs3VideoSinkClass * klass)
{
  GObjectClass *gobject_class;
  GstElementClass *gstelement_class;
  GstBaseSinkClass *gstbase_sink_class;

  gobject_class = (GObjectClass *) klass;
  gstelement_class = (GstElementClass *) klass;
  gstbase_sink_class = (GstBaseSinkClass *) klass;

  gstelement_class->change_state = GST_DEBUG_FUNCPTR (gst_avs3_video_sink_change_state);

  gst_avs3_video_sink_signals[SIGNAL_SEEK] =
    g_signal_new ("seek",
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_RUN_LAST,
                  G_STRUCT_OFFSET (GstAvs3VideoSinkClass, seek),
                  NULL,
                  NULL,
                  g_cclosure_marshal_VOID__UINT_POINTER,
                  G_TYPE_NONE,
                  2, G_TYPE_INT, G_TYPE_POINTER);

  gstbase_sink_class->get_caps = GST_DEBUG_FUNCPTR (gst_avs3_video_sink_getcaps);
  gstbase_sink_class->set_caps = GST_DEBUG_FUNCPTR (gst_avs3_video_sink_setcaps);

  gstbase_sink_class->preroll = GST_DEBUG_FUNCPTR (gst_avs3_video_sink_preroll);
  gstbase_sink_class->render = GST_DEBUG_FUNCPTR (gst_avs3_video_sink_render);

  klass->seek = gst_avs3_video_sink_seek;
}

/* we don't own the returned buffer */
static void
gst_avs3_video_sink_seek (GstAvs3VideoSink * sink, guint frame_nbr, GstBuffer **buffer)
{
  GstClockTime time;
  g_print ("Seek !\n");
  if (!sink->start)
    sink->start = TRUE;

  g_print ("Frame number : %d\n", frame_nbr);
  if (frame_nbr != sink->frame_nbr)
    sink->frame_nbr = frame_nbr;

  time = (GST_SECOND * sink->frame_nbr * sink->fps_den) / sink->fps_num;
  g_print ("Time         : %lld\n", time);
  gst_element_seek( GST_ELEMENT (sink),
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
gst_avs3_video_sink_getcaps (GstBaseSink * bsink)
{
  GstCaps *caps;

  caps = gst_caps_copy (gst_pad_get_pad_template_caps (bsink->sinkpad));

  return caps;
}

static gboolean
gst_avs3_video_sink_setcaps (GstBaseSink * bsink, GstCaps * caps)
{
  GstAvs3VideoSink *sink = GST_AVS3_VIDEO_SINK (bsink);
  GstStructure *structure;
  const GValue *fps;

  structure = gst_caps_get_structure (caps, 0);

  fps = gst_structure_get_value (structure, "framerate");
  if (fps == NULL)
    return FALSE;

  sink->fps_num = gst_value_get_fraction_numerator (fps);
  sink->fps_den = gst_value_get_fraction_denominator (fps);

  return TRUE;
}

static GstFlowReturn
gst_avs3_video_sink_preroll (GstBaseSink * bsink, GstBuffer * buffer)
{
  GstAvs3VideoSink *sink = GST_AVS3_VIDEO_SINK (bsink);

  /* we get the buffer on if gst_avs3_video_sink_seek has been called once */
  if (!sink->start) {
    return GST_FLOW_OK;
  }

  g_print (" ** preroll \n");

  GST_OBJECT_LOCK (sink);

  g_print ("Timestamp    : %lld\n", GST_BUFFER_TIMESTAMP (buffer));
  g_print ("Stream time  : %lld\n",
           gst_segment_to_stream_time (&bsink->segment,
                                       GST_FORMAT_TIME,
                                       (gint64)GST_BUFFER_TIMESTAMP (buffer)));
  g_print ("seek to      : %lld\n", (GST_SECOND * sink->frame_nbr * sink->fps_den) / sink->fps_num);
  g_print ("reste        : %lld\n", (GST_SECOND * sink->frame_nbr * sink->fps_den) % sink->fps_num);
  g_print ("buffer valid ? : %d\n", GST_BUFFER_TIMESTAMP_IS_VALID (buffer));
  g_print ("val 1 : %lld\n", GST_BUFFER_TIMESTAMP (buffer) * sink->fps_num);
  g_print ("val 2 : %lld\n", GST_SECOND * sink->frame_nbr * sink->fps_den);

  if (GST_BUFFER_TIMESTAMP_IS_VALID (buffer) &&
      (GST_BUFFER_TIMESTAMP (buffer) >= (((guint64)GST_SECOND * sink->frame_nbr * sink->fps_den) / sink->fps_num))) {
    /* we do not own the buffer */
/*     if (sink->buffer) */
/*       gst_buffer_unref (sink->buffer); */
    sink->buffer = buffer;
    sink->got_buffer = TRUE;
  }
  g_cond_broadcast (sink->condition);
  GST_OBJECT_UNLOCK (sink);

  return GST_FLOW_OK;
}

/* just in case we are in PLAY state */
static GstFlowReturn
gst_avs3_video_sink_render (GstBaseSink * bsink, GstBuffer * buf)
{
  return GST_FLOW_UNEXPECTED;
}

static GstStateChangeReturn
gst_avs3_video_sink_change_state (GstElement * element, GstStateChange transition)
{
  GstStateChangeReturn ret = GST_STATE_CHANGE_SUCCESS;
  GstAvs3VideoSink *sink = GST_AVS3_VIDEO_SINK (element);

  ret = GST_ELEMENT_CLASS (parent_class)->change_state (element, transition);

  switch (transition) {
    case GST_STATE_CHANGE_PAUSED_TO_READY:
      sink->frame_nbr = 0;
      sink->fps_num = 0;
      sink->fps_den = 1;
      sink->start = FALSE;
      sink->got_buffer = FALSE;
      break;
    case GST_STATE_CHANGE_READY_TO_NULL:
      GST_OBJECT_LOCK (sink);
      if (sink->buffer)
        gst_buffer_unref (sink->buffer);
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
  return gst_element_register (plugin, "avs3videosink", GST_RANK_NONE,
      GST_TYPE_AVS3_VIDEO_SINK);
}

GST_PLUGIN_DEFINE (0,
    10,
    "avs3videosink",
    "video sink plugin for Avisynth 3.0",
    plugin_init, "0.10", "GPL", PACKAGE, "http://avisynth3.unite-video.com/")

/* <wtay> take the segment info,send flush start/stop on the basesink sinkpad, send a newsegment with the saved segment info*/
