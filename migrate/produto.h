#pragma once
#include <database/field>
#include <database/record.h>

struct Produtos {
    std::string tableName = "public.produtos";
    IntegerField id{"id", true};
    StringField nome{"nome", false};
    IntegerField unidades_id{"unidades_id", false};
    StringField codigobarra{"codigobarra", false};
    IntegerField fabricantes_id{"fabricantes_id", false};
    MoneyField estoque_atual{"estoque_atual", false};
    StringField ncm{"ncm", false};
    IntegerField qtd_embalagem{"qtd_embalagem", false};
    StringField registro_ms{"registro_ms", false};
    IntegerField grupos_terapeuticos_id{"grupos_terapeuticos_id", false};
    StringField produto_referencia{"produto_referencia", false};
    IntegerField dcb_id{"dcb_id", false};
    MoneyField vl_fabrica{"vl_fabrica", false};
    MoneyField vl_custo{"vl_custo", false};
    MoneyField vl_venda{"vl_venda", false};
    MoneyField vl_ult_comp{"vl_ult_comp", false};
    MoneyField vl_ult_ven{"vl_ult_ven", false};
    DateTimeField dt_ult_comp{"dt_ult_comp", false};
    DateTimeField dt_ult_ven{"dt_ult_ven", false};
    DateTimeField dt_atualizacao{"dt_atualizacao", false};
    DateTimeField dt_cadastro{"dt_cadastro", false};
    IntegerField tt_entrada{"tt_entrada", false};
    IntegerField tt_saida{"tt_saida", false};
    IntegerField tt_descarte{"tt_descarte", false};
    BoolField ativo{"ativo", false};

    DEFINE_FIELDS(id, nome, unidades_id, codigobarra, fabricantes_id, estoque_atual, ncm, qtd_embalagem, registro_ms, grupos_terapeuticos_id, produto_referencia, dcb_id, vl_fabrica, vl_custo, vl_venda, vl_ult_comp, vl_ult_ven, dt_ult_comp, dt_ult_ven, dt_atualizacao, dt_cadastro, tt_entrada, tt_saida, tt_descarte, ativo)
};
