tests = [
  ['bst', 'tests_bst.c'],
  ['variant', 'tests_variant.c'],
  ['variant_usage', 'tests_variant_usage.c']
]

foreach t : tests
  e = executable(t[0], t[1],
    include_directories : inc,
    link_with : cgs)

  test(t[0], e)
endforeach

