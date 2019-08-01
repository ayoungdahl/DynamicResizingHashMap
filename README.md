# DynamicResizingHashMap
Here's my fun project to roll my own C++ hashmap.  The great thing about STL is its available to use.  The bad thing about STL is
that we never get a chance to code our own structures.

* The key and value are templated.
* If we use a class for which there is no std::hash, then we can also supply our own hash and key equals functions.
* Insert and Get are constant time
  * Each hash bucket has a fixed size chain in which to store keys and values.
  * If a kv-chain overflows, the number of buckets increases.
  
Next up is to add more of the STL provided functionality...  clear, etc. 

BUT first:
There is a problem with this implementation which I must fix!  I'd like to emulate the behavior of STL's iterator which allows mutation of the value from the dereferenced iterator.  But the key must be const... we can't let iterators held by callers change the set of keys held by the map outside of the map's control.  

Currently I'm in a bind and this behavior is allowed.  In order to allow the iterator to reference a pair of K and V I'm storing K and V in a std::pair.  Ofcourse this permits the holder of the iterator to mutate K.  A nice solution would be to make K const.  However, a std::pair<const K, V> doesn't interact well with the backing std::vector used to hold the map's nodes.  std::vector seems to require that inserted nodes must have a valid assignment operator.  Sensible, but the const in our pair disallows assignment.

I think the easiest solution is to store my map nodes either in my own array or my own non-std linked list.  The first will likely be faster at the expense of increased storage, while the second would likely be the opposite.

Next up-- make a decision and implement!
