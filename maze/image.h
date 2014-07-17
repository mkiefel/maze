#ifndef _IMAGE_AF3E236ACED2_
#define _IMAGE_AF3E236ACED2_

#define cimg_use_png
#ifdef WIN32
// let the windows headers know that we do not want any of its max min macros
// they do collide with the standard's min() max()
#define NOMINMAX
#endif

#define cimg_display 0
#include <CImg.h>

#include <cstdint>
#include <array>

namespace maze {
  template <typename T>
  struct Image {
    typedef typename cimg_library::CImg<T> type;
  };

  typedef Image<uint8_t>::type image_type;
}

#endif /* _IMAGE_AF3E236ACED2_ */
