# this is the build script used by SCons
# maintained by zhangyafeikimi
#
# scons [debug==0] [shared=1]
#
debug = ARGUMENTS.get('debug', 1)
shared = ARGUMENTS.get('shared', 0)
env = Environment()
env = env.Clone()

if not env.GetOption('clean'):
  print 'building for %s' % env['PLATFORM']
  print 'debug = %s' % debug
  print 'shared = %s' % shared
  print 'clean = %s' % env.GetOption('clean')
else:
  shared = 1 #ensure to clean shared library

env.Append(CPPPATH = ['.', 'include'])
env.Append(LIBPATH = ['.'])

if env['PLATFORM'] == 'win32':
    env.Append(CPPPATH = ['port\win'])
    env.Append(CPPFLAGS = ['/DWIN32', '/DLEVELDB_PLATFORM_WINDOWS', '/DOS_WIN', '/DCOMPILER_MSVC'])
    env.Append(CCFLAGS = '/Zi /W3 /wd4996 /wd4800 /wd4355 /wd4244 /wd4018')
elif env['PLATFORM'] == 'posix':
    env.Append(CPPFLAGS = ['-DLEVELDB_PLATFORM_POSIX'])

if env['CC'] == 'cl':
    env.Append(CCFLAGS = '/EHsc')
    if int(debug) == 1:
        env.Append(CCFLAGS = '/Od /RTC1 /MTd')
        env.Append(CPPFLAGS = '/D_DEBUG')
    else:
        env.Append(CCFLAGS = '/O2 /Oi /MT /GL /Gy')
        env.Append(CPPFLAGS = ['/DNDEBUG'])
elif env['CC'] == 'gcc':
    env.Append(CCFLAGS = '-g -fno-builtin-memcmp -pthread')
    if int(debug) == 0:
        env.Append(CCFLAGS = '-O2')
        env.Append(CPPFLAGS = '-DNDEBUG')

SOURCE = [
    'db/builder.cc',
    'db/c.cc',
    'db/db_impl.cc',
    'db/db_iter.cc',
    'db/dbformat.cc',
    'db/filename.cc',
    'db/log_reader.cc',
    'db/log_writer.cc',
    'db/memtable.cc',
    'db/repair.cc',
    'db/table_cache.cc',
    'db/version_edit.cc',
    'db/version_set.cc',
    'db/write_batch.cc',
    'helpers/memenv/memenv.cc',
    'table/block.cc',
    'table/block_builder.cc',
    'table/filter_block.cc',
    'table/format.cc',
    'table/iterator.cc',
    'table/merger.cc',
    'table/table.cc',
    'table/table_builder.cc',
    'table/two_level_iterator.cc',
    'util/arena.cc',
    'util/bloom.cc',
    'util/cache.cc',
    'util/coding.cc',
    'util/comparator.cc',
    'util/crc32c.cc',
    'util/env.cc',
    'util/filter_policy.cc',
    'util/hash.cc',
    'util/histogram.cc',
    'util/logging.cc',
    'util/options.cc',
    'util/status.cc',
    'util/testharness.cc',
    'util/testutil.cc',
]

if env['PLATFORM'] == 'win32':
    SOURCE += ['port/port_win.cc', 'util/env_win.cc']
elif env['PLATFORM'] == 'posix':
    SOURCE += ['port/port_posix.cc', 'util/env_posix.cc']

env.StaticLibrary('leveldb',
    SOURCE,
)

if int(shared) == 1 and env['CC'] == 'gcc':
    env.SharedLibrary('leveldb',
        SOURCE,
        LIBS = ['pthread'],
        LINKFLAGS = '-Wl,--no-undefined',
    )

env_test = env.Clone()
env_test.Append(LIBS = ['leveldb'])
if env['CC'] == 'gcc':
    env_test.Append(LINKFLAGS = '-Wl,--rpath=./')
if env_test['PLATFORM'] == 'posix':
    env_test.Append(LIBS = ['pthread'])
    env_test.Program('c_test', 'db/c_test.c', LIBS = ['leveldb', 'pthread', 'stdc++'])
env_test.Program('corruption_test', 'db/corruption_test.cc')
env_test.Program('db_test', 'db/db_test.cc')
env_test.Program('dbformat_test', 'db/dbformat_test.cc')
env_test.Program('filename_test', 'db/filename_test.cc')
env_test.Program('log_test', 'db/log_test.cc')
env_test.Program('skiplist_test', 'db/skiplist_test.cc')
env_test.Program('version_edit_test', 'db/version_edit_test.cc')
env_test.Program('version_set_test', 'db/version_set_test.cc')
env_test.Program('write_batch_test', 'db/write_batch_test.cc')
env_test.Program('memenv_test', 'helpers/memenv/memenv_test.cc')
env_test.Program('filter_block_test', 'table/filter_block_test.cc')
env_test.Program('table_test', 'table/table_test.cc')
env_test.Program('arena_test', 'util/arena_test.cc')
env_test.Program('bloom_test', 'util/bloom_test.cc')
env_test.Program('cache_test', 'util/cache_test.cc')
env_test.Program('coding_test', 'util/coding_test.cc')
env_test.Program('crc32c_test', 'util/crc32c_test.cc')
env_test.Program('env_test', 'util/env_test.cc')

env_test.Program('db_bench', 'db/db_bench.cc')

