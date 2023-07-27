#ifndef TOY_LITERAL_DICTIONARY_H_
#define TOY_LITERAL_DICTIONARY_H_

/*!
 # toy_literal_dictionary.h

 This header defines the dictionary structure (as well as the private entry structure), which manages a series of `Toy_Literal` instances stored in a key-value hash map. The dictionary does not take ownership of given literals, instead it makes an internal copy.

 The dictionary type is one of two fundemental data structures used throughout Toy - the other is the array.
 !*/

#include "toy_common.h"
#include "toy_literal.h"

/*!
 ## Defined Macros
 !*/

/*!
 ### TOY_DICTIONARY_MAX_LOAD

 If the contents of a dictionary exceeds this percentage of it's capacity, then a new buffer is created, the old contents are copied over one-by-one, and the original buffer is freed.

 Since this process can be memory and time intensive, a configurable macro is used to allow for fine-grained control across the lang.

 The current default value is `0.75`, representing 75% capacity.
 !*/

//TODO: benchmark this
#define TOY_DICTIONARY_MAX_LOAD 0.75

typedef struct Toy_private_dictionary_entry {
    Toy_Literal key;
    Toy_Literal value;
} Toy_private_dictionary_entry;

typedef struct Toy_LiteralDictionary {
    Toy_private_dictionary_entry *entries;
    int capacity;
    int count;
    int contains; //count + tombstones, for internal use
} Toy_LiteralDictionary;

/*!
 ## Defined Functions
 !*/

/*!
 ### void Toy_initLiteralDictionary(Toy_LiteralDictionary* dictionary)

 This function initializes the `Toy_LiteralDictionary` pointed to by `dictionary`.
 !*/
TOY_API void Toy_initLiteralDictionary(Toy_LiteralDictionary *dictionary);

/*!
 ### void Toy_freeLiteralDictionary(Toy_LiteralDictionary* dictionary)

 This function frees a `Toy_LiteralDictionary` pointed to by `dictionary`. Every literal within is passed to `Toy_freeLiteral()` before its memory is released.
 !*/
TOY_API void Toy_freeLiteralDictionary(Toy_LiteralDictionary *dictionary);

/*!
 ### void Toy_setLiteralDictionary(Toy_LiteralDictionary* dictionary, Toy_Literal key, Toy_Literal value)

 This function inserts the given key-value pair of literals into `dictionary`, creating it if it doesn't exist, or freeing and overwriting it if `key` is already present. This function may also expand the memory buffer if needed.

 When expanding the memory buffer, a full copy of the existing dictionary's contents is created - this can be memory intensive.

 Literal functions and opaques cannot be used as keys.
 !*/
TOY_API void Toy_setLiteralDictionary(Toy_LiteralDictionary *dictionary, Toy_Literal key, Toy_Literal value);

/*!
 ### Toy_Literal Toy_getLiteralDictionary(Toy_LiteralDictionary* dictionary, Toy_Literal key)

 This function returns the value of the literal within `dictionary` identified by `key`, or a null literal if it doesn't exist.

 Literal functions and opaques cannot be used as keys.
 !*/
TOY_API Toy_Literal Toy_getLiteralDictionary(Toy_LiteralDictionary *dictionary, Toy_Literal key);

/*!
 ### void Toy_removeLiteralDictionary(Toy_LiteralDictionary* dictionary, Toy_Literal key)

 This function removes the key-value pair of literals from `dictionary` identified by `key`, if it exists.

 Literal functions and opaques cannot be used as keys.
 !*/
TOY_API void Toy_removeLiteralDictionary(Toy_LiteralDictionary *dictionary, Toy_Literal key);

/*!
 ### bool Toy_existsLiteralDictionary(Toy_LiteralDictionary* dictionary, Toy_Literal key)

 This function returns true if the key-value pair identified by `key` exists within `dictionary`, otherwise it returns false.
 !*/
TOY_API bool Toy_existsLiteralDictionary(Toy_LiteralDictionary *dictionary, Toy_Literal key);

#endif /* TOY_LITERAL_DICTIONARY_H_ */
