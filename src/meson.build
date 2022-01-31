version = meson.project_version()

sources = [ 'cgs_bst.c', 'cgs_variant.c' ]

cgs = library('cgs', sources,
  include_directories : inc,
  version : version,
  soversion : version.split('.')[0])

