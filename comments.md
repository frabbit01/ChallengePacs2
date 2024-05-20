comments in the source indicated by @note.

There are quite a few errors and you made things uselessly complicated, in partiocular by specialising a whole class. It was sufficient to specialize some methods or even, just using if constexpr wisely.

I appreciated the doxygen documentation and the matrix reader.  However, look at the warnings. The one in mmio could have been silenced with the @pragma GCC DIAGNOSTIC< ass seen at lecture, but the other warnings were telling you comething, in particular
```bash
Matrix.hpp:110:36: warning: ‘default_t’ may be used uninitialized [-Wmaybe-uninitialized]
  110 |                         COOmap[key]=default_t;
      |                         ~~~~~~~~~~~^~~~~~~~~
```
If the compiler complains about an initialized variable it is better having a look. Sometimes it is irrelevant, other times (like in your code) it is not!



If you want to verify the result instead of printing long lines of number you could have printed the norm of the vector. Having long list of numbers on the terminal doens't help much.