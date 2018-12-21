#include "asm.h"
.code16
.globl vbe_switch_mode
vbe_switch_mode:
  	movw    $0x4f02,%ax
  	movw    $GRAPHIC_MODE,%bx
  	int     $0x10

  	movw    $GRAPHIC_BUF,%bx
  	movw    %bx, %es
  	movw    $0x0000,%di
  	movw    $0x4f01,%ax
  	movw    $GRAPHIC_MODE,%cx
  	int     $0x10
	cmp		$0x4F,%ax
	jne		error
	ret
error:
	stc
	jmp error