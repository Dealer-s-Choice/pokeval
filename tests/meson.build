cases = [
  'test',
  'bringin',
  'compare_hands',
  'nothing',
  'pair',
  'straight',
  'sort_hand',
  '6-card-hand',
  '7-card-hand',
  'four-of-a-kind',
  'five-of-a-kind',
  'wild',
]

foreach test : cases

  exe = executable(
    'test_' + test,
    [test + '.c', '00_test.c'],
    dependencies: pokeval_dep,
    override_options: ['b_lto=false'],
  )
  test('test_' + test, exe)
endforeach
