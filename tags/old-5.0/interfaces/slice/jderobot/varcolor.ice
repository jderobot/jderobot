#ifndef VARCOLOR_ICE
#define VARCOLOR_ICE


#include <jderobot/common.ice>

module jderobot{  

  //! Static description of the image source.
  class ImageDescription 
  {
    //! %Image width [pixels]
    int width; 
    
    //! %Image height [pixels]
    int height;

    //! %Image size [bytes]
    int size;
    
    //! %Image format string
    string format;
  };
  
  //! A sequence of integers to pack pixel information
  sequence<int> IntSeq;

  //! A single image served as a sequence of bytes
  class ImageData
  { 
    //! TimeStamp of Data
    Time timeStamp;   
    
    //! ImageDescription of Data, for convienence purposes
    ImageDescription description;
    
    //! The image data itself. The structure of this int sequence
    //! depends on the image format and compression.
    IntSeq pixelData;
  };

  //! Interface to the image provider.
  interface VarColor
  {
    //! Returns the image source description.
    idempotent ImageDescription getDescription();

    //! Returns the latest data.
    idempotent ImageData getData()
      throws DataNotExistException, HardwareFailedException;
  };

}; /*module*/

#endif /*VARCOLOR_ICE*/