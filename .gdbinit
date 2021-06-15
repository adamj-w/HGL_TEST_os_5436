set architecture i386
set disassembly-flavor intel
symbol-file build/hegel-x86_32-develop/kernel.bin
target remote localhost:1234
break arch_main
#layout split
continue
