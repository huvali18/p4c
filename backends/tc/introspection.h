/*
Copyright (C) 2023 Intel Corporation

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing,
software distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions
and limitations under the License.
*/

#ifndef BACKENDS_TC_INTROSPECTION_H_
#define BACKENDS_TC_INTROSPECTION_H_

#include <iosfwd>
#include <optional>

#include "frontends/common/resolveReferences/referenceMap.h"
#include "frontends/p4/typeMap.h"
#include "ir/declaration.h"
#include "ir/ir.h"
#include "ir/visitor.h"
#include "lib/cstring.h"
#include "lib/json.h"
#include "lib/ordered_map.h"
#include "lib/safe_vector.h"

/// This file declares the different structures to be used in the introspection json file and the
/// pass to generate the file An introspection json file generated by the tc backend is used for
/// control plane programming by P4TC in Linux kernel
namespace TC {

struct IntrospectionInfo {
    cstring schemaVersion;
    cstring pipelineName;
    unsigned int pipelineId;
    IntrospectionInfo() {
        schemaVersion = nullptr;
        pipelineName = nullptr;
        pipelineId = 0;
    }
    void initIntrospectionInfo(IR::TCPipeline *tcPipeline) {
        schemaVersion = "1.0.0";
        pipelineName = tcPipeline->pipelineName;
        pipelineId = tcPipeline->pipelineId;
    }
};

struct KeyFieldAttributes {
    unsigned int id;
    cstring name;
    cstring type;
    cstring matchType;
    unsigned int bitwidth;
    KeyFieldAttributes() {
        id = 0;
        name = nullptr;
        type = nullptr;
        matchType = nullptr;
        bitwidth = 0;
    }
};

struct Annotation {
    cstring name;
    Annotation() { name = nullptr; }
    explicit Annotation(cstring n) { name = n; }
};

struct ActionParam {
    unsigned int id;
    cstring name;
    unsigned int dataType;
    unsigned int bitwidth;
    ActionParam() {
        id = 0;
        name = nullptr;
        bitwidth = 0;
    }
};

enum ActionScope { TableOnly, DefaultOnly, TableAndDefault };

struct ActionAttributes {
    unsigned int id;
    cstring name;
    ActionScope scope;
    bool defaultHit;
    bool defaultMiss;
    safe_vector<struct Annotation *> annotations;
    safe_vector<struct ActionParam *> actionParams;
    ActionAttributes() {
        id = 0;
        name = nullptr;
        scope = TableAndDefault;
        defaultHit = false;
        defaultMiss = false;
    }
};

struct TableAttributes {
    cstring name;
    unsigned int id;
    unsigned int tentries;
    unsigned int numMask;
    unsigned int keysize;
    unsigned int keyid;
    safe_vector<struct KeyFieldAttributes *> keyFields;
    safe_vector<struct ActionAttributes *> actions;
    TableAttributes() {
        name = nullptr;
        id = 0;
        tentries = 0;
        numMask = 0;
        keysize = 0;
        keyid = 0;
    }
};

/// This pass generates introspection JSON into user specified file
class IntrospectionGenerator : public Inspector {
    IR::TCPipeline *tcPipeline;
    P4::ReferenceMap *refMap;
    P4::TypeMap *typeMap;
    safe_vector<struct TableAttributes *> tablesInfo;
    ordered_map<cstring, const IR::P4Table *> p4tables;

 public:
    IntrospectionGenerator(IR::TCPipeline *tcPipeline, P4::ReferenceMap *refMap,
                           P4::TypeMap *typeMap)
        : tcPipeline(tcPipeline), refMap(refMap), typeMap(typeMap) {}
    void postorder(const IR::P4Table *t);
    const Util::JsonObject *genIntrospectionJson();
    void genTableJson(Util::JsonArray *tablesJson);
    Util::JsonObject *genTableInfo(struct TableAttributes *tbl);
    void collectTableInfo();
    void collectKeyInfo(const IR::Key *k, struct TableAttributes *tableinfo);
    void collectActionInfo(const IR::ActionList *actionlist, struct TableAttributes *tableinfo,
                           const IR::P4Table *p4table, const IR::TCTable *table);
    Util::JsonObject *genActionInfo(struct ActionAttributes *action);
    Util::JsonObject *genKeyInfo(struct KeyFieldAttributes *keyField);
    bool serializeIntrospectionJson(std::ostream &destination);
    std::optional<cstring> checkValidTcType(const IR::StringLiteral *sl);
    cstring externalName(const IR::IDeclaration *declaration);
};

}  // namespace TC

#endif /* BACKENDS_TC_INTROSPECTION_H_ */
