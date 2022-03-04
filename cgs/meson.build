install_headers(
  'cgs.h',
  'cgs_array.h',
  'cgs_bst.h',
  'cgs_compare.h',
  'cgs_rbt.h',
  'cgs_rbt_private.h',
  'cgs_string_utils.h',
  'cgs_test.h',
  'cgs_variant.h',
  subdir : 'cgs'
)

sources = [
  'cgs_bst.c',
  'cgs_compare.c'
  'cgs_rbt.c',
  'cgs_string_utils.c',
  'cgs_variant.c'
]

version = meson.project_version()

cgs = library('cgs', sources,
  version : version,
  soversion : version.split('.')[0])

