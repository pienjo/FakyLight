#include <lua.h>
#include <lauxlib.h>
#include "nanojpeg.h"
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "imageType.h"
#include "colorRoutines.h"

void PushImage(lua_State *L, struct image *img)
{
  lua_newtable(L);
  lua_pushnumber(L, img->height);
  lua_setfield(L, -2, "height");

  lua_pushnumber(L, img->width);
  lua_setfield(L, -2, "width");

  lua_pushlstring(L, img->data, img->length);
  lua_setfield(L, -2, "data");
}

static int lua_newImage(lua_State *L)
{
  struct image img; 
  img.width = luaL_checkint(L, 1);
  img.height = luaL_checkint(L, 2);
  img.length = 3 * img.width * img.height;
  img.data = malloc (img.length);
  
  PushImage(L, &img);
  return 1;
}

void checkImage(lua_State *L, int idx, struct image *img)
{
  luaL_argcheck(L, lua_type(L, idx) == LUA_TTABLE, idx, "Expected an image table");

  if (idx < 0)
    idx = lua_gettop(L) + idx;

  lua_getfield(L, 1, "width");
  luaL_argcheck(L, lua_type(L, -1) == LUA_TNUMBER, idx, "Expected a 'width' member");
  img->width = lua_tointeger(L, -1);
  lua_pop(L, 1);

  lua_getfield(L, 1, "height");
  luaL_argcheck(L, lua_type(L, -1) == LUA_TNUMBER, idx, "Expected a 'height' member");
  img->height = lua_tointeger(L, -1);
  lua_pop(L, 1);
  
  lua_getfield(L, 1, "data");
  luaL_argcheck(L, lua_type(L, -1) == LUA_TSTRING, idx, "Expected a 'data' member");
  img->data = (uint8_t *) lua_tolstring(L, -1, &(img->length));
  lua_pop(L, 1);
}

static int lua_extract(lua_State *L)
{
  const char *str;
  size_t len;
  str = luaL_checklstring(L, 1, &len);

  njInit();
  
  if (njDecode(str, len)) 
  {
    njDone();
    return 0;
  }

  struct image img;

  img.height = njGetHeight();
  img.width = njGetWidth();
  img.length = njGetImageSize();
  img.data = njGetImage();

  PushImage(L, &img);

  njDone();

  
  return 1;
}

static int lua_readppm(lua_State *L)
{
  const char *fn = luaL_checkstring(L, 1);
  FILE *f = fopen(fn, "rb");
  if (!f)
  {
    return luaL_error(L, "Error opening '%s': %s", fn, strerror(errno));
  }

  char buf[16];
  if (!fgets(buf, sizeof(buf), f))
  {
    fclose(f);
    return luaL_error(L, "Error reading file");
  }
  
  if (buf[0] != 'P' || buf[1] != '6')
  {
    fclose(f);
    return luaL_error(L, "invalid filetype");
  }

  /* skip comments */

  char c = getc(f);
  while (c == '#') {
  while (getc(f) != '\n') ;
     c = getc(f);
  }
  ungetc(c, f);

  struct image img;

  if (fscanf(f, "%d %d", &img.width, &img.height) != 2)
  {
    fclose(f);
    return luaL_error(L, "Error parsing image size");
  }

  int format;
  if (fscanf(f, "%d\n",&format) != 1) 
  {
    fclose(f);
    return luaL_error(L, "Error parsing color component");
  }

  if (format != 255)
  {
    fclose(f);
    return luaL_error(L, "Unsupported depth %d\n", format);
  }

  img.length = 3 * img.width * img.height;
  img.data = malloc(img.length);

  if (fread(img.data, 1, img.length, f) != img.length )
  {
    fclose(f);
    return luaL_error(L, "short read while reading file %s", fn);
  }
  fclose(f);

  PushImage(L, &img);
  free(img.data);
  return 1;
}

static int lua_writeppm(lua_State *L)
{
  const char *fn = luaL_checkstring(L, 2);
  struct image img;

  checkImage(L, 1, &img);

  FILE *f = fopen(fn, "wb");
  if (!f)
  {
    return luaL_error(L, "Error opening '%s'", fn);
  }
  fprintf(f, "P6\n%d %d\n255\n",img.width, img.height);
  fwrite(img.data, 1, img.length, f);
  fclose(f);
  return 0;
}

static void smoothHistogram(const uint32_t *inputHistogram, uint32_t *outputHistogram, int windowSize)
{
  uint32_t begin = 256 - windowSize; 
  uint32_t end = windowSize;
 
  uint32_t total = 0;
  for (uint32_t init = 0; init < windowSize; ++init)
  {
    total += inputHistogram[init];
    total += inputHistogram[begin + init];
  }
  
  for (uint32_t center = 0; center < 256; ++center, end = (end + 1)%256, begin = (begin + 1)%256)
  {
    outputHistogram[center] = total;
    total += inputHistogram[end];
    total -= inputHistogram[begin];
  }
}

static int lua_getHueHistogram(lua_State *L)
{
  struct image img;
  checkImage(L, 1, &img);

  int left = luaL_checkint(L, 2);
  int top = luaL_checkint(L, 3);
  int right = luaL_checkint(L, 4);
  int bottom = luaL_checkint(L, 5);

  uint32_t histogram[256] = {0};
  uint32_t valueHistogram[256] = {0};
  uint8_t avgSat;

  getHueValueHistogram(&img, histogram,valueHistogram, &avgSat, left, top, right, bottom);

  lua_createtable(L, 256, 0);
  lua_createtable(L, 256, 0);
  
  for (int i = 0; i < 256; ++i)
  {
    lua_pushnumber(L, histogram[i]);
    lua_rawseti(L, -3, i+1);
    lua_pushnumber(L, valueHistogram[i]);
    lua_rawseti(L, -2, i+1);
  }
  return 2;
}

static int lua_getDominantColor(lua_State *L)
{
  struct image img;
  checkImage(L, 1, &img);

  int left = luaL_checkint(L, 2);
  int top = luaL_checkint(L, 3);
  int right = luaL_checkint(L, 4);
  int bottom = luaL_checkint(L, 5);

  uint32_t histogram[257] = {0};
  uint32_t valueHistogram[256] = {0};
  uint32_t smoothedHistogram[256];

  uint32_t target_s = 0;
  uint8_t target_h = 0;
  uint8_t avgSat = 0;

  uint32_t total = getHueValueHistogram(&img, histogram, valueHistogram, &avgSat, left, top, right, bottom);
  smoothHistogram(histogram, smoothedHistogram, 7);

  if (total)
  {
    // Find the mode of the smoothed histogram.
    uint32_t mode_value = 0;
    for (int h = 0; h < 256; ++h)
    {
      if (smoothedHistogram[h] > mode_value)
      {
	mode_value = smoothedHistogram[h];
	target_h = h;
      }
    }
    
    // Determine how much of the total response is represented by the smoothed bin.
    target_s = 256 * (mode_value) / total;
    target_s =  ((int)avgSat) * target_s / 128;
    if (target_s > 255)
      target_s = 255;
  }

  // Determine the modal value.
  int total_v = 0;
  for (int v = 10; v < 255; ++v)
    total_v = total_v + valueHistogram[v];

  total_v /= 2;

  int target_v = 10;
  for (; target_v < 256 && total_v > 0; target_v++)
    total_v-= valueHistogram[target_v];

  // take average with 0x80
  target_v+= 96;
  if (target_v > 255)
    target_v = 255;

  uint8_t r, g, b;
  HSVtoRGB(target_h, target_s,target_v, &r,&g, &b);
  lua_pushinteger(L, r);
  lua_pushinteger(L, g);
  lua_pushinteger(L, b);
  return 3;
}

static int lua_setRect(lua_State *L)
{
  struct image img, output;
  checkImage(L, 1, &img);
  
  output = img;
  output.data = malloc(output.length);
  memcpy(output.data, img.data, output.length);
  int left = luaL_checkint(L, 2);
  int top = luaL_checkint(L, 3);
  int right = luaL_checkint(L, 4);
  int bottom = luaL_checkint(L, 5);

  uint8_t r = luaL_checkint(L, 6);
  uint8_t g = luaL_checkint(L, 7);
  uint8_t b = luaL_checkint(L, 8);

  for (int y = top; y < bottom; ++y)
  {
    uint8_t *dst = output.data + 3 *(y * img.width + left);
    for (int x = left; x < right; ++x)
    {
      *dst++ = r;
      *dst++ = g;
      *dst++ = b;
    }
  }
  PushImage(L, &output);
  free(output.data);

  return 1;
}

static const struct luaL_Reg nanojpeg_functions[] =
{
   { "extract", lua_extract },
   { "writeppm", lua_writeppm },
   { "readppm", lua_readppm },
   { "getDominantColor", lua_getDominantColor },
   { "getHueHistogram", lua_getHueHistogram },
   { "newImage", lua_newImage },
   { "setRect", lua_setRect },
   { NULL, NULL }
};

LUALIB_API int luaopen_nanojpeg(lua_State *L)
{
    luaL_openlib(L, "nanojpeg", nanojpeg_functions, 0);
 
    return 1;
}
