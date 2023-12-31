
/* Example hash function with terrible performance */
unsigned long hash_too_simple(const unsigned char *str);

/* source: https://en.wikipedia.org/wiki/Adler-32 */
unsigned long hash_adler(const unsigned char *str);

/* source: https://en.wikipedia.org/wiki/Fletcher%27s_checksum */
unsigned long hash_fletcher(const unsigned char* str);

/* source: https://en.wikipedia.org/wiki/MurmurHash */
unsigned long hash_murmur(const unsigned char* str);

/* Add the header for your own added hash functions here. You may search online
 * for existing solutions for hashing function, as long as you as you
 * attribute the source, meaning links to the used material.  */
