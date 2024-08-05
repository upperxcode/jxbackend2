import re
import argparse

def parse_ddl(ddl_file):
    with open(ddl_file, 'r') as file:
        ddl_content = file.read()
    
    table_name_match = re.search(r'CREATE TABLE (\w+\.\w+)', ddl_content)
    if table_name_match:
        table_name = table_name_match.group(1)
    else:
        raise ValueError("Nome da tabela não encontrado na DDL.")

    # Regex para capturar os campos da tabela, incluindo tipos com parênteses
    fields = re.findall(r'(\w+)\s+(\w+(?:\(\d+(?:,\s*\d+)?\))?)', ddl_content)

    if not fields:
        raise ValueError("Nenhum campo encontrado na DDL.")

    return table_name, fields

def map_field_type(pg_type):
    type_mapping = {
        'serial': 'IntegerField',
        'int4': 'IntegerField',
        'integer': 'IntegerField',
        'varchar': 'StringField',
        'bpchar': 'StringField',
        'float': 'FloatField',
        'numeric': 'MoneyField',  # Mapeando numeric para MoneyField
        'money': 'MoneyField',
        'timestamp': 'DateTimeField',
        'date': 'DateTimeField',  # Mapeando date para DateTimeField
        'bool': 'BoolField',
        'boolean': 'BoolField'
    }
    
    # Remove os parâmetros entre parênteses para fazer o mapeamento
    base_type = re.sub(r'\(.*\)', '', pg_type).lower()
    return type_mapping.get(base_type, 'UnknownField')

def generate_cpp_struct(table_name, fields):
    struct_name = table_name.split('.')[-1].capitalize()
    cpp_code = f'#pragma once\n#include <database/field>\n#include <database/record.h>\n\n'
    cpp_code += f'struct {struct_name} {{\n'
    cpp_code += f'    std::string tableName = "{table_name}";\n'

    valid_fields = []
    for field_name, field_type in fields:
        cpp_type = map_field_type(field_type)
        if cpp_type != 'UnknownField':
            is_primary_key = 'true' if field_name == 'id' else 'false'
            cpp_code += f'    {cpp_type} {field_name}{{"{field_name}", {is_primary_key}}};\n'
            valid_fields.append(field_name)
        else:
            print(f"Tipo de campo desconhecido: {field_name} {field_type}")

    field_names = ', '.join(valid_fields)
    cpp_code += f'\n    DEFINE_FIELDS({field_names})\n'
    cpp_code += '};\n'

    return cpp_code

def main():
    parser = argparse.ArgumentParser(description='Gera uma estrutura C++ baseada em uma DDL do PostgreSQL.')
    parser.add_argument('input', type=str, help='Arquivo de entrada contendo a DDL.')
    parser.add_argument('output', type=str, help='Arquivo de saída para a estrutura C++ gerada.')
    args = parser.parse_args()

    try:
        table_name, fields = parse_ddl(args.input)
        print(f"Nome da tabela: {table_name}")
        print(f"Campos encontrados: {fields}")
        
        cpp_code = generate_cpp_struct(table_name, fields)

        with open(args.output, 'w') as file:
            file.write(cpp_code)

        print(f'Struct C++ gerada com sucesso em {args.output}.')
    except Exception as e:
        print(f"Erro: {e}")

if __name__ == '__main__':
    main()