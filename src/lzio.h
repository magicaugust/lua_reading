/*
** $Id: lzio.h,v 1.30 2014/12/19 17:26:14 roberto Exp $
** Buffered streams
** See Copyright Notice in lua.h
*/


#ifndef lzio_h
#define lzio_h

#include "lua.h"

#include "lmem.h"


#define EOZ	(-1)			/* end of stream */

typedef struct Zio ZIO;

/* z 为 Zio* 类型, 从 z 中读取一个字符 */
#define zgetc(z)  (((z)->n--)>0 ?  cast_uchar(*(z)->p++) : luaZ_fill(z))


typedef struct Mbuffer {
  char *buffer;
  /* buffer 中的有效数据大小 */
  size_t n;
  size_t buffsize;
} Mbuffer;

/* buff 为 Mbuffer 类型 */
#define luaZ_initbuffer(L, buff) ((buff)->buffer = NULL, (buff)->buffsize = 0)

#define luaZ_buffer(buff)	((buff)->buffer)
#define luaZ_sizebuffer(buff)	((buff)->buffsize)
#define luaZ_bufflen(buff)	((buff)->n)

#define luaZ_buffremove(buff,i)	((buff)->n -= (i))
#define luaZ_resetbuffer(buff) ((buff)->n = 0)


/**
 * 为 buff 重新分配 size 字节大小的空间
 */
#define luaZ_resizebuffer(L, buff, size) \
	((buff)->buffer = luaM_reallocvchar(L, (buff)->buffer, \
				(buff)->buffsize, size), \
	(buff)->buffsize = size)

/* buff 为 Mbuffer * 类型 */
#define luaZ_freebuffer(L, buff)	luaZ_resizebuffer(L, buff, 0)


LUAI_FUNC char *luaZ_openspace (lua_State *L, Mbuffer *buff, size_t n);
LUAI_FUNC void luaZ_init (lua_State *L, ZIO *z, lua_Reader reader,
                                        void *data);
LUAI_FUNC size_t luaZ_read (ZIO* z, void *b, size_t n);	/* read next n bytes */



/* --------- Private Part ------------------ */

struct Zio {
  size_t n;			/* bytes still unread */
  const char *p;		/* current position in buffer */
  lua_Reader reader;		/* reader function */
  /* 数据块是用 reader 函数来读取 */
  void *data;			/* additional data */
  lua_State *L;			/* Lua state (for reader) */
};


LUAI_FUNC int luaZ_fill (ZIO *z);

#endif
