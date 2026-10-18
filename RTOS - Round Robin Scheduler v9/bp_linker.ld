ENTRY(Reset_Handler)

MEMORY
{
	FLASH(rx):ORIGIN =0x08000000,LENGTH =128K
	SRAM(rwx):ORIGIN =0x20000000,LENGTH =20K
}

/*initial stack pointer*/
_estack = ORIGIN(SRAM) + LENGTH(SRAM);
/*minimum stack size*/
_Min_Stack_Size = 0x400;


SECTIONS
{
	.text :
	{	
		*(.isr_vector)
		*(.text*)
		*(.rodata*)
		_etext = .;
	}> FLASH

	_la_data = LOADADDR(.data);
	.data :
	{	
		_sdata = .;
		*(.data)
		*(.data*)
		_edata = .;
	}> SRAM AT> FLASH
	
	.bss :
	{
		_sbss = .;
		__bss_start__ = .;
		*(.bss)
		*(.bss*)
		_ebss = .;
		__bss_end__ = .;
		. = ALIGN(4);
		end = .;
	}> SRAM

	/*user stack section reserves some space, linker will fail at compile time if .bss + minimum stack doesn't fit in stack*/
	._user_stack :
	{
		. = ALIGN(8);
		_sstack = .;
		. = . + _Min_Stack_Size;
		. = ALIGN(8);
	}> SRAM
}