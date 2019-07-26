# DynamicResizingHashMap
Here's my fun project to roll my own C++ hashmap.  The great thing about STL is its available to use.  The bad thing about STL is
that we never get a chance to code our own structures.

So far it has only simple functionality.
* The key and value are templated.
* If we use a class for which there is no std::hash, then we can also supply our own hash and key equals functions.
* Insert and Get are constant time
  * Each hash bucket has a fixed size chain in which to store keys and values.
  * If a kv-chain overflows, the number of buckets increases.
  
Next up is to add more of the STL provided functionality... iterator, clear, etc. 
