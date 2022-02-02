install_headers(
  'cgs.h',
  'cgs_bst.h',
  'cgs_string_utils.h',
  'cgs_test.h',
  'cgs_variant.h',
  subdir : 'cgs'
)

sources = [
  'cgs_bst.c',
  'cgs_string_utils.c',
  'cgs_variant.c'
]

version = meson.project_version()

cgs = library('cgs', sources,
  version : version,
  soversion : version.split('.')[0])

