#pragma once
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
	void            consoleinit(void);
	void            cprintf(char*, ...);
	void            cprintf2(char*, ...);
	void            consoleintr(int(*)(void));
	void            panic(char*) __attribute__((noreturn));
#ifdef __cplusplus
}
#endif // __cplusplus
