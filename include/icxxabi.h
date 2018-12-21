#pragma once

#define _ICXXABI_H

#define ATEXIT_MAX_FUNCS	128

#ifdef __cplusplus
extern "C" {
#endif
	__extension__ typedef int __guard __attribute__((mode(__DI__)));


	typedef unsigned uarch_t;

	struct atexit_func_entry_t
	{
		/*
		* Each member is at least 4 bytes large. Such that each entry is 12bytes.
		* 128 * 12 = 1.5KB exact.
		**/
		void(*destructor_func)(void *);
		void *obj_ptr;
		void *dso_handle;
	};

	int __cxa_atexit(void(*f)(void *), void *objptr, void *dso);
	void __cxa_finalize(void *f);

	void *__gxx_personality_v0 = nullptr;
	void *_Unwind_Resume = nullptr;

	int __cxa_guard_acquire(__guard *);
	void __cxa_guard_release(__guard *);
	void __cxa_guard_abort(__guard *);

	int __cxa_guard_acquire(__guard *g)
	{
		return !*(char *)(g);
	}

	void __cxa_guard_release(__guard *g)
	{
		*(char *)g = 1;
	}

	void __cxa_guard_abort(__guard *)
	{

	}
#ifdef __cplusplus
};
#endif
