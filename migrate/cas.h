#pragma once
#include <database/field>
#include <database/record.h>
//Nomenclatura adotada pela Comissão da Farmacopeia Brasileira, baseada em referências científicas, 
//na ausência do número de registro CAS (Chemical Abstract Service Registry Number) e dos outros compêndios.
struct Cas {
    std::string tableName = "public.cas";
    IntegerField id{"id", true};
    StringField nome{"nome", false};

    DEFINE_FIELDS(id, nome)
};
