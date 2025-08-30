ENTRY(Reset_Handler)

MEMORY
{
	FLASH(rx):ORIGIN =0x08000000,LENGTH =128K
	SRAM(rwx):ORIGIN =0x20000000,LENGTH =20K
}

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
}