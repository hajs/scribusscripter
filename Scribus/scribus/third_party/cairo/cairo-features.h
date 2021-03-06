/* Generated by configure.  Do not edit. */
#ifndef CAIRO_FEATURES_H
#define CAIRO_FEATURES_H

#if HAVE_CONFIG_H
#include "scconfig.h"
#endif

#define CAIRO_HAS_FT_FONT 1
#define CAIRO_HAS_FC_FONT 1
#define CAIRO_HAS_IMAGE_SURFACE 1
#define CAIRO_HAS_META_SURFACE 1
#define CAIRO_HAS_PNG_FUNCTIONS 1

#if defined(_WIN32)
 #define CAIRO_HAS_WIN32_SURFACE 1
 #define CAIRO_HAS_WIN32_FONT 1
 #undef CAIRO_HAS_FC_FONT
 #undef CAIRO_HAS_FT_FONT
 #undef CAIRO_HAS_PNG_FUNCTIONS
#endif
#endif
