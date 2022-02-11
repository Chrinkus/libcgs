tests = [
  ['array', 'tests_array.c'],
  ['bst', 'tests_bst.c'],
  ['rbt', 'tests_rbt.c'],
  ['rbt_private', 'tests_rbt_private.c'],
  ['variant', 'tests_variant.c'],
  ['variant_usage', 'tests_variant_usage.c'],
  ['string_utils', 'tests_string_utils.c']
]

foreach t : tests
  e = executable(t[0], t[1],
    include_directories : '../cgs',
    link_with : cgs)

  test(t[0], e)
endforeach

