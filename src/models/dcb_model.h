#pragma once
#include <database/field>
#include <database/record.h>

struct Dcb {
    std::string tableName = "public.dcb";
    IntegerField id{"id", true};
    StringField nome{"nome", false};

    DEFINE_FIELDS(id, nome)
};
