#ifndef GSTPIPELINE_H
#define GSTPIPELINE_H

#include <exception>
#include <string>
#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <gbxsickacfr/gbxiceutilacfr/safethread.h>
#include <jderobotice/component.h>
#include <jderobotice/exceptions.h>
#include <colorspaces/colorspacesmm.h>

namespace cameraserver {

  class Config{
  public:
    Config();
    bool validate() const;
    std::string toString() const;
    bool operator==( const Config & other );
    bool operator!=( const Config & other );
  
    //! pipeline name
    std::string name;
    //! video input uri file
    std::string uri;
    //! video output width [pixels]
    size_t width;
    //! video output height [pixels]
    size_t height;
    //! video output bits per pixel
    size_t bpp;
    //! video output frame rate numerator
    size_t framerateN;
    //! video output frame rate denominator
    size_t framerateD;
    //! video output format string
    colorspaces::Image::FormatPtr format;
  };

  extern int gst_init(int* argcp, char** argvp[]);

  class GSTPipeline: virtual public gbxiceutilacfr::SafeThread{
  public:
    /**
     * Create pipeline
     * After creation pipeline has to be started (start()) in order to activate it
     */
    GSTPipeline(const jderobotice::Context& context, const Config &cfg) throw(jderobotice::ConfigFileException);
    ~GSTPipeline();

    void stop();
    virtual void walk();

    GstBuffer* pull_buffer();
  private:
    static gboolean my_bus_cb(GstBus *bus,
			      GstMessage *message,
			      gpointer data);
    static void newpad_cb(GstElement *decode,
			  GstPad *pad,
			  gboolean last,
			  gpointer data);
    Config config_;
    jderobotice::Context context;
    GMainLoop *mainloop;
    GstElement *pipeline,*source,*decoder,*videoscale,*videorate,*videocolor,*filter,*sink,*bin;
    GstBus *bus;
  };
  typedef IceUtil::Handle<GSTPipeline> GSTPipelinePtr;

}//namespace

#endif /*GSTPIPELINE_H*/
