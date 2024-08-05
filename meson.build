
project('jxbackend', 'cpp', 
  default_options : ['cpp_std=c++20'])


# Adiciona a definição da macro DEBUG
debug_define = ['-DDEBUG']

# Configurações de compilação para o modo de release
release_args = [
  '-O3',  # Otimizações para o modo de release
]

# Define os argumentos de compilação com base no tipo de build
if get_option('buildtype') == 'debug'
  add_project_arguments(debug_define, language: 'cpp')
else
  add_project_arguments(release_args, language: 'cpp')
endif

# Adiciona a biblioteca ormpp
crow_inc = include_directories('include/crow')
asio_inc = include_directories('include/asio')
resource_inc = include_directories('src/resources')
include = include_directories('include')

# Verifica se a opção enable_pg está ativada
enable_pg = get_option('enable_pg')

# Adicionando a dependência de pthread
pthread_dep = dependency('threads')

# Adiciona a dependência do PostgreSQL se enable_pg estiver ativada
postgres_deps = []
if enable_pg
  postgres_deps += [dependency('libpq', required : true)]
  postgres_deps += [dependency('libpqxx', required : true)]
  add_project_arguments('-DORMPP_ENABLE_PG', language: 'cpp')
endif

# Adiciona o executável principal
executable('jxbackend', 
  'src/main.cpp',
  include_directories : [crow_inc, asio_inc, resource_inc, include],
  dependencies : [postgres_deps, pthread_dep],
)