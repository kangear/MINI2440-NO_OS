SECTIONS
{
	. = 0x0;
	.text : {
		start.o
		* (.text)
	}
	.bss : {
		* (.bss)
	}
	.data : {
		* (.data)
	}	
}
