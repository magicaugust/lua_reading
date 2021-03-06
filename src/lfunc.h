/*
** $Id: lfunc.h,v 2.14 2014/06/19 18:27:20 roberto Exp $
** Auxiliary functions to manipulate prototypes and closures
** See Copyright Notice in lua.h
*/

#ifndef lfunc_h
#define lfunc_h


#include "lobject.h"


/**
 * 有 n 个 upvalues 的C闭包的大小
 */
#define sizeCclosure(n)	(cast(int, sizeof(CClosure)) + \
                         cast(int, sizeof(TValue)*((n)-1)))

/**
 * 有 n 个 upvalues 的Lua闭包的大小
 */
#define sizeLclosure(n)	(cast(int, sizeof(LClosure)) + \
                         cast(int, sizeof(TValue *)*((n)-1)))


/* test whether thread is in 'twups' list */
#define isintwups(L)	(L->twups != L)


/*
** Upvalues for Lua closures
*/
struct UpVal {
  TValue *v;  /* points to stack or to its own value */
  lu_mem refcount;  /* reference counter */
  union {
    struct {  /* (when open) */
	  /* 将栈上所有 upvalue 链接在一起, lua_State 中的 openupval 是链表的
	   * 头指针 
	   */
      UpVal *next;  /* linked list */
	  /*
	   * gc traverseLclosure 函数中当 upvalue 为 open 时且 gc 不在 atomic 
	   * phase 时, 将其置为 1 
	   */
      int touched;  /* mark to avoid cycles with dead threads */
    } open;
	/* close 之后 v 就指向这个值, 即 its own value */
    TValue value;  /* the value (when closed) */
  } u;
};

/**
 * open 的意思是当前的 upvalue 还在栈上, 即上层函数还没有返回
 */
#define upisopen(up)	((up)->v != &(up)->u.value)


LUAI_FUNC Proto *luaF_newproto (lua_State *L);
/**
 * 新建一个有 n 个 upvalues 的闭包
 */
LUAI_FUNC CClosure *luaF_newCclosure (lua_State *L, int nelems);
LUAI_FUNC LClosure *luaF_newLclosure (lua_State *L, int nelems);
LUAI_FUNC void luaF_initupvals (lua_State *L, LClosure *cl);
LUAI_FUNC UpVal *luaF_findupval (lua_State *L, StkId level);
LUAI_FUNC void luaF_close (lua_State *L, StkId level);
LUAI_FUNC void luaF_freeproto (lua_State *L, Proto *f);
/*
** Look for n-th local variable at line 'line' in function 'func'.
** Returns NULL if not found.
*/
/**
 * 获取在执行指令 pc 位置(相对位置)可见的局部变量中的第 local_number 
 * 个局部变量
 */
LUAI_FUNC const char *luaF_getlocalname (const Proto *func, int local_number,
                                         int pc);


#endif
