SECTIONS { 
  firtst  	0x0 : { head.o init.o  nand.o }
  second		0x30001000 : AT(4096)	{main.o}
} 
 
