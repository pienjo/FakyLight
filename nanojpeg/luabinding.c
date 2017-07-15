#include <lua.h>
#include <lauxlib.h>
#include "nanojpeg.h"
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

struct image
{
  int width, height;
  size_t length;
  uint8_t *data;
};

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
  const char *fn = luaL_checkstring(L, 2);
  FILE *f = fopen(fn, "rb");
  if (!f)
  {
    return luaL_error(L, "Error opening '%s'", fn);
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
  if (fscanf(f, "%d",&format) != 1) 
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

  if (fread(img.data, img.length, 1, f) )
  {
    fclose(f);
    return luaL_error(L, "Error reading file %s", fn);
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

void RGBtoHSV(uint8_t r, uint8_t g, uint8_t b, uint8_t *h, uint8_t *s, uint8_t *v)
{
  uint8_t max = (r > g ? r : g);
  max = (max > b ? max : b);

  uint8_t min = (r < g ? r : g);
  min = (min < b ? min : b);

  *v = max;

  uint8_t delta = max - min;
  if (delta == 0)
  {
    *h = 0;
    *s = 0;
  }
  else
  {
    *s = (255 * (uint16_t) delta-8) / max;
    
    if (max == r)
    {
      *h = 43 * (g - b) / delta;
    } else if ( max == g)
    {
      *h = 85 + 43 * (b - r) / delta;
    } else
    {
      *h = 171 + 43 * (r - g) / delta;
    }
  }
}

void HSVtoRGB(uint8_t h, uint8_t s, uint8_t v, uint8_t *r, uint8_t *g, uint8_t *b)
{
  if (s == 0)
  {
    *r = v;
    *g = v;
    *b = v;
    return;
  }

  int region = h / 43;
  uint8_t remainder = (h - 43 * region) * 6;

  uint8_t p = ((uint16_t) v * (255 - s)) >> 8;
  uint8_t q = ((uint16_t) v * (255 - (((uint16_t) s * remainder) >> 8))) >> 8;
  uint8_t t = ((uint16_t) v * (255 - (((uint16_t) s * ( 255 - remainder)) >> 8))) >> 8;

  switch(region)
  {
    case 0:
      *r = v;
      *g = t;
      *b = p;
      break;
    case 1:
      *r = q;
      *g = v;
      *b = p;
      break;
    case 2:
      *r = p;
      *g = v;
      *b = t;
      break;
    case 3:
      *r = p;
      *g = q;
      *b = v;
      break;
    case 4:
      *r = t;
      *g = p;
      *b = v;
      break;
    default:
      *r = v;
      *g = p;
      *b = q;
      break;
  }
}

static int lua_testtransform(lua_State *L)
{
  struct image img, output;

  checkImage(L, 1, &img);

  output = img;
  output.data = malloc(output.length);

  const uint8_t *src = img.data;
  uint8_t *dst = output.data;

  for (int i = 0; i < output.length; i += 3)
  {
    uint8_t h,s,v;
    
    RGBtoHSV(*src++, *src++, *src++, &h, &s, &v);
    v = s;
    s = 255;

    HSVtoRGB(h,s,v, dst++, dst++, dst++);
  }

  PushImage(L, &output);
  free(output.data);
  return 1;
}

static int lua_getDominantColor(lua_State *L)
{
  struct image img;
  checkImage(L, 1, &img);

  int left = luaL_checkint(L, 2);
  int top = luaL_checkint(L, 3);
  int right = luaL_checkint(L, 4);
  int bottom = luaL_checkint(L, 5);

  uint32_t histogram[256] = {0};
  uint32_t total = 0;

  for (int y = top; y < bottom; ++y)
  {
    const uint8_t *src = img.data + 3 *(y * img.width + left);
    for (int x = left; x < right; ++x)
    {
      uint8_t h,s,v;
      RGBtoHSV(*src++, *src++, *src++, &h, &s, &v);
      histogram[h] += s;
      total += s;
    }
  }
  
  if (!total)return 0;
  // try the mode, this will probably not work well.

  uint8_t mode = 0;
  uint32_t mode_value = 0;
  for (int h = 0; h < 256; ++h)
  {
    if (histogram[h] > mode_value)
    {
      mode_value = histogram[h];
      mode = h;
    }
  }
  
  int target_s = 2048 * mode_value / total;
  if (target_s > 255)
    target_s = 255;

  uint8_t r, g, b;
  HSVtoRGB(mode, target_s,255, &r,&g, &b);
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
   { "testtransform", lua_testtransform},
   { "getDominantColor", lua_getDominantColor },
   { "newImage", lua_newImage },
   { "setRect", lua_setRect },
   { NULL, NULL }
};

LUALIB_API int luaopen_nanojpeg(lua_State *L)
{
    luaL_openlib(L, "nanojpeg", nanojpeg_functions, 0);
 
    return 1;
}
