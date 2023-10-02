#ifndef BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_TYPEREF_H_
#define BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_TYPEREF_H_

#include "backends/p4tools/modules/smith/util/util.h"
#include "ir/ir.h"

namespace P4Tools {

namespace P4Smith {

using TyperefProbs = struct TyperefProbs {
    int64_t p4_bit;
    int64_t p4_signed_bit;
    int64_t p4_varbit;
    int64_t p4_int;
    int64_t p4_error;
    int64_t p4_bool;
    int64_t p4_string;
    // derived types
    int64_t p4_enum;
    int64_t p4_header;
    int64_t p4_header_stack;
    int64_t p4_struct;
    int64_t p4_header_union;
    int64_t p4_tuple;
    int64_t p4_void;
    int64_t p4_match_kind;
};

class TypeRef {
 public:
    const char *types[5] = {
        "baseType", "typeName",
        "specializedType",  // not
        "headerStackType",
        "tupleType"  // not
    };

    TypeRef() {}
    ~TypeRef() {}
    static IR::Type *pick_rnd_type(TyperefProbs);
};
}  // namespace P4Smith

}  // namespace P4Tools

#endif /* BACKENDS_P4TOOLS_MODULES_SMITH_COMMON_TYPEREF_H_ */
