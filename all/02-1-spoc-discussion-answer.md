###3.4 linux系统调用分析
####1

以x86为例，Linux应用程序发出系统调用请求时，将调用号存入eax，前五个参数按照顺序存放入寄存器ebx, ecx, edx, esi和edi，随后触发系统调用中断“int $0x80”，即进入内核态中系统调用处理程序。返回时，返回值存入eax中，回到用户态。

* objdump：从elf或者二进制机器码文件中反编译出汇编指令
* nm：从elf文件中获取符号信息，如所有函数（非static）所在的地址等，在逆向工程时是很好的分析手段。
* file：根据文件内容猜测文件的格式，通常利用文件头部的数据，基于libmagic中的数据库判断类型

####2

strace是一个跟踪系统调用的工具，是用strace运行目标程序，将依次输出目标程序的系统调用及其参数。可以看到，执行一个程序时，最先的调用为execve，即运行文件。随后用brk分配内存。然后是加载各种动态库，这些步骤包括access、fstat等获取文件信息的调用，和mmap调用映射文件到内存。随后还通过mprotect等调用设置内存段的权限（比如数据段不可执行）。之后就是程序本身的逻辑。程序退出过程包括munmap取消映射、close关闭文件等，最终通过exit_group退出进程。

strace是基于ptrace系统调用实现的，ptrace可以实现一种hook机制，对于一个调用的了ptrace(PTRACE_TRACEME)的子进程，它对于内核的调用请求会提前通知父进程，父进程可以修改参数、阻止调用等。

利用ptrace可以构建沙箱，我曾经在开发一套OJ系统时就使用了这种方法：https://github.com/593141477/bashu-onlinejudge/blob/master/daemon/libsablebox/sablebox.c
