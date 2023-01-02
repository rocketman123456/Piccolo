use eigen as runtime matrix math library, and use custom math lib for reflection,
key components that use math will convert to eigen math lib first, then use it in runtime, 
and convert back to custom math lib if serialization is needed

eigen math lib is much faster than our custom math lib, and has more function, which an be used in animation calculation, etc.
