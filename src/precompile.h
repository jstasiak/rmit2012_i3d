#ifndef PRECOMPILE_H
#define PRECOMPILE_H

#include <cassert>
#include <cmath>

#define M_PI 3.14159265358979323846

#include <cstdio>
#include <exception>

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/signal.hpp>
#include <boost/smart_ptr.hpp>


#include <SDL.h>

// For Windows, because SDL apparently redefines main to
// mean something different
#undef main

#include <SDL_opengl.h>

#include <GL/glu.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>



#endif // PRECOMPILE_H
