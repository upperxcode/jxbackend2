#pragma once
#include <database/field>
#include <database/record.h>

struct Produto {
  std::string tableName = "Produtos";
  IntegerField id{"id", true};
  StringField nome{"nome"};
  IntegerField unidade_id{"unidades_id"};
  StringField codigobarra{"codigobarra"};
  IntegerField fabricante{"fabricantes_id"};
  FloatField estoque_atual{"estoque_atual"};
  StringField ncm{"ncm"};
  IntegerField qtd_embalagem{"qtd_embalagem"};
  StringField registro_ms_id{"registro_ms"};
  IntegerField grupoTerapeutico{"grupos_terapeuticos_id"};
  StringField produtoReferencia{"produto_referencia"};
  FloatField dcb{"dcb_id"};
  MoneyField vlFabrica{"vl_fabrica"};
  MoneyField vlCusto{"vl_custo"};
  MoneyField vlVenda{"vl_venda"};
  MoneyField vlUltComp{"vl_ult_comp"};
  MoneyField vlUltVen{"vl_ult_ven"};
  DateTimeField dtAtualizacao{"dt_atualizacao"};
  DateTimeField dtCadastro{"dt_cadastro"};
  IntegerField ttEntrada{"tt_entrada"};
  IntegerField ttSaida{"tt_saida"};
  IntegerField ttDescarte{"tt_descarte"};
  BoolField ativo{"ativo"};

  DEFINE_FIELDS(id, nome, unidade_id, codigobarra, fabricante, estoque_atual,
                ncm, qtd_embalagem, registro_ms_id, grupoTerapeutico,
                produtoReferencia, dcb, vlFabrica, vlCusto, vlUltComp, vlUltVen,
                vlVenda, dtAtualizacao, dtCadastro, ttEntrada, ttSaida,
                ttDescarte, ativo)
};
