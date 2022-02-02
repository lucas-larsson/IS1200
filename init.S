# init.S
# Based on Labwork.S - 2015-2017 by F. Lundevall
# Repurposed by Alex Lindberg

.global disable_interrupt
.global enable_interrupt

.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
  #
  # Please add your own code below this line
  #

disable_interrupt:
	# ei enables interupts
	di
	jr $ra
	nop

enable_interrupt:
	# ei enables interupts
	ei
	jr $ra
	nop