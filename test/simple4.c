
#include <gnl/gnl.h>

/*
 (-0----->-----------5-------->------10-------->-----15----)
 ! main_timeline                                           !
 !                                                         !
 ! (-----------------------------------------------------) !
 ! ! my_layer1                                           ! !
 ! ! (-------------------------------)                   ! !
 ! ! ! source1                       !                   ! !
 ! ! !                               !                   ! !
 ! ! (-------------------------------)                   ! !
 ! (-----------------------------------------------------) !
 ! (-----------------------------------------------------) !
 ! ! my_composition1                                     ! !
 ! !              (-------------------------------)      ! !
 ! !              ! source3                       !      ! !
 ! !              !                               !      ! !
 ! !              (-------------------------------)      ! !
 ! !                  (-------------------------------)  ! !
 ! !                  ! source2                       !  ! !
 ! !                  !                               !  ! !
 ! !                  (-------------------------------)  ! !
 ! (-----------------------------------------------------) !
 !                                                         !
 (---------------------------------------------------------)
*/

int
main (int argc, gchar *argv[]) 
{
  GstElement *pipeline;
  GnlTimeline *timeline;
  GnlGroup *group;
  GnlLayer *layer1, *layer2, *layer3;
  GnlComposition *comp;
  GnlSource *source1, *source2, *source3;
  GstElement *fakesrc1, *fakesrc2, *fakesrc3, *sink;

  gnl_init (&argc, &argv);

  pipeline = gst_pipeline_new ("main_pipeline");
  timeline = gnl_timeline_new ("main_timeline");

  source1 = gnl_source_new ("my_source1");
  fakesrc1 = gst_elementfactory_make ("fakesrc", "src1");
  gnl_source_set_element (source1, fakesrc1);
  gnl_source_set_start_stop (source1, 0, 6);

  source2 = gnl_source_new ("my_source2");
  fakesrc2 = gst_elementfactory_make ("fakesrc", "src2");
  gnl_source_set_element (source2, fakesrc2);
  gnl_source_set_start_stop (source2, 0, 6);

  source3 = gnl_source_new ("my_source3");
  fakesrc3 = gst_elementfactory_make ("fakesrc", "src3");
  gnl_source_set_element (source3, fakesrc3);
  gnl_source_set_start_stop (source3, 0, 6);

  layer1 = gnl_layer_new ("my_layer1");
  gnl_layer_add_source (layer1, source1, 0);

  layer2 = gnl_layer_new ("my_layer2");
  gnl_layer_add_source (layer2, source2, 2);
  layer3 = gnl_layer_new ("my_layer3");
  gnl_layer_add_source (layer3, source3, 4);

  comp = gnl_composition_new ("my_comosition");

  gnl_composition_append_layer (comp, layer2);
  gnl_composition_append_layer (comp, layer3);

  group = gnl_group_new ("my_group");

  gnl_composition_append_layer (GNL_COMPOSITION (group), layer1);
  gnl_composition_append_layer (GNL_COMPOSITION (group), GNL_LAYER (comp));

  gnl_timeline_add_group (timeline, group);

  sink = gst_elementfactory_make ("fakesink", "sink");
  gst_bin_add (GST_BIN (pipeline), sink);
  gst_element_connect (GST_ELEMENT (group), "src", sink, "sink");

  gst_bin_add (GST_BIN (pipeline), GST_ELEMENT (timeline));

  gst_element_set_state (GST_ELEMENT (pipeline), GST_STATE_PLAYING);

  while (gst_bin_iterate (GST_BIN (pipeline)));
  
  gst_element_set_state (GST_ELEMENT (pipeline), GST_STATE_NULL);
   
}