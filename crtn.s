.section .init
  /* GCC will put content here. */
  popl %ebp
  ret

.section .fini
  /* GCC will put content here. */
  popl %ebp
  ret