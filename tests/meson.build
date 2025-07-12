cases = [
  'test',
  'compare_hands',
  'nothing',
  'pair',
  'straight',
  'sort_hand',
  '7-card-hand',
  'five-of-a-kind'
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
