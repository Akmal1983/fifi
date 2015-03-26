#! /usr/bin/env python
# encoding: utf-8

APPNAME = 'fifi'
VERSION = '11.0.0'


def recurse_helper(ctx, name):
    if not ctx.has_dependency_path(name):
        ctx.fatal('Load a tool to find %s as system dependency' % name)
    else:
        p = ctx.dependency_path(name)
        ctx.recurse([p])


def options(opt):

    import waflib.extras.wurf_dependency_bundle as bundle
    import waflib.extras.wurf_dependency_resolve as resolve

    bundle.add_dependency(opt, resolve.ResolveGitMajorVersion(
        name='waf-tools',
        git_repository='github.com/steinwurf/external-waf-tools.git',
        major_version=2))

    bundle.add_dependency(opt, resolve.ResolveGitMajorVersion(
        name='gtest',
        git_repository='github.com/steinwurf/external-gtest.git',
        major_version=2))

    bundle.add_dependency(opt, resolve.ResolveGitMajorVersion(
        name='boost',
        git_repository='github.com/steinwurf/external-boost-light.git',
        major_version=1))

    bundle.add_dependency(opt, resolve.ResolveGitMajorVersion(
        name='sak',
        git_repository='github.com/steinwurf/sak.git',
        major_version=10))

    bundle.add_dependency(opt, resolve.ResolveGitMajorVersion(
        name='gauge',
        git_repository='github.com/steinwurf/cxx-gauge.git',
        major_version=7))

    bundle.add_dependency(opt, resolve.ResolveGitMajorVersion(
        name='tables',
        git_repository='github.com/steinwurf/tables.git',
        major_version=4))

    bundle.add_dependency(opt, resolve.ResolveGitMajorVersion(
        name='cpuid',
        git_repository='github.com/steinwurf/cpuid.git',
        major_version=3))

    bundle.add_dependency(opt, resolve.ResolveGitMajorVersion(
        name='platform',
        git_repository='github.com/steinwurf/platform.git',
        major_version=1))

    opt.load('wurf_dependency_bundle')
    opt.load('wurf_tools')


def configure(conf):

    if conf.is_toplevel():

        conf.load('wurf_dependency_bundle')
        conf.load('wurf_tools')

        conf.load_external_tool('mkspec', 'wurf_cxx_mkspec_tool')
        conf.load_external_tool('runners', 'wurf_runner')
        conf.load_external_tool('install_path', 'wurf_install_path')
        conf.load_external_tool('project_gen', 'wurf_project_generator')

        recurse_helper(conf, 'boost')
        recurse_helper(conf, 'gtest')
        recurse_helper(conf, 'sak')
        recurse_helper(conf, 'gauge')
        recurse_helper(conf, 'tables')
        recurse_helper(conf, 'cpuid')
        recurse_helper(conf, 'platform')

    set_simd_flags(conf)


def set_simd_flags(conf):
    """
    Sets flags used to compile in SIMD mode
    """
    CXX = conf.env.get_flat("CXX")
    flags = []

    # Matches both /usr/bin/g++ and /user/bin/clang++
    if 'g++' in CXX or 'clang' in CXX:
        # DEST_CPU should be set explicitly for clang cross-compilers
        cpu = conf.env['DEST_CPU']
        # Test different compiler flags based on the target CPU
        if cpu == 'x86' or cpu == 'x86_64':
            flags += conf.mkspec_try_flags('cxxflags', ['-mssse3'])
        elif cpu == 'arm':
            flags += conf.mkspec_try_flags('cxxflags', ['-mfpu=neon'])

    elif 'CL.exe' in CXX or 'cl.exe' in CXX:
        pass

    else:
        conf.fatal('Unknown compiler - no SIMD flags specified')

    conf.env['CFLAGS_FIFI_SIMD'] = flags
    conf.env['CXXFLAGS_FIFI_SIMD'] = flags


def build(bld):

    bld(includes='./src',
        export_includes='./src',
        name='fifi_includes')

    if bld.is_toplevel():

        bld.load('wurf_dependency_bundle')

        recurse_helper(bld, 'boost')
        recurse_helper(bld, 'gtest')
        recurse_helper(bld, 'sak')
        recurse_helper(bld, 'gauge')
        recurse_helper(bld, 'tables')
        recurse_helper(bld, 'cpuid')
        recurse_helper(bld, 'platform')

        # Only build test and benchmarks when executed from the
        # top-level wscript i.e. not when included as a dependency
        # in a recurse call

        bld.recurse('test')
        bld.recurse('benchmark/basic_operations')
        bld.recurse('benchmark/arithmetic')
        bld.recurse('benchmark/prime2325')

    bld.recurse('src/fifi')
