/**
 * @file toy_literal.h
 * @brief
 *
 */

#ifndef TOY_LITERAL_H_
#define TOY_LITERAL_H_

#include <stdbool.h>

#include "toy_common.h"
#include "toy_refstring.h"
#include "toy_reffunction.h"

struct Toy_Literal;
struct Toy_Interpreter;
struct Toy_LiteralArray;
struct Toy_LiteralDictionary;
struct Toy_Scope;

/**
 * @fn int (*Toy_NativeFn)(struct Toy_Interpreter* interpreter, struct Toy_LiteralArray* arguments)
 * @brief
 *
 * @param interpreter
 * @param arguments
 * @return
 */
typedef int (*Toy_NativeFn)(struct Toy_Interpreter* interpreter, struct Toy_LiteralArray* arguments);

/**
 * @fn int (*Toy_HookFn)(struct Toy_Interpreter* interpreter, struct Toy_Literal identifier, struct Toy_Literal alias)
 * @brief
 *
 * @param interpreter
 * @param identifier
 * @param alias
 * @return
 */
typedef int (*Toy_HookFn)(struct Toy_Interpreter* interpreter, struct Toy_Literal identifier, struct Toy_Literal alias);

/**
 * @fn void (*Toy_PrintFn)(const char*)
 * @brief
 *
 * @param
 */
typedef void (*Toy_PrintFn)(const char*);

/**
 * @enum Toy_LiteralType
 * @brief
 *
 */
typedef enum {
    TOY_LITERAL_NULL,       /**< TOY_LITERAL_NULL */
    TOY_LITERAL_BOOLEAN,    /**< TOY_LITERAL_BOOLEAN */
    TOY_LITERAL_INTEGER,    /**< TOY_LITERAL_INTEGER */
    TOY_LITERAL_FLOAT,      /**< TOY_LITERAL_FLOAT */
    TOY_LITERAL_STRING,     /**< TOY_LITERAL_STRING */
    TOY_LITERAL_ARRAY,      /**< TOY_LITERAL_ARRAY */
    TOY_LITERAL_DICTIONARY, /**< TOY_LITERAL_DICTIONARY */
    TOY_LITERAL_FUNCTION,   /**< TOY_LITERAL_FUNCTION */
    TOY_LITERAL_IDENTIFIER, /**< TOY_LITERAL_IDENTIFIER */
    TOY_LITERAL_TYPE,       /**< TOY_LITERAL_TYPE */
    TOY_LITERAL_OPAQUE,     /**< TOY_LITERAL_OPAQUE */
    TOY_LITERAL_ANY,        /**< TOY_LITERAL_ANY */

    //these are meta-level types - not for general use
    TOY_LITERAL_TYPE_INTERMEDIATE,       /**< used to process types in the compiler only */
    TOY_LITERAL_ARRAY_INTERMEDIATE,      /**< used to process arrays in the compiler only */
    TOY_LITERAL_DICTIONARY_INTERMEDIATE, /**< used to process dictionaries in the compiler only */
    TOY_LITERAL_FUNCTION_INTERMEDIATE,   /**< used to process functions in the compiler only */
    TOY_LITERAL_FUNCTION_ARG_REST,       /**< used to process function rest parameters only */
    TOY_LITERAL_FUNCTION_NATIVE,         /**< for handling native functions only */
    TOY_LITERAL_FUNCTION_HOOK,           /**< for handling hook functions within literals only */
    TOY_LITERAL_INDEX_BLANK,             /**< for blank indexing i.e. arr[:] */
} Toy_LiteralType;

/**
 * @typedef Toy_Literal
 * @brief
 *
 */
typedef struct Toy_Literal {
    union {
        bool boolean; /**< 1 */
        int integer;  /**< 4 */
        float number; /**< 4 */

        struct {
            Toy_RefString* ptr; /**< 8 */
            //string hash?
        } string; /**< 8 */

        struct Toy_LiteralArray* array;           /**< 8 */
        struct Toy_LiteralDictionary* dictionary; /**< 8 */

        struct {
            union {
                Toy_RefFunction* ptr; /**< 8 */
                Toy_NativeFn native;  /**< 8 */
                Toy_HookFn hook;      /**< 8 */
            } inner; /**< 8 */
            struct Toy_Scope* scope; /**< */
        } function; /**< 16 */

        struct { //for variable names
            Toy_RefString* ptr; /**< 8 */
            int hash;           /**< 4 */
        } identifier; /**< 16 */

        struct {
            struct Toy_Literal* subtypes; /**< 8 */
            Toy_LiteralType typeOf;       /**< 4 */
            unsigned char capacity;       /**< 1 */
            unsigned char count;          /**< 1 */
            bool constant; /**< 1 */
        } type; /**< 16 */

        struct {
            void* ptr; /**< 8 */
            int tag;   /**< 4 */
        } opaque; /**< 16 */

        void* generic; /**< 8 */
    } as; /**< 16 */

    Toy_LiteralType type; /**< 4 */
    //4 - unused
    //shenanigans with byte alignment reduces the size of Toy_Literal
} Toy_Literal;

#define TOY_IS_NULL(value)                     ((value).type == TOY_LITERAL_NULL)     /**<  */
#define TOY_IS_BOOLEAN(value)                  ((value).type == TOY_LITERAL_BOOLEAN) /**<  */
#define TOY_IS_INTEGER(value)                  ((value).type == TOY_LITERAL_INTEGER) /**<  */
#define TOY_IS_FLOAT(value)                    ((value).type == TOY_LITERAL_FLOAT) /**<  */
#define TOY_IS_STRING(value)                   ((value).type == TOY_LITERAL_STRING) /**<  */
#define TOY_IS_ARRAY(value)                    ((value).type == TOY_LITERAL_ARRAY) /**<  */
#define TOY_IS_DICTIONARY(value)               ((value).type == TOY_LITERAL_DICTIONARY) /**<  */
#define TOY_IS_FUNCTION(value)                 ((value).type == TOY_LITERAL_FUNCTION) /**<  */
#define TOY_IS_FUNCTION_NATIVE(value)          ((value).type == TOY_LITERAL_FUNCTION_NATIVE) /**<  */
#define TOY_IS_FUNCTION_HOOK(value)            ((value).type == TOY_LITERAL_FUNCTION_HOOK) /**<  */
#define TOY_IS_IDENTIFIER(value)               ((value).type == TOY_LITERAL_IDENTIFIER) /**<  */
#define TOY_IS_TYPE(value)                     ((value).type == TOY_LITERAL_TYPE) /**<  */
#define TOY_IS_OPAQUE(value)                   ((value).type == TOY_LITERAL_OPAQUE) /**<  */

#define TOY_AS_BOOLEAN(value)                  ((value).as.boolean) /**<  */
#define TOY_AS_INTEGER(value)                  ((value).as.integer) /**<  */
#define TOY_AS_FLOAT(value)                    ((value).as.number) /**<  */
#define TOY_AS_STRING(value)                   ((value).as.string.ptr) /**<  */
#define TOY_AS_ARRAY(value)                    ((Toy_LiteralArray*)((value).as.array)) /**<  */
#define TOY_AS_DICTIONARY(value)               ((Toy_LiteralDictionary*)((value).as.dictionary)) /**<  */
#define TOY_AS_FUNCTION(value)                 ((value).as.function) /**<  */
#define TOY_AS_FUNCTION_NATIVE(value)          ((value).as.function.inner.native) /**<  */
#define TOY_AS_FUNCTION_HOOK(value)            ((value).as.function.inner.hook) /**<  */
#define TOY_AS_IDENTIFIER(value)               ((value).as.identifier.ptr) /**<  */
#define TOY_AS_TYPE(value)                     ((value).as.type) /**<  */
#define TOY_AS_OPAQUE(value)                   ((value).as.opaque.ptr) /**<  */

#define TOY_TO_NULL_LITERAL                    ((Toy_Literal){{ .integer = 0 }, TOY_LITERAL_NULL}) /**<  */
#define TOY_TO_BOOLEAN_LITERAL(value)          ((Toy_Literal){{ .boolean = value }, TOY_LITERAL_BOOLEAN}) /**<  */
#define TOY_TO_INTEGER_LITERAL(value)          ((Toy_Literal){{ .integer = value }, TOY_LITERAL_INTEGER}) /**<  */
#define TOY_TO_FLOAT_LITERAL(value)            ((Toy_Literal){{ .number = value }, TOY_LITERAL_FLOAT}) /**<  */
#define TOY_TO_STRING_LITERAL(value)           ((Toy_Literal){{ .string = { .ptr = value }},TOY_LITERAL_STRING}) /**<  */
#define TOY_TO_ARRAY_LITERAL(value)            ((Toy_Literal){{ .array = value }, TOY_LITERAL_ARRAY}) /**<  */
#define TOY_TO_DICTIONARY_LITERAL(value)       ((Toy_Literal){{ .dictionary = value }, TOY_LITERAL_DICTIONARY}) /**<  */
#define TOY_TO_FUNCTION_LITERAL(value)         ((Toy_Literal){{ .function = { .inner = { .ptr = value }, .scope = NULL }}, TOY_LITERAL_FUNCTION}) /**<  */
#define TOY_TO_FUNCTION_NATIVE_LITERAL(value)  ((Toy_Literal){{ .function = { .inner = { .native = value }, .scope = NULL }}, TOY_LITERAL_FUNCTION_NATIVE}) /**<  */
#define TOY_TO_FUNCTION_HOOK_LITERAL(value)    ((Toy_Literal){{ .function = { .inner = { .hook = value }, .scope = NULL }}, TOY_LITERAL_FUNCTION_HOOK}) /**<  */
#define TOY_TO_IDENTIFIER_LITERAL(value)       Toy_private_toIdentifierLiteral(value) /**<  */
#define TOY_TO_TYPE_LITERAL(value, c)          ((Toy_Literal){{ .type = { .typeOf = value, .constant = c, .subtypes = NULL, .capacity = 0, .count = 0 }}, TOY_LITERAL_TYPE}) /**<  */
#define TOY_TO_OPAQUE_LITERAL(value, t)        ((Toy_Literal){{ .opaque = { .ptr = value, .tag = t }}, TOY_LITERAL_OPAQUE}) /**<  */

//BUGFIX: For blank indexing
#define TOY_IS_INDEX_BLANK(value)              ((value).type == TOY_LITERAL_INDEX_BLANK) /**<  */
#define TOY_TO_INDEX_BLANK_LITERAL             ((Toy_Literal){{ .integer = 0 }, TOY_LITERAL_INDEX_BLANK}) /**<  */

/**
 * @fn void Toy_freeLiteral(Toy_Literal literal)
 * @brief
 *
 * @param literal
 */
TOY_API void Toy_freeLiteral(Toy_Literal literal);

#define TOY_IS_TRUTHY(x)                       Toy_private_isTruthy(x) /**<  */

#define TOY_AS_FUNCTION_BYTECODE_LENGTH(lit)   (Toy_lengthRefFunction((lit).inner.ptr)) /**<  */

#define TOY_MAX_STRING_LENGTH                  4096 /**<  */
#define TOY_HASH_I(lit)                        ((lit).as.identifier.hash) /**<  */
#define TOY_TYPE_PUSH_SUBTYPE(lit, subtype)    Toy_private_typePushSubtype(lit, subtype) /**<  */
#define TOY_GET_OPAQUE_TAG(o)                  o.as.opaque.tag /**<  */

//BUGFIX: macros are not functions
/**
 * @fn bool Toy_private_isTruthy(Toy_Literal x)
 * @brief
 *
 * @param x
 * @return
 */
TOY_API bool Toy_private_isTruthy(Toy_Literal x);

/**
 * @fn Toy_Literal Toy_private_toIdentifierLiteral(Toy_RefString* ptr)
 * @brief
 *
 * @param ptr
 * @return
 */
TOY_API Toy_Literal Toy_private_toIdentifierLiteral(Toy_RefString* ptr);

/**
 * @fn Toy_Literal* Toy_private_typePushSubtype(Toy_Literal* lit, Toy_Literal subtype)
 * @brief
 *
 * @param lit
 * @param subtype
 * @return
 */
TOY_API Toy_Literal* Toy_private_typePushSubtype(Toy_Literal* lit, Toy_Literal subtype);

//utils
/**
 * @fn Toy_Literal Toy_copyLiteral(Toy_Literal original)
 * @brief
 *
 * @param original
 * @return
 */
TOY_API Toy_Literal Toy_copyLiteral(Toy_Literal original);

/**
 * @fn bool Toy_literalsAreEqual(Toy_Literal lhs, Toy_Literal rhs)
 * @brief
 *
 * @param lhs
 * @param rhs
 * @return
 */
TOY_API bool Toy_literalsAreEqual(Toy_Literal lhs, Toy_Literal rhs);

/**
 * @fn int Toy_hashLiteral(Toy_Literal lit)
 * @brief
 *
 * @param lit
 * @return
 */
TOY_API int Toy_hashLiteral(Toy_Literal lit);

//not thread-safe
/**
 * @fn void Toy_printLiteral(Toy_Literal literal)
 * @brief
 *
 * @param literal
 */
TOY_API void Toy_printLiteral(Toy_Literal literal);

/**
 * @fn void Toy_printLiteralCustom(Toy_Literal literal, Toy_PrintFn)
 * @brief
 *
 * @param literal
 * @param
 */
TOY_API void Toy_printLiteralCustom(Toy_Literal literal, Toy_PrintFn);

#endif /* TOY_LITERAL_H_ */
