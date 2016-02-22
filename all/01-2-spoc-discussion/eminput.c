#include <u.h>

enum { // processor fault codes
  FMEM,   // bad physical address
  FTIMER, // timer interrupt
  FKEYBD, // keyboard interrupt
  FPRIV,  // privileged instruction
  FINST,  // illegal instruction
  FSYS,   // software trap
  FARITH, // arithmetic trap
  FIPAGE, // page fault on opcode fetch
  FWPAGE, // page fault on write
  FRPAGE, // page fault on read
  USER=16 // user mode exception 
};

int in(port)    { asm(LL,8); asm(BIN); }
ivec(void *isr) { asm(LL,8); asm(IVEC); }

out(port, val)
{
  asm(LL,8);   // load register a with port
  asm(LBL,16); // load register b with val
  asm(BOUT);   // output byte to console
}

write(int f, char *s, int n)
{
  while (n--)
    out(f, *s++);
}  

trap(int c, int b, int a, int fc, int pc)
{
  // printf("TRAP: ");

  switch (fc) {
  case FKEYBD:
    out(1, in(0)); 
    break;
  }
}

alltraps()
{
  asm(PSHA);
  asm(PSHB);
  asm(PSHC);
  trap();
  asm(POPC);
  asm(POPB);
  asm(POPA);
  asm(RTI);
}
  
main()
{
  ivec(alltraps);
  asm(STI);
  while(1) asm(IDLE);
  asm(HALT);
}
