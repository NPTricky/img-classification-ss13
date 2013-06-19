#include <GL/glew.h>
#include <QtWidgets>
#include <opencv2/opencv.hpp>
#include <tbb/tbb.h>

// pragma warning macro hack
#define STRINGIZE_HELPER(x) #x
#define STRINGIZE(x) STRINGIZE_HELPER(x)
#define WARNING(desc) message(__FILE__ "(" STRINGIZE(__LINE__) ") : Warning: " #desc)

// usage: #pragma WARNING(TODO: Aw... Hell Naw!)